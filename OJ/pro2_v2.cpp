#include <stdio.h>

using namespace std;

inline int max(int a, int b)
{
    return (a > b) ? a : b;
}

int main()
{
    int h;
    scanf("%d", &h);

    int num_nodes = (1 << h) - 1;

    int *bt_weights = new int[num_nodes];

    for (int k = 0; k < num_nodes; ++k)
    {
        scanf("%d", &bt_weights[k]);
    }

    int *max_path_sums = new int[h];
    for (int k = 0; k < h; ++k)
    {
        max_path_sums[k] = -1000;
    }

    for (int level = h - 1; level >= 0; --level)
    {
        for (int i = (1 << level) - 1; i < (1 << (level + 1)) - 1; ++i)
        {
            if (bt_weights[i] == -99)
            {
                continue;
            }
            if (level == h - 1)
            {
                max_path_sums[level] = max(max_path_sums[level], bt_weights[i]);
            }
            else
            {
                int left_child_index = 2 * i + 1;
                int right_child_index = 2 * i + 2;

                if (left_child_index >= num_nodes || right_child_index >= num_nodes)
                {
                    max_path_sums[level] = max(max_path_sums[level], bt_weights[i]);
                    continue;
                }

                int left_child_val = bt_weights[left_child_index];
                int right_child_val = bt_weights[right_child_index];

                int left_child_sum = (left_child_val > 0 && left_child_val != -99)
                                         ? left_child_val
                                         : 0;
                int right_child_sum = (right_child_val > 0 && right_child_val != -99)
                                          ? right_child_val
                                          : 0;

                int current_sum = bt_weights[i] + left_child_sum + right_child_sum;
                max_path_sums[level] = max(max_path_sums[level], current_sum);

                int left_path_for_parent = (bt_weights[left_child_index] > 0 && bt_weights[left_child_index] != -99)
                                               ? bt_weights[left_child_index]
                                               : 0;
                int right_path_for_parent = (bt_weights[right_child_index] > 0 && bt_weights[right_child_index] != -99)
                                                ? bt_weights[right_child_index]
                                                : 0;

                bt_weights[i] = bt_weights[i] + max(left_path_for_parent, right_path_for_parent);
            }
        }
    }

    int max_sum = -1000;
    for (int k = 0; k < h; ++k)
    {
        max_sum = max(max_sum, max_path_sums[k]);
    }
    printf("%d\n", max_sum);

    delete[] bt_weights;
    delete[] max_path_sums;

    return 0;
}
