
// Red-Black-Tree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "RBTree.h"
template<typename T>
void Print(std::vector<T>& a)
{
    for (size_t i = 0; i < a.size(); i++)
    {
        std::cout << a.at(i) << " ";
    }
}
int main()
{
    RBTree<int> tree;

    tree.Add(10);
    tree.Add(7);
    tree.Add(5);
    tree.Add(3);
    tree.Add(8);
    tree.Remove(5);
    auto v = tree.PreOrder();
    Print(v);
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
