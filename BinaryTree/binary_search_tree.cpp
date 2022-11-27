#include "binary_search_tree.h"
#include <algorithm>
#include <cassert>
#include <string>

namespace binary_search_tree {
template <class T>
void BST<T>::TreeNode::add_node(std::unique_ptr<BST<T>::TreeNode>& p_subtree_root, T key){
    if(!p_subtree_root){
        p_subtree_root = std::make_unique<BST<T>::TreeNode>(key);
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

template <class T>
void BST<T>::TreeNode::remove_node(std::unique_ptr<BST<T>::TreeNode>& p_subtree_root, T key){
    if(!p_subtree_root){
        return;
    }
    if(key < p_subtree_root->data){
        remove_node(p_subtree_root->left, key);
    }else if(key > p_subtree_root->data){
        remove_node(p_subtree_root->right, key);
    }else{
        //Two child case, swap with inorder successor
        if(p_subtree_root -> left && p_subtree_root -> right){
            auto temp_left = std::move(p_subtree_root->left);
            auto temp_right = std::move(p_subtree_root->right);
            remove_with_replacement(p_subtree_root,temp_right); 
            p_subtree_root -> left = std::move(temp_left);
            p_subtree_root -> right = std::move(temp_right);
        }//Zero or one child case
        else{
            auto temp = p_subtree_root -> left ? std::move(p_subtree_root ->left) : std::move(p_subtree_root->right);
            p_subtree_root = std::move(temp);
        }
    }
    //We could have deleted p_subtree_root so check that it is non-null
    if(p_subtree_root){
        p_subtree_root->height = p_subtree_root->compute_height();

        //Ensure that the tree rooted at p_subtree_root is AVL
        rebalance(p_subtree_root);
    }
}

/*class Iterator requires std::input_iterator<T>{

  }*/

template <class T>
const typename BST<T>::TreeNode* BST<T>::TreeNode::find_node(T key) const{
    if(!this){return nullptr;}
    if(data == key){
        return this;
    }else if(key < data){
        return this->left->find_node(key);
    }else{
        return this->right->find_node(key);
    }
}

template <class T>
constexpr int BST<T>::TreeNode::balance_factor() const{
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

template <class T>
constexpr int BST<T>::TreeNode::compute_height() const{
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
template <class T>
void BST<T>::TreeNode::right_rotate(std::unique_ptr<BST<T>::TreeNode>& p_subtree_root){
    auto p_new_subtree_root = std::move(p_subtree_root->left);
    p_subtree_root->left = std::move(p_new_subtree_root->right);
    p_subtree_root->height = p_subtree_root->compute_height();

    p_new_subtree_root->right = std::move(p_subtree_root);
    p_subtree_root = std::move(p_new_subtree_root);
    p_subtree_root-> height = p_subtree_root -> compute_height();

}
//Left rotates; the right node must be non-null
template <class T>
void BST<T>::TreeNode::left_rotate(std::unique_ptr<BST<T>::TreeNode>& p_subtree_root){
    auto p_new_subtree_root = std::move(p_subtree_root->right);
    p_subtree_root->right = std::move(p_new_subtree_root->left);
    p_subtree_root->height = p_subtree_root->compute_height();

    p_new_subtree_root->left = std::move(p_subtree_root);
    p_subtree_root = std::move(p_new_subtree_root);
    p_subtree_root-> height = p_subtree_root -> compute_height();
}

template <class T>
void BST<T>::TreeNode::rebalance(std::unique_ptr<BST<T>::TreeNode>& p_subtree_root){
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

//Replace the node_to_remove with the minimum node in the "current" subtree
template <class T>
void BST<T>::TreeNode::remove_with_replacement(std::unique_ptr<BST<T>::TreeNode>& node_to_remove, std::unique_ptr<BST<T>::TreeNode>& current){
    if(current -> left){
        remove_with_replacement(node_to_remove, current->left);
        current -> height = current->compute_height();
        rebalance(current);
    }else{
        //If "current" has no right element, it becomes null
        node_to_remove = std::move(current);
        current = std::move(node_to_remove->right);
    }
}
template class BST<int>;
template class BST<std::string>;
} // namespace binary_search_tree
