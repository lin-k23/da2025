#include <cstdio>
#include <vector>
#include <queue>
#include <functional>
#include <limits>
#include <algorithm>
#include <cstring> // for memset

using WeightType = short;
const WeightType WT_INF = std::numeric_limits<WeightType>::max();
const int INT_INF = std::numeric_limits<int>::max() / 2;

using TotalPriceType = long long;
const TotalPriceType TOTAL_INF = std::numeric_limits<TotalPriceType>::max();

const int MAX_M_SMALL = 501;
const int MAX_M_LARGE = 4001;

// --- 存储结构 ---
static int fw_dist[MAX_M_SMALL][MAX_M_SMALL];

struct Edge
{
    int to;
    WeightType weight;
};
static std::vector<std::vector<Edge>> adj_list_rev;
static std::vector<std::vector<Edge>> adj_list_fwd;

static int attendee_counts[MAX_M_LARGE];
static WeightType dist_to_C_short[MAX_M_LARGE];  // M > 500
static WeightType temp_dist_short[MAX_M_LARGE];  // M > 500
static TotalPriceType total_saving[MAX_M_LARGE]; // M > 500

static int dist_to_C_int[MAX_M_LARGE]; // M <= 500
static int dist_to_S_int[MAX_M_LARGE]; // M <= 500

void dijkstra_list(int start_node, int M, WeightType *result_dist, const std::vector<std::vector<Edge>> &graph)
{
    std::fill(result_dist, result_dist + M, WT_INF);
    using P = std::pair<WeightType, int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;

    result_dist[start_node] = 0;
    pq.push({0, start_node});

    while (!pq.empty())
    {
        WeightType d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > result_dist[u])
        {
            continue;
        }

        for (const Edge &edge : graph[u])
        {
            int v = edge.to;
            WeightType w = edge.weight;

            if (result_dist[u] != WT_INF && result_dist[u] + w < result_dist[v])
            {
                result_dist[v] = result_dist[u] + w;
                pq.push({result_dist[v], v});
            }
        }
    }
}

int main()
{
    int N, M, K; // N: attendees, M: cities, K: airlines
    scanf("%d %d %d", &N, &M, &K);
    const int C_idx = 0; // 主会场 (城市 1)

    // --- 根据 M 选择策略 ---
    if (M <= MAX_M_SMALL - 1)
    {
        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < M; j++)
            {
                fw_dist[i][j] = (i == j ? 0 : INT_INF);
            }
        }

        // K airlines
        for (int i = 0; i < K; i++)
        {
            int from_k, to_k;
            WeightType price_k;
            scanf("%d %d %hd", &from_k, &to_k, &price_k);
            if (price_k < fw_dist[from_k - 1][to_k - 1])
            {
                fw_dist[from_k - 1][to_k - 1] = price_k;
            }
        }

        for (int k = 0; k < M; k++)
        {
            for (int i = 0; i < M; i++)
            {
                if (fw_dist[i][k] == INT_INF)
                    continue;
                for (int j = 0; j < M; j++)
                {
                    if (fw_dist[k][j] != INT_INF && fw_dist[i][k] + fw_dist[k][j] < fw_dist[i][j])
                    {
                        fw_dist[i][j] = fw_dist[i][k] + fw_dist[k][j];
                    }
                }
            }
        }

        // N start cities
        std::fill(attendee_counts, attendee_counts + M, 0);
        for (int i = 0; i < N; i++)
        {
            int start_city;
            scanf("%d", &start_city);
            attendee_counts[start_city - 1]++;
        }

        for (int i = 0; i < M; i++)
        {
            dist_to_C_int[i] = fw_dist[i][C_idx];
        }

        int best_sub_venue_city = -1;
        TotalPriceType min_total_price = TOTAL_INF;

        for (int sub_venue_idx = 1; sub_venue_idx < M; sub_venue_idx++)
        {
            for (int i = 0; i < M; i++)
            {
                dist_to_S_int[i] = fw_dist[i][sub_venue_idx];
            }

            TotalPriceType current_total_price = 0;
            bool possible_for_all = true;

            for (int start_city_idx = 0; start_city_idx < M; start_city_idx++)
            {
                if (attendee_counts[start_city_idx] == 0)
                    continue;

                int cost_to_main = dist_to_C_int[start_city_idx];
                int cost_to_sub = dist_to_S_int[start_city_idx];
                int chosen_cost = std::min(cost_to_main, cost_to_sub);

                if (chosen_cost == INT_INF) // 使用 int 的 INF
                {
                    possible_for_all = false;
                    break;
                }

                if (current_total_price > TOTAL_INF - (TotalPriceType)chosen_cost * attendee_counts[start_city_idx])
                {
                    current_total_price = TOTAL_INF; // 防止溢出
                    break;
                }
                current_total_price += (TotalPriceType)chosen_cost * attendee_counts[start_city_idx];
            }

            if (possible_for_all && current_total_price < min_total_price)
            {
                min_total_price = current_total_price;
                best_sub_venue_city = sub_venue_idx + 1;
            }
        }
        printf("%d\n", best_sub_venue_city);
        printf("%lld\n", min_total_price);
    }
    else
    {
        adj_list_rev.resize(M);
        adj_list_fwd.resize(M);

        // K airlines
        for (int i = 0; i < K; i++)
        {
            int from_k, to_k;
            WeightType price_k;
            scanf("%d %d %hd", &from_k, &to_k, &price_k);
            from_k--; // 0-indexed
            to_k--;   // 0-indexed
            adj_list_rev[to_k].push_back({from_k, price_k});
            adj_list_fwd[from_k].push_back({to_k, price_k});
        }

        // N start cities
        std::fill(attendee_counts, attendee_counts + M, 0);
        for (int i = 0; i < N; i++)
        {
            int start_city;
            scanf("%d", &start_city);
            attendee_counts[start_city - 1]++;
        }

        dijkstra_list(C_idx, M, dist_to_C_short, adj_list_rev);

        TotalPriceType base_total_price = 0;
        std::fill(total_saving, total_saving + M, 0);

        for (int i = 0; i < M; i++)
        {
            if (attendee_counts[i] == 0)
                continue;

            WeightType D_i = dist_to_C_short[i];
            if (D_i == WT_INF)
            {
                base_total_price = TOTAL_INF;
                break;
            }
            base_total_price += (TotalPriceType)attendee_counts[i] * D_i;

            dijkstra_list(i, M, temp_dist_short, adj_list_fwd);

            for (int s = 1; s < M; s++)
            {
                WeightType dist_i_s = temp_dist_short[s];
                if (dist_i_s < D_i)
                {
                    total_saving[s] += (TotalPriceType)attendee_counts[i] * (D_i - dist_i_s);
                }
            }
        }

        int best_sub_venue_city = -1;
        TotalPriceType min_total_price = TOTAL_INF;

        TotalPriceType max_saving = 0;
        if (base_total_price != TOTAL_INF)
        {
            min_total_price = base_total_price;
            for (int s = 1; s < M; s++)
            {
                TotalPriceType current_total_price = base_total_price - total_saving[s];
                if (current_total_price < min_total_price)
                {
                    min_total_price = current_total_price;
                    best_sub_venue_city = s + 1; // 1-indexed
                }
            }
        }
        printf("%d\n", best_sub_venue_city);
        printf("%lld\n", min_total_price);
    }

    return 0;
}