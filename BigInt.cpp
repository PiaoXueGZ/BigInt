#include <iostream>
#include <deque>
#include <string>
#include "BigInt.h"

UnsignedBigInt::UnsignedBigInt(__uint128_t a)
{
    if(a == 0)  //特判等于0的情况防止进不了循环
        return;
    
    data.clear();
    while (a != 0)
    {
        data.push_front(a % BitWidth);
        a /= BitWidth;
    }
}

UnsignedBigInt::UnsignedBigInt(std::string s)
{
	UnsignedBigInt bit(1);
	*this = UnsignedBigInt(0);

	for (auto rbeg = s.rbegin(), rend = s.rend(); rbeg != rend; rbeg++)
	{
		*this += (*rbeg - '0') * bit;
		bit *= UnsignedBigInt(10);
	}
	
	this->deleteZero();
}

UnsignedBigInt &UnsignedBigInt::deleteZero()
{
    while (data.front() == 0 && data.size() > 1)
    {
        data.pop_front();
    }
    return *this;
}

UnsignedBigInt unsignedBigIntAdd(const UnsignedBigInt &bi1, const UnsignedBigInt &bi2)
{
    UnsignedBigInt ans;
    ans.data.clear();   //清空

    auto rbeg1 = bi1.data.rbegin(), rend1 = bi1.data.rend(), rbeg2 = bi2.data.rbegin(), rend2 = bi2.data.rend();
    register __uint128_t numToAddForward = 0, temp;
    while (rbeg1 != rend1 || rbeg2 != rend2)
    {
        temp = ((__uint128_t)(rbeg1 != rend1 ? *(rbeg1++) : 0) + (__uint128_t)(rbeg2 != rend2 ? *(rbeg2++) : 0) + numToAddForward);
        ans.data.push_front(temp % BitWidth);
        numToAddForward = temp / BitWidth;
    }
    if(numToAddForward != 0)
    {
        ans.data.push_front(numToAddForward);
    }

    return ans;
}

UnsignedBigInt unsignedBigIntMinus(const UnsignedBigInt &bi1, const UnsignedBigInt &bi2)
{
	if (bi1 <= bi2) 
	{
		return UnsignedBigInt(0);
	}

	UnsignedBigInt ans;
	ans.data.clear();

	auto rbeg1 = bi1.data.rbegin(), rend1 = bi1.data.rend(), rbeg2 = bi2.data.rbegin(), rend2 = bi2.data.rend();
	register __int128_t numToMinusForward = 0, temp;
	while (rbeg1 != rend1 || rbeg2 != rend2)
	{
		temp = ((__uint128_t)(rbeg1 != rend1 ? *(rbeg1++) : 0) - (__uint128_t)(rbeg2 != rend2 ? *(rbeg2++) : 0) - numToMinusForward);
		if (temp < 0)
		{
			temp += BitWidth;
			numToMinusForward = 1;
		}
		else
		{
			numToMinusForward = 0;
		}
		ans.data.push_front(temp);
	}

    ans.deleteZero();
	return ans;
}

UnsignedBigInt oneBitUnsignedBigIntMultiply(unsigned long long a, const UnsignedBigInt &bi, unsigned int bitToMoveFront)
{
    UnsignedBigInt ans;
    ans.data.clear();
    register __uint128_t numToAddForward = 0, temp;

    for(auto rbeg = bi.data.rbegin(), rend = bi.data.rend(); rbeg != rend; rbeg++)
    {
        temp = ((__uint128_t)a * (__uint128_t)*rbeg + numToAddForward);
        ans.data.push_front(temp % BitWidth);
        numToAddForward = temp / BitWidth;
    }

    if(numToAddForward != 0)
    {
        ans.data.push_front(numToAddForward);
    }

    while (bitToMoveFront-- > 0)    //补位
        ans.data.push_back(0);

    ans.deleteZero();
    return ans;
}

