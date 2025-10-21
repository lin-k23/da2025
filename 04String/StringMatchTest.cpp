#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // for std::max

// Define the size of the character set (e.g., ASCII)
const int ALPHABET_SIZE = 256;

/******************************************************************************
 * Brute-Force Algorithm
 ******************************************************************************/
std::vector<int> bruteForceMatchAll(const std::string &text, const std::string &pattern)
{
    std::vector<int> match_indices;
    int n = text.length();
    int m = pattern.length();
    if (m == 0)
        return match_indices;
    for (int i = 0; i <= n - m; ++i)
    {
        int j = 0;
        while (j < m && text[i + j] == pattern[j])
        {
            j++;
        }
        if (j == m)
        {
            match_indices.push_back(i);
        }
    }
    return match_indices;
}

/******************************************************************************
 * KMP Algorithm
 ******************************************************************************/
std::vector<int> computeNextTable(const std::string &pattern)
{
    int m = pattern.length();
    std::vector<int> next(m, 0);
    int j = 0;
    for (int i = 1; i < m; ++i)
    {
        while (j > 0 && pattern[i] != pattern[j])
        {
            j = next[j - 1];
        }
        if (pattern[i] == pattern[j])
        {
            j++;
        }
        next[i] = j;
    }
    return next;
}

std::vector<int> kmpMatchAll(const std::string &text, const std::string &pattern)
{
    int n = text.length();
    int m = pattern.length();
    std::vector<int> match_indices;
    if (m == 0)
        return match_indices;
    std::vector<int> next = computeNextTable(pattern);
    int j = 0;
    for (int i = 0; i < n; ++i)
    {
        while (j > 0 && text[i] != pattern[j])
        {
            j = next[j - 1];
        }
        if (text[i] == pattern[j])
        {
            j++;
        }
        if (j == m)
        {
            match_indices.push_back(i - m + 1);
            j = next[j - 1];
        }
    }
    return match_indices;
}

/******************************************************************************
 * Horspool Algorithm (newly added)
 ******************************************************************************/

/**
 * @brief Builds the shift table for the Horspool algorithm.
 * This table stores the last occurrence index of each character in the pattern.
 *
 * @param pattern The pattern string P.
 * @return A std::vector<int> representing the shift table. For a character 'c',
 * table[c] is the index of its last occurrence in the pattern. If 'c' is not in
 * the pattern, the value is -1.
 */
std::vector<int> buildShiftTable(const std::string &pattern)
{
    int m = pattern.length();
    std::vector<int> table(ALPHABET_SIZE, -1);

    // Store the last occurrence of each character found in the pattern.
    // We don't include the last character of the pattern itself in this calculation
    // for the most common variant of the shift logic.
    for (int i = 0; i < m - 1; ++i)
    {
        table[static_cast<unsigned char>(pattern[i])] = i;
    }
    return table;
}

/**
 * @brief Finds all occurrences of a pattern in a text using the Horspool algorithm.
 *
 * This function uses a precomputed shift table based on the "bad character" heuristic.
 * It compares the pattern from right to left.
 *
 * @param text The string to be searched in (Target string T).
 * @param pattern The string to search for (Pattern string P).
 * @return A std::vector<int> containing the starting indices of all occurrences.
 */
std::vector<int> horspoolMatchAll(const std::string &text, const std::string &pattern)
{
    int n = text.length();
    int m = pattern.length();
    std::vector<int> match_indices;

    if (m == 0 || n < m)
        return match_indices;

    // Step 1: Preprocess the pattern to get the shift table.
    std::vector<int> table = buildShiftTable(pattern);

    int i = 0; // i is the starting index of the alignment window in the text
    while (i <= n - m)
    {
        // Get the character in the text that aligns with the last char of the pattern
        char c = text[i + m - 1];

        // Compare this character with the last character of the pattern
        if (c == pattern[m - 1])
        {
            // If they match, check the rest of the pattern from right to left
            int j = m - 2;
            while (j >= 0 && text[i + j] == pattern[j])
            {
                j--;
            }
            // If the inner loop completed, we have a full match
            if (j < 0)
            {
                match_indices.push_back(i);
            }
        }

        // Calculate the shift to the next position.
        // The shift is based on the last occurrence of character 'c' in the pattern.
        int shift = m - 1 - table[static_cast<unsigned char>(c)];
        i += shift;
    }

    return match_indices;
}

int main()
{
    std::string T = "ababaabaaababa";
    std::string P = "aba";

    std::cout << "Target String (T): " << T << std::endl;
    std::cout << "Pattern String (P): " << P << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    // --- Brute-Force Algorithm Results ---
    std::cout << "Brute-Force Algorithm Results:" << std::endl;
    std::vector<int> bf_positions = bruteForceMatchAll(T, P);
    if (!bf_positions.empty())
    {
        std::cout << "Pattern found at indices: ";
        for (int pos : bf_positions)
        {
            std::cout << pos << " ";
        }
        std::cout << std::endl;
    }
    else
    {
        std::cout << "Pattern not found." << std::endl;
    }

    // --- KMP Algorithm Results ---
    std::cout << "\nKMP Algorithm Results:" << std::endl;
    std::vector<int> kmp_positions = kmpMatchAll(T, P);
    if (!kmp_positions.empty())
    {
        std::cout << "Pattern found at indices: ";
        for (int pos : kmp_positions)
        {
            std::cout << pos << " ";
        }
        std::cout << std::endl;
    }
    else
    {
        std::cout << "Pattern not found." << std::endl;
    }

    // --- Horspool Algorithm Results ---
    std::cout << "\nHorspool Algorithm Results:" << std::endl;
    std::vector<int> hs_positions = horspoolMatchAll(T, P);
    if (!hs_positions.empty())
    {
        std::cout << "Pattern found at indices: ";
        for (int pos : hs_positions)
        {
            std::cout << pos << " ";
        }
        std::cout << std::endl;
    }
    else
    {
        std::cout << "Pattern not found." << std::endl;
    }

    return 0;
}