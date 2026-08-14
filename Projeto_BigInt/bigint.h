#ifndef _BIGINT_H_
#define _BIGINT_H_

#include <iostream>
#include <string>
#include <cstdint>

class BigInt
{
private:
    bool neg;
    int nDig;
    int8_t *d;

    BigInt(bool ehNegativo, int tamanho);
    void correct();

    void increment();
    void decrement();

public:
    BigInt();
    ~BigInt();
    BigInt(const BigInt &B);
    BigInt(BigInt &&Temp) noexcept;

    explicit BigInt(const std::string &B);

    inline bool isNeg() const { return neg; }
    inline int size() const { return nDig; }
    inline bool isZero() const { return (nDig == 1 && d[0] == 0); }
    inline int operator[](int i) const { return (i >= 0 && i < nDig) ? d[i] : 0; }

    BigInt &operator=(const BigInt &B);
    BigInt &operator=(BigInt &&temp) noexcept;

    BigInt(long long int N);

    long long int toInt() const;

    friend std::ostream &operator<<(std::ostream &O, const BigInt &B);
    friend std::istream &operator>>(std::istream &I, BigInt &B);

    bool operator==(const BigInt &B) const;
    bool operator!=(const BigInt &B) const;
    bool operator<(const BigInt &B) const;
    bool operator>(const BigInt &B) const;
    bool operator>=(const BigInt &B) const;
    bool operator<=(const BigInt &B) const;

    BigInt &operator++();
    BigInt &operator--();
    BigInt operator++(int);
    BigInt operator--(int);

friend BigInt abs(const BigInt &B);

    BigInt operator-() const;
    const BigInt& operator+() const; 

    BigInt operator+(const BigInt &B) const;
    BigInt operator-(const BigInt &B) const;

    BigInt operator*(const BigInt &B) const;
    BigInt operator!() const;

    BigInt operator<<(int N) const;
    BigInt operator>>(int N) const;

    void division(const BigInt &B, BigInt &Q, BigInt &R) const;

    BigInt operator/(const BigInt &B) const;
    BigInt operator%(const BigInt &B) const;
};

#endif
