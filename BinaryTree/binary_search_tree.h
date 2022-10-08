#if !defined(BINARY_SEARCH_TREE_H)
#define BINARY_SEARCH_TREE_H
#include <memory>
#include <algorithm>
#include <iostream>
#include <cassert>

//TODO: Add deletion, keep track of AVL invariant instead of height
//TODO: Add constructor for the tree given an array of T
//TODO: Add iterator
//Possible extensions: Make constexpr? (Currently not easily possible since unique_ptr isn't constexpr until C++23), add noexcept?

namespace binary_search_tree{

template <typename T> class BST{
    public:
        void add(T key){TreeNode::add_node(root, key);}

        bool find(T key) const{return (bool) root->find(key);}

        void pre_order(){root->pre_order();}

    private:
        class TreeNode;
        std::unique_ptr<TreeNode> root;

        class TreeNode{
            public:
                TreeNode() = default;
                TreeNode(T key): height(1), data(key), left(nullptr), right(nullptr){}

                bool find(T key) const{return (bool) find_node(key);}

                //Remove data from the tree, returning true if successful, and false if the given data was not in the tree
                /*bool remove(T key){
                  bool node_removed = false;
                //Code to remove node here
                //Reupdate heights
                //Ensure that the tree is AVL
                }*/

                void pre_order(){
                    if(this){std::cout << this->data << " ";}
                    if(this->left){this->left->pre_order();}
                    if(this->right){this->right->pre_order();}
                }

                static void add_node(std::unique_ptr<TreeNode>& p_subtree_root, T key){
                    if(!p_subtree_root){
                        p_subtree_root = std::make_unique<TreeNode>(key);
                    }
                    if(key < p_subtree_root->data){
                        add_node(p_subtree_root->left, key);
                    }else if(key > p_subtree_root->data){
                        add_node(p_subtree_root->right, key);
                    }
                    p_subtree_root->height = p_subtree_root->compute_height();

                    //Ensure that the tree rooted at p_subtree_root is AVL
                    rebalance(p_subtree_root);
                }

            private:
                std::unique_ptr<TreeNode> left;
                std::unique_ptr<TreeNode> right;
                T data;
                //Unnecessary; theoretically an AVL tree only needs 2 bits for it's balance factor, but working with height is easier
                int height;

                //Helper method for find, since we don't want to be giving access to nodes in the outwards facing interface
                //We return a raw pointer for a non-owning ``observer'' pointer
                const TreeNode* find_node(T key) const{
                    if(!this){return nullptr;}
                    if(data == key){
                        return this;
                    }else if(key < data){
                        return this->left->find_node(key);
                    }else{
                        return this->right->find_node(key);
                    }
                }

                constexpr int balance_factor() const{
                    int left = 0;
                    int right = 0;
                    if(this->right){
                        right = this->right->height + 1;
                    }
                    if(this->left){
                        left = this->left->height + 1;
                    }
                    return right - left;
                }
                constexpr int compute_height() const{
                    int left = 0;
                    int right = 0;
                    if(this->right){
                        right = this->right->height + 1;
                    }
                    if(this->left){
                        left = this->left->height + 1;
                    }
                    return std::max(left,right);
                }

                //Right rotates; the left node must be non-null
                static void right_rotate(std::unique_ptr<TreeNode>& p_subtree_root){
                    auto p_new_subtree_root = std::move(p_subtree_root->left);
                    p_subtree_root->left = std::move(p_new_subtree_root->right);
                    p_subtree_root->height = p_subtree_root->compute_height();

                    p_new_subtree_root->right = std::move(p_subtree_root);
                    p_subtree_root = std::move(p_new_subtree_root);
                    p_subtree_root-> height = p_subtree_root -> compute_height();

                }
                //Left rotates; the right node must be non-null
                static void left_rotate(std::unique_ptr<TreeNode>& p_subtree_root){
                    auto p_new_subtree_root = std::move(p_subtree_root->right);
                    p_subtree_root->right = std::move(p_new_subtree_root->left);
                    p_subtree_root->height = p_subtree_root->compute_height();

                    p_new_subtree_root->left = std::move(p_subtree_root);
                    p_subtree_root = std::move(p_new_subtree_root);
                    p_subtree_root-> height = p_subtree_root -> compute_height();
                }

                static void rebalance(std::unique_ptr<TreeNode>& p_subtree_root){
                    int root_invar = p_subtree_root->balance_factor();
                    if(root_invar < -1){
                        assert(p_subtree_root -> left);
                        int child_invar = p_subtree_root->left->balance_factor();
                        if(child_invar <= 0){
                            right_rotate(p_subtree_root);
                        }else{
                            assert(p_subtree_root->left->right);
                            left_rotate(p_subtree_root->left);
                            right_rotate(p_subtree_root);
                        }
                    }
                    if(root_invar > 1){
                        assert(p_subtree_root -> right);
                        int child_invar = p_subtree_root->right->balance_factor();
                        if(child_invar >= 0){
                            left_rotate(p_subtree_root);
                        }else{
                            assert(p_subtree_root->right->left);
                            right_rotate(p_subtree_root->right);
                            left_rotate(p_subtree_root);
                        }
                    }
                }
        };
};


} // namespace binary_search_tree
#endif //BINARY_SEARCH_TREE_H
