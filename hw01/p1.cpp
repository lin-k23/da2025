#include <iostream>
#include <time.h>
using namespace std;

int sum(int n)
{
    int s = 0;
    for (int i = 1; i <= n; i++)
    {
        int p = 1;
        for (int j = 1; j <= i; j++)
            p *= j; // p = i!
        s += p;     // s = 1! + 2! + ... + n!
    }
    return s;
}

int fac(int n)
{
    int p = 1, s = 0;
    for (int i = 1; i <= n; i++)
    {
        p *= i; // p = i!
        s += p; // s+= i!
    }
    return s; // s = 1! + 2! + ... + n!
}

// time complexity: O(n^2) vs O(n)
int main()
{
    int n = 10;     // 不会溢出的 n
    int loop = 1e7; // 循环次数
    cout << "测试 n = " << n << "，循环 " << loop << " 次，比较两个算法的速度。" << endl;

    clock_t start_sum = clock();
    int result_sum = 0;
    for (int i = 0; i < loop; ++i)
        result_sum = sum(n);
    clock_t end_sum = clock();
    double duration_sum = double(end_sum - start_sum) / CLOCKS_PER_SEC;
    cout << "sum(n) = " << result_sum << endl;
    cout << "sum(n) 总用时: " << duration_sum << " 秒" << endl;

    clock_t start_fac = clock();
    int result_fac = 0;
    for (int i = 0; i < loop; ++i)
        result_fac = fac(n);
    clock_t end_fac = clock();
    double duration_fac = double(end_fac - start_fac) / CLOCKS_PER_SEC;
    cout << "fac(n) = " << result_fac << endl;
    cout << "fac(n) 总用时: " << duration_fac << " 秒" << endl;

    return 0;
}