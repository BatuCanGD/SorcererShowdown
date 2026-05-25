#pragma once
namespace Utilities {
    namespace Color {
        inline const std::string Clear = "\033[0m";
        inline const std::string Bold = "\033[1m";
        inline const std::string Red = "\033[31m";
        inline const std::string Green = "\033[32m";
        inline const std::string Yellow = "\033[33m";
        inline const std::string Blue = "\033[34m";
        inline const std::string Purple = "\033[35m";
        inline const std::string Cyan = "\033[36m";
        inline const std::string BackgroundRed = "\033[41m";
        inline const std::string BrightRed = "\033[91m";
        inline const std::string BrightMagenta = "\033[95m";
        inline const std::string DimGray = "\033[2;90m";
        inline const std::string LightPink = "\033[38;5;183m";
    }
    template<typename T> 
    T GetInput() {
        T choice;
        while (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::println("Invalid Input!");
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return choice;
    }
    
    int GetRandomNumber(int min, int max);
    void WaitForInput();
    std::string StringPad(const std::string& string, size_t length);
}
