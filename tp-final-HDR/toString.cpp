#include <string>
#include <sstream>

template <typename T>
std::string toString(T const& value) {
    std::stringstream sstr;
    sstr << value;
    return sstr.str();
}
