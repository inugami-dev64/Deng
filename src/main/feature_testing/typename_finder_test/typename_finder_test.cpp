#include "../../../core/deng_core.h"

// test passed
int main() {
    float local_float = 1.0f;
    uint32_t local_uint = 1;
    std::string local_str = "Hello world!";

    if(dengUtils::FindWithTypename::isNumeralType(local_str, DENG_NUMERAL_TYPE_DONT_CARE))
        std::cout << "variable local_str is numeral type!\n";

    if(dengUtils::FindWithTypename::isNumeralType(local_float, DENG_NUMERAL_TYPE_DONT_CARE))
        std::cout << "variable local_float is numeral type!\n";

    if(dengUtils::FindWithTypename::isNumeralType(local_float, DENG_NUMERAL_TYPE_DONT_CARE))
        std::cout << "variable local_uint is numeral type!\n";

    if(dengUtils::FindWithTypename::isStringType(local_str))
        std::cout << "variable local_str is string type!\n";
        
    return 0;
}