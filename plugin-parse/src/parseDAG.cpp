#include "parseDAG.hpp"

int getJobId(std::string task){
      std::string strId = task.substr (5); // task_88 ---> 88
      int jobId = std::stoi(strId);
      return jobId;
}

void init_graph(graph_t &g, std::vector<double> &taskTime){
    // Order the vertices topologically
    std::deque<int> topo_order;
    topological_sort(g, std::front_inserter(topo_order),
    vertex_index_map(identity_property_map()));
    
    // Computes the length of the longest path ending at the v where v is every vertx in the graph in topological order
    int n = 1;
    for (std::deque<int>::iterator i = topo_order.begin(); i != topo_order.end(); ++i, ++n){
        if(getJobId(g[*i].name) >= (int) taskTime.size()){
            g[*i].compilationTime = 0;
        }
        else
            g[*i].compilationTime = getTime(getJobId(g[*i].name),taskTime);
        std::cout << "Execution Time for : " << g[*i].name << " is : " << g[*i].compilationTime << std::endl;
        g[*i].timeElapsed = 0;
        if (in_degree(*i, g)){
            double maxTimeElapsed = 0;
            std::pair<boost::graph_traits<graph_t>::in_edge_iterator, boost::graph_traits<graph_t>::in_edge_iterator> e = in_edges(*i, g);
            while(e.first != e.second){
                boost::graph_traits<graph_t>::edge_descriptor ed = *e.first;
                boost::graph_traits<graph_t>::vertex_descriptor s = source(ed, g);
                maxTimeElapsed = std::max(maxTimeElapsed, g[s].timeElapsed);
                e.first++;
            }
            std::cout << "hello " << g[*i].name << std::endl;        
            g[*i].timeElapsed = maxTimeElapsed + g[*i].compilationTime;
        }
        else{
            g[*i].timeElapsed = g[*i].compilationTime;    
        }
        std::cout << "Time Elapsed for : " << g[*i].name << " is " << g[*i].timeElapsed << std::endl;
    }
}

double criticalPathLength(graph_t &g, std::vector<double> &taskTime){
    // search the criticalPathLength by iterating on vertices
    double maxRecordedValue = 0;
    typedef boost::graph_traits<graph_t>::vertex_iterator vertex_iterator_t;
    for ( std::pair<vertex_iterator_t, vertex_iterator_t> vp = vertices( g ); 
      vp.first != vp.second; ++vp.first ){
        maxRecordedValue = std::max(maxRecordedValue, g[*vp.first].timeElapsed);
    }
    return maxRecordedValue;
}

void drawCriticalPath(graph_t &g, double maxRecordedValue,int lastTaskDone){
    while(in_degree(lastTaskDone, g)){
        std::cout << g[lastTaskDone].name << std::endl;
        std::cout << "    |    " << std::endl;
        double preMaxRecordedValue = 0;
        std::pair<boost::graph_traits<graph_t>::in_edge_iterator, boost::graph_traits<graph_t>::in_edge_iterator> e = in_edges(lastTaskDone, g);
        boost::graph_traits<graph_t>::vertex_descriptor rightTargetTask = lastTaskDone;
        while(e.first != e.second){
            boost::graph_traits<graph_t>::edge_descriptor ed = *e.first;
            boost::graph_traits<graph_t>::vertex_descriptor s = source(ed, g);
            double maxDidNotChange = preMaxRecordedValue;
            preMaxRecordedValue = std::max(preMaxRecordedValue, g[s].timeElapsed);
            if (std::fabs(preMaxRecordedValue - maxDidNotChange) >= 0.0001 || preMaxRecordedValue == 0 ){
                lastTaskDone = s;
            }
            e.first++;            
        }
        remove_edge(lastTaskDone, rightTargetTask, g);
        add_edge(lastTaskDone, rightTargetTask, Edge{"red"}, g);
    }
    std::cout << g[lastTaskDone].name << std::endl;
}

//Return a vector that contain last tasks for all critical paths
std::vector<int> criticalLastTasks(graph_t &g,double maxRecordedValue){
    std::vector<int> criticalLastTasks;
    std::cout << maxRecordedValue << std::endl;
    typedef boost::graph_traits<graph_t>::vertex_iterator vertex_iterator_t;
    for ( std::pair<vertex_iterator_t, vertex_iterator_t> vp = vertices( g ); 
      vp.first != vp.second; ++vp.first ){
        if( g[*vp.first].timeElapsed == maxRecordedValue && out_degree(*vp.first, g) == 0 ){
            criticalLastTasks.push_back(*vp.first);
            std::cout << g[*vp.first].name << std::endl;
        }
    }
    return criticalLastTasks;
}