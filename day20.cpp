#include<iostream>
#include<fstream>
#include<iomanip>
#include<vector>
#include"functions.h"

struct number{
    int rank;
    int value;
    bool moved;

    number(int _rank,int _value){
        rank = _rank;
        value = _value;
        moved = false;
    }

    number(){}
};

std::ostream &operator<<(std::ostream &os, const number &value){std::cout << value.value; return os;}

int main(){
    std::fstream file("day20_encrypted_test.txt"); if(not file.is_open()){std::cout << "File not open!" << std::endl; return -1;}
    
    std::string tmpstr; std::vector<number> sequence; int rank = 0;
    while(std::getline(file,tmpstr)){
        sequence.push_back(number(rank++,std::stoi(tmpstr)));
    }
    file.close();
    print(sequence);

    // length of the sequence
    int N = sequence.size();

    int index,new_index,old_index,n_moves,direction; number tmp;
    // doing the moving for all numbers, i.e. for all ranks
    for(int current_rank=0;current_rank < N;current_rank++){
        // finding the number in the sequence
        for(int j=0;j < N;j++){
            if(sequence[j].rank == current_rank){index = j; break;}
        }

        // extracting the number of moves we should make
        n_moves = sequence[index].value;

        // finding the index at which the old (unmoved) number will lie
        new_index = ((index + n_moves + (n_moves > 0 ? 1 : 0)) % N + N) % N;
        old_index = (new_index < index) ? index + 1 : index;

        // inserting the moved element
        sequence.insert(sequence.begin() + new_index,sequence[index]);
        // deleting the unmoved element
        sequence.erase(sequence.begin() + old_index);

        std::cout << "Moving " << n_moves << ":" << std::endl; print(sequence);
    }

    std::cout << std::endl << "Decoded the message!" << std::endl;

    int i=0;
    for(;i < N;i++){if(sequence[i].value == 0){break;}}
    std::cout << sequence[(i + 1000) % N].value << " + " << sequence[(i + 2000) % N].value << " + " << sequence[(i + 3000) % N].value << " = " << sequence[(i + 1000) % N].value + sequence[(i + 2000) % N].value + sequence[(i + 3000) % N].value << std::endl;

    return 0;
}