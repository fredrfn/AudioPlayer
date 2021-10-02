#ifndef SOUNDS_UI_VIEWS_STRINGS_HPP
#define SOUNDS_UI_VIEWS_STRINGS_HPP

#include <string>

class StringsHelper {
public:
    static std::string formatTime(double timeInSeconds);
    static std::string formatFileSizeInMB(unsigned long long sizeInBytes, const std::string& unitText = "MB");
    static std::string extractFileName(const std::string& filePath);
    static std::string extractFileExtension(const std::string& filePath);
};

#endif