UnsignedBigInt unsignedBigIntMultiply(const UnsignedBigInt &bi1, const UnsignedBigInt &bi2)
{
    UnsignedBigInt ans(0);
    auto len1 = bi1.data.size(), len2 = bi2.data.size();

    if(len1 < len2)
    {
        for(int i = len1 - 1; i >= 0; i--)
        {
            ans = unsignedBigIntAdd(ans, oneBitUnsignedBigIntMultiply(bi1.data[i], bi2, len1 - i - 1));
        }
    }
    else
    {
        for(int i = len2 - 1; i >= 0; i--)
        {
            ans = unsignedBigIntAdd(ans, oneBitUnsignedBigIntMultiply(bi2.data[i], bi1, len2 - i - 1));
        }
    }
    
    ans.deleteZero();
    return ans;
}

unsigned long long searchMaxNum(UnsignedBigInt bi1, const UnsignedBigInt &numToMultiplay, unsigned int bitToMoveForward)
{
    __uint128_t down = 0, up = BitWidth;
    UnsignedBigInt tempBigInt;
    auto rbeg = bi1.data.rbegin();
    for(unsigned int i = 0; i < bitToMoveForward; i++)   //将其后面几位弄成0
    {
        *(rbeg++) = 0;
    }

    while (up - down > 1)   //二分查找
    {
        tempBigInt = oneBitUnsignedBigIntMultiply((up + down) / 2, numToMultiplay, bitToMoveForward);
        if(tempBigInt == bi1)   //等于
        {
            return (up + down) / 2;
        }
        else if(tempBigInt < bi1)   //小于
        {
            down = (up + down) / 2;
        }
        else    //大于
        {
            up = (up + down) / 2;
        }
    }

    return down;
}

UnsignedBigInt unsignedBigIntDivide(UnsignedBigInt bi1, const UnsignedBigInt &bi2)
{
	if (bi1 < bi2)
	{
		return UnsignedBigInt(0);
	}

    UnsignedBigInt ans;
    ans.data.clear();
    auto len1 = bi1.data.size(), len2 = bi2.data.size();
    unsigned long long temp;
    for(size_t i = 0; i <= len1 - len2; i++)
    {
        temp  = searchMaxNum(bi1, bi2, len1 - len2 - i);
        ans.data.push_back(temp);
        bi1 -= oneBitUnsignedBigIntMultiply(temp, bi2, len1 - len2 - i);
    }

    ans.deleteZero();
    return ans;
}

UnsignedBigInt unsignedBigIntMod(UnsignedBigInt bi1, const UnsignedBigInt &bi2)
{
    if (bi1 < bi2)
	{
		return bi1;
	}

    auto len1 = bi1.data.size(), len2 = bi2.data.size();
    for(size_t i = 0; i <= len1 - len2; i++)
    {
        bi1 -= oneBitUnsignedBigIntMultiply(searchMaxNum(bi1, bi2, len1 - len2 - i), bi2, len1 - len2 - i);
    }

    return bi1;
}

inline UnsignedBigInt operator+(const UnsignedBigInt &bi1, const UnsignedBigInt &bi2)
{
    return unsignedBigIntAdd(bi1, bi2);
}

inline UnsignedBigInt operator+=(UnsignedBigInt &bi1, const UnsignedBigInt &bi2)
{
    return (bi1 = bi1 + bi2);
}

inline UnsignedBigInt operator-(const UnsignedBigInt &bi1, const UnsignedBigInt &bi2)
{
	return unsignedBigIntMinus(bi1, bi2);
}

inline UnsignedBigInt operator-=(UnsignedBigInt &bi1, const UnsignedBigInt &bi2)
{
	return (bi1 = bi1 - bi2);
}

inline UnsignedBigInt operator*(const UnsignedBigInt &bi1, const UnsignedBigInt &bi2)
{
    return unsignedBigIntMultiply(bi1, bi2);
}

inline UnsignedBigInt operator*=(UnsignedBigInt &bi1, const UnsignedBigInt &bi2)
{
    return (bi1 = bi1 * bi2);
}

inline UnsignedBigInt operator/(const UnsignedBigInt &bi1, const UnsignedBigInt &bi2)
{
    return unsignedBigIntDivide(bi1, bi2);
}

