#include "view.hpp"

bool Validator::IsLimitValid(const std::string& limit)
{
    if (limit.empty()) {
        return false;
    }

    for (char c : limit) {
        if (!std::isdigit(c)) {
            return false;
        }
    }

    try {
        long long value = std::stoll(limit);
        if (value >= 0 && value <= INT_MAX) {
            return true;
        }
    } catch (...) {
        return false;
    }

    return false;
}