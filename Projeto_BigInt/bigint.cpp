#include <iostream>
#include <cmath>
#include <utility>
#include "bigint.h"

using namespace std;

/// Construtor default.
/// Inicializa com um inteiro de 1 digito, valor 0 (zero).
/// NAO PODE SER MODIFICADO
BigInt::BigInt()
    : neg(false), nDig(1), d(new int8_t[nDig]{0})
{
}

/// Destrutor
/* ACRESCENTAR */
BigInt::~BigInt()
{
  delete[] d;
}

/// Construtor especifico PRIVADO que recebe o sinal e a quantidade de digitos
/* ACRESCENTAR */
BigInt::BigInt(bool ehNegativo, int tamanho) : neg(ehNegativo), nDig(tamanho < 1 ? 1 : tamanho), d(new int8_t[nDig]{0})
{
}

/// Construtor por copia.
/// Delega ao construtor especifico privado.
/// NAO PODE SER MODIFICADO.
BigInt::BigInt(const BigInt &B)
    : BigInt(B.isNeg(), B.size())
{
  // Copia os digitos
  for (int i = 0; i < size(); ++i)
    d[i] = B.d[i];
}

/// Atribuicao por copia
/* ACRESCENTAR */
BigInt &BigInt::operator=(const BigInt &B)
{
  if (this == &B)
    return *this;

  neg = B.neg;

  if (nDig != B.nDig)
  {
    delete[] d;
    nDig = B.nDig;

    d = new int8_t[nDig];
  }

  for (int i = 0; i < nDig; ++i)
    d[i] = B.d[i];

  return *this;
}

/// Construtor por movimento
/* ACRESCENTAR */
BigInt::BigInt(BigInt &&Temp) noexcept : neg(Temp.neg), nDig(Temp.nDig), d(Temp.d)
{
  Temp.neg = false;
  Temp.nDig = 0;
  Temp.d = nullptr;
}

/// Atribuicao por movimento
/* ACRESCENTAR */

BigInt &BigInt::operator=(BigInt &&Temp) noexcept
{
  if (&Temp == this)
    return *this;

  delete[] d;

  neg = false;
  nDig = 0;
  d = nullptr;

  std::swap(neg, Temp.neg);
  std::swap(nDig, Temp.nDig);
  std::swap(d, Temp.d);

  return *this;
}

/// Construtor especifico a partir de inteiro longo.
/// Tambem conversor de long long int para BigInt.
/// NAO PODE SER MODIFICADO NAS PARTES JAH IMPLEMENTADAS.
/// PODE (E PRECISA) RECEBER ACRESCIMOS, APENAS
/// NAS PARTES INDICADAS POR /* ACRESCENTAR */
BigInt::BigInt(long long int N) : BigInt(N < 0, (N == 0 ? 1 : 1 + int(log10(fabs(N)))))
{
  // Calcula os digitos, usando divisao inteira por 10
  for (int i = 0; i < size(); ++i)
  {
    d[i] = abs(N % 10); // Modulo do resto da divisao
    N /= 10;            // Divisao inteira
  }
}

long long int BigInt::toInt() const
{
  long long int val = 0;
  for (int i = nDig - 1; i >= 0; --i)
  {
    val = 10 * val + d[i];
    if (val < 0)
    {
      cerr << "ERRO Long pequeno" << endl;
      return 0;
    }
  }
  if (neg)
    val = -val;
  return val;
}

/// Conversor de BigInt para long long int
/* ACRESCENTAR */

/// ******************
/// * FIM DA PARTE 1 *
/// ******************

