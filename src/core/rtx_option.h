#pragma once

#include <string>

namespace rtx {

template<typename T>
class Option {
public:
    Option() = default;
    Option(T defaultVal) : value_(defaultVal) {}
    
    operator T() const { return value_; }
    T operator()() const { return value_; }
    void set(const T& value) { value_ = value; }

private:
    T value_{};
};

} // namespace rtx
