#pragma once

#include <string>
#include <iostream>

class WeakPtr;

struct Block {
    Block(int sc, int wc) : strong_count(sc), weak_count(wc) {
    }
    int strong_count = 0;
    int weak_count = 0;
};

class SharedPtr {
public:
    SharedPtr() {
        obj_ = nullptr;
        counter_ = nullptr;
    }

    SharedPtr(std::string* obj) {
        obj_ = obj;
        counter_ = new Block(1, 0);
    }

    SharedPtr(const SharedPtr& rhs) {
        obj_ = rhs.obj_;
        counter_ = rhs.counter_;
        if (counter_) {
            ++counter_->strong_count;
        }
    }

    SharedPtr(SharedPtr&& rhs) {
        obj_ = rhs.obj_;
        counter_ = rhs.counter_;
        rhs.obj_ = nullptr;
        rhs.counter_ = nullptr;
    }

    SharedPtr(const WeakPtr& rhs);

    SharedPtr& operator=(SharedPtr rhs) {
        Swap(rhs);
        return *this;
    }

    void Swap(SharedPtr& rhs) {
        std::swap(obj_, rhs.obj_);
        std::swap(counter_, rhs.counter_);
    }

    void Reset(SharedPtr rhs) {
        Swap(rhs);
    }

    ~SharedPtr() {
        if (!counter_) {
            return;
        }

        if (counter_->strong_count > 1) {
            --counter_->strong_count;
        } else if (counter_->strong_count == 1) {
            --counter_->strong_count;
            delete obj_;
        }
        if (counter_->strong_count + counter_->weak_count == 0) {
            delete counter_;
        }
    }

    std::string* Get() {
        return obj_;
    }

    std::string& operator*() {
        return *obj_;
    }

    std::string* operator->() {
        return obj_;
    }

private:
    std::string* obj_;
    Block* counter_;
    friend class WeakPtr;
};

class WeakPtr {
public:
    WeakPtr() {
        obj_ = nullptr;
        counter_ = nullptr;
    }

    WeakPtr(const SharedPtr& rhs) {
        obj_ = rhs.obj_;
        counter_ = rhs.counter_;
        if (counter_) {
            ++counter_->weak_count;
        }
    }

    WeakPtr(const WeakPtr& rhs) {
        obj_ = rhs.obj_;
        counter_ = rhs.counter_;
        if (counter_) {
            ++counter_->weak_count;
        }
    }

    WeakPtr(WeakPtr&& rhs) {
        obj_ = rhs.obj_;
        counter_ = rhs.counter_;
        rhs.obj_ = nullptr;
        rhs.counter_ = nullptr;
    }

    WeakPtr& operator=(WeakPtr rhs) {
        Swap(rhs);
        return *this;
    }

    ~WeakPtr() {
        if (counter_ == nullptr) {
            return;
        }
        --counter_->weak_count;
        if (counter_->strong_count == 0 && counter_->weak_count == 0) {
            delete counter_;
        }
    }

    void Swap(WeakPtr& rhs) {
        std::swap(obj_, rhs.obj_);
        std::swap(counter_, rhs.counter_);
    }

    bool IsExpired() const {
        if (counter_ == nullptr) {
            return true;
        }
        return counter_->strong_count == 0;
    }

    SharedPtr Lock() const {
        if (IsExpired()) {
            return SharedPtr();
        }
        return SharedPtr(*this);
    }

private:
    std::string* obj_;
    Block* counter_;
    friend class SharedPtr;
};

SharedPtr::SharedPtr(const WeakPtr& rhs) {
    if (rhs.counter_ && rhs.counter_->strong_count > 0) {
        obj_ = rhs.obj_;
        counter_ = rhs.counter_;
        ++counter_->strong_count;
    } else {
        obj_ = nullptr;
        counter_ = rhs.counter_;
    }
}
