#include <iostream> /* cerr */
#include <algorithm>
#include <cstdint> /* uint16_t */
#include "supservidor.h"

using namespace std;

/* ========================================
   CLASSE SUPSERVIDOR
   ======================================== */

/// Construtor
SupServidor::SupServidor()
    : Tanks()
    , server_on(false)
    , LU()
      /*ACRESCENTAR*/
      , thr_server()
      , sock_server()
{
  // Inicializa a biblioteca de sockets
  /*ACRESCENTAR*/
  mysocket_status iResult = mysocket::init();

  // Em caso de erro, mensagem e encerra
  if (/*MODIFICAR*/ iResult != mysocket_status::SOCK_OK)
  {
    cerr << "Biblioteca mysocket nao pode ser inicializada";
    exit(-1);
  }
}

/// Destrutor
SupServidor::~SupServidor()
{
  // Deve parar a thread do servidor
  server_on = false;

  // Fecha todos os sockets dos clientes
  for (auto &U : LU)
    U.close();
  // Fecha o socket de conexoes
  /*ACRESCENTAR*/
  sock_server.close();

  // Espera o fim da thread do servidor
  /*ACRESCENTAR*/
  if (thr_server.joinable())
    thr_server.join();

  // Encerra a biblioteca de sockets
  /*ACRESCENTAR*/
  mysocket::end();
}

/// Liga o servidor
bool SupServidor::setServerOn()
{
  // Se jah estah ligado, nao faz nada
  if (server_on)
    return true;

  // Liga os tanques
  setTanksOn();

  // Indica que o servidor estah ligado a partir de agora
  server_on = true;

  try
  {
    // Coloca o socket de conexoes em escuta
    /*ACRESCENTAR*/
    mysocket_status iResult = sock_server.listen(SUP_PORT);

    // Em caso de erro, gera excecao
    if (/*MODIFICAR*/ iResult != mysocket_status::SOCK_OK)
      throw 1;

    // Lanca a thread do servidor que comunica com os clientes
    /*ACRESCENTAR*/
    thr_server = std::thread([this]()
                             { this->thr_server_main(); });

    // Em caso de erro, gera excecao
    if (/*MODIFICAR*/ !thr_server.joinable())
      throw 2;
  }
  catch (int i)
  {
    cerr << "Erro " << i << " ao iniciar o servidor\n";

    // Deve parar a thread do servidor
    server_on = false;

    // Fecha o socket do servidor
    /*ACRESCENTAR*/
    sock_server.close();

    return false;
  }

  // Tudo OK
  return true;
}

/// Desliga o servidor
void SupServidor::setServerOff()
{
  // Se jah estah desligado, nao faz nada
  if (!server_on)
    return;

  // Deve parar a thread do servidor
  server_on = false;

  // Fecha todos os sockets dos clientes
  for (auto &U : LU)
    U.close();
  // Fecha o socket de conexoes
  /*ACRESCENTAR*/
  sock_server.close();

  // Espera pelo fim da thread do servidor
  /*ACRESCENTAR*/
  if (thr_server.joinable())
    thr_server.join();

  // Faz o identificador da thread apontar para thread vazia
  /*ACRESCENTAR*/
  thr_server = std::thread();

  // Desliga os tanques
  setTanksOff();
}

/// Leitura do estado dos tanques
void SupServidor::readStateFromSensors(SupState &S) const
{
  // Estados das valvulas: OPEN, CLOSED
  S.V1 = v1isOpen();
  S.V2 = v2isOpen();
  // Niveis dos tanques: 0 a 65535
  S.H1 = hTank1();
  S.H2 = hTank2();
  // Entrada da bomba: 0 a 65535
  S.PumpInput = pumpInput();
  // Vazao da bomba: 0 a 65535
  S.PumpFlow = pumpFlow();
  // Estah transbordando (true) ou nao (false)
  S.ovfl = isOverflowing();
}

/// Leitura e impressao em console do estado da planta
void SupServidor::readPrintState() const
{
  if (tanksOn())
  {
    SupState S;
    readStateFromSensors(S);
    S.print();
  }
  else
  {
    cout << "Tanques estao desligados!\n";
  }
}