inline UnsignedBigInt operator/=(UnsignedBigInt &bi1, const UnsignedBigInt &bi2)
{
    return (bi1 = bi1 / bi2);
}

inline UnsignedBigInt operator%(const UnsignedBigInt &bi1, const UnsignedBigInt &bi2)
{
    return unsignedBigIntMod(bi1, bi2);
}

inline UnsignedBigInt operator%=(UnsignedBigInt &bi1, const UnsignedBigInt &bi2)
{
    return (bi1 = bi1 % bi2);
}

inline bool operator==(const UnsignedBigInt &bi1, const UnsignedBigInt &bi2)
{
	return (bi1.data == bi2.data);
}

inline bool operator!=(const UnsignedBigInt &bi1, const UnsignedBigInt &bi2)
{
    return !(bi1 == bi2);
}

inline bool operator>(const UnsignedBigInt &bi1, const UnsignedBigInt &bi2)
{
	if (bi1.data.size() > bi2.data.size())
	{
		return true;
	}
    else if(bi1.data.size() < bi2.data.size())
    {
        return false;
    }
	
	for (auto beg1 = bi1.data.begin(), beg2 = bi2.data.begin(), end1 = bi1.data.end(); beg1 != end1; beg1++, beg2++)
	{
		if (*beg1 > *beg2)
		{
			return true;
		}
		else if(*beg1 < *beg2)
		{
			return false;
		}
	}

	return false;	//能到这里说明两个数相等
}

inline bool operator>=(const UnsignedBigInt &bi1, const UnsignedBigInt &bi2)
{
	return ((bi1 == bi2) || (bi1 > bi2));
}

inline bool operator<(const UnsignedBigInt &bi1, const UnsignedBigInt &bi2)
{
	return !(bi1 >= bi2);
}

inline bool operator<=(const UnsignedBigInt &bi1, const UnsignedBigInt &bi2)
{
	return !(bi1 > bi2);
}

std::ostream &operator<<(std::ostream &os, const UnsignedBigInt &bi)
{
    if(bi == UnsignedBigInt(0)) //特判0
    {
        os << "0";
        return os;
    }

    char s[(size_t)(bi.data.size() * (double)19.2659197224948 + 1) + 1] = {0};
    register char *index = s + (size_t)(bi.data.size() * (double)19.2659197224948 + 1) - 1;
    register unsigned long long temp;
    for(auto beg = bi.data.rbegin(), end = bi.data.rend() - 1; beg != end; beg++)
    {
        temp = *beg;
        for(register int i = 0; i < 19; i++)
        {
            *(index--) = temp % 10 + '0';
            temp /= 10;
        }
    }

    temp = bi.data.front();
    while (temp != 0)
    {
        *(index--) = temp % 10 + '0';
        temp /= 10;
    }

    os << index + 1;

    return os;
}

std::istream &operator>>(std::istream &is, UnsignedBigInt &bi)
{
    std::string s;
    is >> s;
    bi = UnsignedBigInt(s);
    return is;
}


/**********************
 * 
 * 
 * 
下面是基于UnsignedBigInt
实现的有符号的BigInt
 *
 *
 *
 *
**********************/

BigInt::BigInt(__int128_t a)
{
    if(a < 0)
    {
        isNegative = true;
        a = -a;
    }
    ubi = UnsignedBigInt(a);
}

BigInt::BigInt(std::string s)
{
    if(s[0] == '-')
    {
        isNegative = true;
        ubi = UnsignedBigInt(s.substr(1));
    }
    else
    {
        ubi = UnsignedBigInt(s);
    }
}

BigInt::BigInt(const UnsignedBigInt &ub, bool _isNegative)
{
    ubi = ub;
    isNegative = _isNegative;
}

BigInt operator+(const BigInt &b1, const BigInt &b2)
{
    if((!b1.isNegative && !b2.isNegative) || (b1.isNegative && b2.isNegative))    //都为正或都为负
    {
        return BigInt(b1.ubi + b2.ubi, b1.isNegative);
    }
    else if(b1.isNegative && !b2.isNegative)    //b1负b2正
    {
        return b2 - (-b1);
    }
    else    //b1正b2负
    {
        return b1 - (-b2);
    }
}

