#pragma once

#include <list>
#include <string>
#include <unordered_map>

class LruCache {
public:
    LruCache(size_t max_size) : max_size_(max_size), current_size_(0) {
    }

    void Set(const std::string& key, const std::string& value) {
        if (cache_map_.find(key) != cache_map_.end()) {
            auto current_it = cache_map_[key];
            auto key_value = *(current_it);
            key_value.second = value;
            cache_list_.erase(current_it);
            cache_list_.push_back(key_value);
            cache_map_[key] = std::prev(cache_list_.end());
        } else {
            if (current_size_ < max_size_) {
                cache_list_.emplace_back(key, value);
                cache_map_[key] = std::prev(cache_list_.end());
                current_size_++;
            } else {
                auto oldest_key = cache_list_.front().first;
                cache_list_.pop_front();
                cache_map_.erase(oldest_key);
                cache_list_.emplace_back(key, value);
                cache_map_[key] = std::prev(cache_list_.end());
            }
        }
    }

    bool Get(const std::string& key, std::string* value) {
        if (cache_map_.find(key) != cache_map_.end()) {
            auto current_it = cache_map_[key];
            auto key_value = *(current_it);
            cache_list_.erase(current_it);
            cache_list_.push_back(key_value);
            cache_map_[key] = std::prev(cache_list_.end());
            *value = key_value.second;
            return true;
        }
        return false;
    }

private:
    std::list<std::pair<std::string, std::string>> cache_list_;
    std::unordered_map<std::string, std::list<std::pair<std::string, std::string>>::iterator>
        cache_map_;
    size_t max_size_, current_size_;
};
