import networkx as nx
import sys
from networkx.drawing.nx_agraph import to_agraph

def readInput():
    inputLines = sys.stdin.read().strip().split('\n')
    firstLine = inputLines[0].split()
    startVillage = firstLine[2]
    endVillage = firstLine[3]

    return inputLines[1:], startVillage, endVillage


def buildGraph(inputLines, startVillage, endVillage):
    graph = nx.DiGraph()
    adjDic = {}

    # Process each village pair and build the graph
    for line in inputLines:
        village1, village2, color, transitType = line.split()

        # Create nodes for traveling from village1 to village2 and vice versa
        # 0 = Leaving, 1 = Entering
        node1L = (village1, village2, 0)  # Leaving village1, going to village2
        node2E = (village1, village2, 1)  # Entering village2 from village1
        node3L = (village2, village1, 0)  # Leaving village2, going to village1
        node4E = (village2, village1, 1)  # Entering village1 from village2

        # Add base edges with a weight of 10 for direct connections
        graph.add_edge(node1L, node2E, weight=10)
        graph.add_edge(node3L, node4E, weight=10)

        # Connect start village to its leaving node and end village to its entering node
        if village1 == startVillage:
            graph.add_edge("start", node1L, weight=0)
        if village1 == endVillage:
            graph.add_edge(node4E, "end", weight=0)
        if village2 == startVillage:
            graph.add_edge("start", node3L, weight=0)
        if village2 == endVillage:
            graph.add_edge(node2E, "end", weight=0)

        # Populate adjacency dictionary for future transition checks
        if village1 in adjDic:
            adjDic[village1] = adjDic[village1] + ((village2, color, transitType), )
        else:
            adjDic[village1] = ((village2, color, transitType), )
        if village2 in adjDic:
            adjDic[village2] = adjDic[village2] + ((village1, color, transitType), )
        else:
            adjDic[village2] = ((village1, color, transitType), )

    return graph, adjDic


def calculateCost(color, adjColor, transitType, adjTransitType):
    if color == adjColor and transitType == adjTransitType:
        return 0  # Same color and transit type
    elif transitType == adjTransitType:
        return 5  # Same transit type
    elif color == adjColor:
        return 2  # Same color
    else:
        return None  # No valid transition


def addTransitionCosts(graph, adjDic):
    for village, neighbors in adjDic.items():
        for adjVillage, color, transitType in neighbors:
            for adjAdjVillage, adjColor, adjTransitType in adjDic.get(adjVillage, []):
                # Calculate the transition cost using the separate function
                cost = calculateCost(color, adjColor, transitType, adjTransitType)

                # Only add edges where a valid cost is found
                if cost is not None and village != adjAdjVillage:
                    # Define the nodes involved in this transition
                    enter = (village, adjVillage, 1)  # Entering the adjacent village
                    leave = (adjVillage, adjAdjVillage, 0)  # Leaving the adjacent village

                    # Add the edge between entering and leaving nodes with the calculated cost
                    graph.add_edge(enter, leave, weight=cost)


def findShortestPath(graph, endVillage):
    try:
        # Calculate the shortest path length and the path itself
        totalTime = nx.dijkstra_path_length(graph, "start", "end", weight="weight")
        path = nx.dijkstra_path(graph, "start", "end", weight="weight")

        # Output the total time to traverse
        print(totalTime)

        # Build the path, ignoring the 0 and 1 states for the final output
        lst = []
        for i in path:
            if i == 'start':
                continue
            elif i == 'end':
                continue
            else:
                lst.append(i[0])
        lst.append(endVillage)
        print(" ".join(lst[::2]))

        return path  # Return the path for visualization

    except nx.NetworkXNoPath:
        print("NO PATH")
        return None


def saveGraph(graph, path=None, filename="graph_output.png"):
    A = to_agraph(graph)

    # Set edge labels to display weights (costs)
    for u, v, data in graph.edges(data=True):
        A.get_edge(u, v).attr['label'] = str(data['weight'])

    # Highlight the shortest path nodes and edges if a path is provided
    if path:
        # Highlight nodes in the shortest path
        for node in path:
            A.get_node(node).attr.update(color='red', style='filled', fillcolor='yellow')

        # Highlight edges in the shortest path
        for u, v in zip(path[:-1], path[1:]):
            A.get_edge(u, v).attr.update(color='blue', penwidth='2')

    A.layout('dot')  # You can use different layouts like 'dot', 'circo', 'neato', etc.
    A.draw(filename)


def main():
    # Read input
    inputLines, startVillage, endVillage = readInput()

    # Build the graph and adjacency dictionary
    graph, adjDic = buildGraph(inputLines, startVillage, endVillage)

    # Add transition costs based on color and transit type
    addTransitionCosts(graph, adjDic)

    # Find the shortest path
    path = findShortestPath(graph, endVillage)

    # Save the graph to a DOT file, including edge weights and highlighting the shortest path
    saveGraph(graph, path, "graph_output.png")


if __name__ == "__main__":
    main()
