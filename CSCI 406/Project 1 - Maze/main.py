import sys
import networkx as nx
# import matplotlib.pyplot as plt

# def get_transfer_time(line1, line2):
#     if line1['type'] == line2['type'] and line1['color'] == line2['color']:
#         return 0
#     elif line1['type'] == line2['type']:
#         return 5
#     elif line1['color'] == line2['color']:
#         return 2
#     else:
#         return 5

# Read input
inputLines = sys.stdin.read().strip().split('\n')
firstLline = inputLines[0].split()

villages = int(firstLline[0])
transit = int(firstLline[1])

startVillage = firstLline[2]
endVillage = firstLline[3]

graph = nx.Graph()

transitLines = []
for line in inputLines[1:]:
    village1, village2, color, transit_type = line.split()
    transitLines.append((village1, village2, color, transit_type))

for line in transitLines:
    village1, village2, color, type = line
    travel_time = 10  # Travel time between neighboring villages
    graph.add_edge((village1, color, type), (village2, color, type), weight=travel_time)
    
# try:
#     shortest_path = nx.dijkstra_path(graph, startVillage, endVillage, weight='weight')
#     shortest_path_length = nx.dijkstra_path_length(graph, startVillage, endVillage, weight='weight')
#     print(shortest_path_length)
#     print(" ".join(shortest_path))
# except nx.NetworkXNoPath:
#     print("NO PATH")                  
                    
# print(graph.edges(data=True)) 

