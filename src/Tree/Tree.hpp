// ©Noam Chen

#ifndef HW1_WET_AVL_TREE_H
#define HW1_WET_AVL_TREE_H

#include "TNode.hpp"
#include "Exceptions.h"
#include <cmath>
#include <ostream>
#include <iomanip>

template<class K, class T> // key & data
class AVL_Tree {
private:
    std::shared_ptr<TNode<K,T>> root;
    std::shared_ptr<TNode<K,T>> left_most;
    std::shared_ptr<TNode<K,T>> right_most;
    int _size;
    std::shared_ptr<TNode<K,T>> rotate(std::shared_ptr<TNode<K,T>> to_cycle);
    std::shared_ptr<TNode<K,T>> rotateLL(std::shared_ptr<TNode<K,T>> A);
    std::shared_ptr<TNode<K,T>> rotateLR(std::shared_ptr<TNode<K,T>> A);
    std::shared_ptr<TNode<K,T>> rotateRL(std::shared_ptr<TNode<K,T>> A);
    std::shared_ptr<TNode<K,T>> rotateRR(std::shared_ptr<TNode<K,T>> A);
    int countChildren(std::shared_ptr<TNode<K,T>> node);
    void balance(std::shared_ptr<TNode<K,T>> last_modified,int old_height, bool insertion = false);
    void deleteNode(std::shared_ptr<TNode<K,T>> to_delete);
    void insertNode(std::shared_ptr<TNode<K,T>> to_insert);
    void print(std::shared_ptr<TNode<K,T>> root);
    void recursiveDelete(std::shared_ptr<TNode<K,T>> to_delete);

public:
    AVL_Tree () = default;
    AVL_Tree(T* arr, int start, int end);
    explicit AVL_Tree(std::shared_ptr<TNode<K,T>> _root);
    AVL_Tree (const AVL_Tree<K,T>& avl_tree) = default;
    AVL_Tree<K,T>& operator=(const AVL_Tree<K,T>& other) = default;
    ~AVL_Tree();
    std::shared_ptr<TNode<K,T>> getRoot() const;
    std::shared_ptr<TNode<K,T>> findNodeWithKey(K key);
    void remove(K key);
    void insert(K key, T data);
    void printTree();
    void clearTree();
    void setLeftMost(std::shared_ptr<TNode<K,T>> _left_most);
    void setRightMost(std::shared_ptr<TNode<K,T>> _right_most);
    std::shared_ptr<TNode<K,T>> getRightMost() const;
    std::shared_ptr<TNode<K,T>> getLeftMost() const;
    bool isEmpty() const;
    std::shared_ptr<TNode<K,T>> sortedArrayToAVL(T* arr, int start, int end);
    T* AVLToSortedArray(int& size);

};


template<class K, class T>
AVL_Tree<K,T>::AVL_Tree(T* arr, int start, int end): root(sortedArrayToAVL(arr, start, end)), left_most(root), right_most(root) , _size(end - start+1){
    while(right_most->right){
        right_most = right_most->right;
    }
    while(left_most->left){
        left_most = left_most->left;
    }
}

template<class K, class T>
std::shared_ptr<TNode<K,T>>
AVL_Tree<K,T>::sortedArrayToAVL(T* arr, int start, int end){
    if(start > end){
        return nullptr;
    }
    int mid = start + (end - start) / 2;
    std::shared_ptr<TNode<K,T>> root(new TNode<K,T>(arr[mid]->getModelID(), arr[mid]));
    root->left = sortedArrayToAVL(arr, start, mid - 1);
    root->right = sortedArrayToAVL(arr, mid + 1, end);
    if(root->left != nullptr){
        root->left->parent = root;
    }
    if(root->right != nullptr){
        root->right->parent = root;
    }
    root->refresh();
    return root;
}

