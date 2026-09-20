#include <iostream>

template <typename T>
struct controlBlock {
    T* pointer;
    int counter;
};

template <typename T>
class shared_ptr {
public:
    shared_ptr(): block(new controlBlock<T>{nullptr, 0}) {std::cout << "nullptr constructor" << std::endl;};

    ~shared_ptr() {
        if (block && --block->counter == 0) {
            delete block->pointer;
            delete block;
        }
    };

    shared_ptr(T* pointer): block(new controlBlock<T>{pointer, 1}) {
        std::cout << "constructor" << std::endl;
    };

    shared_ptr(const shared_ptr& other) : block(other.block) {
        ++block->counter;
        std::cout << "copy constructor" << std::endl;
    }

private:
    controlBlock<T>* block;
};

class testClass{

};

int main() {
    shared_ptr<testClass> testPointer1(new testClass());
    shared_ptr<testClass> testPointer2 = testPointer1;
    return 0;
}