#include <stdio.h>
#include <string.h>

// 实现大整数加法
void bigAdd(const char *a, const char *b, char *result)
{
    int lenA = strlen(a), lenB = strlen(b);
    int i = lenA - 1, j = lenB - 1, k = 0, carry = 0;
    char temp[10010]; // 假设输入不超过10000位
    while (i >= 0 || j >= 0 || carry)
    {
        int x = i >= 0 ? a[i] - '0' : 0;
        int y = j >= 0 ? b[j] - '0' : 0;
        int sum = x + y + carry;
        temp[k++] = (sum % 10) + '0';
        carry = sum / 10;
        i--;
        j--;
    }
    // 逆序输出
    for (int t = 0; t < k; t++)
        result[t] = temp[k - t - 1];
    result[k] = '\0';
}

int main()
{
    char a[10010], b[10010], result[10020];
    scanf("%s %s", a, b);
    bigAdd(a, b, result);
    printf("%s\n", result);
    return 0;
}