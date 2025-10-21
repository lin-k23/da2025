#include <stdio.h>
#include <vector>
#include <stack>

int main()
{
    int M, N;
    scanf("%d", &M);
    scanf("%d", &N);

    double p;
    scanf("%lf", &p);

    std::vector<int> heights(N, 0);

    long long max_perimeter = 0;
    int best_istart = 0, best_iend = 0;
    int best_jstart = 0, best_jend = 0;

    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            double f_ij;
            scanf("%lf", &f_ij);
            if (f_ij > p)
            {
                heights[j]++;
            }
            else
            {
                heights[j] = 0;
            }
        }

        std::stack<int> s;
        for (int j = 0; j <= N; ++j)
        {
            int current_h = (j == N) ? 0 : heights[j];

            while (!s.empty() && heights[s.top()] > current_h)
            {
                int h = heights[s.top()];
                s.pop();

                int w = s.empty() ? j : j - s.top() - 1;

                long long current_perimeter = 2LL * (h + w);

                if (current_perimeter > max_perimeter)
                {
                    max_perimeter = current_perimeter;
                    best_iend = i;
                    best_istart = i - h + 1;
                    best_jstart = s.empty() ? 0 : s.top() + 1;
                    best_jend = j - 1;
                }
            }
            s.push(j);
        }
    }

    printf("%d %d\n", best_istart, best_iend);
    printf("%d %d\n", best_jstart, best_jend);
    printf("%lld\n", max_perimeter);

    return 0;
}