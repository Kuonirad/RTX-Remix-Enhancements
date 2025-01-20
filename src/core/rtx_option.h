#pragma once

#include <string>

namespace rtx {

template<typename T>
class Option {
public:
    Option() = default;
    explicit Option(T defaultValue) : m_value(defaultValue) {}
    
    explicit operator T() const { return m_value; }
    T operator()() const { return m_value; }
    void set(const T& newValue) { m_value = newValue; }

private:
    T m_value{};
};

} // namespace rtx
