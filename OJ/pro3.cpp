#include <cstdio>
#include <vector>
#include <queue>
#include <functional>
#include <limits>
#include <algorithm>
#include <cstring> // for memset

using WeightType = short;
const WeightType INF = std::numeric_limits<WeightType>::max();

using TotalPriceType = long long;
const TotalPriceType TOTAL_INF = std::numeric_limits<TotalPriceType>::max();

const int MAX_M_SMALL = 501;
const int MAX_M_LARGE = 4001;

// --- 存储结构 ---

// 1. 用于 M <= 500 的稠密图 (邻接矩阵)
static WeightType adj_matrix_rev[MAX_M_SMALL][MAX_M_SMALL];

// 2. 用于 M > 500 的稀疏图 (邻接表)
struct Edge
{
    int to;
    WeightType weight;
};
static std::vector<std::vector<Edge>> adj_list_rev;

static int attendee_counts[MAX_M_LARGE];
static WeightType dist_to_C[MAX_M_LARGE];
static WeightType dist_to_S[MAX_M_LARGE]; // 重用此数组

static WeightType dist[MAX_M_LARGE];
static bool visited[MAX_M_LARGE];

void dijkstra_matrix(int start_node, int M, WeightType *result_dist)
{
    std::fill(dist, dist + M, INF);
    std::fill(visited, visited + M, false);
    dist[start_node] = 0;

    for (int i = 0; i < M; i++)
    {
        WeightType min_dist = INF;
        int u = -1;

        for (int v = 0; v < M; v++)
        {
            if (!visited[v] && dist[v] < min_dist)
            {
                min_dist = dist[v];
                u = v;
            }
        }

        if (u == -1)
        {
            break;
        }

        visited[u] = true;

        for (int v = 0; v < M; v++)
        {
            WeightType w = adj_matrix_rev[u][v];
            if (w != INF && !visited[v] && dist[u] != INF && dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
            }
        }
    }

    std::copy(dist, dist + M, result_dist);
}

void dijkstra_list(int start_node, int M, WeightType *result_dist)
{
    std::fill(result_dist, result_dist + M, INF);
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

        for (const Edge &edge : adj_list_rev[u])
        {
            int v = edge.to;
            WeightType w = edge.weight;

            if (result_dist[u] != INF && result_dist[u] + w < result_dist[v])
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
    const int C_idx = 0;
    if (M <= MAX_M_SMALL - 1)
    {
        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < M; j++)
            {
                adj_matrix_rev[i][j] = INF;
            }
        }

        // K airlines
        for (int i = 0; i < K; i++)
        {
            int from_k, to_k;
            WeightType price_k;
            scanf("%d %d %hd", &from_k, &to_k, &price_k);
            if (price_k < adj_matrix_rev[to_k - 1][from_k - 1])
            {
                adj_matrix_rev[to_k - 1][from_k - 1] = price_k;
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

        // 计算到主会场 C 的距离
        dijkstra_matrix(C_idx, M, dist_to_C);

        int best_sub_venue_city = -1;
        TotalPriceType min_total_price = TOTAL_INF;

        for (int sub_venue_idx = 1; sub_venue_idx < M; sub_venue_idx++)
        {
            // 计算到分会场 S 的距离
            dijkstra_matrix(sub_venue_idx, M, dist_to_S);

            TotalPriceType current_total_price = 0;
            bool possible_for_all = true;

            for (int start_city_idx = 0; start_city_idx < M; start_city_idx++)
            {
                if (attendee_counts[start_city_idx] == 0)
                    continue;

                WeightType cost_to_main = dist_to_C[start_city_idx];
                WeightType cost_to_sub = dist_to_S[start_city_idx];
                WeightType chosen_cost = std::min(cost_to_main, cost_to_sub);

                if (chosen_cost == INF)
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

        // K airlines
        for (int i = 0; i < K; i++)
        {
            int from_k, to_k;
            WeightType price_k;
            scanf("%d %d %hd", &from_k, &to_k, &price_k);
            adj_list_rev[to_k - 1].push_back({from_k - 1, price_k});
        }

        // N start cities
        std::fill(attendee_counts, attendee_counts + M, 0);
        for (int i = 0; i < N; i++)
        {
            int start_city;
            scanf("%d", &start_city);
            attendee_counts[start_city - 1]++;
        }

        // 计算到主会场 C 的距离
        dijkstra_list(C_idx, M, dist_to_C);

        int best_sub_venue_city = -1;
        TotalPriceType min_total_price = TOTAL_INF;

        for (int sub_venue_idx = 1; sub_venue_idx < M; sub_venue_idx++)
        {
            // 计算到分会场 S 的距离
            dijkstra_list(sub_venue_idx, M, dist_to_S);

            TotalPriceType current_total_price = 0;
            bool possible_for_all = true;

            for (int start_city_idx = 0; start_city_idx < M; start_city_idx++)
            {
                if (attendee_counts[start_city_idx] == 0)
                    continue;

                WeightType cost_to_main = dist_to_C[start_city_idx];
                WeightType cost_to_sub = dist_to_S[start_city_idx];
                WeightType chosen_cost = std::min(cost_to_main, cost_to_sub);

                if (chosen_cost == INF)
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

    return 0;
}