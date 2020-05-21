#include "Huffman.h"

#include <cassert>
#include <vector>
#include <queue>

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
    explicit Coder(vector<Node*> &v) : bit_count(0)
    {
        assert(!v.empty());
        while (v.size() > 2)
        {
            int min1 = v[0]->frequence, min2 = v[1]->frequence;
            int imin1 = 0, imin2 = 1;
            if (min1 > min2)
            {
                swap(min1, min2);
                swap(imin1, imin2);
            }
            for (size_t i = 2; i < v.size(); i++)
            {
                if (v[i]->frequence < min1)
                {
                    min2 = min1;
                    imin2 = imin1;
                    min1 = v[i]->frequence;
                    imin1 = i;
                }
                else if (v[i]->frequence < min2)
                {
                    min2 = v[i]->frequence;
                    imin2 = i;
                }
            }
            Node *parent = new Node;
            parent->left = v[imin1];
            parent->right = v[imin2];
            parent->frequence = min1 + min2;
            v[imin1]->parent_l = parent;
            v[imin2]->parent_r = parent;
            v[imin1] = parent;
            v.erase(v.begin() + imin2);
        }
        root = new Node;
        root->left = v.front();
        root->right = v.back();
        v.front()->parent_l = root;
        v.back()->parent_r = root;
        update_freq();
    }

    ~Coder()
    {
        in_wide([](const Node *node){ delete node; });
    }

    void encode(vector<byte> &text, vector<byte> &result)
    {
        bit_count = 0;
        for (size_t i = 0; i < text.size(); i++)
            encode_symbol(result, text[i]);

        if (bit_count % 8)
            result.push_back(8 - bit_count % 8);
        else
            result.push_back(0);
    }

    void decode(vector<byte> &text, vector<byte> &result)
    {
        int waste = text.back();
        text.pop_back();
        bit_count = text.size() * 8;
        while (bit_count > waste)
            read_symbol(text, result);
    }

    void encode_tree(Node *node, vector<byte> &buf)
    {
        if (!node)
            return;

        if (!node->left && !node->right)
        {
            write_byte(buf, node->symbol);
            write_byte(buf, node->frequence);
        }
        else
        {
            encode_tree(node->left, buf);
            encode_tree(node->right, buf);
        }
    }

    Node* get_root(void)
    {
        return root;
    }

private:
    Node *root;
    int bit_count;

    void in_wide(void (*handler)(const Node*))
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
            handler(node);
        }
    }

    void update_freq(void)
    {
        if (!root)
            return;

        Node *node = nullptr;
        queue<Node*> q;
        q.push(root);
        int tmp = 1, next = 0;
        int c1 = 0, c2 = 0;
        while (!q.empty())
        {
            next = 0;
            for (int i = 0; i < tmp; i++)
            {
                node = q.front();
                q.pop();
                if (node->left)
                {
                    q.push(node->left);
                    next++;
                }
                if (node->right)
                {
                    q.push(node->right);
                    next++;
                }
            }
            tmp = next;
            c1++;
        }
        q.push(root);
        tmp = 1;
        while (!q.empty())
        {
            next = 0;
            for (int i = 0; i < tmp; i++)
            {
                node = q.front();
                node->frequence = 1 << (c1 - c2 - 1);
                q.pop();
                if (node->left)
                {
                    q.push(node->left);
                    next++;
                }
                if (node->right)
                {
                    q.push(node->right);
                    next++;
                }
            }
            tmp = next;
            c2++;
        }
    }

    void read_bit(vector<byte> &text, byte &bit)
    {
        if (bit_count + 7 < text.size() * 8)
            text.erase(text.begin());

        bit = text.front() & 128;
        text.front() <<= 1;
        bit_count--;
    }

    void read_symbol(vector<byte> &text, vector<byte> &buf)
    {
        byte bit = 0;
        Node *node = root;
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
            write_bit(buf, code.back());
            code.pop_back();
        }
    }
};

static void write(IOutputStream &output, const vector<byte> &text)
{
    for (size_t i = 0; i < text.size(); i++)
        output.Write(text[i]);
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

    vector<byte> archive;
    archive.push_back(alphabet.size());
    Coder coder(alphabet);
    coder.encode_tree(coder.get_root(), archive);
    coder.encode(text, archive);
    write(compressed, archive);
}

void Decode(IInputStream &compressed, IOutputStream &original)
{
    byte symbol;
    compressed.Read(symbol);
    int size = symbol;
    vector<Node*> alphabet(size);
    for (int i = 0; i < size; i++)
    {
        compressed.Read(symbol);
        alphabet[i] = new Node(symbol);
        compressed.Read(symbol);
        alphabet[i]->frequence = symbol;
    }

    vector<byte> archive;
    while(compressed.Read(symbol))
        archive.push_back(symbol);

    Coder coder(alphabet);
    vector<byte> text;
    coder.decode(archive, text);
    write(original, text);
}
