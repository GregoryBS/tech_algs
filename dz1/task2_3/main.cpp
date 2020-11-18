/*
 * 2_3. Даны два массива неповторяющихся целых чисел, упорядоченные по 
 * возрастанию. A[0..n-1] и B[0..m-1]. n >> m. Найдите их пересечение.
 * Требования:  Время работы: O(m * log k), где k - позиция элемента B[m-1] 
 * в массиве A. В процессе поиска очередного элемента B[i] в массиве A 
 * пользуйтесь результатом поиска элемента B[i-1]. Внимание! В этой задаче 
 * для каждого B[i] сначала нужно определить диапазон для бинарного поиска 
 * размером порядка k, а потом уже в нем делать бинарный поиск.
 */

#include <iostream>
#include <cassert>

using namespace std;

int intersec(const int *arr1, int s1, const int *arr2, int s2, int *res);

int main(void)
{
    // input
    int n = 0, m = 0;
    cin >> n >> m;
    assert(n > 0 && m > 0 && n > m);
    int *A = new int[n];
	int *B = new int[m];
    for (int i = 0; i < n; i++)
        cin >> A[i];
	for (int i = 0; i < m; i++)
		cin >> B[i];
    // solving
	int *result = new int[m];
    int result_len = intersec(A, n, B, m, result);
    // output
	for (int i = 0; i < result_len; i++)
		cout << result[i] << " ";
    delete[] A;
	delete[] B;
	delete[] result;
    return 0;
}

bool bin_search(const int *arr, int left, int right, int elem)
{
    assert(arr && left >= 0 && left <= right);
    int mid = 0;
    while (left < right && arr[left] != elem)
    {
        mid = (left + right) / 2;
        if (elem > arr[mid])
            left = mid + 1;
        else
            right = mid;
    }
    return arr[left] == elem ? true : false;
}

int intersec(const int *arr1, int s1, const int *arr2, int s2, int *res)
{
    assert(arr1 && s1 > 0 && arr2 && s2 > 0 && res);
    int left = 0, right = 1, len = 0;
	for (int i = 0; i < s2; i++)
	{
		int gap = 1;
		while (right < s1 && arr1[right] < arr2[i])
		{
			gap <<= 1;
			left = right;
			right += gap;
		}
		if (right >= s1)
            right = s1 - 1;
        if (bin_search(arr1, left, right, arr2[i]))
			res[len++] = arr2[i];
	}
    return len;
}
