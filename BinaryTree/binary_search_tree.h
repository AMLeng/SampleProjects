#if !defined(BINARY_SEARCH_TREE_H)
#define BINARY_SEARCH_TREE_H
#include <memory>
#include <iostream>

//TODO: keep track of AVL invariant instead of height
//TODO: Add iterator
//Possible extensions: Make constexpr? (Currently not easily possible since unique_ptr isn't constexpr until C++23), add noexcept?

//Implementation of an AVL tree to practice working with unique_ptr
//Also implements iterators to practice working with concepts
namespace binary_search_tree{

template <typename T> class BST{
    public:
        void add(T key){TreeNode::add_node(root, key);}

        void remove(T key){TreeNode::remove_node(root, key);}

        bool find(T key) const{return (bool) root->find(key);}

        void pre_order(){root->pre_order();std::cout<<"\n";}
    private:
        class TreeNode{
            public:
                TreeNode() = default;
                TreeNode(T key): height(1), data(key), left(nullptr), right(nullptr){}

                /*bool check_correct_heights(){
                  if(!this){return true;}
                  if(!this->left->check_correct_heights()){
                  return false;
                  }
                  if(!this->right->check_correct_heights()){
                  return false;
                  }
                  return(this->height == this->compute_height()){
                  }*/

                bool find(T key) const{return (bool) find_node(key);}

                void pre_order(){
                    if(this){std::cout << this->data << " ";}
                    if(this->left){this->left->pre_order();}
                    if(this->right){this->right->pre_order();}
                }
                static void add_node(std::unique_ptr<TreeNode>& p_subtree_root, T key);
                static void remove_node(std::unique_ptr<TreeNode>& p_subtree_root, T key);
            private:
                std::unique_ptr<TreeNode> left;
                std::unique_ptr<TreeNode> right;
                T data;
                //Unnecessary; theoretically an AVL tree only needs 2 bits for it's balance factor, but working with height is easier
                int height;
                //Helper method for find, since we don't want to be giving access to nodes in the outwards facing interface
                //We return a raw pointer for a non-owning ``observer'' pointer
                const TreeNode* find_node(T key) const;
                constexpr int balance_factor() const;
                constexpr int compute_height() const;
                static void right_rotate(std::unique_ptr<TreeNode>& p_subtree_root);
                static void left_rotate(std::unique_ptr<TreeNode>& p_subtree_root);
                static void rebalance(std::unique_ptr<TreeNode>& p_subtree_root);
                //Replace the node_to_remove with the minimum node in the "current" subtree
                static void remove_with_replacement(std::unique_ptr<TreeNode>& node_to_remove, std::unique_ptr<TreeNode>& current);

        };
        std::unique_ptr<TreeNode> root;
};
} // namespace binary_search_tree
#endif //BINARY_SEARCH_TREE_H
