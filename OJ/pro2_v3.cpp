#include <stdio.h>
#include <map>
#include <algorithm>
#include <climits>
#include <vector>

using namespace std;

long long max_path_sum;

int getMaxDownPath_map(int idx, map<int, short> &nodes, int k)
{
    if (idx >= k || nodes.find(idx) == nodes.end())
    {
        return 0;
    }
    int left_down_sum = max(0, getMaxDownPath_map(2 * idx + 1, nodes, k));
    int right_down_sum = max(0, getMaxDownPath_map(2 * idx + 2, nodes, k));
    int current_val = (int)nodes[idx];
    long long current_pivot_sum = (long long)current_val + left_down_sum + right_down_sum;
    max_path_sum = max(max_path_sum, current_pivot_sum);
    return current_val + max(left_down_sum, right_down_sum);
}

void solve_map(int k)
{
    map<int, short> nodes;
    bool all_empty = true;
    for (int i = 0; i < k; ++i)
    {
        int val;
        scanf("%d", &val);
        if (val != -99)
        {
            nodes[i] = (short)val;
            all_empty = false;
        }
    }

    if (all_empty)
    {
        printf("0\n");
        return;
    }

    max_path_sum = LLONG_MIN;
    getMaxDownPath_map(0, nodes, k);
    printf("%lld\n", max_path_sum);
}

int getMaxDownPath_array(int idx, short *nodes, int k)
{
    if (idx >= k || nodes[idx] == -99)
    {
        return 0;
    }
    int left_down_sum = max(0, getMaxDownPath_array(2 * idx + 1, nodes, k));
    int right_down_sum = max(0, getMaxDownPath_array(2 * idx + 2, nodes, k));
    int current_val = (int)nodes[idx];
    long long current_pivot_sum = (long long)current_val + left_down_sum + right_down_sum;
    max_path_sum = max(max_path_sum, current_pivot_sum);
    return current_val + max(left_down_sum, right_down_sum);
}

void solve_array(int k)
{
    short *nodes = new short[k];
    bool all_empty = true;
    for (int i = 0; i < k; ++i)
    {
        int val;
        scanf("%d", &val);
        nodes[i] = (short)val;
        if (nodes[i] != -99)
        {
            all_empty = false;
        }
    }

    if (all_empty)
    {
        printf("0\n");
        delete[] nodes;
        return;
    }

    max_path_sum = LLONG_MIN;
    getMaxDownPath_array(0, nodes, k);
    printf("%lld\n", max_path_sum);

    delete[] nodes;
}

int main()
{
    int h;
    scanf("%d", &h);

    int k = (1 << h) - 1;

    if (k <= 0)
    {
        printf("0\n");
        return 0;
    }

    int MEMORY_THRESHOLD_H = 21;

    if (h <= MEMORY_THRESHOLD_H)
    {
        solve_array(k);
    }
    else
    {
        solve_map(k);
    }

    return 0;
}