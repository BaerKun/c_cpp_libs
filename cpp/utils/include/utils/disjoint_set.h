#ifndef DISJOINT_SET_H
#define DISJOINT_SET_H

namespace cpp_libs {
    class DisjointSet {
    public:
        explicit DisjointSet(const int size) : class_(new int[size]), size_(size) {
            for (int i = 0; i < size; ++i)
                class_[i] = -1;
        }

        ~DisjointSet() {
            delete[] class_;
        }

        void mergeClass(const int class1, const int class2) const {
            int *neg_height = class_;
            if (neg_height[class1] > neg_height[class2])
                class_[class1] = class2;
            else {
                if (neg_height[class1] == neg_height[class2])
                    neg_height[class1]--;
                class_[class2] = class1;
            }
        }

        [[nodiscard]] int findClass(const int id) const {
            int cls;
            for (cls = id; class_[cls] >= 0; cls = class_[cls]) {
            }
            for (int i = id; i != cls;) {
                const int next = class_[i];
                class_[i] = cls;
                i = next;
            }
            return cls;
        }

    private:
        int *class_;
        int size_;
    };
}

#endif //DISJOINT_SET_H
