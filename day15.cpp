#include<iostream>
#include<fstream>
#include<iomanip>
#include<vector>
#include<assert.h>
#include<map>
#include"functions.h"

typedef enum{sensor,beacon,blanc} cellstate;

/// @brief Returns the distance between point 1 and point 2, calculated using the Manhatten Distance
int L1(std::vector<int> vec1, std::vector<int> vec2){
    assert(vec1.size() == 2);
    assert(vec2.size() == 2);
    return std::abs(vec1[0] - vec2[0]) + std::abs(vec1[1] - vec2[1]);
}

/// @brief Returns the distance between point 1 and point 2, calculated using the Manhatten Distance
/// @param x1 x-ccordinate of point 1
/// @param y1 y-ccordinate of point 1
/// @param x2 x-ccordinate of point 2
/// @param y2 y-ccordinate of point 2
int L1(int x1,int y1,int x2, int y2){
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

/// @brief Returns the distance between point 1 and point 2, calculated using the Manhatten Distance
/// @param coords[0] x-ccordinate of point 1
/// @param coords[1] y-ccordinate of point 1
/// @param coords[2] x-ccordinate of point 2
/// @param coords[3] y-ccordinate of point 2
int L1(std::vector<int> coords){
    assert(coords.size() == 4);
    return std::abs(coords[0] - coords[2]) + std::abs(coords[1] - coords[3]);
}

/// @brief Checks if there is a beacon at the point [x,y]
/// @param x,y point to check
/// @param coords vector where coords[:,2:] contains the coordinates of all beacons
/// @return true if there is a beacon at [x,y]
bool is_beacon(int x,int y,std::vector<std::vector<int> > coords){
    for(int i=0;i<coords.size();i++){
        if(x == coords[i][2] and y == coords[i][3]){
            return true;
        }
    }
    return false;
}

/// @brief Checks if the point [x,y] is ruled out by any sensor
/// @param x,y point to check
/// @param coords vector where coords[:,0:2] contains the coordinates of all sensors
/// @param distances vector which contains the distances of the sensor and beacons in coords
/// @return true if there exists a sensor such that [x,y] is ruled out
bool is_ruled_out(int x,int y,std::vector<std::vector<int> > coords,std::vector<int> distances){
    std::cout << "    Checking [" << x << "," << y << "]" << std::endl;
    for(int i=0;i<coords.size();i++){
        if(L1(x,y,coords[i][0],coords[i][1]) <= distances[i]){
            return true;
        }
    }
    return false;
}

/// @brief Parses one line of the input file and returns a vector containing sensor and beacon coordinates
/// @param tmp string that contains one line of the input file 
/// @return vector with coordinates in the order [sx,sy,bx,by]
std::vector<int> parse(std::string tmp){
    std::vector<int> returnval(4,0);
    int i=0,j=0;

    tmp = std::string(tmp.begin()+12,tmp.end());
    while(tmp[i]!=','){i++;}
    returnval[0] = std::stoi(std::string(tmp.begin(),tmp.begin()+i));
    while(tmp[i]!='='){i++;} i++;
    while(tmp[j]!=':'){j++;}
    returnval[1] = std::stoi(std::string(tmp.begin()+i,tmp.begin()+j));


    // extracting beacon coordinates
    tmp = std::string(tmp.begin()+j+25,tmp.end());
    i=0;
    while(tmp[i]!=','){i++;}
    returnval[2] = std::stoi(std::string(tmp.begin(),tmp.begin()+i));
    while(tmp[i]!='='){i++;} i++;
    returnval[3] = std::stoi(std::string(tmp.begin()+i,tmp.end()));

    return returnval;
}

struct cell{
    cellstate state;
    int ruled_out;

    cell(cellstate _state = blanc,int _ruled_out=0){
        state = _state;
        ruled_out = _ruled_out;
    }
};

// enabling cell to be forwarded to the output stream
std::ostream &operator<<(std::ostream &os, const cell &value)
{
    if(value.state == sensor){
        std::cout << "S";
        return os;
    }else if(value.state == beacon){
        std::cout << "B";
        return os;
    }else if(value.ruled_out){
        std::cout << ".";
        return os;
    }else{
        std::cout << " ";
        return os;
    }
}

struct{
    // 2D-map that will eventually contain the whole grid
    std::map<long int,std::map<long int,cell> > grid;
    int xmin,xmax,ymin,ymax;

    void print(){
        for(int x=xmin;x<xmax;x++){std::cout << "-";} std::cout << std::endl;

        for(int y=ymax-1;y>=ymin;y--){
            for(int x=xmin;x<xmax;x++){
                std::cout << " " << grid[x][y];
            } std::cout << std::endl;
        }
    }

    /// @brief Recursively marks the region which is ruled out by a sensor with a given distance to the nearest
    /// beacon. Doing this recursively works for the example but not for the actual problem - I think I hit memory
    /// limits.
    /// @param sx,sy coordinates of sensor
    /// @param x,y current point
    /// @param distance between sensor and beacon
    /// @param sensor_index used to distinguish which cell is ruled out be which sensor
    void mark_region_recursively(int sx,int sy,int x,int y, int distance,int sensor_index){
        // marking the current point
        grid[x][y].ruled_out = sensor_index;

        std::cout << "L1(" << sx << "," << sy << "," << x << "," << y << ") = " << L1(sx,sy,x,y) << " <= " << distance << " = distance" << std::endl;
        // recursion time
        if(L1(sx,sy,x+1,y  ) <= distance and grid[x+1][y  ].ruled_out != sensor_index){mark_region_recursively(sx,sy,x+1,y  ,distance,sensor_index);}
        if(L1(sx,sy,x-1,y  ) <= distance and grid[x-1][y  ].ruled_out != sensor_index){mark_region_recursively(sx,sy,x-1,y  ,distance,sensor_index);}
        if(L1(sx,sy,x  ,y+1) <= distance and grid[x  ][y+1].ruled_out != sensor_index){mark_region_recursively(sx,sy,x  ,y+1,distance,sensor_index);}
        if(L1(sx,sy,x  ,y-1) <= distance and grid[x  ][y-1].ruled_out != sensor_index){mark_region_recursively(sx,sy,x  ,y-1,distance,sensor_index);}
    }

    /// @brief Marks the region which is ruled out by a sensor - thus does the same thing
    /// as mark_region_recursively, however not recursively.
    void mark_region(int sx,int sy,int distance){
        int x = sx - distance - 1;
        int y_lim_low = sy+1, y_lim_high = sy-1;

        while(x < sx){
            x++; y_lim_low--; y_lim_high++;
            for(int y=y_lim_low;y<=y_lim_high;y++){
                grid[x][y].ruled_out = true;
            }
        }

        while(x <= sx + distance){
            x++; y_lim_low++; y_lim_high--;
            for(int y=y_lim_low;y<=y_lim_high;y++){
                grid[x][y].ruled_out = true;
            }
        }
    }

    /// @brief Writes supplied beacon and vector coordinates to the grid and marks the area that is ruled out by them
    /// @param coords Coordinates of sensor and beacon in one vector (coords = [sx,sy,bx,by])
    void insert(std::vector<int> coords,int sensor_index){
        assert(coords.size() == 4);
        int sx=coords[0],sy=coords[1],bx=coords[2],by=coords[3];

        // expanding the grid, if necessary
        int distance = L1(coords);
        while(sx - distance <= xmin){grid[xmin-1] = std::map<long int,cell>(); xmin--;}
        while(sx + distance >= xmax){grid[xmax+1] = std::map<long int,cell>(); xmax++;}

        for(int x=xmin;x<xmax;x++){
            while(sy - distance <= ymin){grid[x][ymin-1] = cell(blanc); ymin--;}
            while(sy + distance >= ymax){grid[x][ymax+1] = cell(blanc); ymax++;}
        }

        // inserting new sensor and beacon into the grid
        grid[coords[0]][coords[1]] = cell(sensor,sensor_index);
        grid[coords[2]][coords[3]] = cell(beacon,sensor_index);

        // labeling the cells which are ruled out by this sensor and the beacon it detects
        mark_region_recursively(sx,sy,sx,sy,distance,sensor_index);
        // mark_region(sx,sy,distance);
    }
} grid;

int main(){
    // initialising the grid
    grid.xmin = 0;
    grid.ymin = 0;
    grid.xmax = 0;
    grid.ymax = 0;

    std::fstream file("day15/day15_beacons.txt"); if(not file.is_open()){std::cout << "File not open!" << std::endl; return -1;}

    std::string tmp; // int sensor_index = 0;
    std::vector<std::vector<int> > coords;
    std::vector<int> distances;
    while(std::getline(file,tmp)){
        // sensor_index++;
        coords.push_back(parse(tmp));
        distances.push_back(L1(parse(tmp)));

        /*
        print(coords);
        std::cout << "L1 = " << L1(coords) << std::endl;

        grid.insert(coords,sensor_index);
    
        // grid.print();
        */
    }

    file.close();

    // filling the whole field does not seem to work; we will do it another way

    // finding upper and lower bounds on x
    int x_min = 0, x_max = 0;
    for(int i=0;i<coords.size();i++){
        if(x_min > coords[i][0] - distances[i]){x_min = coords[i][0] - distances[i];}
        if(x_max < coords[i][0] + distances[i]){x_max = coords[i][0] + distances[i];}
    }

    // going through the respective row and checking for each spot if it has been ruled out or not
    int counter = 0, y = 10;
    for(int x=x_min;x<=x_max;x++){
        // checking for every sensor if it rules out this spot
        for(int i=0;i<coords.size();i++){
            if(L1(x,y,coords[i][0],coords[i][1]) <= distances[i] and (not is_beacon(x,y,coords))){
                // the spot [x,y] is as close or closer to a sensor than it's respective beacon - it has thus been ruled out (and there is no beacon at this spot)
                counter++;
                break;
            }
        }
    }

    std::cout << "There are " << counter << " which are ruled out." << std::endl;

    // task two: Strategy: A single remaining not-ruled-out spot must lie at the edge of one the regions which
    // are ruled out by the sensors. Our strategy is thus to check the edges of these regions for spots which
    // are not blocked
    int lim = 4000000,sx,sy,off,antioff;
    for(int i=0;i<coords.size();i++){
        if(coords[i][0] - distances[i] <= lim and coords[i][1] - distances[i] <= lim){
            std::cout << "Sensor at [" << coords[i][0] << "," << coords[i][1] << "] with distance = " << distances[i] << std::endl;
            // the region corresponding to sensor i reaches into the area where we search for a non-ruled-out spot
            sx = coords[i][0];
            sy = coords[i][1];

            // iterating through all points just outside the current region
            for(int j=0;j<=distances[i]+1;j++){
                off = j; antioff = distances[i] + 1 - j;
                if((not is_ruled_out(sx + off,sy + antioff,coords,distances)) and (sx + off <= lim) and (sx + off >= 0) and (sy + antioff <= lim) and (sy + antioff >= 0)){std::cout << "        [" << sx + off << "," << sy + antioff << "] is not ruled out! Distress Frequency = " << (sx + off)*4000000 + sy + antioff << std::endl; return 0;}
                if((not is_ruled_out(sx + off,sy - antioff,coords,distances)) and (sx + off <= lim) and (sx + off >= 0) and (sy - antioff <= lim) and (sy - antioff >= 0)){std::cout << "        [" << sx + off << "," << sy - antioff << "] is not ruled out! Distress Frequency = " << (sx + off)*4000000 + sy - antioff << std::endl; return 0;}
                if((not is_ruled_out(sx - off,sy + antioff,coords,distances)) and (sx - off <= lim) and (sx - off >= 0) and (sy + antioff <= lim) and (sy + antioff >= 0)){std::cout << "        [" << sx - off << "," << sy + antioff << "] is not ruled out! Distress Frequency = " << (sx - off)*4000000 + sy + antioff << std::endl; return 0;}
                if((not is_ruled_out(sx - off,sy - antioff,coords,distances)) and (sx - off <= lim) and (sx - off >= 0) and (sy - antioff <= lim) and (sy - antioff >= 0)){std::cout << "        [" << sx - off << "," << sy - antioff << "] is not ruled out! Distress Frequency = " << (sx - off)*4000000 + sy - antioff << std::endl; return 0;}
            }
        }
    }

    std::cout << "Is [14,11] ruled out? " << is_ruled_out(14,11,coords,distances) << std::endl;

    return 0;
}
