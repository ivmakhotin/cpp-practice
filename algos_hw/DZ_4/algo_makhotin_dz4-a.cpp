#include <cstdlib>
#include <limits.h>
#include <iostream>

struct Node
{
    int data;
    struct Node *left;
    struct Node *right;
};

struct Node* make_node(int data)
{
    struct Node* temporary = new Node;

    temporary->data = data;
    temporary->left = temporary->right = NULL;

    return temporary;
}

struct Node* construct_tree_util(int pre[], size_t* pre_index, int key,
                                 int min, int max, size_t size)
{
    if (*pre_index >= size)
        return NULL;

    struct Node* root = NULL;

    if (key >= min && key < max) {
        root = make_node (key);
        *pre_index = *pre_index + 1;
        
        if (*pre_index < size) {
            root->left = construct_tree_util(pre, pre_index, pre[*pre_index],
                                             min, key, size);
            root->right = construct_tree_util(pre, pre_index, pre[*pre_index],
                                              key, max, size);
        }
    }

    return root;
}

struct Node *construct_tree(int pre[], size_t size) {
    size_t pre_index = 0;
    return construct_tree_util(pre, &pre_index, pre[0], INT_MIN, INT_MAX, size);
}

void print_inorder(struct Node* node) {
    if (node == NULL)
        return;

    print_inorder(node->left);
    std::cout << node->data << " ";
    print_inorder(node->right);
}

void print_postorder(struct Node* node) {
    if (node == NULL)
        return;

    print_postorder(node->left);
    print_postorder(node->right);
    std::cout << node->data << " ";
}

void destroy_tree(Node* node) {
    if (node != NULL) {
      destroy_tree(node->left);
      destroy_tree(node->right);
      delete node;
    }
}

int main() {
    size_t size;
    std::cin >> size; 
    int* pre = new int[size];

    for (size_t i = 0; i < size; ++i) {
        std::cin >> pre[i];
    }

    struct Node *root = construct_tree(pre, size);

    print_postorder(root);
    std::cout << std::endl;

    print_inorder(root);
    std::cout << std::endl;

    destroy_tree(root);
    delete[]  pre;
    return 0;
}
