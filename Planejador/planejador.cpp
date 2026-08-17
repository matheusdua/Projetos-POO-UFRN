#include <stdexcept>
#include <cmath>
#include <fstream>
#include <limits>
/* ACRESCENTE SE NECESSARIO */
#include <algorithm>
#include <list>

#include "planejador.h"

using namespace std;

/* *************************
 * CLASSE IDPONTO        *
 ************************* */

/// Atribuicao de string
/// NAO DEVE SER MODIFICADA
void IDPonto::set(string &&S)
{
  t = move(S);
  if (!valid())
    t.clear();
}

/* *************************
 * CLASSE IDROTA         *
 ************************* */

/// Atribuicao de string
/// NAO DEVE SER MODIFICADA
void IDRota::set(string &&S)
{
  t = move(S);
  if (!valid())
    t.clear();
}

/* *************************
 * CLASSE PONTO          *
 ************************* */

/// Impressao em console
/// NAO DEVE SER MODIFICADA
ostream &operator<<(ostream &X, const Ponto &P)
{
  X << P.id << '\t' << P.nome << " (" << P.latitude << ',' << P.longitude << ')';
  return X;
}

/// Distancia entre 2 pontos (formula de haversine)
/// NAO DEVE SER MODIFICADA
double Ponto::distancia(const Ponto &P) const
{
  // Gera excecao se pontos invalidos
  if (!valid() || !P.valid())
    throw invalid_argument("distancia: ponto(s) invalido(s)");

  // Tratar logo pontos identicos
  if (id == P.id)
    return 0.0;
  // Constantes
  static const double MY_PI = 3.14159265358979323846;
  static const double R_EARTH = 6371.0;
  // Conversao para radianos
  double lat1 = MY_PI * this->latitude / 180.0;
  double lat2 = MY_PI * P.latitude / 180.0;
  double lon1 = MY_PI * this->longitude / 180.0;
  double lon2 = MY_PI * P.longitude / 180.0;
  // Seno das diferencas
  double sin_dlat2 = sin((lat2 - lat1) / 2.0);
  double sin_dlon2 = sin((lon2 - lon1) / 2.0);
  // Quadrado do seno do angulo entre os pontos
  double sin2_ang = sin_dlat2 * sin_dlat2 + cos(lat1) * cos(lat2) * sin_dlon2 * sin_dlon2;
  // Em vez de utilizar a funcao arcosseno, asin(sqrt(sin2_ang)),
  // vou utilizar a funcao arcotangente, menos sensivel a erros numericos.
  // Distancia entre os pontos
  return 2.0 * R_EARTH * atan2(sqrt(sin2_ang), sqrt(1 - sin2_ang));
}

/* *************************
 * CLASSE ROTA           *
 ************************* */

/// Impressao em console
/// NAO DEVE SER MODIFICADA
ostream &operator<<(ostream &X, const Rota &R)
{
  X << R.id << '\t' << R.nome << '\t' << R.comprimento << "km"
    << " [" << R.extremidade[0] << ',' << R.extremidade[1] << ']';
  return X;
}

/// Retorna a outra extremidade da rota, a que nao eh o parametro.
/// Gera excecao se o parametro nao for uma das extremidades da rota.
/// NAO DEVE SER MODIFICADA
IDPonto Rota::outraExtremidade(const IDPonto &ID) const
{
  if (extremidade[0] == ID)
    return extremidade[1];
  if (extremidade[1] == ID)
    return extremidade[0];
  throw invalid_argument("outraExtremidade: invalid IDPonto parameter");
}

/* *************************
 * CLASSE PLANEJADOR     *
 ************************* */

/// Torna o mapa vazio
/// NAO DEVE SER MODIFICADA
void Planejador::clear()
{
  pontos.clear();
  rotas.clear();
}

/// Funcao auxiliar para eliminar eventuais separadores do final de uma string.
inline void trim(string &S)
{
  while (!S.empty() && isspace(S.back()))
    S.pop_back();
}