template<class K, class T>
AVL_Tree<K,T>::AVL_Tree(std::shared_ptr<TNode<K,T>> _root): root(_root), left_most(_root), right_most(_root) , _size(1) {}

template<class K, class T>
std::shared_ptr<TNode<K,T>> AVL_Tree<K,T>::findNodeWithKey(K key)
{
    if(this->root==nullptr)
    {
        return nullptr;
    }
    std::shared_ptr<TNode<K,T>> node_iterator=root;
    while(node_iterator!=nullptr)
    {
        if(node_iterator->getKey() == key)
        {
            return node_iterator;
        }
        else if (node_iterator->getKey() > key)
        {
            node_iterator=node_iterator->left;
        }
        else
        {
            node_iterator=node_iterator->right;
        }
    }
    return nullptr;
}

template<class K, class T>
void AVL_Tree<K,T>::insert(K key, T data){
    std::shared_ptr<TNode<K,T>> to_insert = std::shared_ptr<TNode<K,T>>(new TNode<K,T>(key, data));
    if(root == nullptr){ // first node to be inserted in the tree
        root = to_insert;
        left_most = to_insert;
        right_most = to_insert;
        _size++;
        return;
    }
    if(key < left_most->getKey()){
        left_most = to_insert;
    }
    else if(key > right_most->getKey()){
        right_most = to_insert;
    }
    insertNode(to_insert);
    left_most->refresh();
    right_most->refresh();
    to_insert->refresh();
    _size++;
}

template<class K, class T>
void AVL_Tree<K,T>::insertNode(std::shared_ptr<TNode<K,T>> to_insert) {
    std::shared_ptr<TNode<K,T>> it = root;
    while(it){
        if(to_insert->getKey() < it->getKey()){
            if(it->left){
                it = it->left;
            }
            else{
                it->left = to_insert;
                to_insert->parent = it;
                break;
            }
        }
        else if(to_insert->getKey() > it->getKey()){
            if(it->right){
                it = it->right;
            }
            else{
                it->right = to_insert;
                to_insert->parent = it;
                break;
            }
        }
        else return;
    }
    balance(it,it->getHeight(), true);
}

template<class K, class T>
int AVL_Tree<K,T>::countChildren(std::shared_ptr<TNode<K,T>> node) {
    if(node->left and node->right){
        return 2;
    }
    else if((not node->left) and (not node->right)){
        return 0;
    }
    return 1;
}

template<class K, class T>
void AVL_Tree<K,T>::remove(K key){
    std::shared_ptr<TNode<K,T>> to_delete = findNodeWithKey(key);
    if(to_delete){
        if(to_delete == left_most){
            if(to_delete == root){
                std::shared_ptr<TNode<K,T>> it = root->right;
                while(it and it->left){ // The tree is balanced hence the iteration amount is limited
                    it = it->left;
                }
                left_most = it;
            }
            else{
                std::shared_ptr<TNode<K,T>> it = left_most->parent;
                if(it->right and it->right->left){
                    it = it->right->left;
                    while(it and it->left){
                        it = it->left;
                    }
                }
                left_most = it;
            }
        }
        else if(to_delete == right_most){
            if(to_delete == root){
                std::shared_ptr<TNode<K,T>> it = root->left;
                while(it and it->right){
                    it = it->right;
                }
                right_most = it;
            }
            else{
                std::shared_ptr<TNode<K,T>> it = right_most->parent;
                if(it->left and it->left->right){
                    it = it->left->right;
                    while(it and it->right){
                        it = it->right;
                    }
                }
                right_most = it;
            }
        }
        deleteNode(to_delete);
        _size--;
        return;
    }
    throw NodeNotInTree();
}



