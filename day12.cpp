#include<iostream>
#include<fstream>
#include<vector>
#include<iomanip>
#include"functions.h"

int max_distance = 100000;

/*
 * Dings
 */
struct node{
    std::vector<node*> neighbours;
    int distance;
    int height;
    char heightchar;
    bool visited;

    node(char _height){
        distance = max_distance;
        height = (_height == 'S') ? 0 : ((_height == 'E') ? 'z' - 'a' : _height - 'a');
        heightchar = _height;
        visited = false;
    }

    node(int _height){
        distance = max_distance;
        height = _height;
        visited = false;
        heightchar = '?';
    }
};

// arithmetic using nodes
int operator-(node lhs, node rhs){return lhs.height - rhs.height;}
int operator-(node lhs, int rhs){return lhs.height - rhs;}
int operator-(int lhs, node rhs){return lhs - rhs.height;}
int operator+(node lhs, int rhs){return lhs.height + rhs;}
int operator+(int lhs, node rhs){return lhs + rhs.height;}

// logic using nodes
bool operator<(node lhs, node rhs){return lhs.height < rhs.height;}
bool operator<=(node lhs, node rhs){return lhs.height <= rhs.height;}
bool operator>(node lhs, node rhs){return lhs.height > rhs.height;}
bool operator>=(node lhs, node rhs){return lhs.height >= rhs.height;}

// printing nodes
std::ostream &operator<<(std::ostream &os, node &value){
    if(value.heightchar == 'S' or value.heightchar == 'E'){std::cout << value.heightchar; return os;}
    // std::cout << value.neighbours.size(); return os;
    // std::cout << value.height; return os;
    if(value.distance != max_distance){std::cout << value.distance; return os;}
    std::cout << "_"; return os;
}

/*
 * recursively creating a list of all the nodes in a graph based on the fact that they have a visited attribute
*/
std::vector<node*> unvisited_list(std::vector<node*> list, node* current_node){
    for(int i=0;i < current_node->neighbours.size();i++){
        if(not current_node->neighbours[i]->visited){
            // we have found a node which we havent seen before; better see it and add it to the list
            current_node->neighbours[i]->visited = true;
            list.push_back(current_node->neighbours[i]);
            // starting unvisited_list from this unvisited node
            list = unvisited_list(list,current_node->neighbours[i]);
        }
    }

    return list;
}

void dijkstra(node* current_node){
    // initializing the algorithm
    current_node->distance = 0;
    // creating the list of all unvisited nodes
    std::vector<node*> unvisited = unvisited_list(std::vector<node*>(),current_node);
    // during list creation, all the nodes are marked as visited, which we dont want
    for(std::vector<node*>::iterator itr = unvisited.begin();itr < unvisited.end();itr++){(*itr)->visited = false;}

    // for finding the minimum-distance-node among the unvisited nodes
    int min_distance;
    std::vector<node*>::iterator min_dist_itr;

    // actual algorithm
    while(unvisited.size() > 0){
        // extracting the next node, e.g. the one with the smallest tentative distance
        min_distance = max_distance;
        for(std::vector<node*>::iterator itr = unvisited.begin();itr < unvisited.end();itr++){
            if((*itr)->distance < min_distance){
                min_dist_itr = itr;
                min_distance = (*itr)->distance;
            }
        }

        // setting the next node
        current_node = *min_dist_itr;

        // updating the tentative distance values
        for(std::vector<node*>::iterator itr = current_node->neighbours.begin(); itr < current_node->neighbours.end();itr++){
            if((*itr)->distance > current_node->distance + 1){
                // this neighbour's tentative distance must be updated
                (*itr)->distance = current_node->distance + 1;
            }
        }

        // marking the current node as visited and removing it from the list
        current_node->visited = true;
        unvisited.erase(min_dist_itr);
    }
}

/*
 * Terminiert nicht! Wer weiß warum nicht...
 */
