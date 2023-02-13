#include<iostream>
#include<fstream>
#include<vector>

std::vector<int> find_range(std::string line){
    std::vector<int> result(4,0);
    // takes a line of the file and extracts the range
    std::string range1 = line.substr(0,line.find(","));
    std::string range2 = line.substr(line.find(",")+1,line.size());

    // std::cout << range1 << " " << range2 << std::endl;
    
    result[0] = std::stoi(range1.substr(0,range1.find("-")));
    result[1] = std::stoi(range1.substr(range1.find("-")+1,range1.size()));
    result[2] = std::stoi(range2.substr(0,range2.find("-")));
    result[3] = std::stoi(range2.substr(range2.find("-")+1,range1.size()));

    return result;
}

int main(){
    // reading in the data
    std::fstream assignments("day4_assignments.txt"); if(not assignments.is_open()){std::cout << "File not open!" << std::endl; return -1;}

    std::string tmpstr;
    std::vector<int> tmpvec;
    int counter = 0;
    while(std::getline(assignments,tmpstr)){
        tmpvec = find_range(tmpstr);

        if(tmpvec[1] >= tmpvec[2] and tmpvec[0] <= tmpvec[3]){
            counter++;
        }
    }

    std::cout << "Number of overlaps = " << counter << std::endl;

    return 0;
}