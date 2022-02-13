#ifndef RBBST_H
#define RBBST_H

#include "bst.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <exception>
#include <iostream>

struct KeyError {};

/**
 * A special kind of node for an AVL tree, which adds the height as a data member, plus
 * other additional helper functions. You do NOT need to implement any functionality or
 * add additional data members or helper functions.
 */
template<typename Key, typename Value>
class AVLNode : public Node<Key, Value> {
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int getHeight() const;
    void setHeight(int height);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int height_;
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
 * An explicit constructor to initialize the elements by calling the base class constructor and setting
 * the color to red since every new node will be red when it is first inserted.
 */
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent)
        : Node<Key, Value>(key, value, parent), height_(1) {}

/**
 * A destructor which does nothing.
 */
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode() {}

/**
 * A getter for the height of a AVLNode.
 */
template<class Key, class Value>
int AVLNode<Key, Value>::getHeight() const {
    return height_;
}

/**
 * A setter for the height of a AVLNode.
 */
template<class Key, class Value>
void AVLNode<Key, Value>::setHeight(int height) {
    height_ = height;
}

/**
 * An overridden function for getting the parent since a static_cast is necessary to make sure
 * that our node is a AVLNode.
 */
template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getParent() const {
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
 * Overridden for the same reasons as above.
 */
template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getLeft() const {
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
 * Overridden for the same reasons as above.
 */
template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getRight() const {
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}

/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/

template<class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value> {
public:
    virtual void insert(const std::pair<const Key, Value>& new_item);  // TODO
    virtual void remove(const Key& key);                               // TODO
protected:
    virtual void nodeSwap(AVLNode<Key, Value>* n1, AVLNode<Key, Value>* n2);

    // Add helper functions here
    void leftRotate(AVLNode<Key, Value>* node);
    void rightRotate(AVLNode<Key, Value>* node);
    void balance(AVLNode<Key, Value>* node);
    void removeUpdHeights(AVLNode<Key, Value>* node);
};

template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value>& new_item) {
    // TODO

    Node<Key, Value>* temp
            = BinarySearchTree<Key, Value>::internalFind(new_item.first);  // check if the node in the tree
    if (temp != nullptr) {
        temp->getItem().second = new_item.second;  // replace the value
        return;
    }

    if (this->root_ == nullptr) {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        return;
    }

    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);  // start from the root

    while (1) {
        // if the item's key > the current node's key, move to the right
        if (new_item.first > curr->getItem().first) {
            if (curr->getRight() == nullptr) {
                curr->setRight(new AVLNode<Key, Value>(new_item.first, new_item.second, curr));
                curr = curr->getRight();
                break;
            }
            curr = curr->getRight();  // advance to the right child
        }
        // if the item's key < the current node's key, move to the left
        else {
            if (curr->getLeft() == nullptr) {
                curr->setLeft(new AVLNode<Key, Value>(new_item.first, new_item.second, curr));
                curr = curr->getLeft();
                break;
            }
            curr = curr->getLeft();  // advance to the left child
        }
    }

    curr->setHeight(1);
    curr = curr->getParent();
    AVLNode<Key, Value>* tempr = curr;

    while (tempr != nullptr) {
        if (tempr->getHeight() < tempr->getHeight() + 1) {
            tempr->setHeight(tempr->getHeight() + 1);  // update all parents
        }
        tempr = tempr->getParent();
    }

    // this->print();
    balance(curr);  // balance the updated tree
}

