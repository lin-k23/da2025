#include <cstdio>
#include <cmath>

void f1(int n)
{
    int s = 0, i = 0;
    while (s <= n)
    {
        ++i;         // i = 1, 2, 3, ...
        s += i;      // s = 1, 1+2, 1+2+3, ...
        printf("*"); // num(*) = floor((sqrt(8n+1) - 1) / 2) + 1
    }
}

void f2(int n)
{
    int i, j, k, count = 0;
    for (i = n / 2; i <= n; i++)        // O(n)
        for (j = 1; j < n; j *= 2)      // O(log n)
            for (k = 1; k <= n; k *= 3) // O(log n)
                count++;
}

int main()
{
    // // test f1
    // int n = 0;
    // while (n++ < 20)
    // {
    //     printf("n=%d:  ", n);
    //     f1(n);
    //     printf("\t%f", floor((sqrt(8 * n + 1) - 1) / 2) + 1);
    //     printf("\n");
    // }

    // test f2
    int m = 100;
    f2(m);
}