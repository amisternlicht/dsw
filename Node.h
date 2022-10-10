
#ifndef NODE_H
#define NODE_H
namespace DS {
template <class Value>
class Node {
   public:
    Value& val;
    int height;
    int bf;
    Node<Value>* left;
    Node<Value>* right;
    Node<Value>* up;
    //some nodes need to allocate additional structures in val
    Node(Value& _val, Node* up = nullptr)
        : val(*(new Value(_val))), height(0), bf(0), left(nullptr), right(nullptr), up(up){};
    ~Node() { delete &val; };
};
}  // namespace DS
#endif