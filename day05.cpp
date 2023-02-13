#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>

template<typename T>
void print(std::vector<T> vec){
    std::cout << "[";
    if(vec.size() == 0){std::cout << "]";}
    for(int i=0;i<vec.size();i++){
        std::cout << vec[i];
        if(i==vec.size()-1){std::cout << "]";}else{std::cout << ",";}
    }
    std::cout << std::endl;
}

std::vector<int> readline(std::string line){
    // takes a line in the file (std::string) nd returns the integers inside
    std::stringstream ss;
    ss << line;
    std::string tmp;
    std::vector<int> result;

    while(ss >> tmp){
        if(tmp[0] != 'm' and tmp[0] != 'f' and tmp[0] != 't'){result.push_back(std::stoi(tmp));}
    }

    return result;
}

int main(){
    std::fstream stackfile("day5_stacks.txt"); if(not stackfile.is_open()){std::cout << "File not open!" << std::endl; return -1;}

    std::string tmp;
    std::vector<std::vector<char> > stacks;
    
    // reading in the starting configuration
    int istack = 0;
    while(std::getline(stackfile,tmp)){
        if(tmp[0] != '['){break;}

        for(int i=1;i<tmp.size();i++){
            if(tmp[i-1] == '['){
                while(stacks.size() <= istack){stacks.push_back(std::vector<char>());}

                if(tmp[i] != '_'){
                    stacks[istack].insert(stacks[istack].begin(),tmp[i]);
                    i++;
                }
                istack++;
            }
        }
        istack = 0;
    }

    // let's move some containers around
    std::vector<int> move; std::vector<char> crane;
    while(std::getline(stackfile,tmp)){if(tmp[0] == 'm'){
        // what are we moving where?
        move = readline(tmp);
        // let's do it
        // task 1
        // for(int i=0;i<move[0];i++){
        //     stacks[move[2]-1].push_back(*stacks[move[1]-1].rbegin());       // moving the highest element in stacks[move[1]] to stacks[move[2]] (stack indices are decremented because they start at 1)
        //     stacks[move[1]-1].pop_back();                                   // removing the highest element from stacks[move[1]] (stack indices are decremented because they start at 1)
        // }
        // task 2
        stacks[move[2]-1].insert(stacks[move[2]-1].end(),stacks[move[1]-1].end()-move[0],stacks[move[1]-1].end());
        stacks[move[1]-1].erase(stacks[move[1]-1].end()-move[0],stacks[move[1]-1].end());

        std::cout << "Completed move "; print<int>(move);
    }}

    // outputting the last elements:
    for(int i=0;i<stacks.size();i++){
        if(stacks[i].size() == 0){
            std::cout << "_";
        }else{
            std::cout << *stacks[i].rbegin();
        }
    }
    std::cout << std::endl;

    return 0;
}