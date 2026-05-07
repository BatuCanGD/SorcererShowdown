#include "code/header/GameManagement/Utils.h"


int GetValidInput() {
    int choice;
    while (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::print("Invalid input. Please enter a valid number: ");
    }
    return choice;
}

double GetPreciseInput() {
    double ch;
    while (!(std::cin >> ch)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::print("Invalid input. Please enter a valid float amount: ");
    }
    return ch;
}

int GetRandomNumber(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

std::string ANSIPadding(const std::string& string, size_t length) {
    size_t visible_length = 0;
    bool inside_ansi = false;

    for (char c : string) {
        if (c == '\033') {
            inside_ansi = true;
        }
        else if (inside_ansi) {
            if (c == 'm') {
                inside_ansi = false; 
            }
        }
        else {
            visible_length++;
        }
    }

    if (visible_length < length) {
        return string + std::string(length - visible_length, ' ');
    }
    return string;
}