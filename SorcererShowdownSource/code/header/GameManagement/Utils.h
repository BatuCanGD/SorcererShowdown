#pragma once
namespace Utilities {
    template<typename T> 
    [[nodiscard]] T GetInput() {
        using Type = std::decay_t<T>;
        Type value{};

        if constexpr (std::is_same_v<Type, std::string>) {
            std::getline(std::cin, value);
        } else {
            while (!(std::cin >> value)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Try again: ";
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        return value;
    }
    template<typename T>
    requires (std::integral<T> && !std::same_as<T, bool>) || std::floating_point<T>
    [[nodiscard]] T GetRandom(T mn, T mx) {
        if (mn == mx) return mn;
        T min = std::min(mn, mx), max = std::max(mn, mx);

        thread_local std::mt19937 gen([] {
            std::random_device rd;
            std::seed_seq seed{rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};
            return std::mt19937(seed);
        }());

        if constexpr (std::is_floating_point_v<T>) {
            std::uniform_real_distribution<T> dist(min, max);
            return dist(gen);
        } else {
            using distro = std::conditional_t<std::is_signed_v<T>, long long, unsigned long long>;
            std::uniform_int_distribution<distro> dist(static_cast<distro>(min), static_cast<distro>(max));
            return static_cast<T>(dist(gen));
        }
    }
    inline void WaitForInput() {
        std::cin.clear();
        std::cin.get();
    }
}