/// Impressao em console dos usuarios do servidor
void SupServidor::printUsers() const
{
  for (const auto &U : LU)
  {
    cout << U.login << '\t'
         << "Admin=" << (U.isAdmin ? "SIM" : "NAO") << '\t'
         << "Conect=" << (U.isConnected() ? "SIM" : "NAO") << '\n';
  }
}

/// Adicionar um novo usuario
bool SupServidor::addUser(const string &Login, const string &Senha,
                          bool Admin)
{
  // Nao pode ser chamada com servidor em funcionamento
  if (serverOn())
  {
    cerr << "Nao pode adicionar usuarios com servidor em funcionamento!\n";
    return false;
  }

  // Testa os dados do novo usuario
  if (Login.size() < 6 || Login.size() > 12)
    return false;
  if (Senha.size() < 6 || Senha.size() > 12)
    return false;

  // Testa se jah existe usuario com mesmo login
  auto itr = find(LU.begin(), LU.end(), Login);
  if (itr != LU.end())
    return false;

  // Insere
  LU.push_back(User(Login, Senha, Admin));

  // Insercao OK
  return true;
}

/// Remover um usuario
bool SupServidor::removeUser(const string &Login)
{
  // Nao pode ser chamada com servidor em funcionamento
  if (serverOn())
  {
    cerr << "Nao pode remover usuarios com servidor em funcionamento!\n";
    return false;
  }

  // Testa se existe usuario com esse login
  auto itr = find(LU.begin(), LU.end(), Login);
  if (itr == LU.end())
    return false;

  // Remove
  LU.erase(itr);

  // Remocao OK
  return true;
}

