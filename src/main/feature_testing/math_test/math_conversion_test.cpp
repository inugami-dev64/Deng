#include "../../../core/deng_core.h"

int main() {
    dengMath::Conversion local_conversions;
    std::string local_type, local_numerical_value;
    std::cout << "Enter conversion type (dtr, rtd, htd): \n";
    std::getline(std::cin, local_type);

    std::cout << "Enter numerical value: \n";
    std::getline(std::cin, local_numerical_value);

    if(local_type == "htd")
        std::cout << "Dec value is: " << local_conversions.hexToDec(local_numerical_value) << "\n";
    
    else if(local_type == "rtd")
        std::cout << "Degree value is: " << local_conversions.radToDeg(static_cast<float>(std::stof(local_numerical_value))) << "\n";
    
    else if(local_type == "dtr")
        std::cout << "Radian value is: " << local_conversions.degToRad(static_cast<float>(std::stoi(local_numerical_value))) << "\n";

    return 0;
}