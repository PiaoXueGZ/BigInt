#ifndef _BIGINT_H_
#define _BIGINT_H_ 1

#include <iostream>
#include <deque>
#include <string>

//#define BitWidth ((__uint128_t)UINT64_MAX + 1)
#define BitWidth (__uint128_t)10000000000000000000ULL

struct UnsignedBigInt
{
    std::deque<unsigned long long> data{0};

    UnsignedBigInt() = default;
    UnsignedBigInt(__uint128_t a);
	UnsignedBigInt(std::string s);
    UnsignedBigInt &deleteZero();
};

struct BigInt
{
    UnsignedBigInt ubi = UnsignedBigInt(0);
    bool isNegative = false;

    BigInt() = default;
    BigInt(__int128_t a);
	BigInt(std::string s);
    BigInt(const UnsignedBigInt &ub, bool _isNegative);
};

inline UnsignedBigInt operator+(const UnsignedBigInt &bi1, const UnsignedBigInt &bi2);
inline UnsignedBigInt operator+=(UnsignedBigInt &bi1, const UnsignedBigInt &bi2);
inline UnsignedBigInt operator-(const UnsignedBigInt &bi1, const UnsignedBigInt &bi2);
inline UnsignedBigInt operator-=(UnsignedBigInt &bi1, const UnsignedBigInt &bi2);
inline UnsignedBigInt operator*(const UnsignedBigInt &bi1, const UnsignedBigInt &bi2);
inline UnsignedBigInt operator*=(UnsignedBigInt &bi1, const UnsignedBigInt &bi2);
inline UnsignedBigInt operator/(const UnsignedBigInt &bi1, const UnsignedBigInt &bi2);
inline UnsignedBigInt operator/=(UnsignedBigInt &bi1, const UnsignedBigInt &bi2);
inline UnsignedBigInt operator%(const UnsignedBigInt &bi1, const UnsignedBigInt &bi2);
inline UnsignedBigInt operator%=(UnsignedBigInt &bi1, const UnsignedBigInt &bi2);
std::ostream &operator<<(std::ostream &os, const UnsignedBigInt &bi);
std::istream &operator>>(std::istream &is, UnsignedBigInt &bi);
inline bool operator==(const UnsignedBigInt &bi1, const UnsignedBigInt &bi2);
inline bool operator!=(const UnsignedBigInt &bi1, const UnsignedBigInt &bi2);
inline bool operator>(const UnsignedBigInt &bi1, const UnsignedBigInt &bi2);
inline bool operator>=(const UnsignedBigInt &bi1, const UnsignedBigInt &bi2);
inline bool operator<(const UnsignedBigInt &bi1, const UnsignedBigInt &bi2);
inline bool operator<=(const UnsignedBigInt &bi1, const UnsignedBigInt &bi2);

BigInt operator+(const BigInt &b1, const BigInt &b2);
inline BigInt operator+=(BigInt &b1, const BigInt &b2);
BigInt operator-(const BigInt &b1, const BigInt &b2);
inline BigInt operator-=(BigInt &b1, const BigInt &b2);
inline BigInt operator-(BigInt b1);
inline BigInt operator*(const BigInt &b1, const BigInt &b2);
inline BigInt operator*=(BigInt &b1, const BigInt &b2);
inline BigInt operator/(const BigInt &b1, const BigInt &b2);
inline BigInt operator/=(BigInt &b1, const BigInt &b2);
inline BigInt operator%(const BigInt &b1, const BigInt &b2);
inline BigInt operator%=(BigInt &b1, const BigInt &b2);
std::ostream &operator<<(std::ostream &os, const BigInt &b);
std::istream &operator>>(std::istream &is, BigInt &b);
inline bool operator==(const BigInt &b1, const BigInt &b2);
inline bool operator!=(const BigInt &b1, const BigInt &b2);
inline bool operator>(const BigInt &b1, const BigInt &b2);
inline bool operator>=(const BigInt &b1, const BigInt &b2);
inline bool operator<(const BigInt &b1, const BigInt &b2);
inline bool operator<=(const BigInt &b1, const BigInt &b2);

#endif /*_BIGINT_H_*/