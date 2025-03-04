#include <iostream>
using namespace std;
struct BinTree {
    int val;
    BinTree * left;
    BinTree * right;
};

BinTree * newTreeNode(int value) {
    BinTree *p = (BinTree*)malloc(sizeof(BinTree));
    p->val = value;
    p->left = p->right = NULL;
    return p;
}
void init(BinTree *&tree) {
    tree = newTreeNode(23);
    tree->left = newTreeNode(18);
    tree->right = newTreeNode(35);
    tree->left->left = newTreeNode(10);
    tree->left->right = newTreeNode(20);
    tree->right->left = newTreeNode(34);
    tree->right->right = newTreeNode(60);
}

void recurMiddle(BinTree *tree) {
    if (!tree) {
        return;
    }
    if (tree->left) { recurMiddle(tree->left);}
    cout << tree->val << endl;
    if (tree->right) {recurMiddle(tree->right);}
    return;
}

#include <stack>
BinTree * StackPopOrNull(std::stack<BinTree*> &stack) {
    if (stack.empty()) {
        return NULL;
    } else {
        BinTree *p = stack.top();
        stack.pop();
        return p;
    }
}
BinTree * StackTopOrNull(std::stack<BinTree*> stack) {
    if (stack.empty()) return NULL;
    return stack.top();
}

void nonRecurMiddle(BinTree *tree) {
std::stack<BinTree*> stack;
stack.push(tree);
BinTree *cur = stack.top();
while(!stack.empty() || cur) {
  if(cur == StackTopOrNull(stack)) { // cur == stack.top() means cur.left not accessed
    if (cur->left) {
      stack.push(cur->left);
      cur = cur->left;
      continue;
    } else {
      cur = StackPopOrNull(stack);
      continue;
    }
  } else { //cur.left accessed
    cout << cur->val << endl;
    if (cur->right) {   
      stack.push(cur->right);
      cur = cur->right;
      continue;
    } else {
      cur = StackPopOrNull(stack);
      continue;
    }
    continue;
  }
}
}

int main(int argc, char* argv[]) {
    cout << "--------begin--------" << endl;
    BinTree *root;
    init(root);
    recurMiddle(root);
    cout << "--------middle--------" << endl;
    nonRecurMiddle(root);
    cout << "--------end--------" << endl;
    return 0;
}
