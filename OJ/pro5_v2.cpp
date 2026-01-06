#include <cstdio>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <cstdint>

using namespace std;

const int MEMO_LIMIT = 300000;

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
React a[26];
int start_dist[26], end_dist[26];
vector<Edge> adj[26];
int min_out[26];

long long total_dur = 0;
long long best_ans = -1;

unordered_map<uint64_t, long long> memo;

void dfs(int u, int mask, int cnt, long long cur_cost)
{
    if (best_ans != -1 && cur_cost >= best_ans)
        return;

    long long lb = 0;
    for (int i = 0; i < n; i++)
        if (!(mask & (1 << i)))
            lb += min_out[i];

    if (best_ans != -1 && cur_cost + lb >= best_ans)
        return;

    // 分类
    if (cnt <= 9 && (int)memo.size() < MEMO_LIMIT)
    {
        uint64_t key = (uint64_t(mask) << 5) | u;
        auto it = memo.find(key);
        if (it != memo.end() && it->second <= cur_cost)
            return;
        memo[key] = cur_cost;
    }
    if (cnt == n)
    {
        long long total = cur_cost + end_dist[u];
        if (best_ans == -1 || total < best_ans)
            best_ans = total;
        return;
    }

    vector<Edge> cand;
    for (auto &e : adj[u])
    {
        if (!(mask & (1 << e.to)))
            cand.push_back(e);
    }

    sort(cand.begin(), cand.end(), [&](const Edge &a, const Edge &b)
         { return a.w + min_out[a.to] < b.w + min_out[b.to]; });

    for (auto &e : cand)
    {
        dfs(e.to, mask | (1 << e.to), cnt + 1, cur_cost + e.w);
    }
}

int main()
{
    if (scanf("%d", &n) != 1)
        return 0;

    for (int i = 0; i < n; i++)
    {
        scanf("%d %d %d %d %d",
              &a[i].req_p, &a[i].res_p,
              &a[i].req_t, &a[i].res_t,
              &a[i].dur);
        total_dur += a[i].dur;
    }

    for (int i = 0; i < n; i++)
    {
        start_dist[i] = max(abs(7 - a[i].req_p), abs(25 - a[i].req_t));
        end_dist[i] = max(abs(a[i].res_p - 7), abs(a[i].res_t - 25));
    }

    for (int i = 0; i < n; i++)
    {
        adj[i].clear();
        for (int j = 0; j < n; j++)
        {
            if (i == j)
                continue;
            int d = max(abs(a[i].res_p - a[j].req_p),
                        abs(a[i].res_t - a[j].req_t));
            adj[i].push_back({j, d});
        }
        sort(adj[i].begin(), adj[i].end(),
             [](const Edge &x, const Edge &y)
             {
                 return x.w < y.w;
             });
    }

    for (int i = 0; i < n; i++)
    {
        min_out[i] = end_dist[i];
        for (auto &e : adj[i])
            min_out[i] = min(min_out[i], e.w);
    }

    {
        int cur_p = 7, cur_t = 25;
        vector<bool> used(n, false);
        long long greedy = 0;
        int last = -1;

        for (int step = 0; step < n; step++)
        {
            int sel = -1, best = 1e9;
            for (int i = 0; i < n; i++)
            {
                if (!used[i])
                {
                    int d = max(abs(cur_p - a[i].req_p),
                                abs(cur_t - a[i].req_t));
                    if (d < best)
                    {
                        best = d;
                        sel = i;
                    }
                }
            }
            greedy += best;
            used[sel] = true;
            cur_p = a[sel].res_p;
            cur_t = a[sel].res_t;
            last = sel;
        }
        greedy += max(abs(cur_p - 7), abs(cur_t - 25));
        best_ans = greedy;
    }

    vector<pair<int, int>> starts;
    for (int i = 0; i < n; i++)
        starts.push_back({start_dist[i], i});

    sort(starts.begin(), starts.end());

    for (auto &p : starts)
    {
        int s = p.second;
        dfs(s, 1 << s, 1, start_dist[s]);
    }

    printf("%lld\n", best_ans + total_dur);
    return 0;
}
