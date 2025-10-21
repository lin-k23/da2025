#include <stdio.h>

int main()
{
    int M;
    scanf("%d", &M);
    int single_number = 0;

    for (int i = 0; i < M; ++i)
    {
        int current_num;
        scanf("%d", &current_num);
        single_number ^= current_num;
    }
    printf("%d\n", single_number);

    return 0;
}