/*
 * Дано число N < 10^6 и последовательность пар целых чисел из [-2^31..2^31]
 * длиной N. Построить декартово дерево из N узлов, характеризующихся парами
 * чисел {Xi, Yi}. Каждая пара чисел {Xi, Yi} определяет ключ Xi и приоритет
 * Yi в декартовом дереве. Добавление узла в декартово дерево выполняйте
 * второй версией алгоритма, рассказанного на лекции:
 * При добавлении узла (x, y) выполняйте спуск по ключу до узла P с меньшим
 * приоритетом. Затем разбейте найденное поддерево по ключу x так, чтобы в
 * первом поддереве все ключи меньше x, а во втором больше или равны x.
 * Получившиеся два дерева сделайте дочерними для нового узла (x, y). Новый
 * узел вставьте на место узла P.
 * Построить также наивное дерево поиска по ключам Xi методом из задачи 2.
 * 3_1. Вычислить разницу глубин наивного дерева поиска и декартового дерева. 
 * Разница может быть отрицательна.
 */

#include <iostream>
#include <cassert>
#include <queue>
#include <vector>

#define PARENT 0
#define LEFT 1
#define RIGHT -1

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
    int priority;
    T value;
    Node() : left(nullptr), right(nullptr), priority(0) {}
    Node(const T &v) : 
        left(nullptr), right(nullptr), priority(0), value(v) {}
    Node(const T &v, int p) :
        left(nullptr), right(nullptr), priority(p), value(v) {}
};

template <class T>
class BinTree
{
public:
    explicit BinTree(bool (*cmp)(const T&, const T&) = cmp_default);
    ~BinTree();

    void add(const T &value);
    int depth_max(void);

private:
    Node<T> *root;
    bool (*cmp_func)(const T&, const T&);
};

template <class T>
class Treap
{
public:
    explicit Treap(bool (*cmp)(const T&, const T&) = cmp_default);
    ~Treap();

    void add(const T &value, int priority);
    int depth_max(void);

private:
    Node<T> *root;
    bool (*cmp_func)(const T&, const T&);

    void split(Node<T> *node, const T &key, Node<T> **t1, Node<T> **t2);
};

int main(void)
{
    int n = 0;
    cin >> n;
    assert(n > 0);
    BinTree<int> bin_tree;
    Treap<int> treap;
    for (int i = 0; i < n; i++)
    {
        int value = 0, priority = 0;
        cin >> value >> priority;
        treap.add(value, priority);
        bin_tree.add(value);
    }
    cout << bin_tree.depth_max() - treap.depth_max() << endl;
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
int BinTree<T>::depth_max(void)
{
    if (!root)
        return 0;

    vector<Node<T>*> stack = { root };
    Node<T> *node = nullptr;
    int where = LEFT;
    int max = 0, tmp = 1;
    while (!stack.empty())
    {
        node = stack.back();
        if (where == LEFT)
        {
            if (node->left)
            {
                stack.push_back(node->left);
                tmp++;
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
                tmp++;
            }
            else
                where = PARENT;
        }
        else if (where == PARENT)
        {
            if (tmp > max)
                max = tmp;
            tmp--;
            stack.pop_back();
            if (!stack.empty() && node == stack.back()->left)
                where = RIGHT;
        }
    }
    return max;
}

template <class T>
Treap<T>::Treap(bool (*cmp)(const T&, const T&)) :
    root(nullptr), cmp_func(cmp) {}

template <class T>
Treap<T>::~Treap()
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
void Treap<T>::split(Node<T> *node, const T &key, Node<T> **t1, Node<T> **t2)
{
    assert(t1 && t2);
    if (!node)
    {
        *t1 = nullptr;
        *t2 = nullptr;
    }
    else if (cmp_func(key, node->value))
    {
        split(node->left, key, t1, &(node->left));
        *t2 = node;
    }
    else
    {
        split(node->right, key, &(node->right), t2);
        *t1 = node;
    }
}

template <class T>
void Treap<T>::add(const T &value, int priority)
{
    Node<T> *new_node = new Node<T>(value, priority);
    if (!root)
    {
        root = new_node;
        return;
    }

    Node<T> *node = root, *prev = nullptr;
    while (node && priority <= node->priority)
    {
        prev = node;
        if (cmp_func(value, node->value))
            node = node->left;
        else
            node = node->right;
    }

    Node<T> *left = nullptr, *right = nullptr;
    split(node, value, &left, &right);
    new_node->left = left;
    new_node->right = right;

    if (prev)
    {
        if (cmp_func(value, prev->value))
        {
            prev->left = new_node;
        }
        else
        {
            prev->right = new_node;
        }
    }
    else
        root = new_node;
}

template <class T>
int Treap<T>::depth_max(void)
{
    if (!root)
        return 0;

    vector<Node<T>*> stack = { root };
    Node<T> *node = nullptr;
    int where = LEFT;
    int max = 0, tmp = 1;
    while (!stack.empty())
    {
        node = stack.back();
        if (where == LEFT)
        {
            if (node->left)
            {
                stack.push_back(node->left);
                tmp++;
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
                tmp++;
            }
            else
                where = PARENT;
        }
        else if (where == PARENT)
        {
            if (tmp > max)
                max = tmp;
            tmp--;
            stack.pop_back();
            if (!stack.empty() && node == stack.back()->left)
                where = RIGHT;
        }
    }
    return max;
}
