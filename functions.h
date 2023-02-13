/*
 * prints a vector containing arbitrary type for which the operator << is defined. When width > 0, the width of the
 * items will be width. Overload for a vector containing vector of arbitrary type.
 */
template<typename T>
void print(std::vector<T> vec,int width = 0){
    std::cout << "[";
    if(vec.size() == 0){std::cout << "]";}
    for(int i=0;i<vec.size();i++){
        std::cout << std::setw(width) << vec[i];
        if(i==vec.size()-1){std::cout << "]";}else{std::cout << " ";}
    }
    std::cout << std::endl;
}

/*
 * prints a vector containing arbitrary type for which the operator << is defined. When width > 0, the width of the
 * items will be width. Overload for a vector containing vector of arbitrary type.
 */
template<typename T>
void print(std::vector<std::vector<T> > vec, int width = 0){
    for(int i=0;i<vec.size();i++){
        print(vec[i],width);
    }
}

template<typename T>
int sgn(T x){
    // signum function
    return (T(0) < x) - (x < T(0));
}

template<typename T>
bool duplicates(std::vector<T> vec){
    std::vector<T> elements;
    bool present;
    for(typename std::vector<T>::iterator itr=vec.begin();itr<vec.end();itr++){
        present = false;
        for(typename std::vector<T>::iterator itr2=elements.begin();itr2<elements.end();itr2++){
            if(*itr == *itr2){
                // we found a duplicate
                return true;
            }
            elements.push_back(*itr);
        }
    }
    return true;
}

/// @brief A dictionary for integers where entry i is placed at location i in a vector
struct dict{
    std::vector<int> dict_plus0,dict_minus;

    bool contains(int entry){
        if(entry >= 0){
            for(int i=0;i<dict_plus0.size();i++){
                if(dict_plus0[i] == entry){return true;}
            }
        }else{
            for(int i=0;i<dict_minus.size();i++){
                if(dict_minus[i] == entry){return true;}
            }
        }
        return false;
    }

    void add(int entry){
        if(entry >= 0){
            if(not contains(entry)){
                dict_plus0.push_back(entry);
            }
        }else{
            if(not contains(entry)){
                dict_minus.push_back(entry);
            }
        }
    }
};

template<typename T>
void bubblesort(std::vector<T>& vec){
    // in-place bubble sort
    T tmp; bool sorted = false;
    while(not sorted){
        sorted = true;
        for(int i=1;i<vec.size();i++){
            if(vec[i-1] > vec[i]){
                tmp = vec[i-1];
                vec[i-1] = vec[i];
                vec[i] = tmp;
                sorted = false;
            }
        }
    }
}
