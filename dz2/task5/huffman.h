#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <vector>

typedef unsigned char byte;

#define interface struct

interface IInputStream {
    // Возвращает false, если поток закончился
    bool Read(byte& value)
    {
        if (!buf.empty())
        {
            value = buf.front();
            buf.erase(buf.begin());
            return true;
        }
        return false;
    }
    std::vector<byte> buf;
};

interface IOutputStream {
    void Write(byte value)
    {
        buf.push_back(value);
    }
    std::vector<byte> buf;
};

#endif // HUFFMAN_H
