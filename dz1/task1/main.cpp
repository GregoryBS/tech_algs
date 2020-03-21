/*
 * 1_3. Даны два строго возрастающих массива целых чисел A[0..n) и B[0..m) и
 * число k. Найти количество таких пар индексов (i, j), что A[i] + B[j] = k.
 * n, m ≤ 100000.
 * Указание. Обходите массив B от конца к началу.
 * Требования:  Время работы O(n + m).
 */

#include <iostream>
#include <cassert>

using namespace std;

int count_of_arr_sums(int *arr1, int size1, int *arr2, int size2, int k);

int main(void)
{
    // input
    int n = 0;
    cin >> n;
    assert(n > 0);
    int *A = new int[n];
    for (int i = 0; i < n; i++)
        cin >> A[i];
    int m = 0;
    cin >> m;
    assert(m > 0);
    int *B = new int[m];
    for (int i = 0; i < m; i++)
        cin >> B[i];
    int k = 0;
    cin >> k;
    // solution
    int result = count_of_arr_sums(A, n, B, m, k);
    // output
    cout << result << endl;
    delete[] A;
    delete[] B;
    return 0;
}

int count_of_arr_sums(int *arr1, int size1, int *arr2, int size2, int k)
{
    assert(arr1 && arr2 && size1 > 0 && size2 > 0);
    if (arr1[0] + arr2[0] > k || arr1[size1 - 1] + arr2[size2 - 1] < k)
        return 0; // все числа в сумме либо больше, либо меньше k

    int count = 0;
    int limit = k - arr1[size1 - 1]; // min подходящее число из 2 массива
    int j = size2 - 1, i_min = 0;
    while (j >= 0 && arr2[j] >= limit)
    {
        int require = k - arr2[j];
        int i = i_min;
        while (i < size1 && arr1[i] < require) // пропуск чисел, меньших нужного
            i++;
        if (i < size1 && arr1[i] == require)
            count++;
        i_min = i;
        j--;
    }
    return count;
}
