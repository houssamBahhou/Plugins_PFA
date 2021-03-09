#include "parseDAG.hpp"

int main(int argc, char*argv[]) {
        
    if(argc <3){
        printf("Usage : ./parse filedag.dot filetask.rec\n");
        return 0;
    }

    std::string filename1 = argv[1];
    std::string filename2 = argv[2];

    // Construct an empty graph and prepare the dynamic_property_maps.
    // graph_t graph(graph_p{"LR"});
    graph_t graph(0);

    // Construct table for parsing times for Tasks.rec
    std::vector<double> taskTime (MINSIZE, 0);
    // Parse the tasks file
    parseTask(filename2,taskTime);

    dynamic_properties dp(ignore_other_properties);
    dp.property("node_id",   get(&Vertex::name,      graph));
    dp.property("label",     get(&Vertex::label,     graph));
    dp.property("fillcolor", get(&Vertex::fillcolor, graph));
    dp.property("style", get(&Vertex::style, graph));
    //dp.property("weight",     get(&Edge::weight,     graph));
    dp.property("color",     get(&Edge::color,       graph));
    //dp.property("rankdir", boost::make_constant_property<graph_t*>(std::string("LR")));

    // Use ref_property_map to turn a graph property into a property map
    boost::ref_property_map<graph_t *, std::string> gname(get_property(graph, graph_name));
    dp.property("rankdir",    gname);
    std::ifstream dot(filename1);


    if (read_graphviz(dot, graph, dp/*, "node_id"*/)) {

        // --- // get_property(graph, graph_name) = "Critical Path DAG";
        init_graph(graph,taskTime);
        double criticalLength = criticalPathLength(graph, taskTime);
        std::vector<int> lastTasks = criticalLastTasks(graph,criticalLength);
        drawCriticalPath(graph,criticalLength,lastTasks[0]);
        
        std::cout << " " << std::endl;
        std::cout << "-----> La longueur du chemin critique est " << criticalLength << std::endl;
        std::cout << "<----- Highlighting the critical path in the file traces/CriticalPath_DAG.dot " << std::endl;
        std::cout << "PS: - To draw an image of the original file: make draw_original " << std::endl;
        std::cout << "    - To draw an image of the new file: make draw_new " << std::endl;
        //criticalLastTasks(graph, criticalLength);
        std::ofstream dotOutput("traces/CriticalPath_DAG.dot");
        write_graphviz_dp(dotOutput, graph, dp);
        
    }
    return 0;
}