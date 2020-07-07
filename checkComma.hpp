#include <string>

bool checkComma (std::string string) 
{
    for (auto v : string)
    {
        if (v == ',') { return true; }
    }
    return false;
}
