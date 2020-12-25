#include "Huffman.h"

#include <cassert>
#include <queue>
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;

struct Node
{
    Node *left, *right;
    Node *parent_l, *parent_r;
    int frequence;
    byte symbol;
    Node() :
        left(nullptr), right(nullptr), parent_l(nullptr), parent_r(nullptr),
        frequence(0), symbol(0) {}
    Node(const byte &s) :
        left(nullptr), right(nullptr), parent_l(nullptr), parent_r(nullptr),
        frequence(0), symbol(s) {}
    Node(const byte &s, int f) :
        left(nullptr), right(nullptr), parent_l(nullptr), parent_r(nullptr),
        frequence(f), symbol(s) {}
};

class Coder
{
public:
    Coder() : bit_count(0) {}

    ~Coder() = default;

    void read_bit(vector<byte> &text, byte &bit)
    {
        if (bit_count + 7 < text.size() * 8)
            text.erase(text.begin());

        bit = text.front() & 128;
        text.front() <<= 1;
        bit_count--;
    }

    void read_symbol(Node *node, vector<byte> &text, vector<byte> &buf)
    {
        byte bit = 0;
        while (node->left || node->right)
        {
            read_bit(text, bit);
            if (bit)
                node = node->right;
            else
                node = node->left;
        }
        buf.push_back(node->symbol);
    }

    void write_bit(vector<byte> &buf, byte bit)
    {
        if (bit_count % 8 == 0)
            buf.push_back(0);

        if (bit)
            buf.back() |= 1 << (7 - bit_count % 8);
        bit_count++;
    }

    void write_byte(vector<byte> &buf, byte symbol)
    {
        if (bit_count % 8 == 0)
            buf.push_back(symbol);
        else
        {
            buf.back() |= symbol >> (bit_count % 8);
            buf.push_back(symbol << (8 - bit_count % 8));
        }
        bit_count += 8;
    }

    int bits(void)
    {
        return bit_count;
    }

    void set_bits(int value)
    {
        bit_count = value;
    }

private:
    int bit_count;
};

class Huffman
{
public:
    Huffman(vector<Node*> &alphabet, Coder *coder) : coder(coder)
    {
        create_tree(alphabet);
    }

    ~Huffman()
    {
        if (!root)
            return;

        Node *node = nullptr;
        queue<Node*> q;
        q.push(root);
        while (!q.empty())
        {
            node = q.front();
            q.pop();
            if (node->left)
                q.push(node->left);
            if (node->right)
                q.push(node->right);
            delete node;
        }
    }

    void encode(vector<byte> &text, vector<byte> &result)
    {
        coder->set_bits(0);
        for (size_t i = 0; i < text.size(); i++)
            encode_symbol(result, text[i]);

        if (coder->bits() % 8)
            result.push_back(8 - coder->bits() % 8);
        else
            result.push_back(0);
    }

    void decode(vector<byte> &text, vector<byte> &result)
    {
        int waste = text.back();
        text.pop_back();
        coder->set_bits(text.size() * 8);
        while (coder->bits() > waste)
            coder->read_symbol(root, text, result);
    }

private:
    Node *root;
    Coder *coder;

    void create_tree(vector<Node*> &alphabet)
    {
        assert(!alphabet.empty());
        while (alphabet.size() > 1)
        {
            int min1 = alphabet[0]->frequence, min2 = alphabet[1]->frequence;
            int imin1 = 0, imin2 = 1;
            if (min1 > min2)
            {
                swap(min1, min2);
                swap(imin1, imin2);
            }
            for (size_t i = 2; i < alphabet.size(); i++)
            {
                if (alphabet[i]->frequence < min1)
                {
                    min2 = min1;
                    imin2 = imin1;
                    min1 = alphabet[i]->frequence;
                    imin1 = i;
                }
                else if (alphabet[i]->frequence < min2)
                {
                    min2 = alphabet[i]->frequence;
                    imin2 = i;
                }
            }
            Node *parent = new Node;
            parent->left = alphabet[imin1];
            parent->right = alphabet[imin2];
            parent->frequence = min1 + min2;
            alphabet[imin1]->parent_l = parent;
            alphabet[imin2]->parent_r = parent;
            alphabet[imin1] = parent;
            alphabet.erase(alphabet.begin() + imin2);
        }
        root = alphabet.front();
    }

