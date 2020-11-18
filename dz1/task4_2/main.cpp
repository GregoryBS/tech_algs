/*
 * Решение всех задач данного раздела предполагает использование кучи,
 * реализованной в виде класса. Решение должно поддерживать передачу функции
 * сравнения снаружи.
 * Куча должна быть динамической. 
 * 
 * 4_2. Быстрое сложение.
 * Для сложения чисел используется старый компьютер. Время, затрачиваемое на 
 * нахождение суммы двух чисел равно их сумме. Таким образом для нахождения 
 * суммы чисел 1,2,3 может потребоваться разное время, в зависимости от 
 * порядка вычислений. 
 * ((1+2)+3) -> 1+2 + 3+3 = 9 
 * ((1+3)+2) -> 1+3 + 4+2 = 10 
 * ((2+3)+1) -> 2+3 + 5+1 = 11 
 * Требуется написать программу, которая определяет минимальное время, 
 * достаточное для вычисления суммы заданного набора чисел. Формат входных 
 * данных. Вначале вводится n - количество чисел. Затем вводится n строк - 
 * значения чисел (значение каждого числа не превосходит 10^9, сумма всех 
 * чисел не превосходит 2*10^9). Формат выходных данных. Натуральное 
 * число - минимальное время.
 */


#include <iostream>
#include <cstring>
#include <cassert>

using namespace std;

template <class T>
bool cmp_default(const T &left, const T &right);

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
    void push_back(const T &element);
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
    explicit Heap(bool (*cmp)(const T&, const T&) = cmp_default);
    Heap(const Array<T> &arr, bool (*cmp)(const T&, const T&) = \
                  cmp_default);
    ~Heap();

    T &operator[] (int index);

    void insert(const T &element);
    T get_min(void);
    const T &read_min(void);

private:
    Array<T> array;
    bool (*cmp_func)(const T&, const T&); // function for comparing elements

    void build(void);
    void down(int index);
    void up(int index);
}; // heap class

int get_sum_time(Heap<int> &heap, int size);

int main(void)
{
    // input
    int n = 0;
    cin >> n;
    assert(n > 0);
    Heap<int> numbers;
    int num = 0;
    for (int i = 0; i < n; i++)
    {
        cin >> num;
        numbers.insert(num);
    }
    // solution
    int result = get_sum_time(numbers, n);
    // output
    cout << result << endl;
    return 0;
}

template <class T>
bool cmp_default(const T &left, const T &right)
{
    return left < right;
}

int get_sum_time(Heap<int> &heap, int size)
{
    int sum = 0;
	int min1 = 0, min2 = 0;
    for (int i = 0; i < size - 1; i++)
    {
        min1 = heap.get_min();
		min2 = heap.get_min();
		sum += min1 + min2;
		heap.insert(min1 + min2);
    }
    return sum;
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
void Array<T>::push_back(const T &element)
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
Heap<T>::Heap(bool (*cmp)(const T&, const T&)) : cmp_func(cmp) {}

template <class T>
Heap<T>::Heap(const Array<T> &arr, bool (*cmp)(const T&, const T&))
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
void Heap<T>::insert(const T &element)
{
    array.push_back(element);
    up(array.size() - 1);
}

template <class T>
T Heap<T>::get_min(void)
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
const T &Heap<T>::read_min(void)
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
        int small = index;
        if (left < array.size() && cmp_func(array[left], array[small]))
            small = left;
        if (right < array.size() && cmp_func(array[right], array[small]))
            small = right;
        if (small != index)
        {
            swap(array[index], array[small]);
            index = small;
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
