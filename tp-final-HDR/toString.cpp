#include <string>

template <typename T>
std::string toString(T const& value) {
    stringstream sstr;
    sstr << value;
    return sstr.str();
}