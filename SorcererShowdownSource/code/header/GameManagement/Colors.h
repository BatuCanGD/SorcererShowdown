#pragma once

namespace Color {
    inline constexpr std::string Clear = "\033[0m";
    inline constexpr std::string Bold = "\033[1m";
    
    inline constexpr std::string Black = "\033[30m";
    inline constexpr std::string Red = "\033[31m";
    inline constexpr std::string Green = "\033[32m";
    inline constexpr std::string Yellow = "\033[33m";
    inline constexpr std::string Blue = "\033[34m";
    inline constexpr std::string Purple = "\033[35m";
    inline constexpr std::string Cyan = "\033[36m";
    inline constexpr std::string BackgroundRed = "\033[41m";

    inline constexpr std::string Gray = "\033[90m";
    inline constexpr std::string DimGray = "\033[2;90m";

    inline constexpr std::string BrightRed = "\033[91m";
    inline constexpr std::string BrightGreen = "\033[92m";
    inline constexpr std::string BrightYellow = "\033[93m";
    inline constexpr std::string BrightBlue = "\033[94m";
    inline constexpr std::string BrightMagenta = "\033[95m";
    inline constexpr std::string BrightCyan = "\033[96m";
    
    inline constexpr std::string LightPink = "\033[38;5;183m";
    inline constexpr std::string Orange = "\033[38;5;208m";
    inline constexpr std::string Gold = "\033[38;5;214m";
}