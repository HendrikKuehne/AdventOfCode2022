#include<iostream>
#include<fstream>
#include<vector>
#include"functions.h"

template<typename T>
std::vector<T> operator-(const std::vector<T>& lhs, const std::vector<T>& rhs){
    std::vector<T> result;
    for(int i=0;i<std::max(lhs.size(),rhs.size());i++){result.push_back(lhs[i] - rhs[i]);}
    return result;
}

bool touching(std::vector<int> head, std::vector<int> tail){
    // checks if head and tail are touching
    if(abs(head[0] - tail[0]) < 2 and abs(head[1] - tail[1]) < 2){return true;}
    return false;
}

struct dict{
    std::vector<std::vector<int> > dict;

    bool contains(std::vector<int> entry){
        for(int i=0;i<dict.size();i++){
            if(dict[i] == entry){return true;}
        }
        return false;
    }

    void add(std::vector<int> entry){
        if(not contains(entry)){
            dict.push_back(entry);
        }
    }
};

struct knot{
    // a structure to keep track of where the tail has been
    dict map;
    std::vector<int> loc;
    int counter;
    std::string name;

    void visit(std::vector<int> pos){
        // increment map[i][j] by one and increment the counter if map[i,j] is a new map
        if(not map.contains(pos)){counter++;}
        map.add(pos);
    }

    void move(std::vector<int> head){
        std::vector<int> sep = head - loc;
        // moves the tail towards the head according to the separation specified
        if(not touching(head,loc)){
            loc[1] += sgn(sep[1]);
            loc[0] += sgn(sep[0]);

            this->visit(loc);
        }
    }

    void move(char dir){
        if(dir == 'U'){
            loc[0] += 1;
        }else if(dir == 'R'){
            loc[1] += 1;
        }else if(dir == 'D'){
            loc[0] -= 1;
        }else if(dir == 'L'){
            loc[1] -= 1;
        }else{
            std::cout << "Unknown direction received in move()" << std::endl;
        }
    }

    knot(){
        loc = std::vector<int>(2,0);
        counter = 0;
        visit(loc);
        name = " ";
    }

    knot(std::string name_in){
        loc = std::vector<int>(2,0);
        counter = 0;
        visit(loc);
        name = name_in;
    }
};

// enabling knot to be forwarded to the output stream
std::ostream &operator<<(std::ostream &os, const knot &value){std::cout << value.name;return os;}

void print_knots(std::vector<knot> knots){
    // to print the current situation, we need to find the minimum coordinates in every direction
    int min_x=0,min_y=0;
    for(std::vector<knot>::iterator itr = knots.begin();itr < knots.end();itr++){
        if(itr->loc[0] < min_x){min_x = itr->loc[0];}
        if(itr->loc[1] < min_y){min_y = itr->loc[1];}
    }

    // preparing the field
    std::vector<std::vector<std::string> > field;
    for(std::vector<knot>::iterator itr = knots.begin();itr < knots.end();itr++){
        // extending the field so that it can accomodate the next knot
        while(field.size() <= itr->loc[0] - min_x){field.push_back(std::vector<std::string>());}
        while(field[itr->loc[0]].size() <= itr->loc[1] - min_y){field[itr->loc[0]].push_back(" ");}

        // inserting the current knot
        field[itr->loc[0] - min_x][itr->loc[1] - min_y] = itr->name;
    }

    print(field);
}

int main(){
    // reading the file
    std::fstream file("day9_moves.txt"); if(not file.is_open()){std::cout << "File not open!" << std::endl; return -1;}

    // initializing the head and n subsequent knots
    int n = 9;
    std::vector<knot> knots(n+1,knot());

    // giving names to the knots
    knots[0].name = "H";
    for(int i=1;i<knots.size();i++){knots[i].name = std::to_string(i);}

    // infrastructure
    std::string tmp;
    int n_moves = 0;
    char dir;

    while(std::getline(file,tmp)){
        // extracting the move to be made
        dir = tmp[0];
        n_moves = std::stoi(std::string(tmp.begin()+2,tmp.end()));

        // std::cout << "Moving to " << dir << " " << n_moves << " times." << std::endl;

        // moving
        for(int i=0;i<n_moves;i++){
            // moving the head
            knots[0].move(dir);
            // moving the subsequent knots
            for(int i=1;i<knots.size();i++){
                knots[i].move(knots[i-1].loc);
            }
        }

        // print_knots(knots); std::cout << std::endl;
    }

    std::cout << knots[n].counter << std::endl;

    return 0;
}