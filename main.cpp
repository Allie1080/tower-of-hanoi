#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <array>
#include <chrono>
#include <thread>

struct Event {
    // i dont think its always gonna be applicable, but dont use arrays for stuff that can just be this again
    int oldPillar;
    int targetPillar;
};

const int MAX_SIZE = 5; 
const std::string pillarOutOfRange = "That pillar is out of range!";
const std::string tooManyArguments = "Too many arguments!";
const std::string notInteger = "The provided arguments are not integers!";
const std::string pillarFull = "That pillar is already full!";
const std::string undoEmpty = "There is nothing to undo!";
const std::string redoEmpty = "There is nothing to redo!";
// error messages

void delay(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

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

void displayHistory(std::stack<Event> history) {
    if (history.empty()) {
        return;

    }

    const int historySize = history.size();
    // remember not to use conditions based on something that'd be affected by your loop

    std::cout << "Size: " << history.size() << '\n';

    for (int counter{0}; counter < historySize; counter++) {
        std::cout << counter << history.top().oldPillar << " -> " << history.top().targetPillar << '\n';
        history.pop();
    }

    std::cout << '\n';
}

void makeRingsFall (std::array<std::stack<int>, 3> &pillars) {
    // for rendering purposes
    // just pass the array by pointer next time
    // if you're gonna pass-by-reference, declare the argument as a *const 
    // actually, it still wouldn't work for this one even if you declared it a *const
    // wait no, it does work but only if you remove []
    // man, what the fuck


    for (int pillarIndex{0}; pillarIndex < 3; pillarIndex++) {
        ////std::cout << "Layer " << pillarIndex << " is " << ((pillars[pillarIndex].empty()) ? "empty": "not empty") << '\n';
        int layerSize = (!pillars[pillarIndex].empty()) ? pillars[pillarIndex].size() : 0;

        int amountOfZeroesNeeded = 5 - layerSize;
        /////std::cout << amountOfZeroesNeeded << '\n';


        if (amountOfZeroesNeeded == 0) {
            continue;
        }

        for (int layer{0}; layer < amountOfZeroesNeeded; layer++) {
            /////std::cout << pillarIndex << layer << '\n';
            pillars[pillarIndex].push(0);
            /////std::cout << pillarIndex << layer << '\n';
        }
    }
    /////std::cout << "makeringsfall executed!" << '\n';
}

void displayPillars (std::array<std::stack<int>, 3> pillars) {
    //////std::cout << "First part of displayPillars executed!" << '\n';
    makeRingsFall(pillars);

    for (int layer{0}; layer < 5; layer++) {
        std::cout << pillars[0].top() << " " << pillars[1].top() << " " << pillars[2].top() << '\n';
        pillars[0].pop();
        pillars[1].pop();
        pillars[2].pop();
    }

    std::cout << '\n';
    /////std::cout << "Last part of displayPillars executed!" << '\n';
}

void moveRing (std::array<std::stack<int>, 3> &pillars, int oldPillar, int targetPillar, std::stack<Event> &history, bool isUndo=false) {
    // if isUndo, use frontHistory for history parameter
    if (oldPillar < 0 | oldPillar > 2 | targetPillar < 0 | targetPillar > 2) {
        displayError(pillarOutOfRange);
        return;

    }

    if (pillars[targetPillar].size() == MAX_SIZE) {
        displayError(pillarFull);
        return;

    }

    history.push({oldPillar, targetPillar});
    // not affected by isUndo

    if (isUndo) {
        swapNumbers(oldPillar, targetPillar);
    
    }

    pillars[targetPillar].push(pillars[oldPillar].top());
    pillars[oldPillar].pop();

    displayPillars(pillars);

}

void solvePuzzle() {
    std::cout << "Puzzle solved!" << '\n';
    std::cout <<'\n';
}

void timeTravel(int repeatAmount, std::array<std::stack<int>, 3> &pillars, std::stack<Event> &utilizedHistory, std::stack<Event> &affectedHistory, bool isRedo=false) {
    if (repeatAmount > utilizedHistory.size()) {
        repeatAmount = utilizedHistory.size();

        if (repeatAmount == 0) {
            displayError((isRedo) ? redoEmpty : undoEmpty);
            return;
        }

        std::cout << "Can only " << ((isRedo) ? "redo" : "undo" ) << " " << repeatAmount << " actions..." << '\n';

    } else {
        std::cout << ((isRedo) ? "Redo" : "Undo") << "ing " << repeatAmount << "actions.." << '\n';
    
    }

    std::cout << '\n';

    delay(500);
    
    for (int counter{0}; counter < repeatAmount; counter++) {
        std::cout << '\n';
        int oldPillar = utilizedHistory.top().oldPillar;
        int targetPillar = utilizedHistory.top().targetPillar;

        moveRing(pillars, oldPillar, targetPillar, affectedHistory, !isRedo);
        utilizedHistory.pop();  
        /////std::cout << "Time Travel" << counter << '\n';
        /////displayHistory(utilizedHistory);
        /////displayHistory(affectedHistory);
        delay(250);
    }
}

void parseInput (std::string input, std::array<std::stack<int>, 3> &pillars, std::stack<Event> &backHistory, std::stack<Event> &frontHistory) {
    std::stringstream inputStream(input);
    std::string variable;
    std::string inputArray[64];
    int variableCounter{0};

    std::cout << '\n';

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

        int repeatAmount = convertStringToInt(inputArray[1]);

        timeTravel(repeatAmount, pillars, backHistory, frontHistory);
        
    } else if (inputArray[0] == "-r" | inputArray[0] == "--redo") {
        if (inputArray[1].size() == 0) {
            inputArray[1] = "1";
        }
        
        if (!isdigit(inputArray[1])) {
            displayError(notInteger);
            return;
        }

        int repeatAmount = convertStringToInt(inputArray[1]);

        timeTravel(repeatAmount, pillars, frontHistory, backHistory, true);

    }
}

int main () {
    std::array<std::stack<int>, 3> pillars;
    std::stack<Event> backHistory;
    std::stack<Event> frontHistory;
    std::string userPrompt;

    pillars[0].push(5);
    pillars[0].push(4);
    pillars[0].push(3);
    pillars[0].push(2);
    pillars[0].push(1);

    displayPillars(pillars);

    while (true) {
        std::cout << ">> ";
        getline(std::cin, userPrompt);

        parseInput(userPrompt, pillars, backHistory, frontHistory);
    }


}