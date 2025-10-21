#include <iostream>
#include <algorithm>

// 全局数组
int a[] = {5, 3, 8, 1, 2};

void swap(int &x, int &y)
{
    int temp = x;
    x = y;
    y = temp;
}

void sort(int j, int n)
{
    int i, temp;
    if (j < n)
    {
        for (i = j; i <= n; ++i)
            if (a[i] < a[j])
                swap(a[i], a[j]);
        ++j;
        sort(j, n);
    }
}

// test for sort
int main()
{
    int n = sizeof(a) / sizeof(a[0]);
    sort(0, n - 1);
    for (int i = 0; i < n; ++i)
        std::cout << a[i] << " ";
    return 0;
}
