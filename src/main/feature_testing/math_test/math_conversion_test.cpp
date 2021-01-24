#include "../../../core/deng_core.h"

int main() {
    dengMath::Conversion conversions;
    std::string type, numerical_value;
    std::cout << "Enter conversion type (dtr, rtd, htd): \n";
    std::getline(std::cin, type);

    std::cout << "Enter numerical value: \n";
    std::getline(std::cin, numerical_value);

    if(type == "htd")
        std::cout << "Dec value is: " << conversions.hexToDec(numerical_value) << "\n";
    
    else if(type == "rtd")
        std::cout << "Degree value is: " << conversions.radToDeg(static_cast<float>(std::stof(numerical_value))) << "\n";
    
    else if(type == "dtr")
        std::cout << "Radian value is: " << conversions.degToRad(static_cast<float>(std::stoi(numerical_value))) << "\n";

    return 0;
}