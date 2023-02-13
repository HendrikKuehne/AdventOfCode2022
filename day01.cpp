#include<iostream>
#include<fstream>
#include<vector>

template<typename T>
std::vector<T> bubblesort(std::vector<T> values){
    T tmp;
    bool sorted = false;

    while(not sorted){
        sorted = true;
        for(int i=0;i<values.size()-1;i++){
            if(values[i] < values[i+1]){
                tmp = values[i]; values[i] = values[i+1]; values[i+1] = tmp;
                sorted = false;
            }
        }
    }

    return values;
}

int main(){
    std::cout << "Hello World" << std::endl;
    
    std::ifstream txtfile; txtfile.open("day1prob1_caloriedata.txt",std::ios::in);
    std::string tempstr = "dings";
    int tmp = 0;
    std::vector<int> calories;
    
    while(std::getline(txtfile,tempstr)){
        if(tempstr != ""){
            tmp += std::stoi(tempstr);
        }else{
            calories.push_back(tmp);
            tmp = 0;
        }
    }
    calories.push_back(tmp);

    calories = bubblesort(calories);

    std::cout << "max three calories = " << calories[0] + calories[1] + calories[2] << " = " << calories[0] << " + " << calories[1] << " + " << calories[2] << std::endl;
    
    txtfile.close();
    return 0;
}