/*
 * Author: C. Painter Wakefield
 *
 * Tests for the graph BFS project.
 */

#include <vector>
#include <fstream>
#include <algorithm>

#include "gtest/gtest.h"
#include "graph.h"

using namespace std;

bool pair_compare(vector<int> a, vector<int> b) {
    if (a[0] != b[0]) {
        return a[0] < b[0];
    } else {
        return a[1] < b[1];
    }
}

// fixture to load in large graphs for testing
class GraphTest: public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        ifstream fin("large_graph.txt");
        if (!fin) {
            FAIL() << "Error opening input file, skipping all tests.";
            return;
        }

        int edge_count;
        fin >> large_vertex_count >> edge_count;

        large_graph.resize(edge_count);

        for (size_t i = 0; i < size_t(edge_count); i++) {
            int from, to;
            fin >> from >> to;
            if (from < 0 || from >= large_vertex_count || to < 0 || to >= large_vertex_count) {
                fin.close();
                FAIL() << "Invalid edge " << from << "->" << to << " in input file detected, skipping all tests.";
                return;
            }
            large_graph[i] = {from, to};
        }

        fin.close();
    }

    static void TearDownTestSuite() {
        large_graph.clear();
    }

    static int large_vertex_count;
    static vector<vector<int>> large_graph;
};

int GraphTest::large_vertex_count = 0;
vector<vector<int>> GraphTest::large_graph = vector<vector<int>>();

TEST_F(GraphTest, TwoNodesNoEdge) {
    graph g(2);
    ASSERT_EQ(g.find_shortest_path(0, 1), vector<int>({}));
}

TEST_F(GraphTest, TwoNodesOneEdgeGoodPath) {
    graph g(2);
    g.add_edge(0, 1);
    ASSERT_EQ(g.find_shortest_path(0, 1), vector<int>({0, 1}));
}

TEST_F(GraphTest, TwoNodesOneEdgeNoPath) {
    graph g(2);
    g.add_edge(1, 0);
    ASSERT_EQ(g.find_shortest_path(0, 1), vector<int>({}));
}

TEST_F(GraphTest, ThreeNodesLength2Path1) {
    graph g(3);
    g.add_edge(0, 1);
    g.add_edge(1, 2);
    g.add_edge(2, 1);
    ASSERT_EQ(g.find_shortest_path(0, 2), vector<int>({0, 1, 2}));
}

TEST_F(GraphTest, ThreeNodesLength2Path2) {
    graph g(3);
    g.add_edge(0, 2);
    g.add_edge(1, 2);
    g.add_edge(2, 1);
    ASSERT_EQ(g.find_shortest_path(0, 1), vector<int>({0, 2, 1}));
}

TEST_F(GraphTest, ThreeNodesLength2Path3) {
    graph g(3);
    g.add_edge(0, 2);
    g.add_edge(2, 0);
    g.add_edge(1, 0);
    ASSERT_EQ(g.find_shortest_path(1, 2), vector<int>({1, 0, 2}));
}

TEST_F(GraphTest, ThreeNodes2Paths) {
    graph g(3);
    g.add_edge(0, 1);
    g.add_edge(0, 2);
    g.add_edge(1, 2);
    ASSERT_EQ(g.find_shortest_path(0, 2), vector<int>({0, 2}));
}

TEST_F(GraphTest, ThreeNodesNoPath) {
    graph g(3);
    g.add_edge(0, 1);
    g.add_edge(2, 1);
    ASSERT_EQ(g.find_shortest_path(0, 2), vector<int>({}));
}

TEST_F(GraphTest, FourNodesCycleNoPath) {
    graph g(4);
    g.add_edge(0, 1);
    g.add_edge(1, 2);
    g.add_edge(2, 0);
    ASSERT_EQ(g.find_shortest_path(0, 3), vector<int>({}));
}

TEST_F(GraphTest, TenNodesVariousPaths) {
    graph g(10);
    g.add_edge(4, 6);
    g.add_edge(6, 2);
    g.add_edge(9, 3);
    g.add_edge(3, 7);
    g.add_edge(7, 3);
    g.add_edge(7, 4);
    g.add_edge(8, 1);
    g.add_edge(9, 2);
    g.add_edge(1, 7);
    g.add_edge(0, 7);
    g.add_edge(3, 0);
    g.add_edge(3, 6);
    g.add_edge(4, 5);
    g.add_edge(4, 1);
    g.add_edge(9, 1);
    g.add_edge(0, 9);
    g.add_edge(7, 9);
    ASSERT_EQ(g.find_shortest_path(6, 0), vector<int>({}));
    ASSERT_EQ(g.find_shortest_path(8, 5), vector<int>({8, 1, 7, 4, 5}));
    ASSERT_EQ(g.find_shortest_path(1, 2), vector<int>({1, 7, 9, 2}));
}

TEST_F(GraphTest, LargeGraphTests) {
    graph g(large_vertex_count);
    for (auto edge: large_graph) {
        // undirected graph, add edges in both directions
        g.add_edge(edge[0], edge[1]);
        g.add_edge(edge[1], edge[0]);
    }

    vector<int> p1 = g.find_shortest_path(2, 17819);
    ASSERT_TRUE(p1.empty());

    vector<int> p2 = g.find_shortest_path(56, 17783);
    ASSERT_EQ(p2.size(), 6);
    for (size_t i = 0; i < p2.size() - 1; i++) {
        // require each edge to exist
        bool forward = binary_search(large_graph.begin(), large_graph.end(), vector<int>({p2[i], p2[i+1]}), pair_compare);
        bool backward = binary_search(large_graph.begin(), large_graph.end(), vector<int>({p2[i+1], p2[i]}), pair_compare);
        ASSERT_TRUE(forward || backward);
    }

    vector<int> p3 = g.find_shortest_path(864, 15491);
    ASSERT_EQ(p3.size(), 6);
    for (size_t i = 0; i < p3.size() - 1; i++) {
        // require each edge to exist
        bool forward = binary_search(large_graph.begin(), large_graph.end(), vector<int>({p3[i], p3[i+1]}), pair_compare);
        bool backward = binary_search(large_graph.begin(), large_graph.end(), vector<int>({p3[i+1], p3[i]}), pair_compare);
        ASSERT_TRUE(forward || backward);
    }

    vector<int> p4 = g.find_shortest_path(16603, 17095);
    ASSERT_EQ(p4.size(), 10);
    for (size_t i = 0; i < p4.size() - 1; i++) {
        // require each edge to exist
        bool forward = binary_search(large_graph.begin(), large_graph.end(), vector<int>({p4[i], p4[i+1]}), pair_compare);
        bool backward = binary_search(large_graph.begin(), large_graph.end(), vector<int>({p4[i+1], p4[i]}), pair_compare);
        ASSERT_TRUE(forward || backward);
    }
}