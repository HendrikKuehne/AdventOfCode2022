#include<iostream>
#include<fstream>
#include<vector>
#include"functions.h"

// monkey operations
long long int add(long long int worry_level,int summand){return worry_level + summand;}
long long int multiply(long long int worry_level,int factor){return worry_level * factor;}
long long int square(long long int worry_level,int dummy){return worry_level*worry_level;}

// monkey structure
struct monkey{
    std::vector<long long int> items;
    int test_divisor, throw_true, throw_false, activity;
    // operation will be initialised with a function implementing the operation
    // of the respective monkey. It will take the worry_level as a first argument and an integer as second argumemt
    long long int (*operation)(long long int,int); int second_argument;

    long long int change_worry_level(long long int worry_level){
        return operation(worry_level,second_argument);
    }

    monkey( std::vector<long long int> _items,
            long long int (*_operation)(long long int,int),
            int _second_argument,
            int _test_divisor,
            int _throw_true,
            int _throw_false){
        // which items does the monkey hold?
        items = _items;
        // according to which function does my worry level change?
        operation = _operation;
        second_argument = _second_argument;
        // initializing the test divisor
        test_divisor = _test_divisor;
        // initializing the throw parameters
        throw_true = _throw_true;
        throw_false = _throw_false;

        activity = 0;
    }
};

int main(){
    std::fstream file("day11_monkeys.txt"); if(not file.is_open()){std::cout << "File is not open!" << std::endl; return -1;}

    // gather information about the monkeys
    std::string tmp;
    std::vector<monkey> monkeys; std::vector<long long int> items;
    int i,second_argument, test_divisor, throw_true, throw_false, max_test_divisor;
    long long int (*operation)(long long int,int);
    while(std::getline(file,tmp)){      // this loop is a handful
        if(tmp[0] == 'M'){
            // the next monkey - extracting the items
            std::getline(file,tmp);
            tmp = std::string(tmp.begin()+18,tmp.end());
            items = std::vector<long long int>();
            i = 0;
            while(i < tmp.size()){
                i++;
                if(tmp[i] == ','){
                    items.push_back(std::stoi(std::string(tmp.begin(),tmp.begin()+i)));
                    tmp = std::string(tmp.begin()+i+1,tmp.end());
                    i = 0;
                }
                if(i == tmp.size()){
                    items.push_back(std::stoi(std::string(tmp.begin(),tmp.end())));
                    break;
                }
            }
            
            // extracting the operation
            std::getline(file,tmp);
            tmp = std::string(tmp.begin()+19,tmp.end());
            if(tmp[4] == '+'){
                second_argument = std::stoi(std::string(tmp.begin()+6,tmp.end()));
                operation = &add;
            }else if(tmp[4] == '*' and tmp[6] == 'o'){
                operation = &square;
            }else{
                second_argument = std::stoi(std::string(tmp.begin()+6,tmp.end()));
                operation = &multiply;
            }

            // extracting the test divisor
            std::getline(file,tmp);
            test_divisor = std::stoi(std::string(tmp.begin()+21,tmp.end()));

            // extracting the two next monkeys
            std::getline(file,tmp);
            throw_true = std::stoi(std::string(tmp.begin()+29,tmp.end()));
            std::getline(file,tmp);
            throw_false = std::stoi(std::string(tmp.begin()+30,tmp.end()));

            // adding the new monkey
            monkeys.push_back(monkey(items,operation,second_argument,test_divisor,throw_true,throw_false));
        }
    }
    file.close();
    
    // I'm not interested in the abolute worry levels, only in if they are divisible by certain
    // -> I'm gonna cap the numbers at the product of all the divisors
    int cap = 1;
    for(int i_monkey=0;i_monkey<monkeys.size();i_monkey++){cap *= monkeys[i_monkey].test_divisor;}

    int n_rounds = 10000;
    while(n_rounds > 0){
        for(int i_monkey=0;i_monkey<monkeys.size();i_monkey++){
            // the monkey is going to inspect every item
            monkeys[i_monkey].activity += monkeys[i_monkey].items.size();

            // going through the items
            for(int i_item = 0;i_item < monkeys[i_monkey].items.size();i_item++){
                // changing the worry level of the corresponding item - notice that I am capping it at the cap defnied above
                monkeys[i_monkey].items[i_item] = monkeys[i_monkey].change_worry_level(monkeys[i_monkey].items[i_item]) % cap;

                // throwing the item
                if(monkeys[i_monkey].items[i_item] % monkeys[i_monkey].test_divisor == 0){
                    monkeys[monkeys[i_monkey].throw_true].items.push_back(monkeys[i_monkey].items[i_item]);
                }else{
                    monkeys[monkeys[i_monkey].throw_false].items.push_back(monkeys[i_monkey].items[i_item]);
                }
            }

            monkeys[i_monkey].items = std::vector<long long int>(0);
        }
        n_rounds--;
    }
    std::vector<long long int> activity;
    for(int i_monkey=0;i_monkey<monkeys.size();i_monkey++){activity.push_back(monkeys[i_monkey].activity);}
    bubblesort(activity);

    print(activity);

    std::cout << "Monkey business = " << activity[activity.size()-1] * activity[activity.size()-2] << std::endl;

    return 0;
}