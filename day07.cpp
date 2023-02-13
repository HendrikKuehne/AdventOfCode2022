#include<iostream>
#include<fstream>
#include<vector>
#include<map>

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

struct dir;

struct file{
    std::string name;
    int size;
    // directory which contains this file
    dir* parent;

    file(std::string name_in,int size_in,dir* parent_in){
        name = name_in;
        size = size_in;
        parent = parent_in;
    }
};

struct dir{
    std::string name;
    int size;
    // contained files and directories
    std::vector<file> files;
    std::vector<dir> dirs;
    // directory which contains this directory
    dir* parent;

    int search_file(std::string name_in){
        // searches this directory for a file and returns the corresponding index; if the
        // file does not exist, search_file returns -1
        for(int i=0;i<files.size();i++){
            if(files[i].name == name_in){return i;}
        }
        return -1;
    }

    int search_dir(std::string name_in){
        // searches this directory for a searches and returns the corresponding index; if the
        // searches does not exist, search_file returns -1
        for(int i=0;i<dirs.size();i++){
            if(dirs[i].name == name_in){return i;}
        }
        return -1;
    }

    void add_file(std::string name_in,int size_in){
        // adds the file with name name_in to this directory (if it is not already present)
        if(search_file(name_in) == -1){
            files.push_back(file(name_in,size_in,this));
        }
    }

    void add_file(std::string cmdout){
        // adds the file with name name_in to this directory (if it is not already present)
        // while only being supplied with the command-line output
        int space = 0;
        while(cmdout[space] != ' '){space++;}
        int size_in = std::stoi(std::string(cmdout.begin(),cmdout.begin()+space));
        std::string name_in(cmdout.begin()+space+1,cmdout.end());

        // after we found size and name, we may add the file
        // std::cout << "Adding file " << name_in << " with size " << size_in << " to directory _" << this->name << "_" << std::endl;
        this->add_file(name_in,size_in);
    }

    void add_dir(std::string name_in){
        // adds the directory with name name_in to this directory (if it is not already present)
        if(search_dir(name_in) == -1){
            // std::cout << "Adding directory _" << name_in << "_ to directory _" << this->name << "_" << std::endl;
            dirs.push_back(dir(name_in,this));
        }
    }

    dir(std::string name_in,dir* parent_in){
        name = name_in;
        parent = parent_in;
    }

    dir(){}
};

int dir_size(dir* current_dir){
    // recursively find the size of a directory
    int size = 0;

    for(int i=0;i<current_dir->files.size();i++){size += current_dir->files[i].size;}
    for(int i=0;i<current_dir->dirs.size();i++){size += dir_size(&(current_dir->dirs[i]));}

    current_dir->size = size;

    return size;
}

void find_smallest(dir current_dir,int root_size,int& min_remains,int& result){
    // (70000000 - root_size + current_dir.size) is the total free space if we were to delete current_dir
    if((70000000 - root_size + current_dir.size) >= 30000000 and (70000000 - root_size + current_dir.size) < min_remains){
        min_remains = 70000000 - root_size + current_dir.size;
        result = current_dir.size;
    }
    
    for(int i=0;i<current_dir.dirs.size();i++){
        if(current_dir.dirs[i].size > 0){
            find_smallest(current_dir.dirs[i],root_size,min_remains,result);
        }
    }
}

int main(){
    std::fstream commandsfile("day7_commands.txt"); if(not commandsfile.is_open()){std::cout << "File not open!" << std::endl; return -1;}
    
    // map containing all directories we found so far
    dir root("/",nullptr);
    
    // infrastructure
    std::string cmdout,tmp;
    dir* current_dir;
    int i_dir;

    // we will go through the whole file and assemble the file structure
    while(std::getline(commandsfile,cmdout)){
        // check for every possible thing that can happen in the command line (nothing needs to be done at "$ ls")
        if(cmdout[0] == '$'){if(cmdout[2] == 'c'){
            // some command has been typed in
            if(cmdout[5] == '/'){
                // command line: "$ cd /", we go to the root directory
                current_dir = &root;

                // std::cout << "Going to directory _/_" << std::endl;
            }else if(cmdout[5] == '.'){
                // command line: "$ cd ..", we go to the parent directory

                //std::cout << "Going to the parent directory _" << current_dir->parent->name << "_" << std::endl;
                
                current_dir = current_dir->parent;
            }else{
                // command line: "$ cd xxx" where xxx is a directory name, we go to a different directory

                // extracting the directory name
                tmp = std::string(cmdout.begin()+5,cmdout.end());

                // std::cout << "Going to directory _" << tmp << "_";

                // let's add this directory (add_dir will only add it if we have not seen it before)
                current_dir->add_dir(tmp);

                // going into the directory
                i_dir = current_dir->search_dir(tmp);
                current_dir = &(current_dir->dirs[i_dir]);

                // std::cout << " | I am in directory _" << current_dir->name << "_" << std::endl;
            }
        }}else if(cmdout[0] == 'd'){
            // command line: "dir xxx", an existing directory is listed; we will add it

            // extracting the directory name
            tmp = std::string(cmdout.begin()+4,cmdout.end());
            
            // let's add this directory (add_dir will only add it if we have not seen it before)
            current_dir->add_dir(tmp);
            
        }else{
            // command line: "### xxx" where ### is the file size and xxx is the file name;
            // we found a file and add it
            current_dir->add_file(cmdout);
        }
    }
    
    // finding the root size
    int tmp2 = dir_size(&root);

    // finding the smallest directory that may be deleted
    int min_remains = 70000000,result;
    find_smallest(root,root.size,min_remains,result);

    std::cout << "Size of the directory that may be deleted = " << result << std::endl;

    return 0;
}