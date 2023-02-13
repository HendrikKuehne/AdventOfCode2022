#include<iostream>
#include<fstream>

int main(){
    // opening the file
    std::ifstream guide("day2prob1_guide.txt"); if(not guide.is_open()){std::cout << "Nicht geöffnet!" << std::endl; return 0;}
    
    // going through all the games
    std::string tmp;
    int score = 0;
    while(std::getline(guide,tmp)){
        switch(tmp[0]*tmp[2]){
            case 65*88:
                // A X = lose against Rock -> play Scissors
                score += 3; break;
            case 65*89:
                // A Y = draw against Rock -> play Rock
                score += 4; break;
            case 65*90:
                // A Z = win against Rock -> play Paper
                score += 8; break;
            case 66*88:
                // B X = lose against Paper -> play Rock
                score += 1; break;
            case 66*89:
                // B Y = draw against Paper -> play Paper
                score += 5; break;
            case 66*90:
                // B Z = win against Paper -> play Scissors
                score += 9; break;
            case 67*88:
                // C X = lose against Scissors -> play Paper
                score += 2; break;
            case 67*89:
                // C Y = draw against Scissors -> play Scissors
                score += 6; break;
            case 67*90:
                // C Z = win against Scissors -> play Rock
                score += 7; break;
            default:
                std::cout << "A case was not accounted for!" << std::endl;
                return -1;
        }
    }
    
    guide.close();
    std::cout << "close = " << score << std::endl;

    return 0;
}

// A X -> 65*88 = 5720
// A Y -> 65*89 = 5785
// A Z -> 65*90 = 5850
// B X -> 66*88 = 5808
// B Y -> 66*89 = 5874
// B Z -> 66*90 = 5940
// B X -> 67*88 = 5896
// B Y -> 67*89 = 5963
// B Z -> 67*90 = 6030