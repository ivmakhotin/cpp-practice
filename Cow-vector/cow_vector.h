#pragma once

#include <iostream>
struct State {
    int ref_count = 1;
    std::vector<std::string> vec;
};

class COWVector {
public:
    COWVector() {
        state_ = new State();
    }

    ~COWVector() {
        if (state_->ref_count > 1) {
            state_->ref_count--;
        } else {
            delete state_;
        }
    }

    COWVector(const COWVector& other) {
        state_ = other.state_;
        state_->ref_count++;
    }
    COWVector& operator=(const COWVector& other) {
        this->~COWVector();
        state_ = other.state_;
        state_->ref_count++;
        return *this;
    }

    size_t Size() const {
        return state_->vec.size();
    }

    void Resize(size_t size) {
        if (state_->ref_count > 1) {
            state_->ref_count--;
            State* tmp = new State();
            tmp->vec.resize(size);
            for (size_t i = 0; i < std::min(size, state_->vec.size()); ++i) {
                tmp->vec[i] = state_->vec[i];
            }
            state_ = tmp;
        } else {
            state_->vec.resize(size);
        }
    }

    const std::string& Get(size_t at) {
        return state_->vec[at];
    }
    const std::string& Back() {
        return state_->vec[state_->vec.size() - 1];
    }

    void PushBack(const std::string& value) {
        if (state_->ref_count > 1) {
            state_->ref_count--;
            State* tmp = new State();
            tmp->vec.resize(state_->vec.size() + 1);
            for (size_t i = 0; i < state_->vec.size(); ++i) {
                tmp->vec[i] = state_->vec[i];
            }
            tmp->vec[state_->vec.size()] = value;
            state_ = tmp;
        } else {
            state_->vec.push_back(value);
        }
    }

    void Set(size_t at, const std::string& value) {
        if (state_->ref_count > 1) {
            state_->ref_count--;
            State* tmp = new State();
            tmp->vec.resize(state_->vec.size());
            for (size_t i = 0; i < state_->vec.size(); ++i) {
                tmp->vec[i] = state_->vec[i];
            }
            tmp->vec[at] = value;
            state_ = tmp;
        } else {
            state_->vec[at] = value;
        }
    }

private:
    State* state_;
};