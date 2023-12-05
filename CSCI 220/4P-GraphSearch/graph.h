/*
 * graph.h
 *
 * Declaration of the graph class.
 * 
 * Author: Tristen
 */

#ifndef _GRAPH_BFS_H
#define _GRAPH_BFS_H

#include <vector>

using namespace std;

class graph {
public:
    graph(int vertex_count);
    void add_edge(int from, int to);
    vector<int> find_shortest_path(int from, int to);

private:
    vector<vector<int>> adjacencyList;
    int numVertices;
};

#endif
