#include <iostream>
#include <utility>

template <typename T, bool IsArray = false>
class unique_ptr {
public:
    explicit  unique_ptr(T* ptr) : object(ptr) {std::cout<<"constructor"<<std::endl;}

    ~unique_ptr() {
        if constexpr (IsArray) {
            delete[] object;      // для массива
        } else {
            delete object;        // для одиночного объекта
        }
    }

    unique_ptr(const unique_ptr& other) = delete;
    unique_ptr& operator= (const unique_ptr& other) = delete;

    unique_ptr(unique_ptr&& other) noexcept : object(std::exchange(other.object, nullptr)) {std::cout<<"move constructor"<<std::endl;}
    unique_ptr& operator=(unique_ptr&& other) {
        if (&other != this) {
            delete object;
            object = std::exchange(other.object, nullptr);
        }
        std::cout<<"move operator"<<std::endl;
        return *this;
    }

    T& operator*() const noexcept {
        return *object;
    }

    T* operator->() const noexcept {
        return object;
    }

    T* release(unique_ptr& ptr) {
        T* temp = object;
        object = nullptr;
        return temp;
    }

    T& operator[](int i) const noexcept {
        return object[i];
    }

private:
    T* object;
};

class testClass {
public:
    void write() {};
};

int main() {

    unique_ptr<testClass> ptr(new testClass());
    unique_ptr<testClass> ptr1(new testClass());
    ptr1 = std::move(ptr);

    int arr[3] = {1,2,3};
    unique_ptr<int, true> arrPtr(new int[5]());
    std::cout << arrPtr[1] << std::endl;
    return 0;
}