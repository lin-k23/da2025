#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm> // added: provides std::max_element

using namespace std;

// test case input for debugging
// string testcase1 = "4\n80 90 23 40 46 88 -99 -99 -99 58 -99 83 59 -99 -99";

int main()
{
    int h;
    vector<int> bt_weights; // -99 indicates empty node
    cin >> h;
    // 2^h - 1 nodes
    while (bt_weights.size() < (1 << h) - 1)
    {
        int weight;
        cin >> weight;
        bt_weights.push_back(weight);
    }

    vector<int> max_path_sums(h, -1000);

    // For debugging purposes
    // istringstream input(testcase1);
    // input >> h;
    // int weight;
    // while (input >> weight)
    // {
    //     bt_weights.push_back(weight);
    // }

    // cout << "Height: " << h << endl;
    // cout << "Weights: ";
    // for (const auto &w : max_path_sums)
    // {
    //     cout << w << " ";
    // }
    // cout << endl;

    for (int level = h - 1; level >= 0; --level)
    {
        for (int i = (1 << level) - 1; i < (1 << (level + 1)) - 1; ++i)
        {
            if (bt_weights[i] == -99)
            {
                continue;
            }
            // bottom nodes merged up
            if (level == h - 1)
            {
                max_path_sums[level] = max(max_path_sums[level], bt_weights[i]);
            }
            else
            {
                int left_child_index = 2 * i + 1;
                int right_child_index = 2 * i + 2;

                int left_child_sum = (bt_weights[left_child_index] > 0)
                                         ? bt_weights[left_child_index]
                                         : 0;
                int right_child_sum = (bt_weights[right_child_index] > 0)
                                          ? bt_weights[right_child_index]
                                          : 0;

                int current_sum = bt_weights[i] + left_child_sum + right_child_sum;
                max_path_sums[level] = max(max_path_sums[level], current_sum);
                bt_weights[i] = bt_weights[i] + max(left_child_sum, right_child_sum);
            }
        }
    }

    int max_sum = *max_element(max_path_sums.begin(), max_path_sums.end());
    cout << max_sum << endl;
    return 0;
}