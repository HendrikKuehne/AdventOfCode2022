#include<iostream>
#include<fstream>
#include<iomanip>
#include<vector>
#include<assert.h>
#include<map>
#include"functions.h"

/// @brief Removes the substring " -> " from the lines of the input file and replaces it with a ",". Adds a "," at the beginning and the end of the line.
/// @param line Line of the input file to be trimmed.
std::string stringprep(std::string line){
    int i=0;
    while(i<line.size()){
        if(line[i] == ' ' or line[i] == '-' or line[i] == '>'){
            line[i] = ',';
        }else{
            i++;
        }
    }

    i=0;
    while(i<line.size()-1){
        if(line[i] == ',' and line[i+1] == ','){
            line.erase(line.begin()+i,line.begin()+i+1);
        }else{
            i++;
        }
    }

    line.insert(line.begin(),',');
    line.insert(line.end(),',');

    return line;
}

struct{
    // the slice of the cave
    std::map<int,std::map<int,char> > slice;
    // keeping track of the maximum to see when sand is falling into the abyss
    int y_max = 0;
    
    /// @brief Inserts a feature into the cave slice and updates y_max if necessary.
    /// @param block Character which represents the feature to be inserted.
    /// @param x,y Coordinates where the feature is to be inserted.
    /// @param empty_block The character which represents an empty block in the cave slice. Default is '.'.
    /// @param update_y_max controls whetjer cave::y_max is updated or not. When inserting grains of sand,
    /// we do not want cave::y_max to be updated since, otherwise, the floor would change it's height.
    void insert(char block,int x,int y,char empty_block='.',bool update_y_max = true){
        if(not slice.count(x)){slice[x] = std::map<int,char>();}
        slice[x][y] = block;

        if(update_y_max){if(y > y_max){y_max = y;}}
    }

    /// @brief Checks if a block in the cave slice is occupied by anything.
    /// @param x,y Coordinates to be checked.
    /// @return true if cave.slice[x][y] is unoccupied.
    bool check(int x,int y){
        // are the dimensions sufficiently large?
        if(not slice.count(x)){slice[x] = std::map<int,char>();}
        if(not slice[x].count(y)){slice[x][y] = '.';}

        if(slice[x][y] == '.'){return true;}else{return false;}
    }
} cave;

int main(){
    std::fstream file("day14_cave.txt"); if(not file.is_open()){std::cout << "File not open!" << std::endl; return -1;}
    std::string tmp,number; int i,j,x1,x2,y1,y2; std::vector<int> line;

    // drawing the slice of the cave
    cave.insert('+',500,0);
    while(std::getline(file,tmp)){
        tmp = stringprep(tmp);
        line = std::vector<int>();
        i=0;j=-1;

        // extracting all the coordinates from the current line
        while(i<tmp.size()){
            do{i++;}while(tmp[i] != ',');
            do{j++;}while(tmp[j] != ',');

            line.push_back(std::stoi(std::string(tmp.begin()+j+1,tmp.begin()+i)));

            i++; j++;
        }

        // drawing the paths of rock
        for(int i=0;i+3<line.size();i+=2){
            x1 = line[i]; y1 = line[i+1]; x2 = line[i+2]; y2 = line[i+3];

            int sgn_x = sgn(x2 - x1), sgn_y = sgn(y2 - y1);

            if(sgn_x != 0){
                for(int x=x1;x!=x2;x+=sgn_x){
                    cave.insert('#',x,y1);
                }
                cave.insert('#',x2,y1);
            }else{
                for(int y=y1;y!=y2;y+=sgn_y){
                    cave.insert('#',x1,y);
                }
                cave.insert('#',x1,y2);
            }
        }
    }

    file.close();

    bool finished = false;
    int x = 500,y = 0,n_grains = 0;

    while(false){       // part one
        // letting the current grain of sand fall
        if(cave.check(x,y+1)){
            y++;
        }else if(cave.check(x-1,y+1)){
            x--; y++;
        }else if(cave.check(x+1,y+1)){
            x++; y++;
        }else{
            // there's nowhere for this grain of sand to fall, so it settles down
            cave.insert('o',x,y);
            n_grains++;

            // next grain of sand incoming
            x = 500; y = 0;
        }

        if(y > cave.y_max){
            // a grain of sand reached the abyss
            finished = true;
        }
    }

    while(not finished){
        // checking if there is anywhere for the current grain of sand to fall
        if(not (cave.check(x,y+1) or cave.check(x-1,y+1) or cave.check(x+1,y+1)) or y == cave.y_max + 1){
            cave.insert('o',x,y,'.',false);
            n_grains++;

            if(y == 0){
                // the source is blocked
                finished = true;
            }

            // next grain of sand incoming
            x = 500; y = 0;

            continue;
        }

        if(y == cave.y_max + 1){
            std::cout << "Aha" << std::endl;
        }

        // letting the current grain of sand fall
        if(cave.check(x,y+1)){
            y++;
        }else if(cave.check(x-1,y+1)){
            x--; y++;
        }else if(cave.check(x+1,y+1)){
            x++; y++;
        }
    }

    std::cout << n_grains << " grains of sand settled down." << std::endl;

    return 0;
}