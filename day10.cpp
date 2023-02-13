#include<iostream>
#include<fstream>
#include<vector>
#include"functions.h"

struct instruction{
    int finish;     // cycle at which the instruction is complete
    int value;      // value by which x is changed

    instruction(int _finish, int _value){
        finish = _finish; value = _value;
        // std::cout << "Added instruction " << x << ", finished at cycle " << finish << std::endl;
    }

    instruction(){
        finish = -1;
    };
};

struct screen{
    std::vector<char> pixels;

    void draw(int x){
        // the next pixel being drawn is always the one that's going to be located at pixels.size()
        if(x - 1 <= pixels.size()%40 and pixels.size()%40 <= x + 1){
            // the sprite sees the next pixel - the next pixel is lit
            pixels.push_back('#');
        }else{
            pixels.push_back(' ');
        }
    }

    void print(){
        for(int i=0;i<pixels.size();i++){
            std::cout << pixels[i];
            if((i + 1) % 40 == 0){std::cout << std::endl;}
        }
    }
} screen;

int main(){
    std::fstream file("day10_instructions.txt"); if(not file.is_open()){std::cout << "File not open!" << std::endl; return -1;}

    std::string tmp;
    std::vector<int> queue;

    // getting all the instructions
    while(std::getline(file,tmp)){
        if(tmp[0] == 'a'){
            // an addx instruction  - let's add it to the list of
            // instructions (new instructions are added at the front)
            queue.insert(queue.begin(),std::stoi(std::string(tmp.begin()+5,tmp.end())));
        }else{
            // a noop instruction
            queue.insert(queue.begin(),0);
        }
    }

    file.close();

    int current_cycle = 0, x = 1, signal_strength = 0;
    instruction current_instruction;
    // execution
    while(current_instruction.finish >= current_cycle or queue.size() > 0){
        if(current_instruction.finish == -1 and queue.size() > 0){
            // an instruction is still awaiting execution and we are executing no instruction right now
            if(*queue.rbegin() != 0){
                // an addx-instruction
                current_instruction = instruction(current_cycle+2,*queue.rbegin());
            }else{
                // a noop instruction
                current_instruction = instruction(current_cycle+1,*queue.rbegin());
            }
            queue.pop_back();
        }

        // I know what's happening in this cycle - the cycle starts now
        current_cycle++;

        // are we in one of the cycles of interest?
        if((current_cycle + 20) % 40 == 0){
            // std::cout << "signal_strength += " << current_cycle << " * " << x << std::endl;
            signal_strength += current_cycle * x;
        }

        // let's draw the corresponding pixel
        screen.draw(x);

        // is there an instruction which finishes in this cycle?
        if(current_instruction.finish == current_cycle){
            // the current instruction finishes after this cycle
            x += current_instruction.value;
            current_instruction = instruction();
        }
    }

    std::cout << "Total signal strength = " << signal_strength << std::endl;

    screen.print();

    return 0;
}