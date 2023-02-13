#include<iostream>
#include<fstream>
#include<iomanip>
#include<vector>
#include"functions.h"

struct cube{
    bool is_lava;
    bool visited;

    cube(bool _is_lava = false){
        is_lava = _is_lava;
        visited = false;
    }
};

struct{
    std::vector<std::vector<std::vector<cube> > > grid;
    int area;

    void unvisit(){
        for(int ix=0;ix<grid.size();ix++){
            for(int iy=0;iy<grid[ix].size();iy++){
                for(int iz=0;iz<grid[ix][iy].size();iz++){
                    grid[ix][iy][iz].visited = false;
                }
            }
        }
    }

    /// @brief Determines if the cube at [x,y,z] lies within an air bubble
    /// @param x,y,z Coordinates of the starting cube
    bool is_air_pocket(int x,int y,int z){
        if(grid[x][y][z].is_lava){
            // this cube obviously doesn't belong to an enclosed air pocket
            return false;
        }
        if(x == grid.size() - 1 or y == grid[x].size() - 1 or z == grid[x][y].size() - 1 or x == 0 or y == 0 or z == 0){
            // we're at the edge of the grid and thus at the exterior of the droplet - this is not an enclosed air pocket
            return false;
        }

        grid[x][y][z].visited = true;
        // we'll go recursively through each neighbour of this air cube and see if it leads us to the exterior or not
        if((not grid[x+1][y  ][z  ].is_lava) and (not grid[x+1][y  ][z  ].visited)){if(not is_air_pocket(x+1,y  ,z  )){return false;}}
        if((not grid[x-1][y  ][z  ].is_lava) and (not grid[x-1][y  ][z  ].visited)){if(not is_air_pocket(x-1,y  ,z  )){return false;}}
        if((not grid[x  ][y+1][z  ].is_lava) and (not grid[x  ][y+1][z  ].visited)){if(not is_air_pocket(x  ,y+1,z  )){return false;}}
        if((not grid[x  ][y-1][z  ].is_lava) and (not grid[x  ][y-1][z  ].visited)){if(not is_air_pocket(x  ,y-1,z  )){return false;}}
        if((not grid[x  ][y  ][z+1].is_lava) and (not grid[x  ][y  ][z+1].visited)){if(not is_air_pocket(x  ,y  ,z+1)){return false;}}
        if((not grid[x  ][y  ][z-1].is_lava) and (not grid[x  ][y  ][z-1].visited)){if(not is_air_pocket(x  ,y  ,z-1)){return false;}}
        
        return true;
    }

    /// @brief Fills the air bubble in which [x,y,z] lies with lava cubes - no check is performed if [x,y,z] actually lies within an air bubble!
    /// @param x,y,z Coordinates of the starting cube
    void fill_air_pocket(int x,int y,int z){
        if(x == grid.size() - 1 or y == grid[x].size() - 1 or z == grid[x][y].size() - 1 or x == 0 or y == 0 or z == 0){
            return;
        }
        std::cout << "    Filling [" << x << "," << y << "," << z << "]" << std::endl;
        // fill the current cube
        grid[x][y][z].is_lava = true;
        // add to the area counter since this cube's sides will be removed in the second step of Area_Cleanup()
        area += 6;
        
        // we'll go recursively through each neighbour of this air cube and fill the remainders
        if((not grid[x+1][y  ][z  ].is_lava)){fill_air_pocket(x+1,y  ,z  );}
        if((not grid[x-1][y  ][z  ].is_lava)){fill_air_pocket(x-1,y  ,z  );}
        if((not grid[x  ][y+1][z  ].is_lava)){fill_air_pocket(x  ,y+1,z  );}
        if((not grid[x  ][y-1][z  ].is_lava)){fill_air_pocket(x  ,y-1,z  );}
        if((not grid[x  ][y  ][z+1].is_lava)){fill_air_pocket(x  ,y  ,z+1);}
        if((not grid[x  ][y  ][z-1].is_lava)){fill_air_pocket(x  ,y  ,z-1);}
    }

    void add(std::vector<int> coords){
        // expanding the grid, if necessary (while ensuring that all dimensions have the same size)
        while(grid.size() <= coords[0]+1){grid.push_back(std::vector<std::vector<cube> >());}
        for(int x=0;x<grid.size();x++){
            while(grid[x].size() <= coords[1]+1){grid[x].push_back(std::vector<cube>());}
            for(int y=0;y<grid[x].size();y++){
                while(grid[x][y].size() <= coords[2]+1){grid[x][y].push_back(cube());}
            }
        }

        // saving the current cube
        grid[coords[0]][coords[1]][coords[2]].is_lava = true;
        area += 6;
    }

    void search_air_pockets(){
        // testing every point in the grid if it is an enclosed air pocket or not
        for(int ix=0;ix<grid.size();ix++){
            for(int iy=0;iy<grid[ix].size();iy++){
                for(int iz=0;iz<grid[ix][iy].size();iz++){
                    if(is_air_pocket(ix,iy,iz)){
                        std::cout << "[" << ix << "," << iy << "," << iz << "] lies within an air pocket." << std::endl;
                    }
                    unvisit();
                }
            }
        }
        std::cout << std::endl;
    }

    void Area_Cleanup(){
        // Strategy for task two: Fill all inner air bubbles with cubes such that their surfaces won't be counted
        for(int x=0;x<grid.size();x++){
            for(int y=0;y<grid[x].size();y++){
                for(int z=0;z<grid[x][y].size();z++){
                    if(is_air_pocket(x,y,z)){
                        std::cout << "Found air pocket at [" << x << "," << y << "," << z << "]. Filling:" << std::endl;
                        fill_air_pocket(x,y,z);
                    }
                    unvisit();
                }
            }
        }

        // task one: looking for neighbouring sides
        for(int x=0;x+1<grid.size();x++){
            for(int y=0;y+1<grid[x].size();y++){
                for(int z=0;z+1<grid[x][y].size();z++){
                    // neighbouring sides don't count (this only works because all dimensions have the same size and we padded the grid with one extra entry in each dimension!)
                    if(grid[x][y][z].is_lava and grid[x+1][y][z].is_lava){area -= 2;}
                    if(grid[x][y][z].is_lava and grid[x][y+1][z].is_lava){area -= 2;}
                    if(grid[x][y][z].is_lava and grid[x][y][z+1].is_lava){area -= 2;}
                }
            }
        }
    }
} droplet;

int main(){
    droplet.area = 0;

    std::fstream file("day18_cubes.txt"); if(not file.is_open()){std::cout << "File not open!" << std::endl; return -1;}
    
    std::string tmp;
    while(std::getline(file,tmp)){
        std::vector<int> coords;

        if(tmp.size() == 0){break; /*stopping at an empty line*/}
        if(tmp[0] == '#'){continue; /*# is the comment character*/}

        // reading in the coordinates
        while(tmp.size() > 0){
            if(tmp[0] == ','){tmp.erase(tmp.begin());}
            int i=0;
            while(tmp[i]!=',' and i < tmp.size()){i++;}
            coords.push_back(std::stoi(std::string(tmp.begin(),tmp.begin()+i)));
            for(int j=0;j<i;j++){tmp.erase(tmp.begin());}
        }

        // std::cout << "Reading in "; print(coords);
        if(coords[1] == 10 and coords[2] == 12){
            std::cout << "Reading in "; print(coords);
        }

        droplet.add(coords);
    }
    file.close();

    // droplet.search_air_pockets();

    droplet.Area_Cleanup();
    std::cout << std::endl << "Exterior Surface Area A = " << droplet.area << std::endl;

    return 0;
}