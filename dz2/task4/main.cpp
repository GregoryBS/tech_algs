/*
 * Требование для всех вариантов Задачи 4
 * Решение должно поддерживать передачу функции сравнения снаружи.
 * 4_2. Порядковые статистики. Дано число N и N строк. Каждая строка содержит
 * команду добавления или удаления натуральных чисел, а также запрос на
 * получение k-ой порядковой статистики. Команда добавления числа A задается
 * положительным числом A, команда удаления числа A задается отрицательным
 * числом “-A”. Запрос на получение k-ой порядковой статистики задается
 * числом k. Требования: скорость выполнения запроса - O(log n).
 */

#include <iostream>
#include <cassert>
#include <queue>

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
    int height;
    int stat;
    T value;
    Node() : left(nullptr), right(nullptr), height(1), stat(1) {}
    Node(const T &v) :
        left(nullptr), right(nullptr), height(1), stat(1), value(v) {}
};

template <class T>
class AVLTree
{
public:
    explicit AVLTree(bool (*cmp)(const T&, const T&) = cmp_default);
    ~AVLTree();

    void add(const T &value);
    void remove(const T &value);
    const T& find(int key);

private:
    Node<T> *root;
    bool (*cmp_func)(const T&, const T&);

    int get_height(Node<T> *node);
    void height(Node<T> *node);

    int is_balance(Node<T> *node);
    Node<T>* balance(Node<T> *node);

    Node<T>* left_rotate(Node<T> *node);
    Node<T>* right_rotate(Node<T> *node);

    Node<T>* add_node(Node<T> *node, const T &value);
    Node<T>* remove_node(Node<T> *node, const T &value);

    Node<T>* remove_min(Node<T> *node, Node<T> **min);
    Node<T>* remove_max(Node<T> *node, Node<T> **max);

    int get_stat(Node<T> *node);
    void stat(Node<T> *node);
};

int main(void)
{
    int n = 10;
    cin >> n;
    assert(n > 0);
    AVLTree<int> tree;
    for (int i = 0; i < n; i++)
    {
        int num = 0, stat = 0;
        cin >> num >> stat;
        if (num > 0)
            tree.add(num);
        else
            tree.remove(-num);
        cout << tree.find(stat) << endl;
    }
    return 0;
}

template <class T>
AVLTree<T>::AVLTree(bool (*cmp)(const T&, const T&)) :
    root(nullptr), cmp_func(cmp) {}

template <class T>
AVLTree<T>::~AVLTree()
{
    if (root)
    {
        queue<Node<T>*> queue;
        queue.push(root);
        while (!queue.empty())
        {
            Node<T> *node = queue.front();
            queue.pop();
            if (node->left)
                queue.push(node->left);
            if (node->right)
                queue.push(node->right);
            delete node;
        }
    }
}

template <class T>
int AVLTree<T>::get_height(Node<T> *node)
{
    int height = 0;
    if (node)
        height = node->height;
    return height;
}

template <class T>
void AVLTree<T>::height(Node<T> *node)
{
    if (node)
    {
        int left = get_height(node->left), right = get_height(node->right);
        if (left > right)
            node->height = left + 1;
        else
            node->height = right + 1;
    }
}

template <class T>
int AVLTree<T>::is_balance(Node<T> *node)
{
    return get_height(node->left) - get_height(node->right);
}

template <class T>
Node<T>* AVLTree<T>::balance(Node<T> *node)
{
    height(node);
    stat(node);
    if (is_balance(node) == -2)
    {
        if (is_balance(node->right) > 0)
            node->right = right_rotate(node->right);
        return left_rotate(node);
    }
    if (is_balance(node) == 2)
    {
        if (is_balance(node->left) < 0)
            node->left = left_rotate(node->left);
        return right_rotate(node);
    }
    return node;
}

template <class T>
Node<T>* AVLTree<T>::left_rotate(Node<T> *node)
{
    assert(node);
    Node<T> *tmp = node->right;
    node->right = tmp->left;
    tmp->left = node;
    height(node);
    height(tmp);
    stat(node);
    stat(tmp);
    return tmp;
}

template <class T>
Node<T>* AVLTree<T>::right_rotate(Node<T> *node)
{
    assert(node);
    Node<T> *tmp = node->left;
    node->left = tmp->right;
    tmp->right = node;
    height(node);
    height(tmp);
    stat(node);
    stat(tmp);
    return tmp;
}

template <class T>
void AVLTree<T>::add(const T &value)
{
    root = add_node(root, value);
}

template <class T>
Node<T>* AVLTree<T>::add_node(Node<T> *node, const T &value)
{
    if (node)
    {
        if (node->value != value)
        {
            if (cmp_func(value, node->value))
                node->left = add_node(node->left, value);
            else
                node->right = add_node(node->right, value);
        }
        return balance(node);
    }
    else
    {
        return new Node<T>(value);
    }
}

template <class T>
void AVLTree<T>::remove(const T &value)
{
    root = remove_node(root, value);
}

template <class T>
Node<T>* AVLTree<T>::remove_node(Node<T> *node, const T &value)
{
    if (!node)
        return node;

    if (node->value != value)
    {
        if (cmp_func(value, node->value))
            node->left = remove_node(node->left, value);
        else
            node->right = remove_node(node->right, value);
    }
    else
    {
        Node<T> *left = node->left, *right = node->right;
        if (!left || !right)
        {
            delete node;
            return (!left ? right : left);
        }

        Node<T> *replace = nullptr;
        if (is_balance(node) > 0)
            node->left = remove_max(left, &replace);
        else
            node->right = remove_min(right, &replace);

        node->value = replace->value;
        delete replace;
    }
    return balance(node);
}

template <class T>
Node<T>* AVLTree<T>::remove_min(Node<T> *node, Node<T> **min)
{
    assert(node && min);
    if (!node->left)
    {
        *min = node;
        return node->right;
    }

    node->left = remove_min(node->left, min);
    return balance(node);
}

template <class T>
Node<T>* AVLTree<T>::remove_max(Node<T> *node, Node<T> **max)
{
    assert(node && max);
    if (!node->right)
    {
        *max = node;
        return node->left;
    }

    node->right = remove_max(node->right, max);
    return balance(node);
}

template <class T>
const T& AVLTree<T>::find(int key)
{
    assert(key < get_stat(root));
    Node<T>* node = root;
    while (node && key != get_stat(node->left))
    {
        if (key < get_stat(node->left))
            node = node->left;
        else
        {
            key -= get_stat(node->left) + 1;
            node = node->right;
        }
    }
    assert(node);
    return node->value;
}

template <class T>
int AVLTree<T>::get_stat(Node<T> *node)
{
    int stat = 0;
    if (node)
        stat = node->stat;
    return stat;
}

template <class T>
void AVLTree<T>::stat(Node<T> *node)
{
    if (node)
        node->stat = get_stat(node->left) + get_stat(node->right) + 1;
}