template<class K, class T>
void AVL_Tree<K,T>::deleteNode(std::shared_ptr<TNode<K,T>> to_delete) {
    int old_height = to_delete != root ? to_delete->parent->getHeight() : root->getHeight();
    if(countChildren(to_delete) == 0){ // Node to remove is a leaf node
        if(to_delete == root){
            root = nullptr;
        }
        else{
            std::shared_ptr<TNode<K,T>> it;
            // removing the leaf
            if(to_delete->parent->left == to_delete){
                to_delete->parent->left = nullptr;
                it = to_delete->parent;
                to_delete->parent = nullptr;
            }
            else{
                to_delete->parent->right = nullptr;
                it = to_delete->parent;
                to_delete->parent = nullptr;
            }
            to_delete = nullptr;
            // balancing the search path in the tree
            balance(it, old_height);
        }
    }
    else if(countChildren(to_delete) == 1){
        if(to_delete != root) {
            if (to_delete->right) { // Node to remove has a right subtree but no left subtree
                to_delete->right->parent = to_delete->parent;
                to_delete->parent->right == to_delete ? to_delete->parent->right = to_delete->right
                                                            : to_delete->parent->left = to_delete->right;
                balance(to_delete->right, old_height);
            } else { // Node to remove has left subtree but no right subtree
                to_delete->left->parent = to_delete->parent;
                to_delete->parent->right == to_delete ? to_delete->parent->right = to_delete->left
                                                            : to_delete->parent->left = to_delete->left;
                balance(to_delete->left, old_height);
            }
        }
        else{
            if(to_delete->right){
                root = to_delete->right;
            }
            else{
                root = to_delete->left;
            }
            root->parent = nullptr;
            balance(root, old_height);
        }
        to_delete = nullptr;
    }
    else{ // Node to remove has a both a left subtree and a right subtree
        std::shared_ptr<TNode<K,T>> it = to_delete->right;
        while(it->left){
            it = it->left;
        }
        to_delete->setKey(it->getKey());
        to_delete->setValue(it->getValue());
        deleteNode(it);
    }
}

template<class K, class T>
void AVL_Tree<K,T>::balance(std::shared_ptr<TNode<K,T>> last_modified,int old_height, bool insertion){
    while(last_modified != nullptr){
        last_modified->refresh();
        if(std::abs(last_modified->getBF()) == 2){
            if(insertion) {
                rotate(last_modified);
                return;
            }
            else{
                last_modified = rotate(last_modified);
                last_modified->refresh();
                if(last_modified->getHeight() == old_height){
                    return;
                }
                if(last_modified == root){
                    continue;
                }
                else last_modified = last_modified->parent;
            }
        }
        else{
            last_modified = last_modified->parent;
        }
    }
}

template<class K, class T>
std::shared_ptr<TNode<K,T>> AVL_Tree<K,T>::rotate(std::shared_ptr<TNode<K,T>> to_cycle) {
    if(to_cycle->getBF() == -2 and to_cycle->right->getBF() <= 0){
        return rotateRR(to_cycle->right);
    }
    else if(to_cycle->getBF() == 2 and to_cycle->left->getBF() >= 0){
        return rotateLL(to_cycle->left);
    }
    else if(to_cycle->getBF() == -2 and to_cycle->right->getBF() == 1){
        return rotateRL(to_cycle->right->left);
    }
    else if(to_cycle->getBF() == 2 and to_cycle->left->getBF() == -1){
        return rotateLR(to_cycle->left->right);
    }
    return nullptr;
}

template<class K, class T>
std::shared_ptr<TNode<K,T>> AVL_Tree<K,T>::rotateRR(std::shared_ptr<TNode<K,T>> A) {
    std::shared_ptr<TNode<K,T>> B = A->parent;
    std::shared_ptr<TNode<K,T>> p = B->parent;
    if(p != nullptr){
        if(p->right == B){
            p->right = A;
        }
        else p->left = A;
    }
    else root = A;
    A->parent = p;
    B->parent = A;
    B->right = A->left;
    B->right != nullptr ? B->right->parent = B : B->right = nullptr;
    A->left = B;
    B->refresh();
    A->refresh();
    return A;
}

