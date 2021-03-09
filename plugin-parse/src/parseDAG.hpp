#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adj_list_serialize.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/bellman_ford_shortest_paths.hpp>
#include <boost/property_map/dynamic_property_map.hpp>
#include <boost/property_map/function_property_map.hpp>
#include <boost/graph/graph_utility.hpp>
#include "parseTasks.hpp"
#include <ostream>
#include <fstream>
#include <iostream>
#include <vector>
#include <boost/graph/topological_sort.hpp>

#define MINSIZE 100

using namespace boost;

struct Vertex {
    std::string name, label, fillcolor, style;
    double compilationTime;
    double timeElapsed; // Added by Me
};

struct Edge {
    std::string color;
};

typedef property<graph_name_t, std::string> graph_p;
typedef adjacency_list<vecS, vecS, bidirectionalS, Vertex, Edge, graph_p> graph_t;
using weight_map_t = boost::property_map<graph_t, double Edge::*>::type;


void init_graph(graph_t &g, std::vector<double> &taskTime);
double criticalPathLength(graph_t &g, std::vector<double> &taskTime);
void drawCriticalPath(graph_t &g, double maxRecordedValue,int lastTaskDone);
std::vector<int> criticalLastTasks(graph_t &g,double maxRecordedValue);