#include<iostream>
#include<fstream>
#include<vector>

template<typename T>
void print(std::vector<T> vec){
    if(vec.size() == 0){std::cout << "]";}
    for(int i=0;i<vec.size();i++){
        std::cout << vec[i];
    }
    std::cout << std::endl;
}

void print(std::string vec){
    if(vec.size() == 0){std::cout << "]";}
    for(int i=0;i<vec.size();i++){
        std::cout << vec[i];
    }
    std::cout << std::endl;
}

struct queue {
    // first-in last-out
    std::vector<char> values;
    std::vector<int> dict;
    bool unique;

    bool update_dict(char c,bool remove=false){
        // inserts c into the dictionary and returns true if the string still consists of distinct characters
        // if remove == true, c will be removed instead of added
        while(dict.size() <= c){dict.push_back(0);}
        
        if(remove){
            dict[c] -= 1;
        }else{
            dict[c] += 1;
        }

        // checking if the string is distinct
        for(int i=0;i<dict.size();i++){if(dict[i] > 1){return false;}}
        return true;
    }

    // constructor
    queue(std::string vals,int n=13){
        unique = true;
        // inserts the first n characters of vals into values
        for(int i=0;i<n;i++){
            values.push_back(vals[i]);
            unique = update_dict(vals[i]);
        }
    }

    void insert(char val){
        // inserting a new character and updating the unique-variable
        unique = update_dict(values[0],true);

        for(int i=1;i<values.size();i++){
            values[i-1] = values[i];
        }

        values[values.size()-1] = val;
        unique = update_dict(val);
    }
};

bool check_distinct(std::string str){
    if(str.size() != 14){
        std::cout << "Wrong string size in check_distinct! Got string of length " << str.size() << std::endl;
        return false;
    }

    std::vector<int> dict;
    for(int i=0;i<str.size();i++){
        while(dict.size() <= str[i]){dict.push_back(0);}
        dict[str[i]] += 1;
        if(dict[str[i]] > 1){return false;}
    }
    return true;
}

int main(){
    std::fstream bufferfile("day6_buffer.txt"); if(not bufferfile.is_open()){std::cout << "File not opened!" << std::endl; return -1;}
    
    std::string buffer;
    std::getline(bufferfile,buffer);

    int n=14;
    queue data(buffer,n);

    if(data.unique){std::cout << "Found the first marker after " << n << " characters." << std::endl; return 0;}

    for(int i=14;i<buffer.size();i++){
        // if(check_distinct(std::string(buffer.begin()+i-14,buffer.begin()+i))){
        //     std::cout << "Found the first marker after " << i << " characters." << std::endl;
        //     print(std::string(buffer.begin()+i-14,buffer.begin()+i));
        //     return 0;
        // }
        data.insert(buffer[i]);
        if(data.unique){
            std::cout << "Found the first marker after " << i+1 << " characters." << std::endl;
            print<char>(data.values);
            return 0;
        }
    }

    std::cout << "No marker found!" << std::endl;
    return 0;
}