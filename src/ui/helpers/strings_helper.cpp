#include "ui/helpers/strings_helper.hpp"
#include <algorithm>

std::string StringsHelper::formatTime(double timeInSeconds) {
    std::string minutes = std::to_string((int)(timeInSeconds / 60));
    int seconds = (int)(timeInSeconds) % 60;
    std::string secondsString = seconds < 10 ? "0" + std::to_string(seconds) : std::to_string(seconds);
    return minutes + ":" + secondsString;
}

std::string StringsHelper::formatFileSizeInMB(unsigned long long sizeInBytes, const std::string& unitText) {
    std::string megas = std::to_string(sizeInBytes/1000000);
    unsigned long long kilos = (sizeInBytes % 1000000) / 1000;
    return megas + "." + std::to_string(kilos/10) + unitText;
}

std::string StringsHelper::extractFileName(const std::string& filePath) {
    std::string::size_type idx = filePath.rfind('\\');
    if (idx != std::string::npos) {
        return filePath.substr(idx+1);
    } else {
        idx = filePath.rfind('/');
        return idx != std::string::npos ? filePath.substr(idx+1) : "";
    }
}

std::string StringsHelper::extractFileExtension(const std::string& filePath) {
    std::string::size_type idx = filePath.rfind('.');
    return idx != std::string::npos ? filePath.substr(idx+1) : "";
}

std::string StringsHelper::formatFrequency(float frequency) {
    unsigned int kHz = (unsigned int)(frequency / 1000);
    unsigned int hz = (unsigned int)frequency % 1000;
    if (kHz == 0) {
        return std::to_string(hz) + "Hz";
    } else {
        if (hz == 0) {
            return std::to_string(kHz) + "kHz";
        } else {
            hz = hz / 100;
            return std::to_string(kHz) + "." + std::to_string(hz) + "kHz";
        }
    }
};