    bool find_code(Node *node, vector<byte> &code, byte symbol)
    {
        if (!node)
            return false;

        bool flag = false;
        if (!node->left && !node->right)
        {
            if (node->symbol == symbol)
            {
                flag = true;
                while (node->parent_l || node->parent_r)
                {
                    if (node->parent_l)
                    {
                        code.push_back(0);
                        node = node->parent_l;
                    }
                    else
                    {
                        code.push_back(1);
                        node = node->parent_r;
                    }
                }
            }
        }
        else
        {
            flag = find_code(node->left, code, symbol);
            if (!flag)
                find_code(node->right, code, symbol);
        }
        return flag;
    }

    void encode_symbol(vector<byte> &buf, byte symbol)
    {
        vector<byte> code;
        find_code(root, code, symbol);
        while (!code.empty())
        {
            coder->write_bit(buf, code.back());
            code.pop_back();
        }
    }
};

static void write(IOutputStream &output, const vector<byte> &text)
{
    for (size_t i = 0; i < text.size(); i++)
        output.Write(text[i]);
}

void num_to_byte(int num, vector<byte> &buf)
{
    buf.clear();
    for (int i = 0; i < sizeof(num); i++)
    {
        buf.insert(buf.begin(), num & 255);
        num >>= 8;
    }
}

void encode_alphabet(Coder *coder, vector<Node*> &alphabet, vector<byte> &buf)
{
    for (int i = 0; i < alphabet.size(); i++)
    {
        coder->write_byte(buf, alphabet[i]->symbol);
        vector<byte> freq;
        num_to_byte(alphabet[i]->frequence, freq);
        for (int i = 0; i < freq.size(); i++)
            coder->write_byte(buf, freq[i]);

    }
}

void Encode(IInputStream &original, IOutputStream &compressed)
{
    vector<int> index(256, -1);
    vector<Node*> alphabet;
    vector<byte> text;
    byte symbol;
    while (original.Read(symbol))
    {
        if (index[symbol] < 0)
        {
            alphabet.push_back(new Node(symbol, 1));
            index[symbol] = alphabet.size() - 1;
        }
        else
            alphabet[index[symbol]]->frequence += 1;
        text.push_back(symbol);
    }

    Coder *coder = new Coder;
    vector<byte> archive;
    archive.push_back(alphabet.size());
    encode_alphabet(coder, alphabet, archive);
    Huffman *tree = new Huffman(alphabet, coder);
    tree->encode(text, archive);
    write(compressed, archive);
    delete tree;
    delete coder;
}

void Decode(IInputStream &compressed, IOutputStream &original)
{
    byte symbol;
    compressed.Read(symbol);
    int size = symbol;
    if (!size)
        size += 256;
    vector<Node*> alphabet(size);
    for (int i = 0; i < size; i++)
    {
        compressed.Read(symbol);
        alphabet[i] = new Node(symbol);
        int frequence = 0;
        for (int i = 0; i < sizeof(frequence); i++)
        {
            compressed.Read(symbol);
            frequence *= 256;
            frequence += symbol;
        }
        alphabet[i]->frequence = frequence;
    }

    vector<byte> archive;
    while(compressed.Read(symbol))
        archive.push_back(symbol);

    Coder *coder = new Coder;
    Huffman *tree = new Huffman(alphabet, coder);
    vector<byte> text;
    tree->decode(archive, text);
    write(original, text);
    delete tree;
    delete coder;
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		cout << "Error parameters.\nUsage: main <input> <output>\n";
		return 0;
	}
	
    IInputStream input, encode;
    IOutputStream output, decode;
    byte sym;
    FILE *f1 = fopen(argv[1], "rb"), *f2 = fopen(argv[2], "wb");
	if (f1 && f2)
	{
		while (!feof(f1))
            if (fread(&sym, sizeof(byte), 1, f1) == 1)
                input.buf.push_back(sym);
		fclose(f1);

		Encode(input, output);
        cout << "Encoded\n";
		encode.buf = output.buf;
		Decode(encode, decode);
        cout << "Decoded\n";

		for (int i = 0; i < decode.buf.size(); i++)
			fwrite(&(decode.buf[i]), sizeof(byte), 1, f2);
		fclose(f2);
	}
	else if (f1)
		fclose(f1);
	else if (f2)
		fclose(f2);
    return 0;
}
