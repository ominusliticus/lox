#pragma once


// Just a lightweight wrapper around raw pointers, since I cannot stand the sight of them.
template<typename T>
class nonowning_ptr {
public:
    nonowning_ptr(T* ptr) : m_ptr(ptr) {}

    template<typename U>
    U* cast() {
        return dynamic_cast<U>(m_ptr);
    }

private:
    T* m_ptr;
};
