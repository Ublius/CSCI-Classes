import networkx as nx
import sys

# Read input
inputLines = sys.stdin.read().strip().split('\n')
firstLline = inputLines[0].split()

villages = int(firstLline[0])
transit = int(firstLline[1])

startVillage = firstLline[2]
endVillage = firstLline[3]


graph = nx.DiGraph()

inputLines.pop(0)

adjDic = {}
for i in range(len(inputLines)):
    village1, village2, color, transitType =  inputLines[i].split()

    node1_L = (village1, village2, 'L') # From G to E (leaving)
    node2_E = (village1, village2, 'E') # From G to E (entering)
    node3_L = (village2, village1, 'L') # From E to G (leaving)
    node4_E = (village2, village1, 'E') # From E to G (entering)
    # print("start", startVillage)
    # print()
    # print(node1_L)
    # print(node2_E)
    # print(node3_L)
    # print(node4_E)
    # print()

    graph.add_edge(node1_L, node2_E, weight=10)
    graph.add_edge(node3_L, node4_E, weight=10)

    if village1 == startVillage:
        graph.add_edge("start", node1_L, weight=0)
        # print("hi", village1, startVillage)
    if village1 == endVillage:
        graph.add_edge(node4_E, "end", weight=0)
    if village2 == startVillage:
        graph.add_edge("start", node3_L, weight=0)
        # print("hi", village2, startVillage)
    if village2 == endVillage:
        graph.add_edge(node2_E, "end", weight=0)

    if village1 in adjDic:
        # append the new number to the existing array at this slot
        adjDic[village1] = adjDic[village1] + ((village2, color, transitType), )
    else:
         # create a new array in this slot
        adjDic[village1] = ((village2, color, transitType), )
    if village2 in adjDic:
        # append the new number to the existing array at this slot
        adjDic[village2] = adjDic[village2] + ((village1, color, transitType), )
    else:
        # create a new array in this slot
        adjDic[village2] = ((village1, color, transitType), )


# print(adjDic)

# cost = 0
for villageKey, value in adjDic.items():
    for adjvillage, color, transitType in value:
        for adjAdjVillage in adjDic[adjvillage]:
            if color == adjAdjVillage[1] and transitType == adjAdjVillage[2]:
                cost = 0
            elif transitType == adjAdjVillage[2]:
                cost =  5
            elif color == adjAdjVillage[1]:
                cost =  2
            else:
                continue

            enter = (villageKey, adjvillage, 'E')
            # print("Entern", enter)
            if villageKey != adjAdjVillage[0]:
                # print(villageKey, adjAdjVillage[0])
                leave = (adjvillage, adjAdjVillage[0], 'L')
                # print("Leave", leave)
                graph.add_edge(enter, leave, weight=cost)

try:
    # totalTime = nx.shortest_path_length(graph, "start", "end", weight="weight", method="dijkstra")
    totalTime = nx.dijkstra_path_length(graph, "start", "end", weight="weight")
    elements = nx.dijkstra_path(graph, "start", "end", weight="weight")

    print(totalTime)

    lst = []
    for i in elements:
        if i == 'start':
            continue
        elif i == 'end':
            continue
        else:
            lst.append(i[0])
    lst.append(endVillage)
    print(" ".join(lst[::2]))
except:
    print("NO PATH")
