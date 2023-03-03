#include<iostream>
#include<fstream>
#include<iomanip>
#include<vector>
#include<assert.h>
#include<map>
#include"functions.h"
#include"day17_functions.h"

bool output = false;

int main(){
    std::fstream file("day17_air.txt"); if(not file.is_open()){std::cout << "File not open!" << std::endl; return -1;}

    std::string airblows; std::getline(file,airblows);

    file.close();

    // creating the circular buffers that contain air blow patterns and blocks
    reservoir<std::string,char> airreservoir(airblows);
    std::vector<blocktype> blocktypes = {horizontal,plus,L,vertical,square};
    reservoir<std::vector<blocktype>,blocktype> blockreservoir(blocktypes);

    /*
    the rocks which are falling and have settled down will be contained in a vector of vectors called chamber, with the
    first dimension being the width of the chamber (so it has size 7). The second dimension is the height. The vectors
    contained in the first dimension of chamber must always have the same size!
    */
    
    std::vector<std::vector<char> > chamber(7,std::vector<char>());

    float n_rocks = 2022, min_height; float height = 0;
    block current_block; char air_direction;
    // looping through all blocks
    for(float i_rocks=0;i_rocks<n_rocks;i_rocks++){
        // fetching the next block and the next air blow
        current_block = block(blockreservoir.get_next(),chamber);

        if(::output){current_block.draw(chamber);}

        // fetching the next air blow
        air_direction = airreservoir.get_next();
        // moving the block, if possible
        if(current_block.check_side(air_direction,chamber)){
            current_block.x += (air_direction == '<') ? -1 : 1;
        }

        if(::output){std::cout << "Pushing in direction " << air_direction << std::endl << std::endl; current_block.draw(chamber);}

        do{
            // letting the block fall
            current_block.y -= 1;

            if(::output){std::cout << "Falling" << std::endl << std::endl; current_block.draw(chamber);}

            // fetching the next air blow
            air_direction = airreservoir.get_next();
            // moving the block, if possible
            if(current_block.check_side(air_direction,chamber)){
                current_block.x += (air_direction == '<') ? -1 : 1;
            }

            if(::output){std::cout << "Pushing in direction " << air_direction << std::endl << std::endl; current_block.draw(chamber);}
        }while(current_block.check_below(chamber));

        current_block.place(chamber);

        if(::output){print_chamber(chamber);}

        if(get_height(chamber) > 100000){
            // since new blocks appear at the top of the chamber (i.e. the end of the vectors contained in chamber),
            // we may delete some of the rows to free up space. We can only delete rows which are inaccessible from above,
            // so we introduce the dunction get_min_height which returns the height of the smallest column in the chamber
            // (i.e. the height up to which we can delete).

            min_height = get_min_height(chamber);

            height += min_height;

            for(int i=0;i<7;i++){chamber[i].erase(chamber[i].begin(),chamber[i].begin()+min_height);}

            // status update
            std::cout << i_rocks << std::setw(13) << " out of " << n_rocks << " rocks." << std::endl;
        }
    }

    height += get_height(chamber);

    std::cout << "The stack of rocks is " << height << " units tall." << std::endl;

    return 0;
}