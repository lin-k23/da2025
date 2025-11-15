#include <cstdio>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

using WeightType = int;
const WeightType WT_INF = 1000000000; // sufficiently large
using TotalPriceType = long long;
const TotalPriceType TOTAL_INF = (TotalPriceType)9e18;

const int SMALL_M_LIMIT = 500;

struct Edge
{
    int to;
    WeightType w;
};

// 普通 Dijkstra（无 limit）
void dijkstra_normal(int start, int M, const vector<vector<Edge>> &g, vector<WeightType> &dist)
{
    dist.assign(M, WT_INF);
    using P = pair<WeightType, int>;
    priority_queue<P, vector<P>, greater<P>> pq;
    dist[start] = 0;
    pq.push({0, start});
    while (!pq.empty())
    {
        auto cur = pq.top();
        pq.pop();
        WeightType d = cur.first;
        int u = cur.second;
        if (d != dist[u])
            continue;
        for (const Edge &e : g[u])
        {
            int v = e.to;
            WeightType nd = d + e.w;
            if (nd < dist[v])
            {
                dist[v] = nd;
                pq.push({nd, v});
            }
        }
    }
}

// Dijkstra 带 limit 剪枝：当弹出节点距离 >= limit 时可停止（因为不再贡献节省）
// visited 返回实际被 final pop 的节点（它们的 dist 已定且 < limit）
void dijkstra_with_limit(int start, int M, const vector<vector<Edge>> &g, WeightType limit,
                         vector<WeightType> &dist, vector<int> &visited)
{
    dist.assign(M, WT_INF);
    visited.clear();
    using P = pair<WeightType, int>;
    priority_queue<P, vector<P>, greater<P>> pq;
    dist[start] = 0;
    pq.push({0, start});
    while (!pq.empty())
    {
        auto cur = pq.top();
        pq.pop();
        WeightType d = cur.first;
        int u = cur.second;
        if (d != dist[u])
            continue;
        if (limit >= 0 && d >= limit)
            break; // safe: pq pops in non-decreasing order
        visited.push_back(u);
        for (const Edge &e : g[u])
        {
            int v = e.to;
            WeightType nd = d + e.w;
            if (nd < dist[v])
            {
                dist[v] = nd;
                pq.push({nd, v});
            }
        }
    }
}

