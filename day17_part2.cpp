#include<iostream>
#include<fstream>
#include<iomanip>
#include<vector>
#include<assert.h>
#include<map>
#include"functions.h"
#include"day17_functions.h"

/*
The idea is the following: The pattern in the falling blocks is periodic in n_airblows*n_blocks (n_airblows is the number of
air blows) in the given file, n_blocks is the number of different blocks (5, in our case). We thus do not need to place all
blocks to solve part 2; the periodicity can be exploited, we only need to know how tall the periodic part is.

we need to consider that we do not know how the rocks of the second period will be placed on the first. If the first block
of the second period is placed on a block which is not the peak of the first period, the periodic height (the height that the
stack grows by in subsequent periods) will be uneqal to the height of the first period.

The solution will do the following: It places the first period and the second period and then determines the height of the
second period, which is the periodic height. It adds as many periodic heights as possible to the height and stacks the remaining
blocks normally.
*/

bool output = false;

int main(){
    std::fstream file("day17_air_test.txt"); if(not file.is_open()){std::cout << "File not open!" << std::endl; return -1;}

    std::string airblows; std::getline(file,airblows);

    file.close();

    // creating the circular buffers that contain air blow patterns and blocks
    reservoir<std::string,char> airreservoir(airblows);
    // std::vector<blocktype> blocktypes = {plus,square};
    std::vector<blocktype> blocktypes = {horizontal,plus,L,vertical,square};
    reservoir<std::vector<blocktype>,blocktype> blockreservoir(blocktypes);

    /*
    the rocks which are falling and have settled down will be contained in a vector of vectors called chamber, with the
    first dimension being the width of the chamber (so it has size 7). The second dimension is the height. The vectors
    contained in the first dimension of chamber must always have the same size!
    */
    
    std::vector<std::vector<char> > chamber(7,std::vector<char>());

    // determining the periodicity
    int periodicity = airreservoir.variables.size() * blockreservoir.variables.size();
    int first_period_height, periodic_height;

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

        if(airreservoir.i == 0 and blockreservoir.i == 0){
            std::cout << "Periodicity after " << i_rocks << " rocks!" << std::endl;
        }
    }

    height += get_height(chamber);

    std::cout << "The stack of rocks is " << height << " units tall." << std::endl;

    return 0;
}