#include <cstdio>
#include <algorithm>
#include <cmath>
#include <climits>

using namespace std;

// ================= 静态参数配置 =================
const int MAX_DFS_STEPS = 40000000;

const int SA_ITERATIONS = 45000;
// ==============================================

struct React
{
    int req_p, res_p, req_t, res_t, dur;
};

struct Edge
{
    int to;
    int w;
};

int n;
React a[35];
int start_dist[35];
int end_dist[35];
int dist_mat[35][35];

Edge adj[35][35];
int adj_size[35];

int min_out[35];
long long min_out_sum_total = 0;
long long total_dur = 0;

const long long INF_LL = 9000000000000000000LL;
long long best_ans = INF_LL;

int dfs_counter = 0;
bool stop_search = false;

unsigned int seed = 1337;
inline int my_rand()
{
    seed = seed * 1103515245 + 12345;
    return (seed / 65536) % 32768;
}

inline int calc_dist(int p1, int t1, int p2, int t2)
{
    return max(abs(p1 - p2), abs(t1 - t2));
}

void run_sa()
{
    int path[35];
    for (int i = 0; i < n; i++)
        path[i] = i;

    for (int i = n - 1; i > 0; i--)
    {
        int idx = my_rand() % (i + 1);
        swap(path[i], path[idx]);
    }

    long long cur_cost = start_dist[path[0]];
    for (int i = 0; i < n - 1; i++)
        cur_cost += dist_mat[path[i]][path[i + 1]];
    cur_cost += end_dist[path[n - 1]];

    if (cur_cost < best_ans)
        best_ans = cur_cost;

    double t = 3000.0;
    double cool = 0.99;

    for (int iter = 0; iter < SA_ITERATIONS; iter++)
    {
        int u = my_rand() % n;
        int v = my_rand() % n;
        if (u == v)
            continue;

        long long old_c = cur_cost;

        swap(path[u], path[v]);

        long long new_c = start_dist[path[0]];
        for (int i = 0; i < n - 1; i++)
            new_c += dist_mat[path[i]][path[i + 1]];
        new_c += end_dist[path[n - 1]];

        if (new_c < cur_cost)
        {
            cur_cost = new_c;
            if (cur_cost < best_ans)
                best_ans = cur_cost;
        }
        else
        {
            double delta = (double)(cur_cost - new_c);
            if (exp(delta / t) * 32767.0 > (double)my_rand())
            {
                cur_cost = new_c;
            }
            else
            {
                swap(path[u], path[v]); // 不接受，还原
            }
        }
        t *= cool;
        if (t < 1e-4)
            t = 3000.0;
    }
}

void dfs(int u, unsigned int mask, int cnt, long long cur_cost, long long current_lb)
{
    dfs_counter++;
    if ((dfs_counter & 1023) == 0)
    {
        if (dfs_counter > MAX_DFS_STEPS)
        {
            stop_search = true;
            return;
        }
    }
    if (stop_search)
        return;

    if (current_lb >= best_ans)
        return;

    if (cnt == n)
    {
        long long total = cur_cost + end_dist[u];
        if (total < best_ans)
        {
            best_ans = total;
        }
        return;
    }

    for (int i = 0; i < adj_size[u]; i++)
    {
        int v = adj[u][i].to;
        int w = adj[u][i].w;

        if (!((mask >> v) & 1))
        {
            dfs(v, mask | (1u << v), cnt + 1,
                cur_cost + w,
                current_lb - min_out[u] + w);

            if (stop_search)
                return;
        }
    }
}

struct Node
{
    int id;
    int val;
};

bool cmp_starts(const Node &a, const Node &b)
{
    return a.val < b.val;
}

bool cmp_edges(const Edge &a, const Edge &b)
{
    return a.w + min_out[a.to] < b.w + min_out[b.to];
}

int main()
{
    if (scanf("%d", &n) != 1)
        return 0;
    for (int i = 0; i < n; i++)
    {
        scanf("%d %d %d %d %d", &a[i].req_p, &a[i].res_p, &a[i].req_t, &a[i].res_t, &a[i].dur);
        total_dur += a[i].dur;
    }

    for (int i = 0; i < n; i++)
    {
        start_dist[i] = calc_dist(7, 25, a[i].req_p, a[i].req_t);
        end_dist[i] = calc_dist(a[i].res_p, a[i].res_t, 7, 25);
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i == j)
                dist_mat[i][j] = 0;
            else
                dist_mat[i][j] = calc_dist(a[i].res_p, a[i].res_t, a[j].req_p, a[j].req_t);
        }
    }

    min_out_sum_total = 0;
    for (int i = 0; i < n; i++)
    {
        min_out[i] = end_dist[i];
        for (int j = 0; j < n; j++)
        {
            if (i == j)
                continue;
            min_out[i] = min(min_out[i], dist_mat[i][j]);
        }
        min_out_sum_total += min_out[i];
    }

    for (int i = 0; i < n; i++)
    {
        adj_size[i] = 0;
        for (int j = 0; j < n; j++)
        {
            if (i == j)
                continue;
            adj[i][adj_size[i]].to = j;
            adj[i][adj_size[i]].w = dist_mat[i][j];
            adj_size[i]++;
        }
        sort(adj[i], adj[i] + adj_size[i], cmp_edges);
    }

    for (int k = 0; k < 2; k++)
    {
        run_sa();
    }

    Node starts[35];
    for (int i = 0; i < n; i++)
    {
        starts[i].id = i;
        starts[i].val = start_dist[i] + min_out[i];
    }
    sort(starts, starts + n, cmp_starts);

    for (int i = 0; i < n; i++)
    {
        int s = starts[i].id;
        long long init_lb = start_dist[s] + min_out_sum_total;

        dfs(s, 1u << s, 1, start_dist[s], init_lb);

        if (stop_search)
            break;
    }

    printf("%lld\n", best_ans + total_dur);
    return 0;
}