#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <array>

const std::string pillarOutOfRange = "That pillar is out of range!";
const std::string tooManyArguments = "Too many arguments!";
const std::string notInteger = "The provided arguments are not integers!";

void displayError (std::string error) {
    std::cout << "[ERROR]" << '\n';
    std::cout << error << '\n';
    std::cout << '\n';
}

bool isdigit (std::string text) {
     for (int index{0}; index < text.size(); index++) {
         if (!isdigit(text[index])) {
            // this isdigit takes chars as input 
            return false;
         }
     }

     return true;
}

int convertStringToInt(std::string text) {
    int number{0};

    if (!isdigit(text)) {
        return -1;
    
    }

    for (int index{0}; index < text.size(); index++) {
        number = number*10 + (static_cast<int>(text[index]) - 48);
    }

    return number;
}

void parseInput (std::string input, std::stack<int> pillars[]) {
    std::stringstream inputStream(input);
    std::string variable;
    std::string inputArray[64];
    int variableCounter{0};

    while (std::getline(inputStream, variable, ' ')) {
        if (variableCounter > 3) {
            displayError(tooManyArguments);
            return;
        }
        inputArray[variableCounter++] = variable; 
    }

    if (inputArray[0] == "-s" | inputArray[0] == "--solve") {
        solvePuzzle();
    
    } else if (inputArray[0] == "-m" | inputArray[0] == "--move") {
        if (!isdigit(inputArray[1])) {
            displayError(notInteger);
            return;
        }
        moveRing(pillars, convertStringToInt(inputArray[1]), convertStringToInt(inputArray[2]));

    }
}

void displayPillars (std::stack<int> pillars[]) {
    for (int layer{0}; layer < 5; layer++) {
        std::cout << pillars[0].top() << " " << pillars[1].top() << " " << pillars[2].top() << '\n';
        pillars[0].pop();
        pillars[1].pop();
        pillars[2].pop();
    }
}

void moveRing (std::stack<int> pillars[], int oldPillar, int targetPillar) {
    if (oldPillar < 0 | oldPillar > 2 | targetPillar < 0 | targetPillar > 2) {
        displayError(pillarOutOfRange);
    }

}

void solvePuzzle() {

}

int main () {
    std::stack<int> pillars[3];
    std::stack<int> backHistory[64][2];

    for (int index{0}; index < 3; index++) {
        for (int counter{0}; counter < 5; counter++) {
            pillars[index].push(counter);
        }
    }

    displayPillars(pillars);


}