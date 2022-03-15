#pragma once

#include <iostream>

template <typename T>
class List {
public:
    struct BaseNode {
        BaseNode() {
            next_ = prev_ = this;
        }

        BaseNode(BaseNode* prev, BaseNode* next) : prev_(prev), next_(next) {
        }

        virtual ~BaseNode() {
        }

        BaseNode* prev_;
        BaseNode* next_;
    };

    struct ListNode : BaseNode {

        ListNode(const T& value, BaseNode* prev = nullptr, BaseNode* next = nullptr)
            : BaseNode(prev, next), value_(value) {
        }
        ListNode(T&& value, BaseNode* prev = nullptr, BaseNode* next = nullptr)
            : BaseNode(prev, next), value_(std::move(value)) {
        }

        ~ListNode() {
        }

        T value_;
    };

    size_t size_;
    BaseNode* f_el;

    class Iterator {
    public:
        Iterator(BaseNode* node) : iter_node_(node) {
        }
        Iterator(const Iterator& other) : iter_node_(other.iter_node_) {
        }
        Iterator(Iterator&& other) : iter_node_(std::move(other.iter_node_)) {
        }

        Iterator& operator++() {
            iter_node_ = iter_node_->next_;
            return *this;
        }
        Iterator operator++(int) {
            Iterator old(*this);
            iter_node_ = iter_node_->next_;
            return old;
        }

        Iterator& operator--() {
            iter_node_ = iter_node_->prev_;
            return *this;
        }
        Iterator operator--(int) {
            Iterator old(*this);
            iter_node_ = iter_node_->prev_;
            return old;
        }

        T& operator*() const {
            return (*static_cast<ListNode*>(iter_node_)).value_;
        }
        T* operator->() const {
            return &((*static_cast<ListNode*>(iter_node_)).value_);
        }

        bool operator==(const Iterator& rhs) const {
            return iter_node_ == rhs.iter_node_;
        }
        bool operator!=(const Iterator& rhs) const {
            return iter_node_ != rhs.iter_node_;
        }

    private:
        BaseNode* iter_node_;
    };

    List() : size_(), f_el(new BaseNode()) {
    }
    List(const List& other) : size_(), f_el(new BaseNode()) {
        auto left = f_el;
        auto right = other.f_el;
        while (right->next_ != other.f_el) {
            right = right->next_;
            LinkAfter(left, new ListNode(static_cast<ListNode*>(right)->value_));
            left = left->next_;
        }
    }

    List(List&& other) : size_(other.size_), f_el(other.f_el) {
        other.f_el = new BaseNode();
        other.size_ = 0;
    }

    ~List() {
        auto elem = f_el->next_;
        while (elem != elem->next_) {
            elem = elem->next_;
            Unlink(elem->prev_);
        }
        delete f_el;
    }

    List& operator=(const List& rhs) {
        auto elem = f_el;
        while (elem->next_ != elem) {
            elem = elem->next_;
            Unlink(elem);
        }
        auto left = f_el;
        auto right = rhs.f_el;
        while (right->next_ != rhs.f_el) {
            right = right->next_;
            LinkAfter(left, new ListNode(static_cast<ListNode*>(right)->value_));
            left = left->next_;
        }
        return *this;
    }

    List& operator=(List&& rhs) {
        if (this == &rhs) {
            return *this;
        }
        this->~List();
        new (this) List(std::move(rhs));
        return *this;
    }

    void Unlink(BaseNode* node) {
        if (node != f_el) {
            node->prev_->next_ = node->next_;
            node->next_->prev_ = node->prev_;
            size_--;
            delete node;
        }
    }

    void LinkAfter(BaseNode* target, BaseNode* after) {
        after->next_ = target->next_;
        after->prev_ = target;
        target->next_ = after;
        size_++;
        after->next_->prev_ = after;
    }

    bool IsEmpty() const {
        return size_ == 0;
    }

    size_t Size() const {
        return size_;
    }

    void PushBack(const T& elem) {
        LinkAfter(f_el->prev_, elem);
    }
    void PushBack(T&& elem) {
        BaseNode* new_elem = new ListNode(std::move(elem));
        LinkAfter(f_el->prev_, new_elem);
    }
    void PushFront(const T& elem) {
        LinkAfter(f_el, elem);
    }
    void PushFront(T&& elem) {
        BaseNode* new_elem = new ListNode(std::move(elem));
        LinkAfter(f_el, new_elem);
    }

    T& Front() {
        return static_cast<ListNode*>(f_el->next_)->value_;
    }
    const T& Front() const {
        return static_cast<ListNode*>(f_el->next_)->value_;
    }
    T& Back() {
        return static_cast<ListNode*>(f_el->prev_)->value_;
    }
    const T& Back() const {
        return static_cast<ListNode*>(f_el->prev_)->value_;
    }

    void PopBack() {
        Unlink(f_el->prev_);
    }
    void PopFront() {
        Unlink(f_el->next_);
    }

    Iterator Begin() {
        return Iterator(f_el->next_);
    }
    Iterator End() {
        return Iterator(f_el);
    }

private:
};

template <typename T>
typename List<T>::Iterator begin(List<T>& list) {  // NOLINT
    return list.Begin();
}

template <typename T>
typename List<T>::Iterator end(List<T>& list) {  // NOLINT
    return list.End();
}
