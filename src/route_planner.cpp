#include "route_planner.h"
#include <algorithm>

using std::cout;
using std::sort;

//JAQ: I think my understanding of this needs to improve. FindClosestNeigbour seems to return a reference
//to a node, which is a private member variable. I'm then returning a pointer to this variable, from the reference.
//So I think this pointer is defined on the stack, but as a member variable, it won't be cleaned up in this method
//or the calling method? So this seems fairly safe, but also could result in a bug if the model is cleaned up?
//I wonder why m_Nodes isn't defined as a vector of pointers? 
RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
    start_node = &model.FindClosestNode(start_x, start_y);

    //JAQ: Almost missed this. Without it, you get an loop in the datastructure, and then your path traversal fails
    start_node->visited = true;
    end_node = &model.FindClosestNode(end_x, end_y);
}


// TODO 3: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {

    return node->distance(*end_node);
}


// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// Tips:
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {

     current_node->FindNeighbors();

     for(auto neighbor : current_node->neighbors){

         //This seems already checked for in the FindNeighbor method
         //if(neighbor->visited)
         //   continue;

        neighbor->parent = current_node;

        float distanceToParent = neighbor->distance(*current_node);

        neighbor->h_value = CalculateHValue(neighbor);
        neighbor->g_value = current_node->g_value + distanceToParent;
        neighbor->visited = true;
        open_list.push_back(neighbor);
     }
}


// TODO 5: Complete the NextNode method to sort the open list and return the next node.
// Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.

RouteModel::Node *RoutePlanner::NextNode() {
    sort(open_list.begin(), open_list.end(), RouteModel::Node::Compare);
    
    auto lowestSum = open_list.back();

    open_list.pop_back();

    return lowestSum;

}


// TODO 6: Complete the ConstructFinalPath method to return the final path found from your A* search.
// Tips:
// - This method should take the current (final) node as an argument and iteratively follow the 
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to the distance variable.
// - The returned vector should be in the correct order: the start node should be the first element
//   of the vector, the end node should be the last element.

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    // TODO: Implement your solution here.

    //Follow up the chain of parent nodes until found the starting nodes
    auto evalNode = current_node;
    path_found.push_back(*evalNode);
    distance = evalNode->g_value;

    while(evalNode->parent != nullptr){
        //For each node, accumulate distance and push the node
        //distance += evalNode->distance(*evalNode->parent); //This is already summed in gValue, so why recompute it?
        path_found.push_back(*evalNode->parent);

        evalNode = evalNode->parent;
    }

    //reverse path
    std::reverse(path_found.begin(), path_found.end());

    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}


// TODO 7: Write the A* Search algorithm here.
// Tips:
// - Use the AddNeighbors method to add all of the neighbors of the current node to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits. This path will then be displayed on the map tile.

void RoutePlanner::AStarSearch() {

    
    //RouteModel::Node *current_node = nullptr;
    RouteModel::Node *current_node = start_node; //Is this right??

    // TODO: Implement your solution here.
    AddNeighbors(current_node);
    
    while(open_list.size() > 0){

        auto nextNode = NextNode();

        //Better to compare the pointer addresses, rather than floats
        //if(nextNode->x == end_node->x && nextNode->y == end_node->y){
        if(nextNode == end_node){
            m_Model.path = ConstructFinalPath(nextNode);

            return;
        }

        AddNeighbors(nextNode);
    }
    return;
}