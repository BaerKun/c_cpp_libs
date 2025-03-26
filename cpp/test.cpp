#include <utils/link_like.h>
#include <iostream>

int main() {
    cpp_libs::LinkLike<char> link;
    link.enqueue('a');
    link.enqueue('b');
    link.enqueue('c');

    while (!link.isEmpty()) {
        std::cout << link.front() << std::endl;
        link.dequeue();
    }

    link.push('a');
    link.push('b');
    link.push('c');

    while (!link.isEmpty()) {
        std::cout << link.top() << std::endl;
        link.pop();
    }
}