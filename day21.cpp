#include<iostream>
#include<fstream>
#include<iomanip>
#include<vector>
#include<assert.h>
#include<map>
#include"functions.h"

typedef enum {number,add,subtract,multiply,divide,dummy} operationtype;

/// @brief Struct representing a single monkey that either returns a single value or returns the result
/// of an operation, based upon the template variable.
struct monkey{
    operationtype operation;
    float value;
    monkey* lhs;
    monkey* rhs;

    /// @brief returns what this monkey yells. The specific operation is dependent on the template variable.
    float yell(){
        assert(operation != dummy); // sanity check

        if(operation == number){
            return value;
        }else if(operation == add){
            return lhs->yell() + rhs->yell();
        }else if(operation == subtract){
            return lhs->yell() - rhs->yell();
        }else if(operation == multiply){
            return lhs->yell() * rhs->yell();
        }else if(operation == divide){
            return lhs->yell() / rhs->yell();
        }

        return 0;
    }

    monkey(float _value){
        operation = number;
        value = _value;
    }

    monkey(operationtype _operation,monkey* _lhs,monkey* _rhs){
        assert((_operation != number) and (_operation != dummy)); //sanity check
        operation = _operation;
        lhs = _lhs; rhs = _rhs;
    }

    monkey(){operation = dummy;}
};

int main(){
    std::string filename = "day21_monkeys.txt";
    std::fstream file(filename); if(not file.is_open()){std::cout << "File not open!" << std::endl; return -1;}
    std::fstream file_details(filename); if(not file_details.is_open()){file.close(); std::cout << "File not open!" << std::endl; return -1;}
    std::string tmp,name,lhs,rhs;
    operationtype operation;

    std::map<std::string,monkey> monkeys;
    monkey* rootmonkey;

    // getting the names of all the monkeys
    while(std::getline(file,tmp)){
        name = std::string(tmp.begin(),tmp.begin()+4);
        monkeys[name] = monkey();

        if(name == "root"){rootmonkey = &monkeys[name];}
    }

    file.close();

    // getting the details for each monkey
    while(std::getline(file_details,tmp)){
        name = std::string(tmp.begin(),tmp.begin()+4);

        if(tmp.size() < 17){
            // this monkey yells a number
            monkeys[name] = monkey(std::stof(std::string(tmp.begin()+6,tmp.end())));
        }else{
            // this monkey performs a calculation
            lhs = std::string(tmp.begin()+6,tmp.begin()+10);
            rhs = std::string(tmp.begin()+13,tmp.end());

            if(tmp[11] == '+'){operation = add;}else if(tmp[11] == '-'){operation = subtract;}else if(tmp[11] == '*'){operation = multiply;}else if(tmp[11] == '/'){operation = divide;}

            monkeys[name] = monkey(operation,&monkeys[lhs],&monkeys[rhs]);
        }
    }

    file_details.close();

    std::cout << "The root-monkey yells " << rootmonkey->yell() << std::endl;

    return 0;
}