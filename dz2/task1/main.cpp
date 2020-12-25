/*
 * Реализуйте структуру данных типа “множество строк” на основе динамической
 * хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из
 * строчных латинских букв. Хеш-функция строки должна быть реализована с
 * помощью вычисления значения многочлена методом Горнера.
 * Начальный размер таблицы должен быть равным 8-ми. Перехеширование
 * выполняйте при добавлении элементов в случае, когда коэффициент заполнения
 * таблицы достигает 3/4.
 * Структура данных должна поддерживать операции добавления строки в
 * множество, удаления строки из множества и проверки принадлежности данной
 * строки множеству.
 * 1_1. Для разрешения коллизий используйте квадратичное пробирование. i-ая
 * проба g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.
 * Требования: В таблице запрещено хранение указателей на описатель элемента.
 */

#include <iostream>
#include <cassert>
#include <string>
#include <vector>

#define FIND '?'
#define ADD '+'
#define REMOVE '-'
#define SIZE 8
#define ALPHA 0.75
#define A 73
#define EMPTY 0
#define FILL 1
#define DEL -1

using namespace std;

int hash_gorner(const string& value, size_t m)
{
    int hash = 0;
    for (size_t i = 0; i < value.size(); i++)
        hash = hash * A + value[i];
    return hash % m;
}

template <class T>
struct HashFunc
{
    size_t operator() (const T& key, size_t m)
    {
        return key % m;
    }
};

template <>
struct HashFunc<string>
{
    size_t operator() (const string& key, size_t m)
    {
        return hash_gorner(key, m);
    }
};

template <class T>
struct Node
{
    int flag;
    T key;
    Node() : flag(EMPTY) {}
};

template <class T, class Hash = HashFunc<T>>
class HashTable
{
public:
    explicit HashTable(Hash h = Hash());
    ~HashTable();

    bool find(const T& key);
    bool add(const T& key);
    bool remove(const T& key);

private:
    vector<Node<T>> table;
    Hash hasher;
    size_t capacity;

    void resize(void);
};

int main(void)
{
    HashTable<string> set;
    char operation = 0;
    string word = "";
    bool result = false;
    while (cin >> operation >> word)
    {
        switch (operation)
        {
            case FIND: result = set.find(word); break;
            case ADD: result = set.add(word); break;
            case REMOVE: result = set.remove(word); break;
            default: assert(0); break;
        }
        if (result)
            cout << "OK" << endl;
        else
            cout << "FAIL" << endl;
    }
    return 0;
}

template <class T, class Hash>
HashTable<T, Hash>::HashTable(Hash h) : table(0), hasher(h), capacity(0) {}

template <class T, class Hash>
HashTable<T, Hash>::~HashTable()
{
    table.clear();
}

template <class T, class Hash>
void HashTable<T, Hash>::resize(void)
{
    size_t new_size = SIZE;
	if (table.size() >= new_size)
		new_size = table.size() * 2;
    vector<Node<T>> buf = move(table);
    table.resize(new_size);
    for (size_t i = 0; i < buf.size(); i++)
    {
        if (buf[i].flag == FILL)
        {
            int hi = hasher(buf[i].key, new_size);
            int prob_count = 0;
            while (table[hi].flag != EMPTY)
            {
                prob_count++;
                hi = (hi + prob_count) % new_size;
            }
            table[hi] = buf[i];
        }
    }
}

template <class T, class Hash>
bool HashTable<T, Hash>::find(const T& key)
{
    int hi = hasher(key, table.size()), prob_count = 0;
    bool result = false;
    while (prob_count < table.size() && !result)
    {
        if (table[hi].flag == EMPTY)
            prob_count = table.size();
        else if (table[hi].flag == FILL && table[hi].key == key)
            result = true;
        else
        {
            prob_count++;
            hi = (hi + prob_count) % table.size();
        }
    }
    return result;
}

template <class T, class Hash>
bool HashTable<T, Hash>::add(const T& key)
{
    if (ALPHA * table.size() <= capacity)
        resize();

    int hi = hasher(key, table.size()), prob_count = 0, ins = -1;
    bool result = true;
    while (prob_count < table.size() && result)
    {
        if (table[hi].flag == FILL && table[hi].key == key)
            result = false;
        else
        {
            if (table[hi].flag != FILL && ins < 0)
                ins = hi;
            if (table[hi].flag == EMPTY)
                prob_count = table.size();
            else
            {
                prob_count++;
                hi = (hi + prob_count) % table.size();
            }
        }
    }
    if (result)
    {
        assert(ins >= 0);
        table[ins].key = key;
        table[ins].flag = FILL;
        capacity++;
    }
    return result;
}

template <class T, class Hash>
bool HashTable<T, Hash>::remove(const T& key)
{
    int hi = hasher(key, table.size()), prob_count = 0;
    bool result = false;
    while (prob_count < table.size() && !result)
    {
        if (table[hi].flag == EMPTY)
            prob_count = table.size();
        else if (table[hi].flag == FILL && table[hi].key == key)
        {
            table[hi].flag = DEL;
            capacity--;
            result = true;
        }
        else
        {
            prob_count++;
            hi = (hi + prob_count) % table.size();
        }
    }
    return result;
}