/// Funcao privada que corrige o numero, caso haja inconsistencias
/* ACRESCENTAR */
void BigInt::correct()
{
  int newSize = nDig;
  while ((newSize) > 1 && d[newSize - 1] == 0)
    --newSize;

  if (newSize != nDig)
  {
    BigInt prov(neg, newSize);
    for (int i = 0; i < newSize; ++i)
      prov.d[i] = d[i];
    *this = move(prov);
  }

  if (isZero())
    neg = false;
}
/// Construtor especifico a partir de string.
/// Nao eh conversor de string para BigInt.
/// Delega ao construtor default.
/// NAO PODE SER MODIFICADO NAS PARTES JAH IMPLEMENTADAS.
/// PODE (E PRECISA) RECEBER ACRESCIMOS, APENAS
/// NAS PARTES INDICADAS POR /* ACRESCENTAR */
BigInt::BigInt(const string &S)
    : BigInt() // Valor inicial zero
{
  // Se string vazia, emite erro e permanece com valor inicial zero
  if (S.empty())
  {
    cerr << "empty string cannot create a BigInt\n";
    return;
  }

  // Posicao onde comecam os digitos, inicialmente zero
  size_t ini = 0;
  // Leva em conta o sinal
  bool IsNeg = false;

  if (S[0] == '+' || S[0] == '-')
  {
    // Se nao tem nenhum digito alem do sinal, emite erro e permanece com valor inicial
    if (S.size() == 1)
    {
      cerr << "sign-only string cannot create a BigInt\n";
      return;
    }
    IsNeg = (S[0] == '-');
    ++ini;
  }

  // Faz ter sinal (IsNeg) e numero de digitos (tamanho da string - ini) corretos
  /* ACRESCENTAR */
  *this = BigInt(IsNeg, S.size() - ini);
  // Calculo dos digitos do BigInt
  for (int i = 0; i < size(); ++i)
  {
    char c = S[S.size() - 1 - i];
    if (!isdigit(c))
    {
      *this = BigInt(); // = 0
      cerr << "string with invalid character cannot create a BigInt\n";
      return;
    }
    d[i] = static_cast<int8_t>(c - '0');
  }
  // Corrige eventuais numeros fora da especificacao
  correct();
}
/// ******************
/// * FIM DA PARTE 2 *
/// ******************

/// Insercao (impressao)
/* ACRESCENTAR */
std::ostream &operator<<(std::ostream &O, const BigInt &B)
{
  if (B.neg)
    O << '-';

  for (int i = B.size() - 1; i >= 0; --i)
  {
    int digito = B[i];
    if (digito >= 0 && digito <= 9)
      O << digito;
    else
      O << '#';
  }

  return O;
}

/// Extracao (digitacao).
/// NAO PODE SER MODIFICADO NAS PARTES JAH IMPLEMENTADAS.
/// PODE (E PRECISA) RECEBER ACRESCIMOS, APENAS
/// NAS PARTES INDICADAS POR /* ACRESCENTAR */
std::istream &operator>>(istream &I, BigInt &B)
{
  // Valor inicial zero
  B = BigInt(); // = 0

  // Testa a stream de entrada e descarta eventuais separadores iniciais.
  // Em caso de erro, encerra a digitacao.
  istream::sentry s(I);
  if (!s)
    return I;

  // Caractere lido da stream
  int c;

  // Inspeciona o primeiro caractere que serah lido
  c = I.peek();

  // Testa se o primeiro caractere eh um sinal.
  // Se for, consome (elimina do buffer), processa e inspeciona o proximo caractere.
  if (c == '+' || c == '-')
  {
    // Consome
    c = I.get();
    // Atribui o sinal
    B.neg = (c == '-');
    // Obtem o proximo caractere
    c = I.peek();
  }

  // Numero de digitos que foram digitados
  int numDigitos = 0;

  // Testa se eh um caractere valido: digitos 0 a 9
  while (isdigit(c))
  {
    // Consome do buffer
    c = I.get();
    ++numDigitos;

    if (numDigitos > 1)
    {
      // Faz o BigInt manter o sinal e passar a ter size()+1 digitos,
      // avancando todos para uma posicao mais significativa aa frente.
      /* ACRESCENTAR */
      BigInt prov(B.isNeg(), B.size() + 1);
      for (int i = 0; i < B.size(); ++i)
        prov.d[i + 1] = B[i];

      B = move(prov);
    }

    // Acrescenta o novo digito como sendo o primeiro (o menos significativo)
    B.d[0] = static_cast<int8_t>(c - '0');

    // Inspeciona o proximo caractere que vai ser lido
    c = I.peek();
  }

  // Assinala erro na stream se nenhum digito foi lido
  if (numDigitos == 0)
    I.setstate(ios::failbit);

  // Corrige eventuais erros na digitacao
  B.correct();

  // Encerra a digitacao
  return I;
}

