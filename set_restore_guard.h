//
// Created by qw on 27.10.18.
//

#ifndef SET_RESTORE_GUARD_SET_RESTORE_GUARD_H
#define SET_RESTORE_GUARD_SET_RESTORE_GUARD_H

#include <type_traits>
#include <functional>

template <typename T>
struct set_restore_guard {
    set_restore_guard(set_restore_guard const&) = delete;
    set_restore_guard& operator=(set_restore_guard const&) = delete;

    set_restore_guard() = default;
    set_restore_guard(T& var, T const& new_value) : is_set(true), adr(&var) {
        new(&val) T(var);
        var = new_value;
    }
    void set(T& var, T const& new_value) {
        is_set = true;
        val = var;
        var = new_value;
        adr = &var;
    }
    ~set_restore_guard() {
        if (is_set) {
            *adr = get_value();
            clear();
        }
    }

    T const& old_value() const {
        return *get_value();
    }
    explicit operator bool() const {
        return is_set;
    }

    void restore() {
        if (is_set) {
            is_set = false;
            *adr = get_value();
            clear();
        }
    }


private:
    T* adr;
    typename std::aligned_storage<sizeof(T), alignof(T)>::type val;
    //typename std::aligned_storage<sizeof(T), alignof(T)>::type* adr;
    bool is_set{};

    void clear() {
        if (is_set) {
            get_value().T::~T();
        }
        is_set = false;
    }

    T get_value() {
        return *reinterpret_cast<T*>(&val);
    }

    T const* get_value() const {
        return reinterpret_cast<T const*>(&val);
    }
};

#endif //SET_RESTORE_GUARD_SET_RESTORE_GUARD_H
