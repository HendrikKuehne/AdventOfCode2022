#include<iostream>
#include<fstream>
#include<iomanip>
#include<vector>
#include<assert.h>
#include<map>
#include"functions.h"

/// @brief Shows the current chamber configuration as it is displayed in the instructions for this problem.
/// @param chamber vector of vectors containing chars which describes the current configuration. It is assumed
/// and checked for that all vectors contained in chamber have the same size!
void print_chamber(std::vector<std::vector<char> > chamber){
    // sanity check
    int size = chamber[0].size(); for(int i=1;i<chamber.size();i++){assert(chamber[i].size() == size);}
    
    // printing
    for(int i=chamber[0].size()-1;i>=0;i--){
        std::cout << "|";
        for(int j=0;j<chamber.size();j++){
            std::cout << chamber[j][i];
        }
        std::cout << "|" << std::endl;
    }
    std::cout << "+-------+" << std::endl << std::endl;
}

/// @brief Returns the height of the pile of rocks in chamber.
/// @param chamber as in print_chamber().
int get_height(std::vector<std::vector<char> > chamber){
    // sanity check
    int size = chamber[0].size(); for(int i=1;i<chamber.size();i++){assert(chamber[i].size() == size);}

    int i=chamber[0].size() - 1;

    while(i >= 0){
        if(   chamber[0][i] == '#'
           or chamber[1][i] == '#'
           or chamber[2][i] == '#'
           or chamber[3][i] == '#'
           or chamber[4][i] == '#'
           or chamber[5][i] == '#'
           or chamber[6][i] == '#'){
            return i + 1;
        }
        i--;
    }
    return i + 1;
}

typedef enum {horizontal,plus,L,vertical,square} blocktype;
std::ostream &operator<<(std::ostream &os, const blocktype &value){
    if(value == horizontal){std::cout << "horizontal"; return os;}
    if(value == plus){std::cout << "plus"; return os;}
    if(value == L){std::cout << "L"; return os;}
    if(value == vertical){std::cout << "vertical"; return os;}
    if(value == square){std::cout << "square"; return os;}
    std::cout << value; return os;
}

/// @brief A circular buffer that contains variables and implements a function reservoir::get_next() that always resturns the nect variable from the buffer (starting from the beginning if we have reached the end).
/// @tparam container typename that contains the variables
/// @tparam content typename of the variables themselves
template<typename container,typename content>
struct reservoir{
    container variables;
    int i;

    content get_next(){
        return variables[(i++) % variables.size()];
    }

    reservoir(container _variables){
        variables = _variables;
        i = 0;
    }
};

struct block{
    blocktype block_type;
    int x, y, height, width;

    /// @brief Draws the encompassing block in the chamber and prints the whole chamber.
    void draw(std::vector<std::vector<char> > chamber){
        // drawing the currently falling block
        if(block_type == horizontal){
            chamber[x][y] = '@'; chamber[x+1][y] = '@'; chamber[x+2][y] = '@'; chamber[x+3][y] = '@';
        }else if(block_type == plus){
            chamber[x][y-1] = '@'; chamber[x+1][y] = '@'; chamber[x+1][y-1] = '@'; chamber[x+1][y-2] = '@'; chamber[x+2][y-1] = '@';
        }else if(block_type == L){
            chamber[x][y-2] = '@'; chamber[x+1][y-2] = '@'; chamber[x+2][y] = '@'; chamber[x+2][y-1] = '@'; chamber[x+2][y-2] = '@';
        }else if(block_type == vertical){
            chamber[x][y] = '@'; chamber[x][y-1] = '@'; chamber[x][y-2] = '@'; chamber[x][y-3] = '@';
        }else if(block_type == square){
            chamber[x][y] = '@'; chamber[x][y-1] = '@'; chamber[x+1][y] = '@'; chamber[x+1][y-1] = '@';
        }

        // printing the complete chamber
        print_chamber(chamber);
    }

