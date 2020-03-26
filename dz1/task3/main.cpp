/*
 * Во всех задачах из следующего списка следует написать структуру данных,
 * обрабатывающую команды push* и pop*.
 * Формат входных данных.
 * В первой строке количество команд n. n ≤ 1000000. Каждая команда задаётся
 * как 2 целых числа: a b. a = 1 - push front; a = 2 - pop front;
 * a = 3 - push back; a = 4 - pop back
 * Команды добавления элемента 1 и 3 заданы с неотрицательным параметром b.
 * Для очереди используются команды 2 и 3. Для дека используются все четыре
 * команды. Если дана команда pop*, то число b - ожидаемое значение. Если
 * команда pop вызвана для пустой структуры данных, то ожидается “-1”.
 * Формат выходных данных.
 * Требуется напечатать YES - если все ожидаемые значения совпали. Иначе, если
 * хотя бы одно ожидание не оправдалось, то напечатать NO.
 * 3_3. Реализовать очередь с помощью двух стеков. Использовать стек,
 * реализованный с помощью динамического буфера. Требования: Очередь должна
 * быть реализована в виде класса. Стек тоже должен быть реализован в виде
 * класса.
 */

#include <iostream>
#include <cstring>
#include <cassert>

#define PUSH 3
#define POP 2
#define YES 0
#define NO -1

using namespace std;

template <class T>
class Stack
{
public:
    Stack();
    ~Stack();

    T &operator[] (int index);
    bool empty(void);
    void clear(void);
    int size(void);
    void push_back(const T &element);
    T pop_back(void);

private:
    T *data;
    int len, count;
    int step;
}; // dynamic stack class

template <class T>
class Queue
{
public:
    Queue();
    ~Queue();

    int size(void);
    bool empty(void);
    void push_back(const T &element);
    int pop_front(T &element);

private:
    Stack<T> *first;
    Stack<T> *second;
}; // queue class

int work_with_queue(int *operation, int *expect, int n);

int main(void)
{
    // input
    int n = 0;
    cin >> n;
    assert(n > 0);
    int *a = new int[n];
    int *b = new int[n];
    for (int i = 0; i < n; i++)
        cin >> a[i] >> b[i];
    // solution
    int rc = work_with_queue(a, b, n);
    // output
    if (rc == YES)
        cout << "YES" << endl;
    else
        cout << "NO" << endl;
    delete[] a;
    delete[] b;
    return 0;
}

int work_with_queue(int *operation, int *expect, int n)
{
    assert(operation && expect && n > 0);
    int rc = YES, i = 0, element = -1;
    Queue<int> q;
    while (rc == YES && i < n)
    {
        if (operation[i] == PUSH)
            q.push_back(expect[i]);
        else if (operation[i] == POP)
        {
            rc = q.pop_front(element);
            if ((expect[i] == NO && rc == NO) || \
                (expect[i] != NO && expect[i] == element))
                rc = YES;
            else
                rc = NO;
        }
        i++;
    }
    return rc;
}

template <class T>
Stack<T>::Stack() : data(nullptr), len(0), count(0), step(2) {}

template <class T>
Stack<T>::~Stack()
{
    if (data)
        delete[] data;
}

template <class T>
T &Stack<T>::operator[] (int index)
{
    assert(index >= 0 && index < len);
    return data[index];
}

template <class T>
bool Stack<T>::empty(void)
{
    return len == 0;
}

template <class T>
void Stack<T>::clear(void)
{
    len = 0;
}

template <class T>
int Stack<T>::size(void)
{
    return len;
}

template <class T>
void Stack<T>::push_back(const T &element)
{
    if (this->empty())
    {
        data = new T[1];
        count = 1;
    }
    else if (len == count)
    {
        T *new_data = new T[count * step];
        memcpy(new_data, data, len * sizeof(T));
        delete[] data;
        data = new_data;
        count *= step;
    }
    data[len++] = element;
}

template <class T>
T Stack<T>::pop_back(void)
{
    assert(!this->empty());
    return data[--len];
}

template <class T>
Queue<T>::Queue()
{
    first = new Stack<T>;
    second = new Stack<T>;
}

template <class T>
Queue<T>::~Queue()
{
    if (first)
        delete first;
    if (second)
        delete second;
}

template <class T>
int Queue<T>::size(void)
{
    return first->size() + second->size();
}

template <class T>
bool Queue<T>::empty(void)
{
    return first->empty() && second->empty();
}

template <class T>
void Queue<T>::push_back(const T &element)
{
    first->push_back(element);
}

template <class T>
int Queue<T>::pop_front(T &element)
{
    if (this->empty())
        return NO;

    if (second->empty())
    {
        while (!first->empty())
            second->push_back(first->pop_back());
    }
    element = second->pop_back();
    return YES;
}
