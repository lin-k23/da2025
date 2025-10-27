#include <iostream>
#include <cstring> // For strlen
#include <iomanip> // For std::setw
#include <vector>  // Include the vector header

/**
 * @brief Helper function to print the N vector cleanly.
 * @param N The Next vector.
 */
void printN(const std::vector<int> &N)
{
    int m = N.size();
    std::cout << "  N: [";
    for (int k = 0; k < m; ++k)
    {
        std::cout << std::setw(2) << N[k];
        if (k < m - 1)
            std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

/**
 * @brief Computes the KMP 'Next' array with detailed logging.
 * @param P The pattern string.
 * @param N A reference to the vector to store the Next values.
 */
void Next_with_log(char *P, std::vector<int> &N)
{
    int m = strlen(P);
    if (m == 0)
        return;

    // N[0] = 0; (The vector is already 0-initialized)

    int i = 1; // Current index in pattern P
    int j = 0; // Length of the previous longest prefix-suffix

    std::cout << "--- Initial State ---" << std::endl;
    std::cout << "  Pattern: " << P << " (m = " << m << ")" << std::endl;
    std::cout << "  i = " << i << ", j = " << j << std::endl;
    printN(N);
    std::cout << "---------------------" << std::endl;

    while (i < m)
    {
        std::cout << "Step: (i=" << i << ", j=" << j << ")" << std::endl;
        std::cout << "  Comparing P[i] (P[" << i << "] = '" << P[i] << "') with P[j] (P[" << j << "] = '" << P[j] << "')" << std::endl;

        if (P[i] == P[j])
        {
            std::cout << "  > Match!" << std::endl;
            N[i] = j + 1;
            std::cout << "  > Set N[" << i << "] = j + 1 = " << N[i] << std::endl;
            i++;
            j++;
        }
        else
        {
            std::cout << "  > Mismatch." << std::endl;
            if (j > 0)
            {
                std::cout << "  > j > 0 (" << j << "), backtracking." << std::endl;
                std::cout << "  > Set j = N[j - 1] = N[" << j - 1 << "] = " << N[j - 1] << std::endl;
                j = N[j - 1];
            }
            else
            {
                std::cout << "  > j is 0, cannot backtrack." << std::endl;
                N[i] = 0;
                std::cout << "  > Set N[" << i << "] = 0" << std::endl;
                i++;
            }
        }
        std::cout << "  End of step. New state: (i=" << i << ", j=" << j << ")" << std::endl;
        printN(N);
        std::cout << "---------------------" << std::endl;
    }

    std::cout << "--- Final N Array ---" << std::endl;
    printN(N);
    std::cout << "---------------------" << std::endl;
}

int main()
{
    // Example pattern 1
    char P1[] = "ABABCABAB";
    int m1 = strlen(P1);
    // Create a vector of size m1, all elements initialized to 0
    std::vector<int> N1(m1, 0);

    std::cout << "Calculating Next for pattern: " << P1 << "\n"
              << std::endl;
    Next_with_log(P1, N1);

    // Example pattern 2
    std::cout << "\n\nCalculating Next for pattern: AAAA\n"
              << std::endl;
    char P2[] = "AAAA";
    int m2 = strlen(P2);
    std::vector<int> N2(m2, 0);

    Next_with_log(P2, N2);

    // Example pattern 3
    std::cout << "\n\nCalculating Next for pattern: ABCDE\n"
              << std::endl;
    char P3[] = "ABCDE";
    int m3 = strlen(P3);
    std::vector<int> N3(m3, 0);

    Next_with_log(P3, N3);

    // No delete[] needed! The vectors clean themselves up.
    return 0;
}