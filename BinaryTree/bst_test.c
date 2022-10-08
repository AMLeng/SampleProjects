#include "binary_search_tree.h"
#include <string>

void test_left_rot_add(){
    std::cout << "Test left rotation when adding\n";
    binary_search_tree::BST<std::string> tree = binary_search_tree::BST<std::string>();
    tree.add("Apple");
    tree.pre_order();
    std::cout << "\n";
    tree.add("Banana");
    tree.pre_order();
    std::cout << "\n";
    tree.add("Cherry");
    tree.pre_order();
    std::cout << "\n";
}
void test_right_rot_add(){
    std::cout << "Test right rotation when adding\n";
    binary_search_tree::BST<std::string> tree = binary_search_tree::BST<std::string>();
    tree.add("Cherry");
    tree.pre_order();
    std::cout << "\n";
    tree.add("Banana");
    tree.pre_order();
    std::cout << "\n";
    tree.add("Apple");
    tree.pre_order();
    std::cout << "\n";
}
void test_left_right_rot_add(){
    std::cout << "Test left right rotation when adding\n";
    binary_search_tree::BST<std::string> tree = binary_search_tree::BST<std::string>();
    tree.add("Cherry");
    tree.pre_order();
    std::cout << "\n";
    tree.add("Apple");
    tree.pre_order();
    std::cout << "\n";
    tree.add("Banana");
    tree.pre_order();
    std::cout << "\n";
}
void test_right_left_rot_add(){
    std::cout << "Test right left rotation when adding\n";
    binary_search_tree::BST<std::string> tree = binary_search_tree::BST<std::string>();
    tree.add("Apple");
    tree.pre_order();
    std::cout << "\n";
    tree.add("Cherry");
    tree.pre_order();
    std::cout << "\n";
    tree.add("Banana");
    tree.pre_order();
    std::cout << "\n";
}
int main(){
    test_left_rot_add();
    test_right_rot_add();
    test_left_right_rot_add();
    test_right_left_rot_add();
}
