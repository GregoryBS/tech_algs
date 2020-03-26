/*
 * Во всех задачах данного раздела необходимо реализовать и использовать
 * сортировку слиянием. Решение должно поддерживать передачу функции
 * сравнения снаружи. Общее время работы алгоритма O(n log n).
 * 5_4. Закраска прямой 2. На числовой прямой окрасили N отрезков. Известны
 * координаты левого и правого концов каждого отрезка (Li и Ri). Найти сумму
 * длин частей числовой прямой, окрашенных ровно в один слой.
 */

#include <iostream>
#include <cstring>
#include <cassert>

using namespace std;

typedef struct segment_t
{
    int begin;
    int end;
    segment_t(): begin(0), end(0) {}
} segments;

int col_seg_len_sum(segments *arr, const int size);

int main(void)
{
    // input
    int n = 0;
    cin >> n;
    assert(n > 0);
    segments *arr = new segments[n];
    for (int i = 0; i < n; i++)
        cin >> arr[i].begin >> arr[i].end;
    // solution
    int result = col_seg_len_sum(arr, n);
    // output
    cout << result << endl;
    delete[] arr;
    return 0;
}

template <class T>
bool cmp_default(const T &left, const T &right)
{
    return left < right;
}

bool cmp_segments(const segments &left, const segments &right)
{
    return left.begin < right.begin;
}

template <class T>
void merge(T *arr1, T *arr2, const int size, bool (*cmp)(const T&, const T&) = cmp_default)
{
    assert(arr1 && arr2 && cmp && size > 0);
    T *buf = new T[size];
    int s1 = arr2 - arr1, s2 = size - s1;
    int i = 0, j = 0, k = 0;
    while (i < s1 && j < s2)
    {
        if (cmp(arr1[i], arr2[j]))
        {
            buf[k] = arr1[i];
            i++;
        }
        else
        {
            buf[k] = arr2[j];
            j++;
        }
        k++;
    }
    if (j == s2)
        memcpy(buf + k, arr1 + i, (s1 - i) * sizeof(T));
    else
        memcpy(buf + k, arr2 + j, (s2 - j) * sizeof(T));
    memcpy(arr1, buf, size * sizeof(T));
    delete[] buf;
}

template <class T>
void merge_sort(T *arr, const int size, bool (*cmp)(const T&, const T&) = cmp_default)
{
    assert(arr && size > 0 && cmp);
    if (size == 1)
        return;

    int half = size / 2;
    merge_sort(arr, half, cmp);
    merge_sort(arr + half, size - half, cmp);
    merge(arr, arr + half, size, cmp);
}

int col_seg_len_sum(segments *arr, const int size)
{
    merge_sort(arr, size, cmp_segments);
    int col_min = arr[0].begin, col_max = arr[0].end;
    int sum = 0;
    for (int i = 1; i < size; i++)
    {
        if (arr[i].begin >= col_max)
        {
            sum += col_max - col_min;
            col_max = arr[i].end;
            col_min = arr[i].begin;
        }
        else if (arr[i].begin > col_min)
        {
            sum += arr[i].begin - col_min;
            if (arr[i].end >= col_max)
            {
                col_min = col_max;
                col_max = arr[i].end;
            }
            else
                col_min = arr[i].end;
        }
        else if (arr[i].end >= col_max)
        {
            col_min = col_max;
            col_max = arr[i].end;
        }
        else if (arr[i].end > col_min)
            col_min = arr[i].end;
    }
    sum += col_max - col_min;
    return sum;
}
