namespace math
{
        //  Function to return gcd of a and b
    template <typename t>
    constexpr t gcd(const t &a, const t &b)
    {
        if (a == 0)
            return b;
        return gcd(b % a, a);
    }

    template <typename t>
    // Function for extended Euclidean Algorithm
    constexpr t gcdExtended(const t &a, const t &b, t &x, t &y)
    {

        // Base Case
        if (a == 0)
        {
            x = 0, y = 1;
            return b;
        }

        // To store results of recursive call
        //variables have to be initialized in constant expressions
        t x1 = 0, y1 = 0;
        t gcd = gcdExtended(b % a, a, x1, y1);

        // Update x and y using results of recursive
        // call
        x = y1 - (b / a) * x1;
        y = x1;

        return gcd;
    }
} // namespace math