void dijkstra_hendrik(node* current_node){
    // initializing the algorithm
    current_node->distance = 0;
    std::vector<node*> unvisited;

    for(int i=0;i<current_node->neighbours.size();i++){
        unvisited.push_back(current_node->neighbours[i]);
    }
    unvisited.push_back(current_node);

    // for finding the minimum-distance-node among the unvisited nodes
    int min_distance;
    std::vector<node*>::iterator min_dist_itr;

    while(unvisited.size() > 0){
        // find the unvisited node with the shortest distance and set it as the current node
        min_distance = max_distance;
        for(std::vector<node*>::iterator itr = unvisited.begin();itr < unvisited.end();itr++){
            if((*itr)->distance < min_distance){
                min_distance = (*itr)->distance;
                min_dist_itr = itr;
            }
        }
        current_node = *min_dist_itr;
        unvisited.erase(min_dist_itr);

        // update all the unvisited neighbours of the current node
        for(int i=0;i<current_node->neighbours.size();i++){
            if(current_node->distance + 1 < current_node->neighbours[i]->distance and (not current_node->neighbours[i]->visited)){
                current_node->neighbours[i]->distance = current_node->distance + 1;
            }
        }

        // mark the current node as visited
        current_node->visited = true;

        // add all the unvisited neighbours of the current node to the list of unvisited nodes
        for(int i=0;i<current_node->neighbours.size();i++){
            if(not current_node->neighbours[i]->visited){
                unvisited.push_back(current_node->neighbours[i]);
            }
        }

        // debugging
    }
}

int main(){
    std::fstream file("day12_heightmap.txt"); if(not file.is_open()){std::cout << "File not open!" << std::endl; return -1;}
    
    // reading in the file
    std::string tmp;
    std::vector<std::vector<char> > height;
    int i=0, istart, jstart, ifinish, jfinish;
    while(std::getline(file,tmp)){
        height.push_back(std::vector<char>());
        for(int j=0;j<tmp.size();j++){
            // adding the new node
            height[i].push_back(tmp[j]);
            // std::cout << "field[" << i << "][" << j << "]: Node with height " << field[i][j].height << std::endl;

            if(tmp[j] == 'S'){istart = i; jstart = j;}
            if(tmp[j] == 'E'){ifinish = i; jfinish = j;}
        }
        i++;
    }
    file.close();

    // creating a graph with linked nodes (field_reverselinks is used for part two of day twelve - it's connections are
    // reversed when compared to field because it is traversed beginning from the end node)
    std::vector<std::vector<node> > field,field_reverselinks;
    for(int i = 0;i < height.size();i++){
        field.push_back(std::vector<node>());
        field_reverselinks.push_back(std::vector<node>());
        for(int j=0;j < height[i].size();j++){
            // creating the node
            field[i].push_back(node(height[i][j]));
            field_reverselinks[i].push_back(node(height[i][j]));
        }
    }

    // making the connections between nodes in field and field_reverselinks
    for(int i=0;i<field.size();i++){
        for(int j=0;j<field[i].size();j++){
            // upper conncetion
            if(i-1>=0){if(field[i][j] + 2 > field[i-1][j]){
                field[i][j].neighbours.push_back(&(field[i-1][j]));
                field_reverselinks[i-1][j].neighbours.push_back(&(field_reverselinks[i][j]));
            }}
            // left connection
            if(j-1>=0){if(field[i][j] + 2 > field[i][j-1]){
                field[i][j].neighbours.push_back(&(field[i][j-1]));
                field_reverselinks[i][j-1].neighbours.push_back(&(field_reverselinks[i][j]));
            }}
            // lower connection
            if(i+1<field.size()){if(field[i][j] + 2 > field[i+1][j]){
                field[i][j].neighbours.push_back(&(field[i+1][j]));
                field_reverselinks[i+1][j].neighbours.push_back(&(field_reverselinks[i][j]));
            }}
            // right connection
            if(j+1<field[i].size()){if(field[i][j] + 2 > field[i][j+1]){
                field[i][j].neighbours.push_back(&(field[i][j+1]));
                field_reverselinks[i][j+1].neighbours.push_back(&(field_reverselinks[i][j]));
            }}
        }
    }

    std::cout << "Field linked" << std::endl;

    // dijkstra(&(field[istart][jstart]));
    // if(field[ifinish][jfinish].distance == max_distance){std::cout << "Endpoint is unreachable?!?" << std::endl; return -1;}
    // std::cout << "The endpoint is " << field[ifinish][jfinish].distance << " units away." << std::endl;

    // for the second part, I will start dijkstra in field_reverselinks from field_reverselinks[ifinish][jfinish] and,
    // afterwards, search all nodes where heightchar == 'a' for the minimum distance
    dijkstra(&(field_reverselinks[ifinish][jfinish]));

    // searching for the minimum distance
    int min_distance = max_distance;
    for(int i=0;i<field_reverselinks.size();i++){
        for(int j=0;j<field_reverselinks[i].size();j++){
            if(field_reverselinks[i][j].distance < min_distance and field_reverselinks[i][j].height == 0){
                min_distance = field_reverselinks[i][j].distance;
            }
        }
    }

    std::cout << "The shortest path is " << min_distance << " units long." << std::endl;

    return 0;
}