#pragma once
namespace Utilities {
    template<typename T> 
    T GetInput() {
        T choice{};
        while (!(std::cin >> choice)) {
            if (std::cin.eof()){
                throw std::runtime_error("Input stream ended");
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::println("Invalid Input!");
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return choice;
    }
    template<typename T>
    T GetRandomNumber(T min, T max){
        thread_local std::random_device rd;
        thread_local std::mt19937 gen(rd());
        using distribution = std::conditional_t<std::is_floating_point_v<T>, std::uniform_real_distribution<T>, std::uniform_int_distribution<T>>;
        distribution dist(min, max);
        return dist(gen);
    }
    inline void WaitForInput() {
        std::cin.clear();
        std::cin.get();
    }
}
