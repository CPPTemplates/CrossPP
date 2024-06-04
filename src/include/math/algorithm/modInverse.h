#include "math/algorithm/gcd.h"
namespace math
{
    // https://www.geeksforgeeks.org/multiplicative-inverse-under-modulo-m/
    // To compute x raised to power y under modulo M
    // To compute x^y under modulo m
    template <typename t>
    constexpr t power(const t &x, const t &y, const t &M)
    {
        if (y == 0)
            return 1;

        t p = power(x, y / 2, M) % M;
        p = (p * p) % M;

        return (y % 2 == 0) ? p : (x * p) % M;
    }

    // when M = maxvalue + 1 (aka 0)
    template <typename t>
    constexpr t powerM0(const t &x, const t &y)
    {
        if (y == 0)
            return 1;

        t p = powerM0(x, y / 2);
        p = (p * p);

        return (y % 2 == 0) ? p : (x * p);
    }

    // Function to find modular inverse of a under modulo M
    template <typename t>
    constexpr t modInverse(const t &A, const t &M)
    {
        // variables have to be initialized in constant expressions
        t x = 0, y = 0;
        const t &g = gcdExtended(A, M, x, y);
        if (g != 1)
            return 0;
        // cout << "Inverse doesn't exist";
        else
        {

            // m is added to handle negative x
            t res = (x % M + M) % M;
            return res;
            // cout << "Modular multiplicative inverse is " << res;
        }
        // int g = gcd(A, M);
        // if (g != 1)
        //     return 3;
        //// cout << "Inverse doesn't exist";
        // else
        //{
        //     // If a and m are relatively prime, then modulo
        //     // inverse is a^(m-2) mode m
        //     // cout << "Modular multiplicative inverse is "
        //     return M ? power(A, M - 2, M) : powerM0(A, M - 2);
        //     // return power(A, M - 2, M);
        // }
    }

    // https://www.geeksforgeeks.org/multiplicative-inverse-under-modulo-m/
    //  Returns modulo inverse of a with respect
    //  to m using extended Euclid Algorithm
    //  Assumption: a and m are coprimes, i.e.,
    //  gcd(A, M) = 1
    template <typename t>
    constexpr t modInverseIterative(t A, t M)
    {

        // copy
        const t m0 = M;
        t y = 0, x = 1;

        if (M == 1)
            return 0;

        while (A > 1)
        {
            // q is quotient
            const t &q = A / M;
            t temp = M;

            // m is remainder now, process same as
            // Euclid's algo
            M = A % M;
            A = temp;
            temp = y;

            // Update y and x
            y = x - q * y;
            x = temp;
        }

        // Make x positive
        if (x < 0)
            x += m0;

        return x;
    }

    constexpr int modInverseTest = modInverseIterative(11, 16);
    constexpr int testResult = (((3 * 11) % 16) * modInverseTest) % 16;
}

//constexpr unsigned int test = math::modInverseIterative((unsigned int)0b10000101110111001111001010110111, (unsigned int)0);
//constexpr unsigned int test2 = 1 * 0b10000101110111001111001010110111 * test;
//constexpr unsigned int test = math::modInverseIterative((unsigned int)5, (unsigned int)8);