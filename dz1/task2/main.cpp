/*
 * 2_2. Дан массив целых чисел А[0..n-1]. Известно, что на интервале [0, m]
 * значения массива строго возрастают, а на интервале [m, n-1] строго убывают.
 * Найти m за O(log m).
 * Требования:  Время работы O(log m).
 */

#include <iostream>
#include <cassert>

using namespace std;

int find_m(const int *arr, const int size);

int main(void)
{
    // input
    int n = 0;
    cin >> n;
    assert(n > 0);
    int *A = new int[n];
    for (int i = 0; i < n; i++)
        cin >> A[i];
    // solving
    int m = find_m(A, n);
    // output
    cout << m << endl;
    delete[] A;
    return 0;
}

int bin_search(const int *arr, int left, int right)
{
    assert(arr && left >= 0 && left <= right);
    int mid = 0;
    while (left + 1 < right)
    {
        mid = (left + right) / 2;
        if (arr[left] < arr[mid])
        {
            if (arr[mid - 1] < arr[mid])
                left = mid;
            else
                right = mid;
        }
        else
            right = mid;
    }
    return arr[left] > arr[right] ? left : right;
}

int find_m(const int *arr, const int size)
{
    assert(arr && size > 0);
    int left = 0, right = 1, gap = 1;
    while (right < size && arr[left] < arr[right] && \
           arr[right] > arr[right - 1])
    {
        gap <<= 1;
        left = right;
        right += gap;
    }
    if (right >= size)
        right = size - 1;
    return bin_search(arr, left, right);
}