/// Leh um mapa dos arquivos arq_pontos e arq_rotas.
/// Caso nao consiga ler dos arquivos, deixa o mapa inalterado e
/// gera excecao ios_base::failure.
/// Deve receber ACRESCIMOS
void Planejador::ler(const std::string &arq_pontos,
                     const std::string &arq_rotas)
{
  // Vetores temporarios para armazenamento dos Pontos e Rotas lidos.
  vector<Ponto> temp_Pontos;
  vector<Rota> temp_Rotas;

  // Leh os Pontos do arquivo e armazena no vetor temporario de Pontos.
  // Em caso de qualquer erro, gera excecao ios_base::failure com mensagem:
  //   "Erro <CODIGO> na leitura do arquivo de pontos <ARQ_PONTOS>"
  try
  {
    // 1) Abre uma stream associada ao arquivo de Pontos
    //    (Em caso de erro, codigo 1)
    ifstream arq_pt(arq_pontos);
    if (!arq_pt.is_open())
      throw 1;

    // 2) Consome eventuais separadores, leh o cabecalho do arquivo, elimina eventuais
    //    separadores no final da string e testa o cabecalho:
    //    "ID;Nome;Latitude;Longitude"
    //    (Em caso de erro ou valor lido diferente, codigo 2)
    //    Consome os separadores apos o cabecalho
    arq_pt >> ws;
    string cabecalho;
    getline(arq_pt, cabecalho);
    trim(cabecalho);

    if (arq_pt.fail() || cabecalho != "ID;Nome;Latitude;Longitude")
      throw 2;

    arq_pt >> ws;
    // 3) Enquanto o arquivo n�o acabar (eof), repita a leitura de cada um dos Pontos:
    while (!arq_pt.eof())
    {
      Ponto pontoTemp;
      string strTemp;
      char c;
      //    | 3.1) Leh a ID e elimina eventuais separadores no final da string
      //    |      (Em caso de erro ou conteudo lido vazio, codigo 3)
      //    |      O teste se a ID eh valida serah feito ao testar o Ponto

      getline(arq_pt, strTemp, ';');
      trim(strTemp);
      if (arq_pt.fail() || strTemp.empty())
        throw 3;
      pontoTemp.id.set(move(strTemp));

      //    | 3.2) Consome os separadores, leh o nome e elimina eventuais separadores no final
      //    |      da string
      //    |      (Em caso de erro ou conteudo lido vazio, codigo 4)
      //    |      O teste se o nome eh valido serah feito ao testar o Ponto
      arq_pt >> ws;
      getline(arq_pt, pontoTemp.nome, ';');
      trim(pontoTemp.nome);
      if (arq_pt.fail() || pontoTemp.nome.empty())
        throw 4;

      //    | 3.3) Leh a latitude
      //    |      (Em caso de erro, codigo 5)
      //    |      O teste se a latitude eh valida serah feito ao testar o Ponto
      arq_pt >> pontoTemp.latitude;
      if (arq_pt.fail())
        throw 5;

      //    | 3.4) Leh o caractere ';'
      //    |      (Em caso de erro ou valor lido diferente, codigo 6)
      arq_pt >> c;
      if (arq_pt.fail() || c != ';')
        throw 6;

      //    | 3.5) Leh a longitude
      //    |      (Em caso de erro, codigo 7)
      //    |      O teste se a longitude eh valida serah feito ao testar o Ponto
      arq_pt >> pontoTemp.longitude;
      if (arq_pt.fail())
        throw 7;

      //    | 3.6) Consome os separadores apos o Ponto
      arq_pt >> ws;

      //    | 3.7) Testa se o Ponto com os parametros lidos eh valido
      //    |      (Em caso de erro, codigo 8)
      if (!pontoTemp.valid())
        throw 8;

      //    | 3.8) Testa que nao existe Ponto com a mesma ID no vetor temporario
      //    |      de Pontos lidos ateh agora
      //    |      (Em caso de erro, codigo 9)
      if (find(temp_Pontos.begin(), temp_Pontos.end(), pontoTemp.id) != temp_Pontos.end())
        throw 9;

      //    | 3.9) Insere o Ponto lido no vetor temporario de Pontos
      temp_Pontos.push_back(pontoTemp);
    }

    // 4) Se n�o foi lido nenhum Ponto, gera erro (codigo 10)
    if (temp_Pontos.empty())
      throw 10;

    // 5) Fecha o arquivo de Pontos
    arq_pt.close();
  }
  catch (int i)
  {
    // Chama o destrutor de todas as variaveis criadas dentro do try, inclusive da
    // stream associada ao arquivo. Portanto, nao precisa fechar a stream.
    string msg = "Erro " + to_string(i) + " na leitura do arquivo de pontos " + arq_pontos;
    throw ios_base::failure(msg);
  }

  // Leh as Rotas do arquivo e armazena no vetor temporario de Rotas.
  // Em caso de qualquer erro, gera excecao ios_base::failure com mensagem:
  //   "Erro <CODIGO> na leitura do arquivo de rotas <ARQ_ROTAS>"
  try
  {
    // 1) Abre uma stream associada ao arquivo de Rotas
    //    (Em caso de erro, codigo 1)
    ifstream arq_rt(arq_rotas);
    if (!arq_rt.is_open())
      throw 1;
    // 2) Consome eventuais separadores, leh o cabecalho do arquivo, elimina eventuais
    //    separadores no final da string e testa o cabecalho:
    //    "ID;Nome;Extremidade 1;Extremidade 2;Comprimento"
    //    (Em caso de erro ou valor lido diferente, codigo 2)
    //    Consome os separadores apos o cabecalho
    string cabecalho;

    arq_rt >> ws;
    getline(arq_rt, cabecalho);
    trim(cabecalho);

    if (arq_rt.fail() || (cabecalho != "ID;Nome;Extremidade 1;Extremidade 2;Comprimento"))
      throw 2;
    arq_rt >> ws;

    // 3) Enquanto o arquivo n�o acabar (eof), repita a leitura de cada uma das Rotas:
    while (!arq_rt.eof())
    {
      Rota rotaTemp;
      string strTemp;
      //    | 3.1) Leh a ID e elimina eventuais separadores no final da string
      //    |      (Em caso de erro ou conteudo lido vazio, codigo 3)
      //    |      O teste se a ID eh valida serah feito ao testar a Rota
      getline(arq_rt, strTemp, ';');
      trim(strTemp);

      if (arq_rt.fail() || strTemp.empty())
        throw 3;

      rotaTemp.id.set(move(strTemp));
      //    | 3.2) Consome os separadores, leh o nome e elimina eventuais separadores no final
      //    |      da string
      //    |      (Em caso de erro ou conteudo lido vazio, codigo 4)
      //    |      O teste se o nome eh valido serah feito ao testar a Rota
      arq_rt >> ws;
      getline(arq_rt, rotaTemp.nome, ';');
      trim(rotaTemp.nome);
      if (arq_rt.fail() || rotaTemp.nome.empty())
        throw 4;

      //    | 3.3) Consome os separadores, leh a ID da extremidade[0] e elimina eventuais
      //    |      separadores no final da string
      //    |      (Em caso de erro ou conteudo lido vazio, codigo 5)
      //    |      O teste se a ID eh valida serah feito ao testar a Rota

      arq_rt >> ws;
      getline(arq_rt, strTemp, ';');
      trim(strTemp);

      if (arq_rt.fail() || strTemp.empty())
        throw 5;

      rotaTemp.extremidade[0].set(move(strTemp));

      //    | 3.4) Consome os separadores, leh a ID da extremidade[1] e elimina eventuais
      //    |      separadores no final da string
      //    |      (Em caso de erro ou conteudo lido vazio, codigo 6)
      //    |      O teste se a ID eh valida serah feito ao testar a Rota]

      arq_rt >> ws;
      getline(arq_rt, strTemp, ';');
      trim(strTemp);

      if (arq_rt.fail() || strTemp.empty())
        throw 6;

      rotaTemp.extremidade[1].set(move(strTemp));

      //    | 3.5) Leh o comprimento
      //    |      (Em caso de erro na leitura, codigo 7)
      //    |      O teste se o comprimento eh valido serah feito ao testar o Ponto

      arq_rt >> rotaTemp.comprimento;
      if (arq_rt.fail())
        throw 7;
      //    | 3.6) Consome os separadores apos a Rota
      arq_rt >> ws;

      //    | 3.7) Testa se a Rota com esses parametros lidos eh valida
      //    |      (Em caso de erro, codigo 8)
      if (!rotaTemp.valid())
        throw 8;

      //    | 3.8) Testa que a Id da extremidade[0] corresponde a um ponto lido
      //    |      no vetor temporario de Pontos
      //    |      (Em caso de erro, codigo 9)
      if (find(temp_Pontos.begin(), temp_Pontos.end(), rotaTemp.extremidade[0]) == temp_Pontos.end())
        throw 9;

      //    | 3.9) Testa que a Id da extremidade[1] corresponde a um ponto lido
      //    |      no vetor temporario de Pontos
      //    |      (Em caso de erro, codigo 10)
      if (find(temp_Pontos.begin(), temp_Pontos.end(), rotaTemp.extremidade[1]) == temp_Pontos.end())
        throw 10;

      //    | 3.10)Testa que nao existe Rota com a mesma ID no vetor temporario
      //    |      de Rotas lidas ateh agora
      //    |      (Em caso de erro, codigo 11)
      if (find(temp_Rotas.begin(), temp_Rotas.end(), rotaTemp.id) != temp_Rotas.end())
        throw 11;

      //    | 3.11)Insere a Rota lida no vetor temporario de Rotas

      temp_Rotas.push_back(rotaTemp);
    }

    // 4) Se n�o foi lido nenhuma Rota, gera erro (codigo 12)
    if (temp_Rotas.empty())
      throw 10;
    // 5) Fecha o arquivo de Rotas
    arq_rt.close();
  }
  catch (int i)
  {
    // Chama o destrutor de todas as variaveis criadas dentro do try, inclusive da
    // stream associada ao arquivo. Portanto, nao precisa fechar a stream.
    string msg = "Erro " + to_string(i) + " na leitura do arquivo de rotas " + arq_rotas;
    throw ios_base::failure(msg);
  }

  // Faz os vetores de Pontos e Rotas do planejador assumirem o conteudo dos
  // vetores temporarios de Pontos e Rotas
  swap(this->pontos, temp_Pontos);
  swap(this->rotas, temp_Rotas);
}

