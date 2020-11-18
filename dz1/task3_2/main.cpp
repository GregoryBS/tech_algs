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
 * 3_2. Реализовать дек с динамическим зацикленным буфером. 
 * Требования: Дек должен быть реализован в виде класса.
 */

#include <iostream>
#include <cassert>

#define PUSHF 1
#define PUSHB 3
#define POPF 2
#define POPB 4

#define YES 0
#define NO -1

#define STEP 2

using namespace std;

template <class T>
class Deck
{
public:
    Deck();
    ~Deck();

    int size(void);
    bool empty(void);
    void push_front(const T &element);
    void push_back(const T &element);
    int pop_front(T &element);
    int pop_back(T &element);


private:
    T *buffer = NULL;
    int b_size;
    int begin, end;
    bool full;

    void mem_realloc(void);
}; // deck class

int work_with_deck(char *operation, int *expect, int n);

int main(void)
{
    // input
    int n = 0;
    cin >> n;
    assert(n > 0);
    char *a = new char[n];
    int *b = new int[n];
    for (int i = 0; i < n; i++)
        cin >> a[i] >> b[i];
    // solution
    int rc = work_with_deck(a, b, n);
    // output
    if (rc == YES)
        cout << "YES" << endl;
    else
        cout << "NO" << endl;
    delete[] a;
    delete[] b;
    return 0;
}

int work_with_deck(char *operation, int *expect, int n)
{
    assert(operation && expect && n > 0);
    int rc = YES, i = 0, element = -1;
    Deck<int> deck;
    while (rc == YES && i < n)
    {
        int op = operation[i] - '0';
        if (op == PUSHF)
            deck.push_front(expect[i]);
        else if (op == PUSHB)
            deck.push_back(expect[i]);
        else if (op == POPB || op == POPF)
        {
            rc = (op == POPF ? deck.pop_front(element) 
                             : deck.pop_back(element));
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
Deck<T>::Deck() : buffer(nullptr), b_size(0), begin(0), end(0), full(false) {}

template <class T>
Deck<T>::~Deck()
{
    if (buffer)
        delete[] buffer;
}

template <class T>
int Deck<T>::size(void)
{
    int result = b_size;
    if (!full)
    {
        if (end > begin)
            result = end - begin;
        else
            result = b_size + end - begin;
    }
    return result;
}

template <class T>
bool Deck<T>::empty(void)
{
    return (full ? false : (begin == end));
}

template <class T>
void Deck<T>::push_front(const T &element)
{
    if (full || !buffer)
        this->mem_realloc();
    begin = (begin == 0 ? (b_size - 1) : (begin - 1));
    buffer[begin] = element;
    if (begin == end)
        full = true;
}

template <class T>
void Deck<T>::push_back(const T &element)
{
    if (full || !buffer)
        this->mem_realloc();
    buffer[end] = element;
    end = (end + 1) % b_size;
    if (begin == end)
        full = true;
}

template <class T>
int Deck<T>::pop_front(T &element)
{
    if (this->empty())
        return NO;

    element = buffer[begin];
    begin = (begin + 1) % b_size;
    full = false;
    return YES;
}

template <class T>
int Deck<T>::pop_back(T &element)
{
    if (this->empty())
        return NO;

    end = (end == 0 ? (b_size - 1) : (end - 1));
    element = buffer[end];
    full = false;
    return YES;
}

template <class T>
void Deck<T>::mem_realloc(void)
{
    full = false;
    if (buffer)
    {
        T *new_buffer = new T[b_size * STEP];
        for (int i = 0; i < b_size; i++)
        {
            new_buffer[i] = buffer[begin];
            begin = (begin + 1) % b_size;
    	}
        begin = 0;
        end = b_size;
        b_size *= STEP;
        delete[] buffer;
        buffer = new_buffer;
    }
    else
    {
        buffer = new T[1];
        b_size = 1;
    }
}
