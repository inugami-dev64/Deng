#ifndef TYPENAME_FINDER_H
#define TYPENAME_FINDER_H

namespace dengUtils {
    struct FindWithTypename {
        template<typename T>
        static dengBool isNumeralType(T &sample_object, const dengNumeralType &numeral_type);
        
        template<typename T>
        static dengBool isStringType(T &sample_object);
    };

    template<typename T>
    dengBool FindWithTypename::isNumeralType(T &sample_object, const dengNumeralType &numeral_type) {
        switch (numeral_type)
        {
        case DENG_NUMERAL_TYPE_NON_FLOATING:
            if(typeid(sample_object) == typeid(int) || typeid(sample_object) == typeid(int8_t) || typeid(sample_object) == typeid(int16_t) ||
            typeid(sample_object) == typeid(int32_t) || typeid(sample_object) == typeid(uint8_t) || typeid(sample_object) == typeid(uint16_t) || 
            typeid(sample_object) == typeid(uint32_t)) return DENG_TRUE;
            break;

        case DENG_NUMERAL_TYPE_FLOAT:
            if(typeid(sample_object) == typeid(float)) return DENG_TRUE;
            break;

        case DENG_NUMERAL_TYPE_DOUBLE:
            if(typeid(sample_object) == typeid(double)) return DENG_TRUE;
            break;

        case DENG_NUMERAL_TYPE_BOOL:
            if(typeid(sample_object) == typeid(bool)) return DENG_TRUE;
            break;

        case DENG_NUMERAL_TYPE_DONT_CARE:
            if(typeid(sample_object) == typeid(int) || typeid(sample_object) == typeid(int8_t) || typeid(sample_object) == typeid(int16_t) ||
            typeid(sample_object) == typeid(int32_t) || typeid(sample_object) == typeid(uint8_t) || typeid(sample_object) == typeid(uint16_t) || 
            typeid(sample_object) == typeid(uint32_t) || typeid(sample_object) == typeid(float) || typeid(sample_object) == typeid(double)) {
                return DENG_TRUE;
        }
        break;
            
        default:
            break;
        }

        return DENG_FALSE;
    }

    template<typename T>
    dengBool FindWithTypename::isStringType(T &sample_object) {
        if(typeid(sample_object) == typeid(std::string) || typeid(sample_object) == typeid(const char*) || typeid(sample_object) == typeid(char*) || typeid(sample_object) == typeid(char[])) return DENG_TRUE;
        else return DENG_FALSE;
    }
}

#endif