int main()
{
    int N, M, K;
    if (scanf("%d %d %d", &N, &M, &K) != 3)
        return 0;
    const int C_idx = 0; // city 1 (0-indexed)

    if (M <= SMALL_M_LIMIT)
    {
        // small M: use FW
        static int fw[SMALL_M_LIMIT + 5][SMALL_M_LIMIT + 5];
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < M; ++j)
                fw[i][j] = (i == j ? 0 : WT_INF);

        for (int i = 0; i < K; ++i)
        {
            int f, t;
            short w;
            scanf("%d %d %hd", &f, &t, &w);
            --f;
            --t;
            if (w < fw[f][t])
                fw[f][t] = w;
        }

        // cache-friendly Floyd–Warshall
        for (int k = 0; k < M; ++k)
        {
            int *fwk = fw[k];
            for (int i = 0; i < M; ++i)
            {
                int dik = fw[i][k];
                if (dik == WT_INF)
                    continue;
                int *fwi = fw[i];
                for (int j = 0; j < M; ++j)
                {
                    int vkj = fwk[j];
                    if (vkj == WT_INF)
                        continue;
                    int alt = dik + vkj;
                    if (alt < fwi[j])
                        fwi[j] = alt;
                }
            }
        }

        static int attendee_cnt[SMALL_M_LIMIT + 5];
        for (int i = 0; i < M; ++i)
            attendee_cnt[i] = 0;
        for (int i = 0; i < N; ++i)
        {
            int s;
            scanf("%d", &s);
            --s;
            attendee_cnt[s]++;
        }

        // dist to main
        static int dist_to_C[SMALL_M_LIMIT + 5];
        for (int i = 0; i < M; ++i)
            dist_to_C[i] = fw[i][C_idx];

        int best_sub = -1;
        TotalPriceType best_cost = TOTAL_INF;

        // enumerate sub venues (exclude main at index 0)
        for (int s = 1; s < M; ++s)
        {
            TotalPriceType cur = 0;
            bool possible_for_all = true;
            for (int i = 0; i < M; ++i)
            {
                int cnt = attendee_cnt[i];
                if (cnt == 0)
                    continue;
                int cost_main = dist_to_C[i];
                int cost_sub = fw[i][s];
                int chosen = (cost_main <= cost_sub ? cost_main : cost_sub);
                if (chosen >= WT_INF)
                {
                    possible_for_all = false;
                    break;
                }
                TotalPriceType add = (TotalPriceType)chosen * cnt;
                if (cur > TOTAL_INF - add)
                {
                    cur = TOTAL_INF;
                    break;
                }
                cur += add;
                if (cur >= best_cost)
                {
                    possible_for_all = false;
                    break;
                } // pruning
            }
            if (possible_for_all && cur < best_cost)
            {
                best_cost = cur;
                best_sub = s + 1; // 1-indexed
            }
        }

        if (best_cost >= TOTAL_INF)
        {
            // no feasible solution
            printf("-1\n-1\n");
        }
        else
        {
            printf("%d\n", best_sub);
            printf("%lld\n", best_cost);
        }
        return 0;
    }

    // large M branch
    vector<vector<Edge>> adj_fwd(M), adj_rev(M);
    adj_fwd.assign(M, {});
    adj_rev.assign(M, {});

    for (int i = 0; i < K; ++i)
    {
        int f, t;
        short w;
        scanf("%d %d %hd", &f, &t, &w);
        --f;
        --t;
        adj_fwd[f].push_back({t, (int)w});
        adj_rev[t].push_back({f, (int)w}); // reversed edge for computing dist(i->C)
    }

    vector<int> attendee_cnt(M, 0);
    for (int i = 0; i < N; ++i)
    {
        int s;
        scanf("%d", &s);
        --s;
        attendee_cnt[s]++;
    }

    // compute dist(i -> C) by running Dijkstra from C on reversed graph
    vector<WeightType> dist_to_C;
    dijkstra_normal(C_idx, M, adj_rev, dist_to_C);

    // base total (everyone to main)
    TotalPriceType base_total = 0;
    bool unreachable = false;
    for (int i = 0; i < M; ++i)
    {
        if (attendee_cnt[i] == 0)
            continue;
        if (dist_to_C[i] >= WT_INF)
        {
            unreachable = true;
            break;
        }
        TotalPriceType add = (TotalPriceType)attendee_cnt[i] * dist_to_C[i];
        if (base_total > TOTAL_INF - add)
            base_total = TOTAL_INF;
        base_total += add;
    }
    if (unreachable)
    {
        printf("-1\n-1\n");
        return 0;
    }

    // total_saving[s] = total saving if choose s as sub venue (s != C)
    vector<TotalPriceType> total_saving(M, 0);
    vector<WeightType> dist_tmp;
    vector<int> visited;

    // for every city i that has attendees, run Dijkstra from i on forward graph,
    // stop expanding when popped distance >= dist_to_C[i] (no saving beyond that).
    for (int i = 0; i < M; ++i)
    {
        int cnt = attendee_cnt[i];
        if (cnt == 0)
            continue;
        WeightType Di = dist_to_C[i];
        if (Di >= WT_INF)
            continue; // unreachable to main (should have been caught)
        dijkstra_with_limit(i, M, adj_fwd, Di, dist_tmp, visited);
        for (int v : visited)
        {
            if (v == C_idx)
                continue;
            WeightType d_iv = dist_tmp[v];
            if (d_iv < Di)
            {
                TotalPriceType save = (TotalPriceType)(Di - d_iv) * cnt;
                total_saving[v] += save;
                if (total_saving[v] > TOTAL_INF)
                    total_saving[v] = TOTAL_INF;
            }
        }
    }

    int best_sub = -1;
    TotalPriceType best_cost = base_total;
    for (int s = 1; s < M; ++s)
    {
        TotalPriceType cur = base_total - total_saving[s];
        if (cur < best_cost)
        {
            best_cost = cur;
            best_sub = s + 1;
        }
    }

    if (best_cost >= TOTAL_INF)
    {
        printf("-1\n-1\n");
    }
    else
    {
        printf("%d\n", best_sub);
        printf("%lld\n", best_cost);
    }

    return 0;
}
