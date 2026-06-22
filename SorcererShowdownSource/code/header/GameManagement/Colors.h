#pragma once

namespace Color {
    inline const std::string Clear = "\033[0m";
    inline const std::string Bold = "\033[1m";
    
    inline const std::string Black = "\033[30m";
    inline const std::string Red = "\033[31m";
    inline const std::string Green = "\033[32m";
    inline const std::string Yellow = "\033[33m";
    inline const std::string Blue = "\033[34m";
    inline const std::string Purple = "\033[35m";
    inline const std::string Cyan = "\033[36m";
    inline const std::string BackgroundRed = "\033[41m";

    inline const std::string Gray = "\033[90m";
    inline const std::string DimGray = "\033[2;90m";

    inline const std::string BrightRed = "\033[91m";
    inline const std::string BrightGreen = "\033[92m";
    inline const std::string BrightYellow = "\033[93m";
    inline const std::string BrightBlue = "\033[94m";
    inline const std::string BrightMagenta = "\033[95m";
    inline const std::string BrightCyan = "\033[96m";
    
    inline const std::string LightPink = "\033[38;5;183m";
    inline const std::string Orange = "\033[38;5;208m";
    inline const std::string Gold = "\033[38;5;214m";
}