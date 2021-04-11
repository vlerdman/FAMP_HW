#include <memory>
#include <tuple>

template <class T, class D = std::default_delete<T>>
class UniquePtr {
private:
    std::tuple<T*, D> ptr;

public:
    explicit UniquePtr(T* other = nullptr) {
        std::get<0>(ptr) = other;
    }
    UniquePtr(T* other, const D& deleter) {
        std::get<0>(ptr) = other;
        std::get<1>(ptr) = deleter;
    }
    UniquePtr(UniquePtr&& other) noexcept {
        this -> swap(other);
    }
    UniquePtr(const UniquePtr& other) = delete;
    UniquePtr& operator= (const UniquePtr& other) = delete;
    UniquePtr& operator= (std::nullptr_t) {
        std::get<1>(ptr)(std::get<0>(ptr));
        std::get<0>(ptr) = nullptr;
        return *this;
    }
    UniquePtr& operator= (UniquePtr&& other) noexcept {
        this -> swap(other);
        return *this;
    }
    ~UniquePtr() {
        std::get<1>(ptr)(std::get<0>(ptr));
    }
    const T* operator-> () const {
        return std::get<0>(ptr);
    }
    const T& operator* () const {
        return *std::get<0>(ptr);
    }
    T* release() {
        T* tmp = std::get<0>(ptr);
        std::get<0>(ptr) = nullptr;
        return tmp;
    }
    void reset(T* other) {
        if (std::get<0>(ptr) != other) {
            std::get<1>(ptr)(std::get<0>(ptr));
            std::get<0>(ptr) = other;
        }
    }
    void swap(UniquePtr& other) {
        if (this == &other) {
            return;
        }
        T* tmp = std::get<0>(ptr);
        std::get<0>(ptr) = std::get<0>(other.ptr);
        std::get<0>(other.ptr) = tmp;
        D tmp2 = std::get<1>(ptr);
        std::get<1>(ptr) = std::get<1>(other.ptr);
        std::get<1>(other.ptr) = tmp2;
    }
    T* get() const {
        return std::get<0>(ptr);
    }
    explicit operator bool() const {
        return std::get<0>(ptr) != nullptr;
    }
    const D& get_deleter() const {
        return std::get<1>(ptr);
    }
    D& get_deleter() {
        return std::get<1>(ptr);
    }
};