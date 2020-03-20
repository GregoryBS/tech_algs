/*
 * Решение всех задач данного раздела предполагает использование кучи,
 * реализованной в виде класса. Решение должно поддерживать передачу функции
 * сравнения снаружи.
 * Куча должна быть динамической.

 * На вокзале есть некоторое количество тупиков, куда прибывают электрички.
 * Этот вокзал является их конечной станцией. Дано расписание движения
 * электричек, в котором для каждой электрички указано время ее прибытия,
 * а также время отправления в следующий рейс. Электрички в расписании
 * упорядочены по времени прибытия. Когда электричка прибывает, ее ставят
 * в свободный тупик с минимальным номером. При этом если электричка из
 * какого-то тупика отправилась в момент времени X, то электричку, которая
 * прибывает в момент времени X, в этот тупик ставить нельзя, а электричку,
 * прибывающую в момент X+1 — можно. В данный момент на вокзале достаточное
 * количество тупиков для работы по расписанию.
 * Напишите программу, которая по данному расписанию определяет, какое
 * минимальное количество тупиков требуется для работы вокзала.
 */


#include <iostream>
#include <cstring>
#include <cassert>

using namespace std;

typedef struct times_t
{
    int arrive;
    int departure;
} times;

bool cmp_default(int left, int right);

bool cmp_times(times left, times right);

int get_dead_end_number(times *arr, int size);

template <class T>
class Array
{
public:
    Array();
    ~Array();

    T &operator[] (int index);
    bool empty(void);
    void clear(void);
    int size(void);
    void push_back(T element);
    T pop_back(void);

private:
    T *data;
    int len, count;
    int step;
}; // dynamic array class

template <class T>
class Heap
{
public:
    Heap(bool (*cmp)(T, T) = cmp_default);
    explicit Heap(const Array<T> &arr, bool (*cmp)(T, T) = cmp_default);
    ~Heap();

    T &operator[] (int index);

    void insert(T element);
    T get_max(void);
    T read_max(void);

private:
    Array<T> array;
    bool (*cmp_func)(T, T); // function for comparing elements

    void build(void);
    void down(int index);
    void up(int index);
}; // heap class

int main(void)
{
    // input
    int n = 0;
    cin >> n;
    assert(n > 0);
    times *timetable = new times[n];
    for (int i = 0; i < n; i++)
        cin >> timetable[i].arrive >> timetable[i].departure;
    // solution
    int result = get_dead_end_number(timetable, n);
    // output
    cout << result << endl;
    delete[] timetable;
    return 0;
}

bool cmp_default(int left, int right)
{
    return left > right;
}

bool cmp_times(times left, times right)
{
    return left.departure < right.departure;
}

int get_dead_end_number(times *arr, int size)
{
    int max = 0, count = 0;
    Heap<times> heap(cmp_times);
    for (int i = 0; i < size; i++)
    {
        if (count > 0)
        {
            bool flag = true;
            while (count > 0 && flag)
            {
                times tmp = heap.read_max();
                if (tmp.departure < arr[i].arrive)
                {
                    heap.get_max();
                    count--;
                }
                else
                    flag = false;
            }
        }
        heap.insert(arr[i]);
        count++;
        if (count > max)
            max = count;
    }
    return max;
}

template <class T>
Array<T>::Array() : data(nullptr), len(0), count(0), step(2) {}

template <class T>
Array<T>::~Array()
{
    if (data)
        delete[] data;
}

template <class T>
T &Array<T>::operator[] (int index)
{
    assert(index >= 0 && index < len);
    return data[index];
}

template <class T>
bool Array<T>::empty(void)
{
    return len == 0;
}

template <class T>
void Array<T>::clear(void)
{
    len = 0;
}

template <class T>
int Array<T>::size(void)
{
    return len;
}

template <class T>
void Array<T>::push_back(T element)
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
T Array<T>::pop_back(void)
{
    assert(!this->empty());
    return data[--len];
}

template <class T>
Heap<T>::Heap(bool (*cmp)(T, T)) : cmp_func(cmp) {}

template <class T>
Heap<T>::Heap(const Array<T> &arr, bool (*cmp)(T, T))
{
    cmp_func = cmp;
    for (int i = 0; i < arr.size(); i++)
        array.push_back(arr[i]);
    build();
}

template <class T>
Heap<T>::~Heap()
{
    cmp_func = nullptr;
    array.clear();
}

template <class T>
void Heap<T>::insert(T element)
{
    array.push_back(element);
    up(array.size() - 1);
}

template <class T>
T Heap<T>::get_max(void)
{
    assert(!array.empty());
    T result = array[0];
    if (array.size() > 1)
    {
        array[0] = array[array.size() - 1];
        down(0);
    }
    array.pop_back();
    return result;
}

template <class T>
T Heap<T>::read_max(void)
{
    assert(!array.empty());
    return array[0];
}

template <class T>
T &Heap<T>::operator[] (int index)
{
    assert(index < array.size());
    return array[index];
}

template <class T>
void Heap<T>::build(void)
{
    for (int i = array.size() / 2 - 1; i >= 0; i--)
        down(i);
}

template <class T>
void Heap<T>::down(int index)
{
    bool flag = true;
    while (index < array.size() / 2 && flag)
    {
        int left = index * 2 + 1;
        int right = index * 2 + 2;
        int big = index;
        if (left < array.size() && cmp_func(array[left], array[big]))
            big = left;
        if (right < array.size() && cmp_func(array[right], array[big]))
            big = right;
        if (big != index)
        {
            swap(array[index], array[big]);
            index = big;
        }
        else
            flag = false;
    }
}

template <class T>
void Heap<T>::up(int index)
{
    bool flag = true;
    while (index > 0 && flag)
    {
        int parent = (index - 1) / 2;
        if (cmp_func(array[index], array[parent]))
        {
            swap(array[index], array[parent]);
            index = parent;
        }
        else
            flag = false;
    }
}
