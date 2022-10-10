
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
    Node(Value& _val, Node* up = nullptr)
        : val(*(new Value(_val))), height(0), bf(0), left(nullptr), right(nullptr), up(up){};
    // Node(const& Node other) = default;
    ~Node() { delete &val; };
};
}  // namespace DS
#endif