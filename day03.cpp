#include<iostream>
#include<fstream>
#include<vector>

struct {
    std::vector<int> dict;

    bool insert(char letter){
        // inserting the item
        while(dict.size() <= letter){dict.push_back(0);}
        
        // let's see if we already had this item
        if(dict[letter] == 2){
            // we're seeing this item for the third time; we found the badge
            return true;
        }else{
            dict[letter] += 1;
            return false;
        }
    }
} dict;

int priority(char letter){
    if(letter > 96){
        // lowercase letter
        return letter - 96;
    }else{
        // capital letter
        return letter - 38;
    }
}

std::string bubblesort(std::string str){
    // sorting and removing duplicates
    bool ready = false;
    char tmp;
    while(not ready){
        ready = true;
        for(int i=0;i<str.size()-1;i++){
            if(str[i] > str[i+1]){
                tmp = str[i];
                str[i] = str[i+1];
                str[i+1] = tmp;
                ready = false;
            }else if(str[i] == str[i+1]){
                str.erase(str.begin()+i);
                ready = false;
            }
        }
    }

    return str;
}

int main(){
    std::fstream items("day3_items.txt"); if(not items.is_open()){std::cout << "File nicht geöffnet!" << std::endl; return -1;}

    int result = 0;
    std::string tmp;

    // going through all the rucksacks
    while(std::getline(items,tmp)){
        // removing duplicates
        tmp = bubblesort(tmp);

        // going through all the items in the rucksack
        for(int i=0;i < tmp.size();i++){
            if(dict.insert(tmp[i])){
                // we found an item for the third time
                result += priority(tmp[i]);
                // wiping the dictionary
                dict.dict = std::vector<int>();
                // the badge is unique; we move on to the next rucksack
                break;
            }
        }
    }

    std::cout << "Sum of priorities of the badges = " << result << std::endl;

    return 0;
}