#include<iostream>
#include<fstream>
#include<vector>

template<typename T>
void print(std::vector<T> vec){
    std::cout << "[";
    if(vec.size() == 0){std::cout << "]";}
    for(int i=0;i<vec.size();i++){
        std::cout << vec[i];
        if(i==vec.size()-1){std::cout << "]";}else{std::cout << ",";}
    }
    std::cout << std::endl;
}

template<typename T>
void print(std::vector<std::vector<T> > vec){
    for(int i=0;i<vec.size();i++){
        print(vec[i]);
    }
}

void print(std::string vec){
    for(int i=0;i<vec.size();i++){
        std::cout << vec[i];
    }
    std::cout << std::endl;
}

struct tree{
    bool visible;
    int height,scenicscore;

    tree(int height_in){height = height_in; visible = false;}
};

// enabling tree to be forwarded to the output stream
std::ostream &operator<<(std::ostream &os, const tree &value)
{
    if(value.visible){std::cout << value.height;}else{std::cout << " ";}
    return os;
}

// enbaling tree to be compared using <,<=,>,>= :
bool operator<(const tree& lhs,const tree& rhs){if(lhs.height < rhs.height){return true;} return false;}
bool operator<=(const tree& lhs,const tree& rhs){if(lhs.height <= rhs.height){return true;} return false;}
bool operator>(const tree& lhs,const tree& rhs){if(lhs.height > rhs.height){return true;} return false;}
bool operator>=(const tree& lhs,const tree& rhs){if(lhs.height >= rhs.height){return true;} return false;}

int main(){
    std::fstream file("day8_treepatch.txt"); if(not file.is_open()){std::cout << "File not open!" << std::endl; return -1;}

    // constructing the treepatch
    std::vector<std::vector<tree> > treepatch;
    std::string tmp;

    while(std::getline(file,tmp)){
        treepatch.push_back(std::vector<tree>());
        for(int i=0;i<tmp.size();i++){
            treepatch[treepatch.size()-1].push_back(tree(tmp[i] - '0'));
        }
    }
    file.close();
    // print<tree>(treepatch);

    // We will look at each row and column of the tree patch from west->east, north->south, east->west
    // and south->north to determine which trees are visible
    int height = treepatch.size(), width = treepatch[0].size(), max_size, visible_trees = 0;

    // west->east
    for(int i=0;i<height;i++){max_size = -1;for(int j=0;j<width;j++){
        if(treepatch[i][j].height > max_size){
            // treepatch[i][j] is visible
            if(not treepatch[i][j].visible){
                // we have not seen this tree before
                treepatch[i][j].visible = true;
                visible_trees++;
            }
            max_size = treepatch[i][j].height;

            if(max_size == 9){
                // the maximum size of trees is 9, so from here on no more trees will be visible, we want to break out of the loop
                break;
            }
        }
    }}
    // std::cout << "After west->east:" << std::endl;
    // print<tree>(treepatch);

    // east->west
    for(int i=0;i<height;i++){max_size = -1;for(int j=width-1;j>=0;j--){
        if(treepatch[i][j].height > max_size){
            // treepatch[i][j] is visible
            if(not treepatch[i][j].visible){
                // we have not seen this tree before
                treepatch[i][j].visible = true;
                visible_trees++;
            }
            max_size = treepatch[i][j].height;

            if(max_size == 9){
                // the maximum size of trees is 9, so from here on no more trees will be visible, we want to break out of the loop
                break;
            }
        }
    }}
    // std::cout << "After east->west:" << std::endl;
    // print<tree>(treepatch);

    // north->south
    for(int i=0;i<width;i++){max_size = -1;for(int j=0;j<height;j++){
        if(treepatch[j][i].height > max_size){
            // treepatch[j][i] is visible
            if(not treepatch[j][i].visible){
                // we have not seen this tree before
                treepatch[j][i].visible = true;
                visible_trees++;
            }
            max_size = treepatch[j][i].height;

            if(max_size == 9){
                // the maximum size of trees is 9, so from here on no more trees will be visible, we want to break out of the loop
                break;
            }
        }
    }}
    // std::cout << "After north->south:" << std::endl;
    // print<tree>(treepatch);

    // south->north
    for(int i=0;i<width;i++){max_size = -1;for(int j=height-1;j>=0;j--){
        if(treepatch[j][i].height > max_size){
            // treepatch[j][i] is visible
            if(not treepatch[j][i].visible){
                // we have not seen this tree before
                treepatch[j][i].visible = true;
                visible_trees++;
            }
            max_size = treepatch[j][i].height;

            if(max_size == 9){
                // the maximum size of trees is 9, so from here on no more trees will be visible, we want to break out of the loop
                break;
            }
        }
    }}
    // std::cout << "After south->north:" << std::endl;
    // print<tree>(treepatch);

    std::cout << "Number of visible trees = " << visible_trees << std::endl;

    // go trough the whole grid to determine the scenic score of each tree
    int westview,northview,eastview,southview,scenicscore = 0,maxi,maxj;
    for(int i=0;i<height;i++){for(int j=0;j<width;j++){
        westview = 0;northview = 0;eastview = 0;southview = 0;

        // looking west
        for(int iwest = j-1;iwest>-1;iwest--){if(treepatch[i][iwest] < treepatch[i][j]){westview++;}else{westview++;break;}}
        // looking north
        for(int inorth = i-1;inorth>-1;inorth--){if(treepatch[inorth][j] < treepatch[i][j]){northview++;}else{northview++;break;}}
        // looking east
        for(int ieast = j+1;ieast<width;ieast++){if(treepatch[i][ieast] < treepatch[i][j]){eastview++;}else{eastview++;break;}}
        // looking south
        for(int isouth = i+1;isouth<height;isouth++){if(treepatch[isouth][j] < treepatch[i][j]){southview++;}else{southview++;break;}}

        if(westview*northview*eastview*southview > scenicscore){
            scenicscore = westview*northview*eastview*southview;
            maxi = i;
            maxj = j;
        }
    }}

    std::cout << "Maximum scenic score is " << scenicscore << " at grid location [" << maxi << "," << maxj << "]." << std::endl;

    print<tree>(treepatch);

    return 0;
}