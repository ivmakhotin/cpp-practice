#pragma once

#include <initializer_list>

class Vector {
public:
    Vector() : size_(0), capacity_(1) {
        head_ = new int[1];
    }

    Vector(size_t n) : size_(n), capacity_(n) {
        head_ = new int[n];
        for (size_t i = 0; i < n; ++i) {
            *(head_ + i) = 0;
        }
    }
    Vector(std::initializer_list<int> list) {
        head_ = new int[list.size()];
        size_ = list.size();
        capacity_ = size_;
        for (size_t i = 0; i < list.size(); ++i) {
            *(head_ + i) = *(list.begin() + i);
        }
    }

    Vector(const Vector& rhs) : size_(rhs.size_), capacity_(rhs.capacity_) {
        if (capacity_ == 1 && size_ == 0) {
            head_ = new int[1];
        } else {
            head_ = new int[size_];
        }
        for (size_t i = 0; i < size_; ++i) {
            *(head_ + i) = *(rhs.head_ + i);
        }
    }
    Vector& operator=(const Vector& rhs) {
        size_ = rhs.size_;
        capacity_ = rhs.capacity_;
        for (size_t i = 0; i < size_; i++) {
            *(head_ + i) = *(rhs.head_ + i);
        }
        return *this;
    }

    Vector(Vector&& rhs) : head_(rhs.head_), size_(rhs.size_), capacity_(rhs.capacity_) {
        rhs.head_ = nullptr;
    }

    Vector& operator=(Vector&& rhs) {
        if (this == &rhs) {
            return *this;
        }
        size_ = rhs.size_;
        capacity_ = rhs.capacity_;
        delete[] head_;
        head_ = rhs.head_;
        rhs.head_ = nullptr;
        return *this;
    }

    size_t Size() const {
        return size_;
    }

    size_t Capacity() {
        return capacity_;
    }

    void Swap(Vector& rhs) {
        std::swap(head_, rhs.head_);
        std::swap(size_, rhs.size_);
        std::swap(capacity_, rhs.capacity_);
    }

    void PopBack() {
        --size_;
    }

    void Reserve(size_t n) {
        if (capacity_ < n) {
            int* tmp = new int[n];
            for (size_t i = 0; i < size_; ++i) {
                *(tmp + i) = *(head_ + i);
            }
            std::swap(head_, tmp);
            capacity_ = n;
            delete[] tmp;
        }
    }

    void PushBack(int e) {
        if (size_ == capacity_) {
            int* tmp = new int[2 * capacity_];
            for (size_t i = 0; i < size_; ++i) {
                *(tmp + i) = *(head_ + i);
            }
            *(tmp + size_) = e;
            std::swap(head_, tmp);
            capacity_ *= 2;
            delete[] tmp;
        } else {
            *(head_ + size_) = e;
        }
        ++size_;
    }

    void Clear() {
        size_ = 0;
    }

    int& operator[](size_t i) {
        return *(head_ + i);
    }

    int operator[](size_t i) const {
        return *(head_ + i);
    }

    class Iterator : public std::iterator<std::input_iterator_tag, int, int, const int*, int> {
    public:
        Iterator() : ptr_(nullptr) {
        }
        Iterator(int* ptr) : ptr_(ptr) {
        }
        Iterator& operator++() {
            ++ptr_;
            return *this;
        }
        Iterator operator++(int) {
            Iterator retval = *this;
            ++(*this);
            return retval;
        }
        Iterator& operator--() {
            --ptr_;
            return *this;
        }
        Iterator operator--(int) {
            Iterator retval = *this;
            --(*this);
            return retval;
        }
        Iterator operator+(int n) {
            Iterator res = *this;
            while (n > 0) {
                ++res.ptr_;
                n--;
            }
            while (n < 0) {
                --res.ptr_;
                n++;
            }
            return res;
        }
        Iterator& operator+=(int n) {
            *this = *this + n;
            return *this;
        }
        Iterator operator-(int n) {
            Iterator res = *this;
            while (n > 0) {
                --res.ptr_;
                n--;
            }
            while (n < 0) {
                ++res.ptr_;
                n++;
            }
            return res;
        }
        Iterator& operator-=(int n) {
            *this = *this - n;
            return *this;
        }
        bool operator==(Iterator other) const {
            return ptr_ == other.ptr_;
        }
        bool operator!=(Iterator other) const {
            return ptr_ != other.ptr_;
        }
        int operator-(Iterator& rhs) {
            Iterator tmp = *this;
            int n = 0;
            while (tmp != rhs) {
                --tmp;
                ++n;
            }
            return n;
        }
        int operator*() const {
            return *ptr_;
        }
        int* operator->() {
            return ptr_;
        }

    private:
        int* ptr_;
    };

    Iterator Begin() {
        return begin();
    }

    Iterator End() {
        return end();
    }

    Iterator begin() {  // NOLINT
        return Iterator(head_);
    }

    Iterator end() {  // NOLINT
        return Iterator(head_ + size_);
    }

    ~Vector() {
        delete[] head_;
        size_ = 0;
        capacity_ = 0;
    }

private:
    int* head_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;
};