template<class Key, class Value>
void AVLTree<Key, Value>::balance(AVLNode<Key, Value>* node) {
    AVLNode<Key, Value>* z = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::doComparison(node));

    if (z != nullptr) {
        int right_child_height, left_child_height, right_right_child_height, right_left_child_height,
                left_left_child_height, left_right_child_height;

        if (z->getRight() == nullptr) {
            right_child_height = 0;
            right_right_child_height = 0;
            right_left_child_height = 0;
        } else {
            right_child_height = z->getRight()->getHeight();

            if (z->getRight()->getRight() == nullptr)
                right_right_child_height = 0;
            else
                right_right_child_height = z->getRight()->getRight()->getHeight();

            if (z->getRight()->getLeft() == nullptr)
                right_left_child_height = 0;
            else
                right_left_child_height = z->getRight()->getLeft()->getHeight();
        }

        if (z->getLeft() == nullptr) {
            left_child_height = 0;
            left_left_child_height = 0;
            left_right_child_height = 0;
        } else {
            left_child_height = z->getLeft()->getHeight();

            if (z->getLeft()->getLeft() == nullptr)
                left_left_child_height = 0;
            else
                left_left_child_height = z->getLeft()->getLeft()->getHeight();

            if (z->getLeft()->getRight() == nullptr)
                left_right_child_height = 0;
            else
                left_right_child_height = z->getLeft()->getRight()->getHeight();
        }

        /* case 1: left rotate on z */
        if (right_child_height > left_child_height && right_right_child_height >= right_left_child_height) {
            leftRotate(z);
        }
        /* case 2: right rotate on z */
        else if (left_child_height > right_child_height && left_left_child_height >= left_right_child_height) {
            rightRotate(z);
        }
        /* case 3: right rotate on y, then left rotate on z */
        else if (right_child_height > left_child_height && right_left_child_height > right_right_child_height) {
            rightRotate(z->getRight());
            leftRotate(z);
        }
        /* case 4: left rotate on y, then right rotate on z */
        else if (left_child_height > right_child_height && left_right_child_height > left_left_child_height) {
            leftRotate(z->getLeft());
            rightRotate(z);
        }
    }
}

template<typename Key, typename Value>
// template<class Key, class Value>
void AVLTree<Key, Value>::rightRotate(AVLNode<Key, Value>* z) {
    AVLNode<Key, Value>* orphaned_child = z->getLeft()->getRight();
    AVLNode<Key, Value>* y = z->getLeft();

    int z_right_child_height, orphaned_height;

    if (z->getRight() == nullptr)
        z_right_child_height = 0;
    else
        z_right_child_height = z->getRight()->getHeight();

    if (orphaned_child == nullptr)
        orphaned_height = 0;
    else {
        orphaned_height = orphaned_child->getHeight();
        orphaned_child->setParent(z);
    }

    // set the new parent for y
    y->setParent(z->getParent());

    // set the new child for z's parent
    if (z->getParent() != nullptr) {
        if (z->getParent()->getRight() == z) {
            z->getParent()->setRight(y);
        } else {
            z->getParent()->setLeft(y);
        }
    }

    // set parent for z
    z->setParent(y);

    // set the new right child for y
    y->setRight(z);

    // set the new left child for z
    z->setLeft(orphaned_child);

    // update z's height
    z->setHeight(std::max(orphaned_height, z_right_child_height) + 1);

    if (z == this->root_)
        this->root_ = y;
}

