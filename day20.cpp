#include<iostream>
#include<fstream>
#include<iomanip>
#include<vector>
#include"functions.h"

int N;

/// @brief Returns the index at which std::vector::insert should insert the new value
/// @param i index of the entry we are placing elsewhere
/// @param shift how many places we shift. shift = 1 means we insert the new entry after the nect element!
int index(int i,int shift){
    if(shift >= 0){
        // we go forward in the sequence
        return (i + shift + 1) % ::N;
    }else if((i + shift - 1) % ::N >= 0){
        // we go backward but do not wrap around
        return (i + shift - 1) % ::N;
    }else{
        // we go backward and wrap around
        return N + (i + shift) % ::N;
    }
}

std::vector<int> encode(std::vector<int> vec,int i){
    int i_inbounds = index(i,vec[i]);
    int element = vec[i];
    if(i_inbounds == i){
        return vec;
    }else if(i_inbounds < i){
        // the element will be inserted before the original location; we will need to delete one index later
        i += 1;
    }

    // inserting the element at its new location
    vec.insert(vec.begin()+i_inbounds,element);
    // deleting it at it's old location
    vec.erase(vec.begin()+i);

    return vec;
}

int main(){
    std::fstream file("day20_encrypted.txt"); if(not file.is_open()){std::cout << "File not open!" << std::endl; return -1;}
    
    std::string tmp;
    std::vector<int> sequence;
    while(std::getline(file,tmp)){
        sequence.push_back(std::stoi(tmp));
    }
    file.close();
 
    // if(not duplicates(sequence)){std::cout << "There are duplicates!" << std::endl;}
    // print(sequence);

    ::N = sequence.size();

    int i=0;
    dict shifted_numbers;
    while(i < ::N){
        if(not shifted_numbers.contains(sequence[i])){
            // we have not shifted by the value sequence[i]
            shifted_numbers.add(sequence[i]);
            // std::cout << "Shifting entry sequence[" << i << "] = " << sequence[i] << std::endl;
            sequence = encode(sequence,i);
            // print(sequence);
            i = 0;
        }else{
            i++;
        }
    }

    std::cout << "Decoded the message!" << std::endl;

    i=0;
    for(;i < ::N;i++){if(sequence[i] == 0){break;}}
    std::cout << sequence[(i + 1000) % ::N] << " + " << sequence[(i + 2000) % ::N] << " + " << sequence[(i + 3000) % ::N] << " = " << sequence[(i + 1000) % ::N] + sequence[(i + 2000) % ::N] + sequence[(i + 3000) % ::N] << std::endl;

    return 0;
}