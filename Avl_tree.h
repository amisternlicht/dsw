#include "Node.h"
#include "library.h"

#ifndef AVLTREE_H
#define AVLTREE_H
namespace DS {
// Id is either 'CarType' or 'Model'
template <class Value, class Cmp>
class Tree {
   public:
    const Cmp& Compare;
    // Value& val;

    Node<Value>* min;
    Node<Value>* max;
    Node<Value>* root;

    Tree(){};
    Tree(const Cmp& _compare)
        : /* val(*(new Value(_val))),*/ Compare(_compare), min(nullptr), max(nullptr), root(nullptr){};
    /* Tree(const Cmp& _compare, Node<Value>* sortedNodes[], int nodesCount)
            : Compare(*(new Cmp(_compare))), min(nullptr), max(nullptr), root(nullptr)
    {
            if (nodesCount < 1) return;
            min = sortedNodes[0];
            max = sortedNodes[nodesCount - 1];
            root = insertArray(sortedNodes, 0, nodesCount - 1);
    };*/
    ~Tree() { /*delete &Compare;*/ }
    void Roll(Node<Value>* to_roll)
    {
        if (to_roll->bf == 2) {
            if ((to_roll->left)->bf == -1) {
                RollLR(to_roll);
            }
            else
                RollLL(to_roll);
        }
        else if (to_roll->bf == -2) {
            if ((to_roll->right)->bf == 1) {
                RollRL(to_roll);
            }
            else
                RollRR(to_roll);
        }
    };
    void RollLL(Node<Value>* A)
    {
        // pick up left side
        Node<Value>* B = A->left;
        // changes A node
        A->left = B->right;
        if (A->left) A->left->up = A;
        B->right = A;

        if (A->up) {
            if (A->up->left == A) {
                A->up->left = B;
            }
            if (A->up->right == A) {
                A->up->right = B;
            }
            B->up = A->up;
        }
        else {
            root = B;
            B->up = nullptr;
        }
        A->up = B;
        // update heights
        Verify(A);
    };
    void RollRR(Node<Value>* A)
    {
        // pick up right side
        Node<Value>* B = A->right;
        // changes A node
        A->right = B->left;
        if (A->right) A->right->up = A;
        B->left = A;
        if (A->up) {
            B->up = A->up;
            if (A->up->left == A) {
                A->up->left = B;
            }
            if (A->up->right == A) {
                A->up->right = B;
            }
        }
        else {
            root = B;
            B->up = nullptr;
        }
        A->up = B;
        // update heights
        Verify(A);
    };
    void RollRL(Node<Value>* A)
    {
        // temp to roll
        Node<Value>* C = (A->right)->left;
        Node<Value>* B = A->right;

        // moving sons of LR node which will be top
        // B left son will be C's right son
        B->left = C->right;
        if (B->left) B->left->up = B;
        // A right son will be C's left son
        A->right = C->left;
        if (A->right) A->right->up = A;
        // moving C node to Top

        C->right = B;
        C->left = A;

        // change other nodes to point to top
        B->up = C;
        if (A->up == nullptr) {
            root = C;
            A->up = C;
            C->up = nullptr;
        }
        else {
            if (A->up->left == A) {
                A->up->left = C;
            }
            if (A->up->right == A) {
                A->up->right = C;
            }
            C->up = A->up;
            A->up = C;
        }

        // update heights
        int hl = A->left != nullptr ? A->left->height : -1;
        int hr = A->right != nullptr ? A->right->height : -1;
        A->height = hl > hr ? hl + 1 : hr + 1;
        A->bf = hl - hr;
        Verify(B);
    };
    void RollLR(Node<Value>* A)
    {
        // nodes to roll
        Node<Value>* C = (A->left)->right;
        Node<Value>* B = A->left;

        // moving sons of LR node which will be top
        // B left son will be C's right son
        B->right = C->left;
        A->left = C->right;
        C->right = A;
        C->left = B;

        // Link upstream neighbors
        if (A->up == nullptr) {
            root = C;
            A->up = C;
            C->up = nullptr;
        }
        else {
            if (A->up->left == A) {
                A->up->left = C;
            }
            if (A->up->right == A) {
                A->up->right = C;
            }
            C->up = A->up;
            A->up = C;
        }

        B->up = C;
        if (A->left) A->left->up = A;
        if (B->left) B->left->up = B;
        if (A->right) A->right->up = A;
        if (B->right) B->right->up = B;

        int hl = A->left != nullptr ? A->left->height : -1;
        int hr = A->right != nullptr ? A->right->height : -1;
        A->height = hl > hr ? hl + 1 : hr + 1;
        A->bf = hl - hr;
        Verify(B);
    };
    // builds an inner tree from a sorted array by recursion algorithm in O(n)
    Node<Value>* insertArray(int typeID, int start, int end)
    {
        if (start > end) {
            return nullptr;
        }
        int mid = (start + end) / 2;
        Value val(typeID, mid);
        Node<Value>* to_add = new Node<Value>(val);
        to_add->left = insertArray(typeID, start, mid - 1);
        if (to_add->left) {
            to_add->left->up = to_add;
        }
        to_add->right = insertArray(typeID, mid + 1, end);
        if (to_add->right) {
            to_add->right->up = to_add;
        }
        int hl = to_add->left != nullptr ? to_add->left->height : -1;
        int hr = to_add->right != nullptr ? to_add->right->height : -1;
        to_add->height = hl > hr ? hl + 1 : hr + 1;
        to_add->bf = hl - hr;

        return to_add;
    };
    StatusType AddNode(Node<Value>* node)
    {
        // if tree is empty
        if (root == nullptr) {
            root = node;
            max = node;
            min = node;
            return SUCCESS;
        }

        Node<Value>* p = root;
        bool nodeAdded = false;
        while (!nodeAdded) {
            if (Compare(p->val, node->val) <= 0)  // if p<node
            {
                p->bf--;
                if (p->right != nullptr) {  // keep going down
                    p = p->right;
                }
                else {  // add as right child
                    p->right = node;
                    node->up = p;
                    if (p == max) {
                        max = node;
                    }
                    nodeAdded = true;
                }
            }
            else if (Compare(p->val, node->val) > 0)  // if p>node
            {
                p->bf++;
                if (p->left != nullptr) {  // keep going down
                    p = p->left;
                }
                else {  // add as left child
                    p->left = node;
                    node->up = p;
                    if (p == min) {
                        min = node;
                    }
                    nodeAdded = true;
                }
            }
        }
        Verify(p);
        return SUCCESS;
    }

