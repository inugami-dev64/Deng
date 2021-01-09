#include "../../../core/deng_core.h"

// test passed
int main() {
    float float = 1.0f;
    uint32_t uint = 1;
    std::string str = "Hello world!";

    if(dengUtils::FindWithTypename::isNumeralType(str, DENG_NUMERAL_TYPE_DONT_CARE))
        std::cout << "variable str is numeral type!\n";

    if(dengUtils::FindWithTypename::isNumeralType(float, DENG_NUMERAL_TYPE_DONT_CARE))
        std::cout << "variable float is numeral type!\n";

    if(dengUtils::FindWithTypename::isNumeralType(float, DENG_NUMERAL_TYPE_DONT_CARE))
        std::cout << "variable uint is numeral type!\n";

    if(dengUtils::FindWithTypename::isStringType(str))
        std::cout << "variable str is string type!\n";
        
    return 0;
}