template<class K, class T>
std::shared_ptr<TNode<K,T>> AVL_Tree<K,T>::rotateLL(std::shared_ptr<TNode<K,T>> A) {
    std::shared_ptr<TNode<K,T>> B = A->parent;
    std::shared_ptr<TNode<K,T>> p = B->parent;
    if(p != nullptr){
        if(p->right == B){
            p->right = A;
        }
        else p->left = A;
    }
    else root = A;
    A->parent = p;
    B->parent = A;
    B->left = A->right;
    A->right != nullptr ? A->right->parent = B : B->left = nullptr;
    A->right = B;
    B->refresh();
    A->refresh();
    return A;
}

template<class K, class T>
std::shared_ptr<TNode<K,T>> AVL_Tree<K,T>::rotateLR(std::shared_ptr<TNode<K,T>> A){
    std::shared_ptr<TNode<K,T>> node = rotateRR(A);
    std::shared_ptr<TNode<K,T>>to_return = rotateLL(node);
    to_return = to_return != root ? to_return->parent : to_return;
    return to_return;
}

template<class K, class T>
std::shared_ptr<TNode<K,T>> AVL_Tree<K,T>::rotateRL(std::shared_ptr<TNode<K,T>> A){
    std::shared_ptr<TNode<K,T>> node = rotateLL(A);
    std::shared_ptr<TNode<K,T>> to_return = rotateRR(node);
    to_return = to_return != root ? to_return->parent : to_return;
    return to_return;
}

template<class K, class T>
AVL_Tree<K,T>::~AVL_Tree<K, T>() {
    clearTree();
}

template<class K, class T>
void AVL_Tree<K,T>::print(std::shared_ptr<TNode<K,T>> root)
{
    if(root == nullptr) {
        return;
    }
    print(root->left);
    std::cout << root->getKey() << " BF: " << root->getBF()
              << " Height: " << root->getHeight() << std::endl;
    print(root->right);
}

template<class K, class T>
void AVL_Tree<K,T>::printTree(){
    print(root);
    std::cout << "-----------------" << std::endl;
}

template<class K, class T>
std::shared_ptr<TNode<K,T>> AVL_Tree<K,T>::getRoot() const {
    return root;
}

template<class K, class T>
void AVL_Tree<K,T>::clearTree(){
    recursiveDelete(root);
    left_most = nullptr;
    right_most = nullptr;
    _size = 0;
}



template<class K, class T>
void AVL_Tree<K,T>::recursiveDelete(std::shared_ptr<TNode<K,T>> to_delete)
{
    if(to_delete == nullptr) {
        return;
    }
    // post order deleting
    recursiveDelete(to_delete->left);
    recursiveDelete(to_delete->right);
    if(to_delete == root) {
        root = nullptr;
    }
    // im a leaf
    if(to_delete->parent != nullptr) {
        if(to_delete->parent->left == to_delete) {
            to_delete->parent->left = nullptr;
        }
        else {
            to_delete->parent->right = nullptr;
        }
    }
    to_delete->parent = nullptr;
}

template<class K, class T>
void AVL_Tree<K,T>::setLeftMost(std::shared_ptr<TNode<K, T>> _left_most) {
    left_most = _left_most;
}

template<class K, class T>
void AVL_Tree<K,T>::setRightMost(std::shared_ptr<TNode<K, T>> _right_most) {
    right_most = _right_most;
}

template<class K, class T>
std::shared_ptr<TNode<K,T>> AVL_Tree<K,T>::getRightMost() const{
    return right_most;
}

template<class K, class T>
std::shared_ptr<TNode<K,T>> AVL_Tree<K,T>::getLeftMost() const{
    return left_most;
}

template<class K, class T>
bool AVL_Tree<K,T>::isEmpty() const
{
   // return root != nullptr;
   return _size == 0;
}








#endif //HW1_WET_AVL_TREE_H
