#include <iostream>

unsigned int sqr(unsigned int a)
{
    return a * a; // Return the square of a
}
unsigned int fun(unsigned int a, unsigned n)
{
    if (n == 0)
        return 1;
    return (n & 1)
               ? sqr(fun(a, n >> 1)) * a // If n is odd, multiply the square of fun(a, n >> 1) by a
               : sqr(fun(a, n >> 1));    // If n is even, just return the square
}

int main()
{
    unsigned int a, n;
    std::cin >> a >> n;
    std::cout << fun(a, n) << std::endl;
    return 0;
}