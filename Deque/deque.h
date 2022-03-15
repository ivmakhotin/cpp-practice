#pragma once

#include <initializer_list>
#include <algorithm>
#include <iostream>

class Deque {
public:
    Deque() = default;

    Deque(const Deque& rhs) {
        for (size_t i = 0; i < rhs.Size(); ++i) {
            PushBack(rhs[i]);
        }
    }

    Deque(Deque&& rhs) {
        for (size_t i = 0; i < rhs.Size(); ++i) {
            PushBack(rhs[i]);
        }
        rhs.Clear();
    }

    explicit Deque(size_t size) {
        for (size_t i = 0; i < size; ++i) {
            PushBack(0);
        }
    }

    Deque(std::initializer_list<int> list) {
        for (auto val : list) {
            PushBack(val);
        }
    }

    Deque& operator=(Deque rhs) {
        Swap(rhs);
        return *this;
    }

    void Swap(Deque& rhs) {
        std::swap(data_, rhs.data_);
        std::swap(size_, rhs.size_);
        std::swap(capacity_, rhs.capacity_);
        std::swap(begin_block_, rhs.begin_block_);
        std::swap(end_block_, rhs.end_block_);
        std::swap(begin_in_block_, rhs.begin_in_block_);
        std::swap(end_in_block_, rhs.end_in_block_);
    }

    void PushBack(int value) {
        if (size_ == capacity_ * kBlockSize) {
            Reallocate();
        }

        if (size_ == 0) {
            data_[end_block_] = new Block();
        } else if (end_in_block_ == kBlockSize - 1) {
            end_in_block_ = 0;
            end_block_ = IncBlockNum(end_block_);
            if (end_block_ != begin_block_) {
                data_[end_block_] = new Block();
            }
        } else {
            end_in_block_++;
        }

        data_[end_block_]->block[end_in_block_] = value;
        size_++;
    }

    void PopBack() {
        if (size_ == 1) {
            delete data_[end_block_];
            size_ = 0;
            begin_in_block_ = 0;
            end_in_block_ = 0;
            begin_block_ = 0;
            end_block_ = 0;
            return;
        }

        if (end_in_block_ == 0) {
            end_in_block_ = kBlockSize - 1;
            if (end_block_ != begin_block_) {
                delete data_[end_block_];
            }
            end_block_ = DecBlockNum(end_block_);
        } else {
            end_in_block_--;
        }
        size_--;
    }

    void PushFront(int value) {
        if (size_ == capacity_ * kBlockSize) {
            Reallocate();
        }
        if (size_ == 0) {
            data_[begin_block_] = new Block();
            begin_in_block_ = kBlockSize - 1;
            end_in_block_ = kBlockSize - 1;
        } else if (begin_in_block_ == 0) {
            begin_in_block_ = kBlockSize - 1;
            begin_block_ = DecBlockNum(begin_block_);
            if (end_block_ != begin_block_) {
                data_[begin_block_] = new Block();
            }
        } else {
            begin_in_block_--;
        }

        data_[begin_block_]->block[begin_in_block_] = value;
        size_++;
    }

    void PopFront() {
        if (size_ == 1) {
            delete data_[begin_block_];
            size_ = 0;
            begin_in_block_ = 0;
            end_in_block_ = 0;
            begin_block_ = 0;
            end_block_ = 0;
            return;
        }

        if (begin_in_block_ + 1 == kBlockSize) {
            begin_in_block_ = 0;
            if (end_block_ != begin_block_) {
                delete data_[begin_block_];
            }
            begin_block_ = IncBlockNum(begin_block_);
        } else {
            begin_in_block_++;
        }

        size_--;
    }

    int& operator[](size_t idx) {
        auto [block, elem] = GetIndex(idx);
        return data_[block]->block[elem];
    }

    int operator[](size_t idx) const {
        auto [block, elem] = GetIndex(idx);
        return data_[block]->block[elem];
    }

    size_t Size() const {
        return size_;
    }

    void Clear() {
        while (size_ > 0) {
            PopBack();
        }

        if (data_ != nullptr) {
            delete[] data_;
            data_ = nullptr;
        }

        size_ = 0;
        capacity_ = 0;
        begin_in_block_ = 0;
        end_in_block_ = 0;
        begin_block_ = 0;
        end_block_ = 0;
    }

    ~Deque() {
        Clear();
    }

private:
    constexpr static size_t kBlockSize = 512 / sizeof(int);

    struct Block {
        int block[kBlockSize];
    };

    typedef Block* BlockPtr;
    BlockPtr* data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;
    size_t begin_in_block_ = 0;
    size_t end_in_block_ = 0;
    size_t begin_block_ = 0;
    size_t end_block_ = 0;

    void Reallocate() {
        size_t new_capacity;
        if (capacity_ == 0) {
            new_capacity = 1;
        } else {
            new_capacity = 2 * capacity_;
        }

        BlockPtr* new_data = new BlockPtr[new_capacity];

        size_t cur_block = begin_block_;
        for (size_t i = 0; i < capacity_; ++i) {
            new_data[i] = std::move(data_[cur_block]);
            cur_block = IncBlockNum(cur_block);
        }

        delete[] data_;
        data_ = new_data;
        end_block_ = begin_block_ == end_block_ ? 0 : capacity_ - 1;
        capacity_ = new_capacity;
        begin_block_ = 0;
    }

    size_t IncBlockNum(size_t block_num) const {
        return (block_num + 1) % capacity_;
    }

    size_t DecBlockNum(size_t block_num) const {
        if (block_num == 0) {
            return capacity_ - 1;
        } else {
            return block_num - 1;
        }
    }

    std::pair<size_t, size_t> GetIndex(size_t idx) const {
        size_t block = begin_block_;
        size_t elem = begin_in_block_;
        if (idx / kBlockSize > 0) {
            block = (block + (idx / kBlockSize)) % capacity_;
            idx -= kBlockSize * (idx / kBlockSize);
        }

        while (idx > 0) {
            if (elem + 1 == kBlockSize) {
                elem = 0;
                block = IncBlockNum(block);
            } else {
                elem++;
            }
            idx--;
        }

        return {block, elem};
    }
};
