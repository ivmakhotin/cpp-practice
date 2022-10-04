#include <iostream>
#include <cstdlib>
#include <string>

const int UNDEFINED = -1;

struct Element {
    int value;
    size_t array_idx;
};

int cnt = 0;
bool misstake = false;

class BinaryHeap {
private:
    Element* data;
    int* heap_idx_by_arridx;
    int heap_size;
    int array_size;
    bool is_min_heap;

    int get_left_child_index(int node_index) {
        return 2 * node_index + 1;
    }
 
    int get_right_child_index(int node_index) {
        return 2 * node_index + 2;
    }
 
    int get_parent_index(int node_index) {
        return (node_index - 1) / 2;
    }

    void sift_up(int node_index) {
        int parent_index;
        Element temporary;
        if (node_index != 0) {
            parent_index = get_parent_index(node_index);
            if ((is_min_heap && data[parent_index].value > data[node_index].value) ||
                (!is_min_heap && data[parent_index].value < data[node_index].value)
                ) {
                temporary = data[parent_index];
                data[parent_index] = data[node_index];
                data[node_index] = temporary;
                heap_idx_by_arridx[data[node_index].array_idx] = node_index;
                heap_idx_by_arridx[data[parent_index].array_idx] = parent_index;
                sift_up(parent_index);
            }
        }

        if (is_min_heap && misstake) {
            for (int i = 0; i < heap_size; i++) std::cout << data[i].value << " ";
            std::cout << std::endl; 
        }
    }

    void sift_down(int node_index) {
        int left_child_index, right_child_index, m_index;
        Element temporary;
        left_child_index = get_left_child_index(node_index);
        right_child_index = get_right_child_index(node_index);
        if (right_child_index >= heap_size) {
            if (left_child_index >= heap_size)
                return;
            else
                m_index = left_child_index;
        } else {
            if ((is_min_heap && data[left_child_index].value <= data[right_child_index].value) ||
                (!is_min_heap && data[left_child_index].value > data[right_child_index].value)
                )
                m_index = left_child_index;
            else
                m_index = right_child_index;
        }
        if ((is_min_heap && data[node_index].value > data[m_index].value) ||
            (!is_min_heap && data[node_index].value =< data[m_index].value)
            ) {
            temporary = data[m_index];
            data[m_index] = data[node_index];
            data[node_index] = temporary;
            heap_idx_by_arridx[data[node_index].array_idx] = node_index;
            heap_idx_by_arridx[data[m_index].array_idx] = m_index;
            sift_down(m_index);
        }
    }
 
public:
    BinaryHeap(int size, bool is_min) {
        data = new Element[size];
        heap_idx_by_arridx = new int[100000];
        for (size_t i = 0; i < 100000; ++i) {
            heap_idx_by_arridx[i] = UNDEFINED;
        }
        heap_size = 0;
        array_size = size;
        is_min_heap = is_min;
    }    
    
    int get_heap_size() {
        return heap_size;
    }

    Element get_m() {
        if (is_empty())
            throw std::string("Heap is empty");
        else
            return data[0];
    }
 
    bool is_empty() {
        return (heap_size == 0);
    }
     
    void insert(Element element) {
        if (heap_size == array_size)
            throw std::string("Heap's underlying storage is overflow");
        else {
            heap_size++;
            data[heap_size - 1] = element;
            heap_idx_by_arridx[element.array_idx] = heap_size - 1; 
            sift_up(heap_size - 1);
        }
    }

    bool is_in(int array_idx) {
        return heap_idx_by_arridx[array_idx] >= 0;
    }

    void remove(int array_idx) {
        if (is_empty())
            throw std::string("Heap is empty");
        else {
            int heap_idx = heap_idx_by_arridx[array_idx];
            heap_idx_by_arridx[array_idx] = UNDEFINED;
            data[heap_idx] = data[heap_size - 1];
            heap_size--;
            if (heap_size > 0) {
                heap_idx_by_arridx[data[heap_idx].array_idx] = heap_idx;
                sift_down(heap_idx);
            }
        }
    }
     
