#include <iostream>
#include <vector>
#include <numeric> // For std::iota

// 辅助函数，用于打印 vector 内容
void print_vector(const std::string &message, const std::vector<int> &vec)
{
    std::cout << message << " ";
    for (int x : vec)
    {
        std::cout << x << " ";
    }
    std::cout << "| Size: " << vec.size() << std::endl;
}

int main()
{
    // 创建一个 std::vector，相当于 InitList(&L)
    std::vector<int> my_vector;
    print_vector("Initial state:", my_vector);

    // --- 插入操作 ---
    // 相当于 ListInsert(&L, 0, 10);
    my_vector.insert(my_vector.begin() + 0, 10);
    print_vector("After inserting 10 at index 0:", my_vector);

    // 相当于 ListInsert(&L, 1, 30);
    my_vector.insert(my_vector.begin() + 1, 30);
    print_vector("After inserting 30 at index 1:", my_vector);

    // 在中间插入
    my_vector.insert(my_vector.begin() + 1, 20);
    print_vector("After inserting 20 at index 1:", my_vector);

    // --- 删除操作 ---
    // 相当于 ListDelete(&L, 1, &e);
    my_vector.erase(my_vector.begin() + 1);
    print_vector("After deleting element at index 1:", my_vector);

    // --- 访问和查找 ---
    // 相当于 GetElem(L, 1, &e);
    std::cout << "Element at index 1 is: " << my_vector[1] << std::endl;

    // 清空
    my_vector.clear();
    print_vector("After clearing the vector:", my_vector);

    return 0;
}