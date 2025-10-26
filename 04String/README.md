# String

## Concept

- A string is a character sequence with a **limited length**, or a linear list whose elements are *characters*.
- SubString and main String
  - The position of the substring: Index of the **first character** in the main string.
- String Storage
  - Fixed-length sequencial storage:
    - Predefine the maximum length of the string.
    - Truncate string if exceeds the fixed length. 
  - Variable-length sequencial storage: 
    - Use a special character (e.g. `'\0'`) to mark the end of the string.
    - It avoids truncation and improves the utilization of storage space

## Operations

- [x] Minimum Operation set
- StrAssign(S, T): Assign string T to S
- StrCopy(S, T): Copy string T to S
- StrCompare(S, T): Compare strings S and T
- StrLength(S): Return the length of string S
- StrCat(S, T): Concatenate strings S and T
- SubString(Sub, S, pos, len): Get the substring Sub of string S starting from position pos with length len
  
- [x] Additional Operations can be implemented using the minimum operation set

## String Matching

See implementation in [StringMatchTest.cpp](./StringMatchTest.cpp)

- [x] Brute Force (BF) Algorithm
- Time Complexity: $O(mn)$
- Low Efficiency, suitable for **small-large** (small pattern, large target string) problems

    ```cpp
    int BruteForceMatch(char *T, char *P){
        int n = strlen(T); //Calculate the length of target string T
        int m = strlen(P); //Calculate the length of pattern string P
        O(n-m+1)
        int i, j;
        for(i=0;i<=n-m;i++){ j = 0;
        while( j<m && T[i+j] == P[j]) j++; //Try positions of target string one by one
        if(j == m)
        return i; return -1; //Comparison fails
        //Compare with pattern string
        O(m)
        //Comparison succeeds at i
        }
    }
    ```

- [x] Knuth-Morris-Pratt (KMP) Algorithm