inline BigInt operator+=(BigInt &b1, const BigInt &b2)
{
    return (b1 = b1 + b2);
}

BigInt operator-(const BigInt &b1, const BigInt &b2)
{
    if(!b1.isNegative && !b2.isNegative)    //都为正
    {
        if(b1.ubi < b2.ubi)
        {
            return -(b2 - b1);
        }
        else
        {
            return BigInt(b1.ubi - b2.ubi, false);
        }
    }
    else if(b1.isNegative && b2.isNegative) //都为负
    {
        return (-b2) - (-b1);
    }
    else if(!b1.isNegative && b2.isNegative)    //b1正b2负
    {
        return b1 + (-b2);
    }
    else    //b1负b2正
    {
        return b1 + (-b2);
    }
}

inline BigInt operator-=(BigInt &b1, const BigInt &b2)
{
    return (b1 = b1 - b2);
}

inline BigInt operator-(BigInt b1)
{
    b1.isNegative = !b1.isNegative;
    return b1;
}

inline BigInt operator*(const BigInt &b1, const BigInt &b2)
{
    return BigInt(b1.ubi * b2.ubi, (int)b1.isNegative ^ (int)b2.isNegative);
}

inline BigInt operator*=(BigInt &b1, const BigInt &b2)
{
    return (b1 = b1 * b2);
}

inline BigInt operator/(const BigInt &b1, const BigInt &b2)
{
    return BigInt(b1.ubi / b2.ubi, (int)b1.isNegative ^ (int)b2.isNegative);
}

inline BigInt operator/=(BigInt &b1, const BigInt &b2)
{
    return (b1 = b1 / b2);
}

inline BigInt operator%(const BigInt &b1, const BigInt &b2)
{
    return BigInt(b1.ubi % b2.ubi, b1.isNegative);
}

inline BigInt operator%=(BigInt &b1, const BigInt &b2)
{
    return (b1 = b1 % b2);
}

std::ostream &operator<<(std::ostream &os, const BigInt &b)
{
    if(b.ubi == UnsignedBigInt(0))  //特判0
    {
        return (os << "0");
    }

    return (os << (b.isNegative ? "-" : "") << b.ubi);
}

std::istream &operator>>(std::istream &is, BigInt &b)
{
    std::string s;
    is >> s;
    b = BigInt(s);
    return is;
}

inline bool operator==(const BigInt &b1, const BigInt &b2)
{
    return (b1.ubi == b2.ubi && (b1.isNegative == b2.isNegative || b1.ubi == UnsignedBigInt(0)));
}

inline bool operator!=(const BigInt &b1, const BigInt &b2)
{
    return !(b1 == b2);
}

inline bool operator>(const BigInt &b1, const BigInt &b2)
{
    if(b1.ubi == UnsignedBigInt(0) && b2.ubi == UnsignedBigInt(0))
        return false;
    
    if(b1.ubi == UnsignedBigInt(0))
        return b2.isNegative;
    if(b2.ubi == UnsignedBigInt(0))
        return !b1.isNegative;

    if(!b1.isNegative && b2.isNegative)   //b1正而b2负
    {
        return true;
    }
    else if(b1.isNegative && !b2.isNegative)    //b1负而b2正
    {
        return false;
    }
    else if(!b1.isNegative && !b2.isNegative)   //都正
    {
        return b1.ubi > b2.ubi;
    }
    else    //都负
    {
        return b1.ubi < b2.ubi;
    }
}

inline bool operator>=(const BigInt &b1, const BigInt &b2)
{
    return (b1 > b2 || b1 == b2);
}

inline bool operator<(const BigInt &b1, const BigInt &b2)
{
    return !(b1 >= b2);
}

inline bool operator<=(const BigInt &b1, const BigInt &b2)
{
    return !(b1 > b2);
}