    StatusType Remove(const Value& val)
    {
        bool nodeFound = false;
        // if tree is empty
        if (root == nullptr) {
            return INVALID_INPUT;
        }
        Node<Value>* parent = nullptr;
        Node<Value>* p = root;
        // find node
        while (p != nullptr) {
            if (Compare(val, p->val) > 0) {  // if node > p
                if (p->right != nullptr)
                    p = p->right;
                else
                    return FAILURE;
            }
            else if (Compare(val, p->val) < 0) {  // if node < p
                if (p->left != nullptr)
                    p = p->left;
                else
                    return FAILURE;
            }
            else if (Compare(val, p->val) == 0) {  // if node = p
                nodeFound = true;
                parent = p->up;
                if (p->right || p->left) {
                    // if left child exists - check if max
                    // we defined the swap to the left son first if has both sons
                    if (p->left != nullptr) {
                        if (max == p) {
                            Node<Value>* newMax = p->left;
                            while (newMax->right != nullptr) newMax = newMax->right;
                            max = newMax;
                        }
                        Node<Value>* replacement = p->left;
                        while (replacement->right != nullptr) replacement = replacement->right;
                        Swap(replacement, p);
                        if (root == p) {
                            root = replacement;
                        }
                        // if has left son change responsibility to parent
                        /*if (p->left != nullptr) {
                            p->up->right = p->left;
                        }
                        else {
                            p->left = nullptr;
                        }*/
                    }
                    // if right child exists - check if min
                    else if (p->right != nullptr) {
                        if (min == p) {
                            Node<Value>* newMin = p->right;
                            if (newMin->left != nullptr) newMin = p->left;
                            min = newMin;
                        }
                        Node<Value>* replacement = p->right;
                        while (replacement->left != nullptr) replacement = replacement->left;
                        Swap(replacement, p);
                        if (root == p) {
                            root = replacement;
                        }
                        /*if (p->right != nullptr) {
                            p->up->left = p->right;
                        }
                        else {
                            p->right = nullptr;
                        }*/
                    }
                }
                // if leaf - check if min, max or root
                else {
                    if (min == p) min = p->up;
                    if (max == p) max = p->up;

                    parent = p->up;
                    if (parent != nullptr) {
                        if (parent->right == p) parent->right = nullptr;
                        if (parent->left == p) parent->left = nullptr;
                    }
                    else {
                        root = nullptr;
                    }

                    delete p;
                    p = nullptr;

                }
                
            }
        }
        Verify(parent);
        if (nodeFound)
            return SUCCESS;
        else
            return FAILURE;
    }
    /*
    Climbs up the tree and verifies balance factor for
    each node up the chain. Rolls if necessary.
    */
    void Verify(Node<Value>* p)
    {
        while (p != nullptr) {
            int hl = p->left != nullptr ? p->left->height : -1;
            int hr = p->right != nullptr ? p->right->height : -1;
            p->height = hl > hr ? hl + 1 : hr + 1;
            p->bf = hl - hr;
            Roll(p);
            p = p->up;
        }
    }
    /*
     * Searches for node with matching Value
     * returns pointer to that node
     * if such node does not exists returns NULL
     */
    Node<Value>* find(const Value& val)
    {
        Node<Value>* p = root;
        while (p != nullptr) {
            if (Compare(val, p->val) > 0) {  // if node > p
                p = p->right;
            }
            else if (Compare(val, p->val) < 0) {  // if node < p
                p = p->left;
            }
            else if (Compare(val, p->val) == 0) {  // if node = p
                return p;
            }
        }
        return p;
    }

