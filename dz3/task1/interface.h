#ifndef INTERFACE_H
#define INTERFACE_H

#include <iostream>
#include <vector>
#include <unordered_set>
#include <assert.h>

using std::vector;
using std::unordered_set;

struct IGraph {
    virtual ~IGraph() {}

    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const  = 0;

    virtual vector<int> GetNextVertices(int vertex) const = 0;
    virtual vector<int> GetPrevVertices(int vertex) const = 0;
};

#endif // INTERFACE_H
