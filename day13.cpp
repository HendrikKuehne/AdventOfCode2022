#include<iostream>
#include<fstream>
#include<iomanip>
#include<vector>
#include"functions.h"

std::string make_list(std::string input){
    input.insert(input.begin(),'[');

    std::string::iterator itr = input.begin();

    do{itr++;}while(*itr != ',' and *itr != ']');

    input.insert(itr,']');

    return input;
}

std::string extract_element(std::string& input){
    int level = 0;
    std::string::iterator itr = input.begin();

    while(itr < input.end()){
        if(*itr == '['){level++;}else if(*itr == ']'){level--;}
        itr++;
        if(*itr == ',' and level == 0){
            std::string tmp = std::string(input.begin(),itr);
            input.erase(input.begin(),itr);
            return tmp;
        }
    }
    std::string tmp = input;
    input = "";
    return tmp;
}

void compare(std::string& left, std::string& right, bool& result){
    // We compare the packets left and right recursively; one call to compare() compares the outermost bracket
    // in left and right

    // removing the outermost brackets
    left.erase(left.begin()); right.erase(right.begin());

    while(left.size() > 0 and right.size() > 0){
        // both packets contain elements to compare. We deal with the two next elements and remove them afterwards.
        // the possibilities are: Two lists, one list in either left or right or to integers

        if(left[0] == '[' and right[0] == '['){
            // the next elements are two lists, so we go one recursion deeper
            left.erase(left.begin()); right.erase(right.begin());
            compare(left,right,result);
        }
    }

    if(left.size() > 0){
        // the left packet contains more elements than the right one, the packets are not ordered
        result = false;
    }
}

int main(){
    std::fstream file("day13_packets_test.txt"); if(not file.is_open()){std::cout << "File not open!" << std::endl; return -1;}

    std::string left, right, tmp;
    int pair_index = 1,sum = 0;
    bool comparison;
    while(std::getline(file,left) and std::getline(file,right)){
        // the next two lines are contained in the strings left and right
        left.erase(left.begin()); right.erase(right.begin());
        left.erase(left.end()-1); right.erase(right.end()-1);
        
        std::cout << "pair " << std::setw(3) << pair_index << "" << std::endl;
        // let's compare and, if ordered, add the corresponding pair index to the sum:
        comparison = true;
        // compare(left,right,comparison);
        if(comparison){
            sum += pair_index;

            // std::cout << left << std::endl << right << right << std::endl << std::endl;
        }


        // fetching the empty line
        std::getline(file,tmp);
        pair_index++;
    }

    file.close();

    std::cout << "Sum of components = " << sum << std::endl;

    return 0;
}