    Node<Value>* customFind(const Value& val, int (*compare)(const Value& v1, const Value& v2))
    {
        Node<Value>* p = root;
        while (p != nullptr) {
            if (compare(val, p->val) > 0) {  // if node > p
                p = p->right;
            }
            else if (compare(val, p->val) < 0) {  // if node < p
                p = p->left;
            }
            else if (compare(val, p->val) == 0) {  // if node = p
                return p;
            }
        }
        return p;
    }

    Node<Value>* getNext(Node<Value>* curr)
    {
        if (curr == nullptr) return nullptr;

        if (curr->right != nullptr) {
            Node<Value>* p = curr->right;
            while (p->left != nullptr) {
                p = p->left;
            }
            return p;
        }
        // stop when we came from left son
        Node<Value>* tmp = curr->up;
        while (tmp != nullptr && curr == tmp->right) {
            curr = tmp;
            tmp = tmp->up;
        }
        curr = tmp;

        return curr;
    }
    Node<Value>* getPrev(Node<Value>* curr)
    {
        if (curr == nullptr) return nullptr;
        // if has no left son
        if (curr->left != nullptr) {
            Node<Value>* p = curr->left;
            while (p->right != nullptr) {
                p = p->right;
            }
            return p;
        }
        // stop when we came from right son
        Node<Value>* tmp = curr->up;
        while (tmp != nullptr && curr == tmp->left) {
            curr = tmp;
            tmp = tmp->up;
        }
        curr = tmp;

        return curr;
    }
    /*
    Swaps the location of two nodes in the tree
    */
    void Swap(Node<Value>* a, Node<Value>* b)
    {
        // swap between nodes
        // check if nodes are neighbors
        Node<Value>* tmp = a->up;
        if (a->up == b) {
            a->up = b->up;
            b->up = a;
        }
        else {
            if (b->up == a) {
                b->up = a->up;
                a->up = b;
            }
            else {
                a->up = b->up;
                b->up = tmp;
            }
        }

        tmp = a->right;
        if (a->right == b) {
            a->right = b->right;
            b->right = a;
        }
        else {
            if (b->right == a) {
                b->right = a->right;
                a->right = b;
            }
            else {
                a->right = b->right;
                b->right = tmp;
            }
        }
        tmp = a->left;
        if (a->left == b) {
            a->left = b->left;
            b->left = a;
        }
        else {
            if (b->left == a) {
                b->left = a->left;
                a->left = b;
            }
            else {
                a->left = b->left;
                b->left = tmp;
            }
        }

        // fix neighbors
        // fix parents
        if (a->up) {
            // if was left son
            if (a->up->left && a->up->left == b) {
                a->up->left = a;
            }
            // if was right son
            else if (a->up->right && a->up->right == b) {
                a->up->right = a;
            }
        }
        if (b->up) {
            // if was left son
            if (b->up->left && b->up->left == a) {
                b->up->left = b;
            }
            // if was right son
            else if (b->up->right && b->up->right == a) {
                b->up->right = b;
            }
        }
        if (b->right) {
            // change right to new node
            b->right->up = b;
        }
        if (b->left) {
            // change left to new node
            b->left->up = b;
        }
        if (a->right) {
            // change right to new node
            a->right->up = a;
        }
        if (a->left) {
            // change left to new node
            a->left->up = a;
        }
    }
};

}  // namespace DS
#endif
