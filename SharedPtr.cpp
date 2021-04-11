#include <cstddef>
#include <utility>

template<typename T>
class SharedPtr {
private:
    class Core {
    public:
        T* ptr;
        size_t sz = 1;

        Core(T* other) {
            ptr = other;
        }

        void Decrease() {
            --sz;
            if (sz == 0) {
                delete ptr;
            }
        }

        void Increase() {
            ++sz;
        }

        ~Core() {
        }
    };

    Core* corePtr = nullptr;

public:
    SharedPtr() = default;

    SharedPtr(T* other) {
        if (other != nullptr) {
            corePtr = new Core(other);
        }
    }
    SharedPtr(const SharedPtr& other) {
        if (other.corePtr != nullptr) {
            corePtr = other.corePtr;
            corePtr -> Increase();
        }
    }

    SharedPtr(SharedPtr && other) {
        swap(other);
    }

    SharedPtr& operator= (T* other) {
        if (corePtr == nullptr) {
            corePtr = new Core(other);
        } else {
            corePtr -> Decrease();
            if (corePtr -> sz == 0) {
                delete corePtr;
            }
            corePtr = new Core(other);
        }
        return *this;
    }

    SharedPtr& operator= (const SharedPtr& other) {
        if (this == &other) {
            return *this;
        }
        SharedPtr tmp(other);
        swap(tmp);
        return *this;
    }

    void swap(SharedPtr& other) {
        if (this != &other) {
            std::swap(corePtr, other.corePtr);
        }
    }

    SharedPtr& operator= (SharedPtr && other) {
        if (this == &other) {
            return *this;
        }
        swap(other);
        SharedPtr tmp;
        tmp.swap(other);
        return *this;
    }

    ~SharedPtr() {
        if (corePtr != nullptr) {
            corePtr -> Decrease();
            if (corePtr -> sz == 0) {
                delete corePtr;
            }
        }
    }

    const T* operator ->() const {
        if (corePtr == nullptr) {
            return nullptr;
        }
        return corePtr -> ptr;
    }

    const T& operator* () const {
        return *(corePtr -> ptr);
    }

    T& operator* () {
        return *(corePtr -> ptr);
    }

    void reset(T* other) {
        if (other == nullptr) {
            SharedPtr tmp;
            swap(tmp);
        } else {
            SharedPtr tmp(other);
            swap(tmp);
        }
    }

    T* get() const {
        if (corePtr == nullptr) {
            return nullptr;
        }
        return corePtr -> ptr;
    }

    operator bool() const {
        return corePtr != nullptr;
    }
};