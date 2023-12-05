/*
 * graph.cpp
 *
 * Method definitions for the graph class.
 *
 * Author: Tristen
 */

#include <algorithm>
#include <queue>
#include <vector>
#include "graph.h"

graph::graph(int vertex_count)
{
    numVertices = vertex_count;
    adjacencyList.resize(numVertices);
}

void graph::add_edge(int from, int to)
{
    adjacencyList[from].push_back(to);
}

vector<int> graph::find_shortest_path(int from, int to)
{
    vector<int> shortestPath;
    queue<int> q;
    vector<int> parent(numVertices, -1);

    // Initialization
    // Mark the source vertex as visited and enqueue it
    vector<bool> visited(numVertices, false);
    visited[from] = true;
    q.push(from);

    // BFS traversal
    while(!q.empty() && !visited[to]) 
    {
        // Dequeue a vertex and explore its neighbors
        int u = q.front();
        q.pop();

        for(int v : adjacencyList[u]) 
        {
            // If the neighbor is not visited, mark it as visited, enqueue it, and set its parent
            if(!visited[v]) 
            {
                visited[v] = true;
                q.push(v);
                parent[v] = u;
            }
        }
    }

    // Reconstruct the shortest path
    if(visited[to]) 
    {
        for(int v = to; v != -1; v = parent[v])
            shortestPath.push_back(v);
        // Reverse the path to get it from source to destination
        reverse(shortestPath.begin(), shortestPath.end());
    }
    // Return the resulting shortest path
    return shortestPath;
    
}
