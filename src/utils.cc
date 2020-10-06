#include "utils.h"

std::string randomString(int len) {
    std::string ret;
    srand(time(NULL));
    for (int i = 0; i < len; ++i) {
        switch(rand()%3) {
        case 1:
            ret.push_back('A'+rand()%26);
            break;
        case 2:
            ret.push_back('a'+rand()%26);
            break;
        case 3:
            ret.push_back('0'+rand()%10);
            break;
        }
    }
    return ret;
}
