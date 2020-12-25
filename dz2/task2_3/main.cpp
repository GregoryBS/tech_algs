/*
 * Дано число N < 10^6 и последовательность целых чисел из [-2^31..2^31]
 * длиной N. Требуется построить бинарное дерево, заданное наивным порядком
 * вставки. Т.е., при добавлении очередного числа K в дерево с корнем root,
 * если root→Key ≤ K, то узел K добавляется в правое поддерево root; иначе
 * в левое поддерево root.
 * Требования: Рекурсия запрещена. Решение должно поддерживать передачу
 * функции сравнения снаружи.
 * 2_3. Выведите элементы в порядке post-order (снизу вверх).
 */

#include <iostream>
#include <cassert>
#include <queue>
#include <vector>

#define LEFT 1
#define RIGHT -1
#define PARENT 0

using namespace std;

template <class T>
bool cmp_default(const T &l, const T &r)
{
    return l < r;
}

template <class T>
struct Node
{
    Node *left;
    Node *right;
    T value;
    Node() : left(nullptr), right(nullptr) {}
    Node(const T &v) : 
        left(nullptr), right(nullptr), value(v) {}
};

template <class T>
class BinTree
{
public:
    explicit BinTree(bool (*cmp)(const T&, const T&) = cmp_default);
    ~BinTree();

    void add(const T &value);
    void in_depth(void (*handler)(const Node<T>*));

private:
    Node<T> *root;
    bool (*cmp_func)(const T&, const T&);
};

int main(void)
{
    int n = 0;
    cin >> n;
    assert(n > 0);
    BinTree<int> tree;
    for (int i = 0; i < n; i++)
    {
        int value;
        cin >> value;
        tree.add(value);
    }
    tree.in_depth([](const Node<int> *node){ cout << node->value << " "; });
    return 0;
}

template <class T>
BinTree<T>::BinTree(bool (*cmp)(const T&, const T&)) :
    root(nullptr), cmp_func(cmp) {}

template <class T>
BinTree<T>::~BinTree()
{
    if (!root)
        return;

    Node<T> *node = nullptr;
    queue<Node<T>*> q;
    q.push(root);
    while (!q.empty())
    {
        node = q.front();
        q.pop();
        if (node->left)
            q.push(node->left);
        if (node->right)
            q.push(node->right);
        delete node;
    }
}

template <class T>
void BinTree<T>::add(const T &value)
{
    Node<T> *node = root, *prev = nullptr;
    while (node)
    {
        prev = node;
        if (cmp_func(value, node->value))
            node = node->left;
        else
            node = node->right;
    }
    if (prev)
    {
        if (cmp_func(value, prev->value))
        {
            prev->left = new Node<T>(value);
        }
        else
        {
            prev->right = new Node<T>(value);
        }
    }
    else
        root = new Node<T>(value);
}

template <class T>
void BinTree<T>::in_depth(void (*handler)(const Node<T>*))
{
    if (!root)
        return;

	vector<Node<T>*> stack = { root };
    Node<T> *node = nullptr;
    int where = LEFT;
    while (!stack.empty())
    {
		node = stack.back();
        if (where == LEFT)
        {
            if (node->left)
            {
                stack.push_back(node->left);
            }
            else
                where = RIGHT;
        }
        else if (where == RIGHT)
        {
            if (node->right)
            {
                where = LEFT;
                stack.push_back(node->right);
            }
            else
				where = PARENT;
        }
        else if (where == PARENT)
        {
            handler(node);
			stack.pop_back();
            if (!stack.empty() && node == stack.back()->left)
				where = RIGHT;
        }        
    }
}
