#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    virtual void rotateLeft(AVLNode<Key,Value>* node);
    virtual void rotateRight(AVLNode<Key,Value>* node);
    virtual void removeFix(AVLNode<Key,Value>* node, int diff);
    virtual void insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* child);


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
     // TODO
    Node<Key, Value>* curr = BinarySearchTree<Key, Value>::root_;
    if (curr == NULL){
        BinarySearchTree<Key, Value>::root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        return;
    }

    if (BinarySearchTree<Key, Value>::internalFind(new_item.first)){
        Node<Key, Value>* c = BinarySearchTree<Key, Value>::internalFind(new_item.first);
        c->setValue(new_item.second);
        return;
    }

    AVLNode<Key, Value>* curr_temp_node = static_cast<AVLNode<Key, Value>*>(curr);

    while (true){
        if (new_item.first < curr_temp_node->getKey()){
            if (curr_temp_node->getLeft() == NULL){
                AVLNode<Key, Value>* new_node = new AVLNode<Key, Value>(new_item.first, new_item.second, curr_temp_node);
                curr_temp_node->setLeft(new_node);
                
                if(curr_temp_node->getBalance() == 1){
                    curr_temp_node->updateBalance(-1);
                }
                else{
                    curr_temp_node->updateBalance(-1);
                    insertFix(curr_temp_node, new_node);
                }

                return;
            }
            curr_temp_node = curr_temp_node->getLeft();
        }
        else{
            if (curr_temp_node->getRight() == NULL){
                AVLNode<Key, Value>* new_node = new AVLNode<Key, Value>(new_item.first, new_item.second, curr_temp_node);
                curr_temp_node->setRight(new_node);
                
                if(curr_temp_node->getBalance() == -1){
                    curr_temp_node->updateBalance(1);
                }
                else{
                    curr_temp_node->updateBalance(1);
                    insertFix(curr_temp_node, new_node);
                }

                return;
            }
            curr_temp_node = curr_temp_node->getRight();
        }
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    if (BinarySearchTree<Key, Value>::internalFind(key) == NULL){
        return;
    }
    AVLNode<Key, Value>* nodeToRemove = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::root_);

    while(true){
        if (nodeToRemove->getKey() == key){
            break;
        }
        else if (nodeToRemove->getKey() < key){
            nodeToRemove = nodeToRemove->getRight();
        }
        else{
            nodeToRemove = nodeToRemove->getLeft();
        }
    }
 
    if (nodeToRemove->getLeft() != NULL && nodeToRemove->getRight() != NULL) {
        // Case 1: Node has two children
        AVLNode<Key, Value> *nodeToReplace = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(nodeToRemove));
        nodeSwap(nodeToRemove, nodeToReplace);
    } 

    AVLNode<Key, Value>* parent = nodeToRemove->getParent();
    int diff = 0;
    if (parent != NULL) {
        if (nodeToRemove == parent->getLeft()) {
            diff = 1;
        } 
        else {
            diff = -1;
        }
    }

    AVLNode<Key, Value>* child = NULL;
    if (nodeToRemove->getLeft() != NULL){
        child = nodeToRemove->getLeft();
    }
    else if (nodeToRemove->getRight() != NULL){
        child = nodeToRemove->getRight();
    }

    if (child == NULL){
        if(nodeToRemove->getParent() == NULL){
            BinarySearchTree<Key,Value>::root_ = NULL;
        }
        else if (nodeToRemove == nodeToRemove->getParent()->getLeft()){
            nodeToRemove->getParent()->setLeft(NULL);
        }
        else{
            nodeToRemove->getParent()->setRight(NULL);
        }
    }

    else{
        if(nodeToRemove->getParent() == NULL){
            BinarySearchTree<Key, Value>::root_ = child;
        }
        else if (nodeToRemove == nodeToRemove->getParent()->getLeft()){
            nodeToRemove->getParent()->setLeft(child);
        }
        else{
            nodeToRemove->getParent()->setRight(child);
        }

        child->setParent(nodeToRemove->getParent());
    }

    delete nodeToRemove;
    removeFix(parent, diff);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* child)
{
  // AHHHHHH
  if (parent == NULL || parent->getParent() == NULL){
    return;
  }


  AVLNode<Key, Value>* grandparent = parent->getParent();
  if (grandparent->getLeft() == parent){
    grandparent->updateBalance(-1);

    if (grandparent->getBalance() == 0){
        return;
    }
    else if (grandparent->getBalance() == -1){
        insertFix(grandparent, parent);
    }
    else if (grandparent->getBalance() == -2){
        if (parent->getBalance() == -1){
            rotateRight(grandparent);
            grandparent->setBalance(0);
            parent->setBalance(0);
        }
        else if (parent->getBalance() == 1){
            rotateLeft(parent);
            rotateRight(grandparent);

            if (child->getBalance() == -1){
                parent->setBalance(0);
                grandparent->setBalance(1);
                child->setBalance(0);
            }
            else if (child->getBalance() == 0){
                parent->setBalance(0);
                grandparent->setBalance(0);
                child->setBalance(0);
            }
            else if (child->getBalance() == 1){
                parent->setBalance(-1);
                grandparent->setBalance(0);
                child->setBalance(0);
            }
        }
    }
  }


  else if (grandparent->getRight() == parent){
    grandparent->updateBalance(1);

    if (grandparent->getBalance() == 0){
        return;
    }
    else if (grandparent->getBalance() == 1){
        insertFix(grandparent, parent);
    }
    else if (grandparent->getBalance() == 2){
        if (parent->getBalance() == 1){
            rotateLeft(grandparent);
            grandparent->setBalance(0);
            parent->setBalance(0);
        }
        else if (parent->getBalance() == -1){
            rotateRight(parent);
            rotateLeft(grandparent);

            if (child->getBalance() == 1){
                parent->setBalance(0);
                grandparent->setBalance(-1);
                child->setBalance(0);
            }
            else if (child->getBalance() == 0){
                parent->setBalance(0);
                grandparent->setBalance(0);
                child->setBalance(0);
            }
            else if (child->getBalance() == -1){
                parent->setBalance(1);
                grandparent->setBalance(0);
                child->setBalance(0);
            }
        }
    }
  }

}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key,Value>* node, int diff)
{
    if (node == NULL){
        return;
    }

    AVLNode<Key,Value>* parent = node->getParent();
    int ndiff = 0;
    if (parent != NULL){
        if (parent->getLeft() == node){
            ndiff = 1;
        }
        else{
            ndiff = -1;
        }
    }

    if (diff == -1){
        if ((node->getBalance() + diff) == -2){
            AVLNode<Key,Value>* c = node->getLeft();
            if (c->getBalance() == -1){
                rotateRight(node);
                node->setBalance(0);
                c->setBalance(0);
                
                removeFix(parent, ndiff);
            }

            else if (c->getBalance() == 0){
                rotateRight(node);
                node->setBalance(-1);
                c->setBalance(1);
            }

            else if (c->getBalance() == 1){
                AVLNode<Key,Value>* g = c->getRight();
                rotateLeft(c);
                rotateRight(node);

                if (g->getBalance() == 1){
                    node->setBalance(0);
                    c->setBalance(-1);
                    g->setBalance(0);
                }
                else if (g->getBalance() == 0){
                    node->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else if (g->getBalance() == -1){
                    node->setBalance(1);
                    c->setBalance(0);
                    g->setBalance(0);
                }

                removeFix(parent, ndiff);
            }
        }

        else if ((node->getBalance() + diff) == -1){
            node->setBalance(-1);
        }
        else if ((node->getBalance() + diff) == 0){
            node->setBalance(0);
            removeFix(parent, ndiff);
        }
    }

    else if (diff == 1){
        if ((node->getBalance() + diff) == 2){
            AVLNode<Key,Value>* c = node->getRight();
            if (c->getBalance() == 1){
                rotateLeft(node);
                node->setBalance(0);
                c->setBalance(0);
                
                removeFix(parent, ndiff);
            }

            else if (c->getBalance() == 0){
                rotateLeft(node);
                node->setBalance(1);
                c->setBalance(-1);
            }

            else if (c->getBalance() == -1){
                AVLNode<Key,Value>* g = c->getLeft();
                rotateRight(c);
                rotateLeft(node);

                if (g->getBalance() == -1){
                    node->setBalance(0);
                    c->setBalance(1);
                    g->setBalance(0);
                }
                else if (g->getBalance() == 0){
                    node->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else if (g->getBalance() == 1){
                    node->setBalance(-1);
                    c->setBalance(0);
                    g->setBalance(0);
                }

                removeFix(parent, ndiff);
            }
        }

        else if ((node->getBalance() + diff) == 1){
            node->setBalance(1);
        }
        else if ((node->getBalance() + diff) == 0){
            node->setBalance(0);
            removeFix(parent, ndiff);
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* node)
{
    if (node == NULL || node->getRight() == NULL){
        return;
    }
    AVLNode<Key,Value>* x = node->getRight();
    x->setParent(node->getParent());
    if (node->getParent() != NULL){
        if(node == node->getParent()->getLeft()){
            node->getParent()->setLeft(x);
        }
        else{
            node->getParent()->setRight(x);
        }
    }
    else{
        BinarySearchTree<Key, Value>::root_ = static_cast<Node<Key, Value>*>(x);
    }

    AVLNode<Key,Value>* T2 = x->getLeft();
    node->setRight(T2);
    if(T2 != NULL){
        T2->setParent(node);
    }
    x->setLeft(node);
    node->setParent(x);
}


template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* node)
{
    if (node == NULL || node->getLeft() == NULL){
        return;
    }
    AVLNode<Key,Value>* x = node->getLeft();
    x->setParent(node->getParent());
    if (node->getParent() != NULL){
        if(node == node->getParent()->getLeft()){
            node->getParent()->setLeft(x);
        }
        else{
            node->getParent()->setRight(x);
        }
    }
    else{
        BinarySearchTree<Key, Value>::root_ = static_cast<Node<Key, Value>*>(x);
    }

    AVLNode<Key,Value>* T2 = x->getRight();
    node->setLeft(T2);
    if(T2 != NULL){
        T2->setParent(node);
    }
    x->setRight(node);
    node->setParent(x);
}

#endif
