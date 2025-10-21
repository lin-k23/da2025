#include <stdio.h>

#define MOD 9973

typedef struct
{
    int m[2][2];
} Matrix;

// 矩阵乘法
Matrix mul(Matrix a, Matrix b)
{
    Matrix res;
    res.m[0][0] = (a.m[0][0] * b.m[0][0] + a.m[0][1] * b.m[1][0]) % MOD;
    res.m[0][1] = (a.m[0][0] * b.m[0][1] + a.m[0][1] * b.m[1][1]) % MOD;
    res.m[1][0] = (a.m[1][0] * b.m[0][0] + a.m[1][1] * b.m[1][0]) % MOD;
    res.m[1][1] = (a.m[1][0] * b.m[0][1] + a.m[1][1] * b.m[1][1]) % MOD;
    return res;
}

// 矩阵快速幂
Matrix pow(Matrix base, int n)
{
    Matrix res = {{{1, 0}, {0, 1}}};
    while (n)
    {
        if (n & 1)
            res = mul(res, base);
        base = mul(base, base);
        n >>= 1;
    }
    return res;
}

// 计算F(n) % MOD
int fib(int n)
{
    if (n == 0)
        return 0;
    Matrix base = {{{1, 1}, {1, 0}}};
    Matrix res = pow(base, n - 1);
    return res.m[0][0];
}

int main()
{
    int n, a;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i)
    {
        scanf("%d", &a);
        printf("%d\n", fib(a));
    }
    return 0;
}