/// ******************
/// * FIM DA PARTE 3 *
/// ******************

/// Teste de igualdade
/* ACRESCENTAR */
bool BigInt::operator==(const BigInt &B) const
{
  if (neg == B.neg && B.nDig == nDig)
  {
    for (int i = 0; i < nDig; ++i)
      if (d[i] != B[i])
      {
        return false;
      }
    return true;
  }
  return false;
}

bool BigInt::operator!=(const BigInt &B) const
{
  return !(*this == B);
}
/// Menor que
/* ACRESCENTAR */
bool BigInt::operator<(const BigInt &B) const
{

  if (this->isNeg() != B.isNeg())
    return neg;

  if (!(this->isNeg()))
  {
    if (this->size() != B.size())
      return this->size() < B.size();

    for (int i = nDig - 1; i >= 0; --i)
    {
      if (d[i] != B[i])
        return d[i] < B[i];
    }
  }
  else
  {
    if (this->size() != B.size())
      return this->size() > B.size();

    for (int i = nDig - 1; i >= 0; --i)
    {
      if (d[i] != B[i])
        return (d[i] > B[i]);
    }
  }

  return false;
}

bool BigInt::operator>(const BigInt &B) const { return B < *this; }
bool BigInt::operator>=(const BigInt &B) const { return !(*this < B); }
bool BigInt::operator<=(const BigInt &B) const { return !(B < *this); }

/// ******************
/// * FIM DA PARTE 4 *
/// ******************

/// Funcao privada que incrementa os digitos (o modulo) do numero
/* ACRESCENTAR */
void BigInt::increment()
{
  int k = 0;
  while (k < nDig && d[k] == 9)
  {
    d[k] = 0;
    ++k;
  }

  if (k < size())
  {
    ++d[k];
  }
  else
  {
    BigInt prov(neg, nDig + 1);
    prov.d[nDig] = 1;
    *this = move(prov);
  }
}

/// Funcao privada que decrementa os digitos (o modulo) do numero
/* ACRESCENTAR */
void BigInt::decrement()
{

  if (isZero())
  {
    neg = true;
    d[0] = 1;
    return;
  }

  int k = 0;
  while (k < nDig && d[k] == 0)
  {
    d[k] = 9;
    ++k;
  }

  --d[k];

  if (d[nDig - 1] == 0)
  {
    correct();
  }
}
/// Operador de incremento pre-fixado
/// NAO PODE SER MODIFICADO
BigInt &BigInt::operator++()
{
  if (!isNeg())
    increment();
  else
    decrement();
  return *this;
}

/// Operador de decremento pre-fixado
/// NAO PODE SER MODIFICADO
BigInt &BigInt::operator--()
{
  if (isNeg())
    increment();
  else
    decrement();
  return *this;
}

/// Operador de incremento pos-fixado
/* ACRESCENTAR */
BigInt BigInt::operator++(int)
{
  BigInt prov = *this;
  ++(*this);
  return prov;
}
/// Operador de decremento pos-fixado
/* ACRESCENTAR */
BigInt BigInt::operator--(int)
{
  BigInt prov = *this;
  --(*this);
  return prov;
}
/// ******************
/// * FIM DA PARTE 5 *
/// ******************

/// Modulo (abs)
/* ACRESCENTAR */
BigInt abs(const BigInt &B)
{
  BigInt prov = B;
  prov.neg = false;
  return prov;
}

/// Negativo (unario)
/* ACRESCENTAR */
BigInt BigInt::operator-() const
{
  BigInt prov = *this;
  if (isZero())
  {
    return prov;
  }
  prov.neg = !(isNeg());
  return prov;
}

const BigInt &BigInt::operator+() const
{
  return *this;
}
/// Soma
/* ACRESCENTAR */
BigInt BigInt::operator+(const BigInt &B) const
{
  if (isNeg() == B.isNeg())
  {
    BigInt C(neg, 1 + max(size(), B.size()));
    int carry = 0;
    for (int i = 0; i < C.size(); ++i)
    {
      C.d[i] = (*this)[i] + B[i] + carry;
      if (C.d[i] > 9)
      {
        C.d[i] -= 10;
        carry = 1;
      }
      else
      {
        carry = 0;
      }
    }
    C.correct();
    return C;
  }
  else
  {
    if (abs(*this) >= abs(B))
    {
      BigInt C(neg, nDig);
      int borrow = 0;
      for (int i = 0; i < C.size(); ++i)
      {
        C.d[i] = (*this)[i] - B[i] - borrow;
        if (C.d[i] < 0)
        {
          C.d[i] += 10;
          borrow = 1;
        }
        else
        {
          borrow = 0;
        }
      }
      C.correct();
      return C;
    }
    return B + (*this);
  }
}

