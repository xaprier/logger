#include <array>
#include <iterator>
#include <ostream>
#include <string>
#include <vector>

// Template operator<< for std::ostringstream
template <typename Container>
std::ostringstream &operator<<(std::ostringstream &out, const Container &container)
    requires requires { typename Container::const_iterator; }
{
    out << '[';
    for (auto it = container.begin(); it != container.end(); ++it) {
        out << *it;
        if (std::next(it) != container.end()) {
            out << ", ";
        }
    }
    out << ']';
    return out;
}
