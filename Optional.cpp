#include <memory>
struct BadOptionalAccess {
};

template <typename T>
class Optional {
private:
    alignas(T) unsigned char data[sizeof(T)];
    bool defined = false;

public:
    Optional() = default;
    Optional(const T& elem) {
        defined = true;
        new (data) T(elem);
    }
    Optional(T && elem) {
        defined = true;
        new (data) T(std::move(elem));
    }
    Optional(const Optional& other) {
        if (other.defined) {
            defined = true;
            new (data) T(other.value());
        }
    }

    Optional& operator=(const Optional& other) {
        if (this == &other) {
            return *this;
        }
        if (has_value()) {
            if (other.has_value()) {
                value() = other.value();
            } else {
                reset();
            }
        } else {
            if (other.has_value()) {
                new (data) T(other.value());
                defined = true;
            } else {
                reset();
            }
        }
        return *this;
    }
    Optional& operator=(const T& elem) {
        if (has_value()) {
            value() = elem;
        } else {
            new (data) T(elem);
            defined = true;
        }
        return *this;
    }
    Optional& operator=(T&& elem) {
        if (has_value()) {
            value() = std::move(elem);
        } else {
            new (data) T(std::move(elem));
            defined = true;
        }
        return *this;
    }

    bool has_value() const {
        return defined;
    }

    T& operator*() {
        return value();
    }
    const T& operator*() const {
        return value();
    }

    T* operator->() {
        return reinterpret_cast<T*>(data);
    }
    const T* operator->() const {
        return reinterpret_cast<const T*>(data);
    }

    T& value() {
        if (!has_value()) {
            throw BadOptionalAccess();
        }
        return *(reinterpret_cast<T*>(data));
    }
    const T& value() const {
        if (!has_value()) {
            throw BadOptionalAccess();
        }
        return *(reinterpret_cast<const T*>(data));
    }

    void reset() {
        if (defined) {
            value().~T();
            defined = false;
        }
    }

    ~Optional() {
        reset();
    }
};