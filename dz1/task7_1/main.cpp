/*
 * 7_1. MSD для строк.
 * Дан массив строк. Количество строк не больше 10^5. Отсортировать массив
 * методом поразрядной сортировки MSD по символам. Размер алфавита - 256
 * символов. Последний символ строки = ‘\0’.
 */

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <string>

#define ALPHABET 256

using namespace std;

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

void msd_sort(Array<string> &arr, int symbol, int begin, int end);

int main(void)
{
    // input
    Array<string> arr;
    string line = "";
    while (getline(cin, line))
    {
        arr.push_back(line);
    }
    /*for (int i = 0; i < arr.size(); i++)
    {
        for (int j = 0; j <= arr[i].length(); j++)
            cout << (int) arr[i][j] << " ";
        cout << endl;
    }*/
    // solution
    msd_sort(arr, 0, 0, arr.size());
    /*for (int i = 0; i < arr.size(); i++)
    {
        for (int j = 0; j <= arr[i].length(); j++)
            cout << (int) arr[i][j] << " ";
        cout << endl;
    }*/
    // output
    for (int i = 0; i < arr.size(); i++)
    {
        cout << arr[i] << endl;
    }
    return 0;
}

int get_width(Array<string> &arr)
{
    int width = 0;
    for (int i = 0; i < arr.size(); i++)
    {
        if (arr[i].length() > width)
            width = arr[i].length();
    }
    return width;
}

void sort_symbol(Array<string> &arr, int symbol)
{
    Array <Array <string> > buckets;
    for (int i = 0; i < ALPHABET; i++)
        buckets.push_back(Array<string>());

    int index = 0;
    for (int i = 0; i < arr.size(); i++)
    {
        if (arr[i].length() <= symbol)
        {
            buckets[arr[i][0]].push_back(arr[i]);
        }
        else
            buckets[arr[i][symbol]].push_back(arr[i]);
    }

    arr.clear();
    for (int i = 0; i < ALPHABET; i++)
    {
        if (buckets[i].empty() == false)
        {
            for (int j = 0; j < buckets[i].size(); j++)
                arr.push_back(buckets[i][j]);
        }
    }
}

void msd_sort(Array<string> &arr, int symbol, int begin, int end)
{
	Array<int> counts;
	for (int i = 0; i <= ALPHABET; i++)
		counts.push_back(0);
	
	Array<string> buf;
	for (int i = begin; i < end; i++)
	{
		buf.push_back("");
		counts[arr[i][symbol]]++;
	}
	
	for (int i = 1; i <= ALPHABET; i++)
		counts[i] += counts[i - 1];

	int index = 0;
	for (int i = end - 1; i >= begin; i--)
	{
		index = --counts[arr[i][symbol]];
		buf[index] = arr[i];
	}
	for (int i = 0; i < buf.size(); i++)
		arr[i + begin] = buf[i];
	
	for (int i = 0; i < ALPHABET; i++)
	{
		bool flag = true;
		int j = counts[i];
		while (j < counts[i + 1] && flag)
		{
			if (buf[j].length() > symbol)
				flag = false;
			else
				j++;
		}
		if (j != counts[i + 1])
			msd_sort(arr, symbol + 1, j + begin, counts[i + 1] + begin);
	}
}

template <class T>
Array<T>::Array() : data(NULL), len(0), count(0), step(2) {}

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
        copy(data, data + count, new_data);
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
