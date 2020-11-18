/*
 * Даны неотрицательные целые числа n,k и массив целых чисел из [0..10^9] 
 * размера n. Требуется найти k-ю порядковую статистику. т.е. напечатать 
 * число, которое бы стояло на позиции с индексом k (0..n-1) в отсортированном 
 * массиве. 
 * Требования: к дополнительной памяти: O(n). Среднее время работы: O(n).
 * Должна быть отдельно выделенная функция partition. Рекурсия запрещена.
 * Решение должно поддерживать передачу функции сравнения снаружи.
 * 6_4. Реализуйте стратегию выбора опорного элемента “случайный элемент”.
 * Функцию Partition реализуйте методом прохода двумя итераторами от конца
 * массива к началу.
 */

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <ctime>

using namespace std;

template <class T>
bool cmp_default(const T &left, const T &right);

bool cmp(const int &left, const int &right);

template <class T>
int find_k_stat(T *arr, int begin, int end, int k, \
                bool (*cmp)(const T&, const T&) = cmp_default);

int main(void)
{
    srand(time(0));
    // input
    int n = 0, k = 0;
    cin >> n >> k;
    assert(n >= 0 && k >= 0);
    int *arr = new int[n];
    for (int i = 0; i < n; i++)
        cin >> arr[i];
    // solution
    int result = find_k_stat(arr, 0, n - 1, k, cmp);
    // output
    cout << result << endl;
    delete[] arr;
    return 0;
}

template <class T>
bool cmp_default(const T &left, const T &right)
{
    return left > right;
}

bool cmp(const int &left, const int &right)
{
    return left > right;
}

template <class T>
int partition(T *arr, int begin, int end, bool (*cmp)(const T&, const T&))
{
    assert(arr && begin >= 0 && begin <= end);
    if (begin == end)
        return begin;

    int pivot = rand() % (end - begin) + begin;
    swap(arr[begin], arr[pivot]);
    int i = end, j = end;
    while (j > begin)
    {
        if (cmp(arr[j], arr[begin]))
        {
            swap(arr[i], arr[j]);
            i--;
        }
        j--;
    }
    swap(arr[i], arr[begin]);
    return i;
}

template <class T>
int find_k_stat(T *arr, int begin, int end, int k, \
                bool (*cmp)(const T&, const T&))
{
    assert(arr && begin >= 0 && k >= 0 && begin <= end && k <= end);
    int pivot = 0;
    while ((pivot = partition(arr, begin, end, cmp)) != k)
    {
        if (pivot > k)
            end = pivot - 1;
        else
            begin = pivot + 1;
    }
    return arr[pivot];
}