    void remove_m() {
        if (is_empty())
            throw std::string("Heap is empty");
        else {
            heap_idx_by_arridx[data[0].array_idx] = UNDEFINED;
            data[0] = data[heap_size - 1];
            heap_size--;
            if (heap_size > 0) {
                heap_idx_by_arridx[0] = 0;
                sift_down(0);
            }
        }
    }
 
    ~BinaryHeap() {
        delete[] data;
        delete[] heap_idx_by_arridx;
    }
};


int main() {
    int array_length, requests_number, k_statistic;
    char operation;
    std::cin >> array_length >> requests_number >> k_statistic;

    int* array = new int[array_length];
    for (size_t i = 0; i < array_length; ++i) {
        std::cin >> array[i];
    }

    BinaryHeap min_heap = BinaryHeap(array_length - (k_statistic - 1), true);
    BinaryHeap max_heap = BinaryHeap(k_statistic - 1, false);

    int left = 0, right = 0;
    Element right_element;
    right_element.value = array[right];
    right_element.array_idx = right;
    if (k_statistic > 1) 
        max_heap.insert(right_element);
    else
        min_heap.insert(right_element);

    for (size_t current_request = 0; current_request < requests_number; ++current_request) {
        cnt++;
        if (cnt == 391) misstake = true;
        std::cin >> operation;
        if (operation == 'L') {
            if (max_heap.is_in(left)) {
                max_heap.remove(left);
                if (!min_heap.is_empty()) {
                    max_heap.insert(min_heap.get_m());
                    min_heap.remove(min_heap.get_m().array_idx);

                    if (!min_heap.is_empty())
                        std::cout << min_heap.get_m().value << std::endl;
                    else
                        std::cout << "-1" << std::endl;
                }
                else
                    std::cout << "-1" << std::endl;
            }
            else {
                min_heap.remove(left);
                if (!min_heap.is_empty())
                        std::cout << min_heap.get_m().value << std::endl;
                else
                    std::cout << "-1" << std::endl;           
            }
            left++;
        }
        else if (operation == 'R') {
            right++;
            Element right_element;
            right_element.value = array[right];
            right_element.array_idx = right;
            if (k_statistic == 1) {
                min_heap.insert(right_element);
                std::cout << min_heap.get_m().value << std::endl;
                continue;
            }

            if (!min_heap.is_empty()) {
                if (right_element.value <= max_heap.get_m().value) {
                    min_heap.insert(max_heap.get_m());
                    max_heap.remove(max_heap.get_m().array_idx);
                    max_heap.insert(right_element);
                    std::cout << min_heap.get_m().value << std::endl;
                }
                else {
                    min_heap.insert(right_element);
                    std::cout << min_heap.get_m().value << std::endl;   
                }
            }
            else if (max_heap.get_heap_size() == k_statistic - 1) {
                if (right_element.value > max_heap.get_m().value) {
                    min_heap.insert(right_element);
                    std::cout << min_heap.get_m().value << std::endl;   
                }
                else {
                    min_heap.insert(max_heap.get_m());
                    max_heap.remove(max_heap.get_m().array_idx);
                    max_heap.insert(right_element);
                    std::cout << min_heap.get_m().value << std::endl;
                }
            }
            else {
                max_heap.insert(right_element);
                std::cout << "-1" << std::endl;
            }

        }
        else {
            throw std::string("Wrong operation!");
        }

        if (misstake) break;
    }

    /*while(!min_heap.is_empty()) {
        std::cout << min_heap.get_m().value << " ";
        min_heap.remove(min_heap.get_m().array_idx);
    }

    std::cout << std::endl;

    while(!max_heap.is_empty()) {
        std::cout << max_heap.get_m().value << " ";
        max_heap.remove(max_heap.get_m().array_idx);
    }

    std::cout << std::endl;

    std::cout << left << " " << right << std::endl;*/
    
    delete[] array; 
    return 0;
}