/// Retorna um Ponto do mapa, passando a id como parametro.
/// Se a id for inexistente, gera excecao.
/// Deve receber ACRESCIMOS
Ponto Planejador::getPonto(const IDPonto &Id) const
{
  // Procura um ponto que corresponde aa Id do parametro
  auto itr = find(pontos.begin(), pontos.end(), Id);

  // Em caso de sucesso, retorna o ponto encontrado
  if (itr != pontos.end())
    return *itr;

  // Se nao encontrou, gera excecao
  throw invalid_argument("getPonto: invalid IDPonto parameter");
}

/// Retorna um Rota do mapa, passando a id como parametro.
/// Se a id for inexistente, gera excecao.
/// Deve receber ACRESCIMOS
Rota Planejador::getRota(const IDRota &Id) const
{
  // Procura uma rota que corresponde aa Id do parametro
  auto itr = find(rotas.begin(), rotas.end(), Id);
  // Em caso de sucesso, retorna a rota encontrada
  if (itr != rotas.end())
    return *itr;

  // Se nao encontrou, gera excecao
  throw invalid_argument("getRota: invalid IDRota parameter");
}

/// *******************************************************************************
/// Calcula o caminho entre a origem e o destino do planejador usando o algoritmo A*
/// *******************************************************************************

/// Noh: struct/classe dos elementos dos conjuntos de busca do algoritmo A*.
/// Deve ser DECLARADA E IMPLEMENTADA inteiramente.
/* ***********  /
/  implementado  /
/  *********** */