BigInt BigInt::operator-(const BigInt &B) const
{
  return *this + (-B);
}

/// ******************
/// * FIM DA PARTE 6 *
/// ******************

/// Multiplicacao
/* ACRESCENTAR */
BigInt BigInt::operator*(const BigInt &B) const
{
  if (this->isZero() || B.isZero())
    return BigInt(0);

  BigInt C((isNeg() != B.isNeg()), (size() + B.size()));

  for (int i = 0; i < size(); ++i)
  {
    if (d[i] != 0)
    {
      for (int j = 0; j < B.size(); ++j)
      {
        if (B.d[j] != 0)
        {
          int k = i + j;
          C.d[k] += (d[i] * B.d[j]);
          while (C.d[k] > 9)
          {
            int carry = C.d[k] / 10;
            C.d[k] %= 10;
            ++k;
            C.d[k] += carry;
          }
        }
      }
    }
  }
  C.correct();
  return C;
}

/// Fatorial
/* ACRESCENTAR */
BigInt BigInt::operator!() const
{
  if (this->isNeg())
  {
    cerr << "ERRO: FATORIAL NÃO PODE SER NEGATIVO\n";
    return 0;
  }

  BigInt C(1);
  for (BigInt N(2); N <= (*this); ++N)
  {
    C = C * N;
  }
  return C;
}
/// ******************
/// * FIM DA PARTE 7 *
/// ******************

/// Deslocamento aa esquerda
/* ACRESCENTAR */
BigInt BigInt::operator<<(int N) const
{
  if (N <= 0 || this->isZero())
    return *this;

  BigInt C(neg, size() + N);
  for (int i = N; i < C.size(); ++i)
    C.d[i] = d[i - N];

  return C;
}
/// Deslocamento ah direita
/* ACRESCENTAR */
BigInt BigInt::operator>>(int N) const
{
  if (N <= 0 || isZero())
    return *this;
  if (N >= this->size())
    return BigInt(0);

  BigInt C(neg, this->size() - N);
  for (int i = 0; i < C.size(); ++i)
    C.d[i] = d[i + N];

  return C;
}

/// Divisao de *this por D.
/// Armazena o resultado (quociente) em Q e o resto da divisao em R.
/* ACRESCENTAR */
void BigInt::division(const BigInt &D, BigInt &Q, BigInt &R) const
{
  Q = BigInt(0);
  if (isZero() || D.isZero())
  {
    if (D.isZero())
      cerr << "ERRO: NÃO PODE DIVIDIR POR 0 \n";

    R = BigInt(0);
    return;
  }

  BigInt absD = abs(D);

  if (abs(*this) < absD)
  {
    R = *this;
    return;
  }

  R = BigInt(0);

  for (int i = this->size() - 1; i >= 0; --i)
  {
    if (!(R.isZero()))
      R = R << 1;

    R.d[0] = d[i];
    int div = 0;

    while (R >= absD)
    {
      R = R - absD;
      ++div;
    }

    if (!(Q.isZero()))
      Q = Q << 1;

    Q.d[0] = div;
  }

  Q.neg = (isNeg() != D.isNeg());

  if (!(R.isZero()))
    R.neg = this->isNeg();
}

/// Quociente da divisao inteira
/* ACRESCENTAR */
BigInt BigInt::operator/(const BigInt &B) const
{
  BigInt Q, R;
  this->division(B, Q, R);
  return Q;
}

/// Resto da divisao inteira
/* ACRESCENTAR */
BigInt BigInt::operator%(const BigInt &B) const
{
  BigInt Q, R;
  this->division(B, Q, R);
  return R;
}
/// ******************
/// * FIM DA PARTE 8 *
/// ******************
