#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <array>

struct Event {
    // i dont think its always gonna be applicable, but dont use arrays for stuff that can just be this again
    int oldPillar;
    int targetPillar;
};

const int MAX_SIZE = 5; 
const std::string pillarOutOfRange = "That pillar is out of range!";
const std::string tooManyArguments = "Too many arguments!";
const std::string notInteger = "The provided arguments are not integers!";
const std::string pillarFull = "That pillar is already fool!";
// error messages

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

int convertStringToInt (std::string text) {
    int number{0};

    if (!isdigit(text)) {
        return -1;
    
    }

    for (int index{0}; index < text.size(); index++) {
        number = number*10 + (static_cast<int>(text[index]) - 48);
    }

    return number;
}

void swapNumbers (int &num1, int &num2) {
    int temp = num1;
    num1 = num2;
    num2 = temp;
}

void makeRingsFall (std::stack<int> *pillars[]) {
    // for rendering purposes
    // just pass the array by pointer next time
    // if you're gonna pass-by-reference, declare the argument as a *const 
    // actually, it still wouldn't work for this one even if you declared it a *const
    // wait no, it does work but only if you remove []
    // man, what the fuck


    for (int pillarIndex{0}; pillarIndex < 3; pillarIndex++) {
        int amountOfZeroesNeeded = 5 - pillars[pillarIndex]->size();

        if (amountOfZeroesNeeded == 0) {
            continue;
        }

        for (int layer{0}; layer < amountOfZeroesNeeded; layer++) {
            pillars[pillarIndex]->push(0);
        }
    }
}

void displayPillars (std::stack<int> pillars[]) {
    makeRingsFall(&pillars);

    for (int layer{0}; layer < 5; layer++) {
        std::cout << pillars[0].top() << " " << pillars[1].top() << " " << pillars[2].top() << '\n';
        pillars[0].pop();
        pillars[1].pop();
        pillars[2].pop();
    }
}

void moveRing (std::stack<int> (&pillars)[], int oldPillar, int targetPillar, std::stack<Event> &history, bool isUndo=false) {
    // if isUndo, use frontHistory for history parameter
    if (oldPillar < 0 | oldPillar > 2 | targetPillar < 0 | targetPillar > 2) {
        displayError(pillarOutOfRange);
        return;

    }

    if (pillars[targetPillar].size() == MAX_SIZE) {
        displayError(pillarFull);
        return;

    }

    if (isUndo) {
        swapNumbers(oldPillar, targetPillar);
    
    }

    Event event = {oldPillar, targetPillar};

    pillars[targetPillar].push(pillars[oldPillar].top());
    pillars[oldPillar].pop();

    history.push(event);

    displayPillars(pillars);

}

void solvePuzzle() {
    std::cout << "Puzzle solved!" << '\n';
    std::cout <<'\n';
}

void parseInput (std::string input, std::stack<int> (&pillars)[], std::stack<Event> &backHistory, std::stack<Event> &frontHistory) {
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
        if (!isdigit(inputArray[1]) | !isdigit(inputArray[2])) {
            displayError(notInteger);
            return;
        }

        int oldPillar = convertStringToInt(inputArray[1]);
        int targetPillar = convertStringToInt(inputArray[2]);

        moveRing(pillars, oldPillar, targetPillar, backHistory);
    
    } else if (inputArray[0] == "-u" | inputArray[0] == "--undo") {
        if (inputArray[1].size() == 0) {
            inputArray[1] = "1";
        }
        
        if (!isdigit(inputArray[1])) {
            displayError(notInteger);
            return;
        }

        int oldPillar = backHistory.top().oldPillar;
        int targetPillar = backHistory.top().targetPillar;

        moveRing(pillars, oldPillar, targetPillar, frontHistory, true);
        backHistory.pop();    
        
    } 
}

int main () {
    std::stack<int> pillars[3];
    std::stack<Event> backHistory;
    std::stack<Event> frontHistory;

    pillars[0].push(5);
    pillars[0].push(4);
    pillars[0].push(3);
    pillars[0].push(2);
    pillars[0].push(1);

    displayPillars(pillars);


}