/// Calcula o caminho mais curto no mapa entre origem e destino, usando o algoritmo A*
/// Retorna o comprimento do caminho encontrado (<0 se nao existe caminho).
/// O parametro C retorna o caminho encontrado (vazio se nao existe caminho).
/// O parametro NumAberto retorna o numero de nos (>=0) em Aberto ao termino do algoritmo A*,
/// mesmo quando nao existe caminho.
/// O parametro NumFechado retorna o numero de nos (>=0) em Fechado ao termino do algoritmo A*,
/// mesmo quando nao existe caminho.
/// Em caso de parametros de entrada invalidos ou de erro no algoritmo, gera excecao.
/// Deve receber ACRESCIMOS.
double Planejador::calculaCaminho(const IDPonto &id_origem,
                                  const IDPonto &id_destino,
                                  Caminho &C, int &NumAberto, int &NumFechado)
{
  try
  {
    // Mapa vazio
    if (empty())
      throw 1;

    Ponto pt_origem, pt_destino;
    // Calcula os pontos que correspondem a id_origem e id_destino.
    // Se algum nao existir, throw 2
    try
    {
      pt_origem = getPonto(id_origem);
      pt_destino = getPonto(id_destino);
    }
    catch (...)
    {
      throw 2;
    }

    /* *****************************  /
    /  IMPLEMENTACAO DO ALGORITMO A*  /
    /  ***************************** */

    Noh atual(id_origem, IDRota(), 0.0, pt_origem.distancia(pt_destino));

    list<Noh> aberto;
    vector<Noh> fechado;

    aberto.push_back(atual);

    do
    {

      atual = aberto.front();
      aberto.pop_front();

      fechado.push_back(atual);

      if (atual.id_pt != id_destino)
      {
        auto itr_rotas = rotas.begin();
        bool rotaExiste = false;

        auto testa_conexao = [&](const Rota &R)
        { return R.extremidade[0] == atual.id_pt || R.extremidade[1] == atual.id_pt; };

        do
        {
          itr_rotas = find_if(itr_rotas, rotas.end(), testa_conexao);

          rotaExiste = (itr_rotas != rotas.end());

          if (rotaExiste)
          {
            Rota &rota_suc = *itr_rotas;

            IDPonto id_pt_suc = rota_suc.outraExtremidade(atual.id_pt);
            Noh suc(id_pt_suc, rota_suc.id, atual.g + rota_suc.comprimento, getPonto(id_pt_suc).distancia(pt_destino));

            bool eh_inedito = true;

            auto oldF = find(fechado.begin(), fechado.end(), suc.id_pt);

            if (oldF != fechado.end())
            {
              eh_inedito = false;
            }
            else
            {
              auto oldA = find(aberto.begin(), aberto.end(), suc.id_pt);

              if (oldA != aberto.end())
              {

                if (suc.f() < oldA->f())
                  aberto.erase(oldA);
                else
                  eh_inedito = false;
              }
            }

            if (eh_inedito)
            {
              auto big = upper_bound(aberto.begin(), aberto.end(), suc);
              aberto.insert(big, suc);
            }
            ++itr_rotas;
          }
        } while (rotaExiste);
      }
    } while (!aberto.empty() && atual.id_pt != id_destino);

    NumAberto = aberto.size();
    NumFechado = fechado.size();

    C = Caminho();

    double Compr;

    if (atual.id_pt != id_destino)
    {
      Compr = -1.0;
    }
    else
    {
      Compr = atual.g;

      while (atual.id_rt.valid())
      {
        C.push(Trecho(atual.id_rt, atual.id_pt));

        Rota rota_ant = getRota(atual.id_rt);

        IDPonto id_pt_ant = rota_ant.outraExtremidade(atual.id_pt);
        auto itr_ant = find(fechado.begin(), fechado.end(), id_pt_ant);

        if (itr_ant != fechado.end())
          atual = *itr_ant;
        else
          throw 3;
      }
      C.push(Trecho(IDRota(), atual.id_pt));
    }
    return Compr;
  }
  // end A*

  catch (int i)
  {
    string msg_err = "Erro " + to_string(i) + " no calculo do caminho\n";
    throw invalid_argument(msg_err);
  }

  // Zera o caminho resultado em caso de erro
  C = Caminho();

  // Retorna -1 em caso de erro
  return -1.0;
}