/// A thread que implementa o servidor.
/// Comunicacao com os clientes atraves dos sockets.
void SupServidor::thr_server_main(void)
{
  // Fila de sockets para aguardar chegada de dados
  /*ACRESCENTAR*/
  mysocket_queue f;
  tcp_mysocket t;
  uint16_t cmd;
  string login, password;
  mysocket_status iResult;

  while (server_on)
  {
    // Erros mais graves que encerram o servidor
    // Parametro do throw e do catch eh uma const char* = "texto"
    try
    {
      // Encerra se o socket de conexoes estiver fechado
      if (/*MODIFICAR*/ !sock_server.accepting())
      {
        throw "socket de conexoes fechado";
      }

      // Inclui na fila de sockets todos os sockets que eu
      // quero monitorar para ver se houve chegada de dados

      // Limpa a fila de sockets
      /*ACRESCENTAR*/
      f.clear();

      // Inclui na fila o socket de conexoes
      /*ACRESCENTAR*/
      f.include(sock_server);

      // Inclui na fila todos os sockets dos clientes conectados
      /*ACRESCENTAR*/
      for (auto &U : LU)
      {
        if (U.isConnected())
          f.include(U.sock);
      }

      // Espera ateh que chegue dado em algum socket (com timeout)
      /*ACRESCENTAR*/
      iResult = f.wait_read(1000 * SUP_TIMEOUT);

      // De acordo com o resultado da espera:
      switch (iResult)
      {

      // SOCK_TIMEOUT:
      case mysocket_status::SOCK_TIMEOUT:

        // Saiu por timeout: nao houve atividade em nenhum socket
        // Aproveita para salvar dados ou entao nao faz nada
        break;

      // SOCK_ERROR:
      case mysocket_status::SOCK_ERROR:
        // Erro no select: encerra o servidor
        throw "erro no select da fila";
        break;

      // SOCK_OK:
      case mysocket_status::SOCK_OK:

        // Houve atividade em algum socket da fila:
        //   Testa se houve atividade nos sockets dos clientes. Se sim:
        for (auto &U : LU)
        {
          if (server_on && U.isConnected() && f.had_activity(U.sock))
          {
            try
            {

              //   - Leh o comando
              iResult = U.sock.read_uint16(cmd);
              if (iResult != mysocket_status::SOCK_OK)
                throw 1;

              //   - Executa a acao
              //   = Envia resposta
              if (cmd == CMD_GET_DATA)
              {
                SupState S;
                readStateFromSensors(S);
                iResult = U.sock.write_uint16(CMD_DATA);
                if (iResult != mysocket_status::SOCK_OK)
                  throw 2;
                iResult = U.sock.write_bytes((const mybyte *)&S, sizeof(SupState));
                if (iResult != mysocket_status::SOCK_OK)
                  throw 3;
              }
              else if (cmd == CMD_SET_V1 || cmd == CMD_SET_V2)
              {
                uint16_t estado;
                iResult = U.sock.read_uint16(estado, 1000 * SUP_TIMEOUT);
                if (iResult != mysocket_status::SOCK_OK)
                  throw 4;

                if (!U.isAdmin)
                {
                  U.sock.write_uint16(CMD_ERROR);
                  cout << (cmd == CMD_SET_V1 ? "CMD_SET_V1 " : "CMD_SET_V2 ") << estado << " DE " << U.login << " (ERROR)\n";
                }
                else
                {
                  if (cmd == CMD_SET_V1)
                    setV1Open(estado != 0);
                  else
                    setV2Open(estado != 0);
                  U.sock.write_uint16(CMD_OK);
                  cout << (cmd == CMD_SET_V1 ? "CMD_SET_V1 " : "CMD_SET_V2 ") << estado << " DE " << U.login << " (OK)\n";
                }
              }
              else if (cmd == CMD_SET_PUMP)
              {
                uint16_t entrada;
                iResult = U.sock.read_uint16(entrada, 1000 * SUP_TIMEOUT);
                if (iResult != mysocket_status::SOCK_OK)
                  throw 5;

                if (!U.isAdmin)
                {
                  U.sock.write_uint16(CMD_ERROR);
                  cout << "CMD_SET_PUMP " << entrada << " DE " << U.login << " (ERROR)\n";
                }
                else
                {
                  setPumpInput(entrada);
                  U.sock.write_uint16(CMD_OK);
                  cout << "CMD_SET_PUMP " << entrada << " DE " << U.login << " (OK)\n";
                }
              }
              else if (cmd == CMD_LOGOUT)
              {
                cout << "CMD_LOGOUT " << U.login << endl;
                throw 6;
              }
              else
              {
                throw 7;
              }
            }
            catch (int erro)
            {
              U.close();
            }
          }
        }
        //   Depois, testa se houve atividade no socket de conexao. Se sim:
        if (server_on && sock_server.connected() && f.had_activity(sock_server))
        {

          //   - Estabelece nova conexao em socket temporario
          iResult = sock_server.accept(t);
          if (iResult != mysocket_status::SOCK_OK)
            throw "nova conexao falhou";

          try
          {

            //   - Leh comando, login e senha
            iResult = t.read_uint16(cmd, 1000 * SUP_TIMEOUT);
            if (iResult != mysocket_status::SOCK_OK)
              throw 1;
            if (cmd != CMD_LOGIN)
              throw 2;

            iResult = t.read_string(login, 1000 * SUP_TIMEOUT);
            if (iResult != mysocket_status::SOCK_OK)
              throw 3;

            iResult = t.read_string(password, 1000 * SUP_TIMEOUT);
            if (iResult != mysocket_status::SOCK_OK)
              throw 4;

            //   - Testa usuario
            auto iU = find(LU.begin(), LU.end(), login);

            if (iU == LU.end())
            {
              t.write_uint16(CMD_ERROR);
              cout << "CMD_LOGIN inexistente (ERROR)\n";
              throw 5;
            }
            else if (iU->password != password || iU->isConnected())
            {
              t.write_uint16(CMD_ERROR);
              cout << "CMD_LOGIN " << login << " (ERROR)\n";
              throw 6;
            }

            //   - Se deu tudo certo, faz o socket temporario ser o novo socket
            //     do cliente e envia confirmacao
            iU->sock.swap(t);
            iU->sock.write_uint16(iU->isAdmin ? CMD_ADMIN_OK : CMD_OK);
            cout << "CMD_LOGIN " << login << " (OK)\n";
          }
          catch (int erro)
          {
            t.close();
          }
        }
        break;
      }

    } // fim try - Erros mais graves que encerram o servidor
    catch (const char *err) // Erros mais graves que encerram o servidor
    {
      cerr << "Erro no servidor: " << err << endl;

      // Sai do while e encerra a thread
      server_on = false;

      // Fecha todos os sockets dos clientes
      for (auto &U : LU)
        U.close();
      // Fecha o socket de conexoes
      /*ACRESCENTAR*/
      sock_server.close();

      // Os tanques continuam funcionando

    } // fim catch - Erros mais graves que encerram o servidor
  } // fim while (server_on)
}