template<typename Key, typename Value>
// template<class Key, class Value>
void AVLTree<Key, Value>::leftRotate(AVLNode<Key, Value>* z) {
    AVLNode<Key, Value>* orphaned_child = z->getRight()->getLeft();
    AVLNode<Key, Value>* y = z->getRight();

    int z_left_child_height, orphaned_height;

    if (z->getLeft() == nullptr)
        z_left_child_height = 0;
    else
        z_left_child_height = z->getLeft()->getHeight();

    if (orphaned_child == nullptr)
        orphaned_height = 0;
    else {
        orphaned_height = orphaned_child->getHeight();
        orphaned_child->setParent(z);
    }

    // set the new parent for y
    y->setParent(z->getParent());

    // set the new child for z's parent
    if (z->getParent() != nullptr) {
        if (z->getParent()->getRight() == z) {
            z->getParent()->setRight(y);
        } else {
            z->getParent()->setLeft(y);
        }
    }

    // set parent for z
    z->setParent(y);

    // set the new left child for y
    y->setLeft(z);

    // set the new right child for z
    z->setRight(orphaned_child);

    // update z's height
    z->setHeight(std::max(orphaned_height, z_left_child_height) + 1);

    if (z == this->root_)
        this->root_ = y;
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeUpdHeights(AVLNode<Key, Value>* node) {
    if (node == nullptr)
        return;

    int right_child_height, left_child_height;

    if (node->getRight() == nullptr)
        right_child_height = 0;
    else
        right_child_height = node->getRight()->getHeight();

    if (node->getLeft() == nullptr)
        left_child_height = 0;
    else
        left_child_height = node->getLeft()->getHeight();

    node->setHeight(std::max(right_child_height, left_child_height) + 1);
    removeUpdHeights(node->getParent());
}

template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key) {
    // TODO
    AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(
            BinarySearchTree<Key, Value>::internalFind(key));  // check if the node in the tree
    // if not in the tree
    if (temp == nullptr)
        return;

    AVLNode<Key, Value>* parent = temp->getParent();
    // if root without any children
    if (temp->getRight() == nullptr && temp->getLeft() == nullptr && parent == nullptr) {
        this->root_ = nullptr;
        delete temp;
        return;
    }
    // if root with only right child
    else if (temp->getRight() != nullptr && temp->getLeft() == nullptr && parent == nullptr) {
        this->root_ = temp->getRight();
        delete temp;
        return;
    }
    // if root with only left child
    else if (temp->getRight() == nullptr && temp->getLeft() != nullptr && parent == nullptr) {
        this->root_ = temp->getLeft();
        delete temp;
        return;
    }
    // if leaf node
    else if (temp->getRight() == nullptr && temp->getLeft() == nullptr) {
        if (parent->getRight() == temp) {
            parent->setRight(nullptr);
            if (parent->getLeft() == nullptr)
                parent->setHeight(1);
            else
                parent->setHeight(parent->getLeft()->getHeight() + 1);
            removeUpdHeights(parent->getParent());
        } else {
            parent->setLeft(nullptr);
            if (parent->getRight() == nullptr)
                parent->setHeight(1);
            else
                parent->setHeight(parent->getRight()->getHeight() + 1);
            removeUpdHeights(parent->getParent());
        }
        delete temp;
        balance(parent);
        return;
    }
    // if has 1 left child
    else if (temp->getRight() == nullptr && temp->getLeft() != nullptr) {
        int right_child_height, left_child_height;

        if (parent->getRight() == temp) {
            parent->setRight(temp->getLeft());
        } else {
            parent->setLeft(temp->getLeft());
        }

        if (parent->getRight() == nullptr)
            right_child_height = 0;
        else
            right_child_height = parent->getRight()->getHeight();

        if (parent->getLeft() == nullptr)
            left_child_height = 0;
        else
            left_child_height = parent->getLeft()->getHeight();

        temp->getLeft()->setParent(temp->getParent());
        parent->setHeight(std::max(right_child_height, left_child_height) + 1);
        removeUpdHeights(parent->getParent());
        delete temp;
        balance(parent);
        return;
    }
    // if has 1 right child
    else if (temp->getRight() != nullptr && temp->getLeft() == nullptr) {
        int right_child_height, left_child_height;

        if (parent->getRight() == temp) {
            parent->setRight(temp->getRight());
        } else {
            parent->setLeft(temp->getRight());
        }

        if (parent->getRight() == nullptr)
            right_child_height = 0;
        else
            right_child_height = parent->getRight()->getHeight();

        if (parent->getLeft() == nullptr)
            left_child_height = 0;
        else
            left_child_height = parent->getLeft()->getHeight();

        temp->getRight()->setParent(parent);
        parent->setHeight(std::max(right_child_height, left_child_height) + 1);
        removeUpdHeights(parent->getParent());
        delete temp;
        balance(parent);
        return;
    }
    // if 2 children
    else {
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(temp));
        nodeSwap(temp, pred);
    }
    remove(temp->getKey());  // recursive call
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap(AVLNode<Key, Value>* n1, AVLNode<Key, Value>* n2) {
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int tempH = n1->getHeight();
    n1->setHeight(n2->getHeight());
    n2->setHeight(tempH);
}

#endif
