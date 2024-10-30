#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <array>
#include <chrono>
#include <thread>

struct Event {
    // i dont think its always gonna be applicable, but dont use arrays for stuff that can just be a struct again
    int oldPillar;
    int targetPillar;
};

struct History {
    std::stack<Event> &frontHistory;
    std::stack<Event> &backHistory;
};

const int PILLAR_MAX_SIZE = 7; 
const int PILLAR_AMOUNT = 4;

const std::string cannotStack = "You cannot stack a bigger ring onto a smaller ring!";
const std::string invalidCommand = "Invalid command!";
const std::string notInteger = "The provided arguments are not integers!";
const std::string pillarEmpty = "That pillar is empty!";
const std::string pillarFull = "That pillar is already full!";
const std::string pillarOutOfRange = "That pillar is out of range!";
const std::string redoEmpty = "There is nothing to redo!";
const std::string undoEmpty = "There is nothing to undo!";
const std::string tooManyArguments = "Too many arguments!";
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
    // mostly just for debugging

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

void makeRingsFall (std::array<std::stack<int>, PILLAR_AMOUNT> &pillars) {
    // for rendering purposes
    // just pass the array by pointer next time
    // if you're gonna pass-by-reference, declare the argument as a *const 
    // actually, it still wouldn't work for this one even if you declared it a *const
    // wait no, it does work but only if you remove []
    // man, what the fuck


    for (int pillarIndex{0}; pillarIndex < PILLAR_AMOUNT; pillarIndex++) {
        ////std::cout << "Layer " << pillarIndex << " is " << ((pillars[pillarIndex].empty()) ? "empty": "not empty") << '\n';
        int layerSize = (!pillars[pillarIndex].empty()) ? pillars[pillarIndex].size() : 0;

        int amountOfZeroesNeeded = PILLAR_MAX_SIZE - layerSize + 1;
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

std::string displayRing (int ringSize) {
    std::stringstream ring;
    int spaces = PILLAR_MAX_SIZE - ringSize;

    for (int counter{0}; counter < spaces; counter++) {
        ring << " ";
    }

    if (ringSize == 0) {
        ring << "||";
    
    } else {
        for (int counter{0}; counter < ringSize; counter++) {
            ring << "[";
        }

        ring << "0" << ringSize;

        for (int counter{0}; counter < ringSize; counter++) {
            ring << "]";
        }

    }

    for (int counter{0}; counter < spaces; counter++) {
        ring << " ";
    }

    return ring.str();
}

void displayPillars (std::array<std::stack<int>, PILLAR_AMOUNT> pillars) {
    //////std::cout << "First part of displayPillars executed!" << '\n';
    makeRingsFall(pillars);

    for (int layer{0}; layer < PILLAR_MAX_SIZE + 1; layer++) {
        for (int pillarIndex{0}; pillarIndex < PILLAR_AMOUNT; pillarIndex++) {
            std::cout << " " << displayRing(pillars[pillarIndex].top());
            pillars[pillarIndex].pop();
        }
        std::cout << '\n';
    }

    for (int counter{0}; counter < ((PILLAR_MAX_SIZE + 1) * 2 * PILLAR_AMOUNT) + 4; counter++) {
        // 1 for the ringSize (where PILLAR_MAX_SIZE is incremented by 1 to be able to dispaly the ring size number)
        // 6 is 2 * 3
        // 2 cause ringSize uses PILLAR_MAX_SIZE twice
        // 4 for the extra spaces between the ring pillars
        std::cout << "=";
    }

    std::cout << '\n';

    std::cout << '\n';
    /////std::cout << "Last part of displayPillars executed!" << '\n';
}

void moveRing (std::array<std::stack<int>, PILLAR_AMOUNT> &pillars, int oldPillar, int targetPillar, std::stack<Event> &history, bool &undoUsed, bool isUndo=false) {
    // if isUndo, use frontHistory for history parameter
    //std::cout << "Move is " << oldPillar << " -> " << targetPillar << '\n';

    if (isUndo) {
        swapNumbers(oldPillar, targetPillar);
        undoUsed = true;
        std::cout << "Used undo!" << '\n';
    
    }

    if (oldPillar < 0 | oldPillar > PILLAR_AMOUNT - 1 | targetPillar < 0 | targetPillar > PILLAR_AMOUNT - 1) {
        displayError(pillarOutOfRange);
        return;

    } else if (pillars[oldPillar].empty()) {
        displayError(pillarEmpty);
        return;

    } else if (pillars[targetPillar].size() == PILLAR_MAX_SIZE) {
        displayError(pillarFull);
        return;

    } else if (pillars[oldPillar].top() > ((pillars[targetPillar].empty()) ? PILLAR_MAX_SIZE + 1 : pillars[targetPillar].top())) {
        displayError(cannotStack);
        return;
    }

    pillars[targetPillar].push(pillars[oldPillar].top());
    pillars[oldPillar].pop();

    displayPillars(pillars);

    if (isUndo) {
        swapNumbers(oldPillar, targetPillar);
        // for recording history

    }

    history.push({oldPillar, targetPillar});
}

void solvePuzzle() {
    std::cout << "Puzzle solved!" << '\n';
    std::cout <<'\n';
}

void timeTravel(int repeatAmount, std::array<std::stack<int>, PILLAR_AMOUNT> &pillars, std::stack<Event> &utilizedHistory, std::stack<Event> &affectedHistory, bool &undoUsed, bool isRedo=false) {
    if (repeatAmount > utilizedHistory.size()) {
        repeatAmount = utilizedHistory.size();

        if (repeatAmount == 0) {
            displayError((isRedo) ? redoEmpty : undoEmpty);
            return;
        }

        std::cout << "Can only " << ((isRedo) ? "redo" : "undo" ) << " " << repeatAmount << " actions" << ((repeatAmount > 1) ? "s" : "") << "..." << '\n';

    } else {
        std::cout << ((isRedo) ? "Redo" : "Undo") << "ing " << repeatAmount << " action" << ((repeatAmount > 1) ? "s" : "") << "..." << '\n';
    
    }

    std::cout << '\n';

    delay(1000);
    
    for (int counter{0}; counter < repeatAmount; counter++) {
        std::cout << '\n';
        int oldPillar = utilizedHistory.top().oldPillar;
        int targetPillar = utilizedHistory.top().targetPillar;

        moveRing(pillars, oldPillar, targetPillar, affectedHistory, undoUsed, !isRedo);
        utilizedHistory.pop();  
        /////std::cout << "Time Travel" << counter << '\n';
        /////displayHistory(utilizedHistory);
        /////displayHistory(affectedHistory);
        delay(250);
    }
}

void parseInput (std::string input, std::array<std::stack<int>, PILLAR_AMOUNT> &pillars, std::stack<Event> &backHistory, std::stack<Event> &frontHistory, bool &undoUsed) {
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

        moveRing(pillars, --oldPillar, --targetPillar, backHistory, undoUsed);
    
    } else if (inputArray[0] == "-u" | inputArray[0] == "--undo") {
        if (inputArray[1].size() == 0) {
            inputArray[1] = "1";
        }
        
        if (!isdigit(inputArray[1])) {
            displayError(notInteger);
            return;
        }

        int repeatAmount = convertStringToInt(inputArray[1]);

        timeTravel(repeatAmount, pillars, backHistory, frontHistory, undoUsed);
        
    } else if (inputArray[0] == "-r" | inputArray[0] == "--redo") {
        if (inputArray[1].size() == 0) {
            inputArray[1] = "1";
        }
        
        if (!isdigit(inputArray[1])) {
            displayError(notInteger);
            return;
        }

        int repeatAmount = convertStringToInt(inputArray[1]);

        timeTravel(repeatAmount, pillars, frontHistory, backHistory, undoUsed, true);

    } else {
        displayError(invalidCommand);

    }
}

int main () {
    std::array<std::stack<int>, PILLAR_AMOUNT> pillars;
    std::stack<Event> frontHistory;
    std::stack<Event> backHistory;
    History history {frontHistory, backHistory};
    
    std::string userPrompt;
    bool undoUsed = false;
    // for clearing frontHistory if an new move is made after undoing

    for (int counter{PILLAR_MAX_SIZE}; counter > 0; counter--) {
        pillars[0].push(counter);
    }

    displayPillars(pillars);

    while (true) {
        std::cout << ">> ";
        getline(std::cin, userPrompt);

        parseInput(userPrompt, pillars, backHistory, frontHistory, undoUsed);
    }


}