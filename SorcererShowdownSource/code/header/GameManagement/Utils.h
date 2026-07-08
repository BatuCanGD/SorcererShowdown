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
            std::println(stderr, "Invalid Input!");
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return choice;
    }
    template<typename T>
    requires std::integral<T> || std::floating_point<T>
    T GetRandom(T min, T max){ 
        assert(min <= max);
        thread_local std::mt19937 gen(std::random_device{}());
        if constexpr (std::is_floating_point_v<T>) {
            std::uniform_real_distribution<T> dist(min, max);
            return dist(gen);
        } else {
            using distro = std::conditional_t<sizeof(T) < sizeof(int), std::conditional_t<std::is_signed_v<T>, int, unsigned int>, T>;
            std::uniform_int_distribution<distro> dist(min, max);
            return static_cast<T>(dist(gen));
        }
    }
    inline void WaitForInput() {
        std::cin.clear();
        std::cin.get();
    }
}
