#include<iostream>
#include<fstream>
#include<iomanip>
#include<vector>
#include<assert.h>
#include<map>
#include"functions.h"

typedef enum {number,add,subtract,multiply,divide,dummy} operationtype;

/// @brief Struct representing a single monkey that either returns a single value or returns the result of an operation.
struct monkey{
    operationtype operation;
    double value;
    monkey* lhs;
    monkey* rhs;

    std::string name;
    std::string lhs_name;
    std::string rhs_name;

    /// @brief returns what this monkey yells. The specific operation is dependent on the operation variable.
    double yell(){
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

    /// @brief Creates a monkey that yells a number.
    monkey(double _value){
        operation = number;
        value = _value;
    }

    /// @brief Creates a monkey that yells a number and has a name.
    monkey(double _value,std::string _name){
        operation = number;
        value = _value;
        name = _name;
    }

    /// @brief Creates a monkey that performs an operation.
    monkey(operationtype _operation,monkey* _lhs,monkey* _rhs){
        assert((_operation != number) and (_operation != dummy)); //sanity check
        operation = _operation;
        lhs = _lhs; rhs = _rhs;
    }

    /// @brief Creates a monkey that performs an operation, has a name and knows the names of it's two operands.
    monkey(operationtype _operation,monkey* _lhs,monkey* _rhs,std::string _name,std::string _lhs_name,std::string _rhs_name){
        assert((_operation != number) and (_operation != dummy)); //sanity check
        operation = _operation;
        lhs = _lhs; rhs = _rhs;

        name = _name; lhs_name = _lhs_name; rhs_name = _rhs_name;
    }

    /// @brief Creates a dummy monkey.
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
            monkeys[name] = monkey(std::stod(std::string(tmp.begin()+6,tmp.end())),name);
        }else{
            // this monkey performs an operation
            lhs = std::string(tmp.begin()+6,tmp.begin()+10);
            rhs = std::string(tmp.begin()+13,tmp.end());

            if(tmp[11] == '+'){operation = add;}else if(tmp[11] == '-'){operation = subtract;}else if(tmp[11] == '*'){operation = multiply;}else if(tmp[11] == '/'){operation = divide;}

            monkeys[name] = monkey(operation,&monkeys[lhs],&monkeys[rhs],name,lhs,rhs);
        }
    }

    file_details.close();

    std::cout << std::setprecision(20) << "The root-monkey yells " << rootmonkey->yell() << std::endl;

    return 0;
}