    /// @brief Checks if a block can move to either side. Returns true if the block can move in the given direction.
    bool check_side(char direction,std::vector<std::vector<char> > chamber){
        // sanity check
        assert(direction == '<' or direction == '>');

        // are we at the walls of the chamber?
        if((x <= 0 and direction == '<') or (x + width >= 7 and direction == '>')){return false;}

        // checking the neighbouring space
        int move = (direction == '<') ? -1 : 1;

        if(block_type == horizontal){
            if(chamber[x+move][y] == '#' or chamber[x+move+1][y] == '#' or chamber[x+move+2][y] == '#' or chamber[x+move+3][y] == '#'){return false;}
        }else if(block_type == plus){
            if(chamber[x+move][y-1] == '#' or chamber[x+move+1][y] == '#' or chamber[x+move+1][y-1] == '#' or chamber[x+move+1][y-2] == '#' or chamber[x+move+2][y-1] == '#'){return false;}
        }else if(block_type == L){
            if(chamber[x+move][y-2] == '#' or chamber[x+move+1][y-2] == '#' or chamber[x+move+2][y] == '#' or chamber[x+move+2][y-1] == '#' or chamber[x+move+2][y-2] == '#'){return false;}
        }else if(block_type == vertical){
            if(chamber[x+move][y] == '#' or chamber[x+move][y-1] == '#' or chamber[x+move][y-2] == '#' or chamber[x+move][y-3] == '#'){return false;}
        }else if(block_type == square){
            if(chamber[x+move][y] == '#' or chamber[x+move][y-1] == '#' or chamber[x+move+1][y] == '#' or chamber[x+move+1][y-1] == '#'){return false;}
        }

        return true;
    }

    /// @brief Checks if a block can fall.
    bool check_below(std::vector<std::vector<char> > chamber){
        // are we at the bottom of the chamber?
        if(y - height < 0){return false;}

        if(block_type == horizontal){
            if(chamber[x][y-1] == '#' or chamber[x+1][y-1] == '#' or chamber[x+2][y-1] == '#' or chamber[x+3][y-1] == '#'){return false;}
        }else if(block_type == plus){
            if(chamber[x][y-2] == '#' or chamber[x+1][y-3] == '#' or chamber[x+2][y-2] == '#'){return false;}
        }else if(block_type == L){
            if(chamber[x][y-3] == '#' or chamber[x+1][y-3] == '#' or chamber[x+2][y-3] == '#'){return false;}
        }else if(block_type == vertical){
            if(chamber[x][y-4] == '#'){return false;}
        }else if(block_type == square){
            if(chamber[x][y-2] == '#' or chamber[x+1][y-2] == '#'){return false;}
        }

        return true;
    }

    /// @brief Places the encompassing block in the chamber.
    void place(std::vector<std::vector<char> >& chamber){
        // palcing the current block
        if(block_type == horizontal){
            chamber[x][y] = '#'; chamber[x+1][y] = '#'; chamber[x+2][y] = '#'; chamber[x+3][y] = '#';
        }else if(block_type == plus){
            chamber[x][y-1] = '#'; chamber[x+1][y] = '#'; chamber[x+1][y-1] = '#'; chamber[x+1][y-2] = '#'; chamber[x+2][y-1] = '#';
        }else if(block_type == L){
            chamber[x][y-2] = '#'; chamber[x+1][y-2] = '#'; chamber[x+2][y] = '#'; chamber[x+2][y-1] = '#'; chamber[x+2][y-2] = '#';
        }else if(block_type == vertical){
            chamber[x][y] = '#'; chamber[x][y-1] = '#'; chamber[x][y-2] = '#'; chamber[x][y-3] = '#';
        }else if(block_type == square){
            chamber[x][y] = '#'; chamber[x][y-1] = '#'; chamber[x+1][y] = '#'; chamber[x+1][y-1] = '#';
        }
    }

    block(blocktype _block_type,std::vector<std::vector<char> >& chamber){
        block_type = _block_type;

        // setting block height and width
        if(block_type == horizontal){
            height = 1; width = 4;
        }else if(block_type == plus){
            height = 3; width = 3;
        }else if(block_type == L){
            height = 3; width = 3;
        }else if(block_type == vertical){
            height = 4; width = 1;
        }else if(block_type == square){
            height = 2; width = 2;
        }

        // setting the initial position of the block
        x = 2;
        y = get_height(chamber) + height + 2;

        // extending the chamber such that it can contain the new block
        while(chamber[0].size() <= y){
            for(int i=0;i<7;i++){chamber[i].push_back('.');}
        }
    }

    block(){};
};

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

    int n_rocks = 2022;
    block current_block; char air_direction;
    // looping through all blocks
    for(int i_rocks=0;i_rocks<n_rocks;i_rocks++){
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
    }

    std::cout << "The stack of rocks is " << get_height(chamber) << " units tall." << std::endl;

    return 0;
}