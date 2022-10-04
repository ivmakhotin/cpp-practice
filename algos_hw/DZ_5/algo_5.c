#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
const int32_t LEFT = -1;
const int32_t ROOT = 0;
const int32_t RIGHT = 1;
const size_t ROOT_PTR = 1;
const size_t NOT_EXIST = 0;

struct Node {
    int32_t value;
    int32_t orientation;
    size_t ancestor_ptr;
    size_t l_son_ptr;
    size_t r_son_ptr;
};

struct Parking {
    int32_t value;
    size_t leaf_ptr;
};

int32_t min(int32_t num1, int32_t num2) {
    return num1 < num2 ? num1 : num2;
}

int32_t modified_min(int32_t num1, int32_t num2) {
    if (num1 < 0) {
        return (num2 < 0) ? -1 : num2;
    }
    else {
        return (num2 < 0) ? num1 : min(num1, num2);
    }
}

void update_tree(struct Node *tree, size_t vertex, int32_t new_value) {
    tree[vertex].value = new_value;
    if (tree[vertex].orientation == ROOT) return;

    while (tree[vertex].orientation != ROOT) {
        vertex = tree[vertex].ancestor_ptr;
        tree[vertex].value = modified_min(tree[tree[vertex].l_son_ptr].value,
            tree[tree[vertex].r_son_ptr].value);
    }
}

int32_t leave_parking_place(struct Node *tree, struct Parking *parking_places,
                             int32_t leaving_place) {
    if (parking_places[leaving_place - 1].value != -1) return -2;

    parking_places[leaving_place - 1].value = leaving_place;
    update_tree(tree, parking_places[leaving_place - 1].leaf_ptr,
                  leaving_place);
    return 0;
}

int32_t search_parking_place(struct Node *tree, size_t vertex) {
    if (tree[vertex].orientation == ROOT)
        return tree[vertex].value;

    if (tree[vertex].orientation == RIGHT)
        search_parking_place(tree, tree[vertex].ancestor_ptr);
    else {
        int32_t right_brother_ptr = tree[tree[vertex].ancestor_ptr].r_son_ptr;
        if (right_brother_ptr != NOT_EXIST && tree[right_brother_ptr].value != -1)
            return tree[right_brother_ptr].value;
        else
            search_parking_place(tree, tree[vertex].ancestor_ptr);
    }
}

int32_t take_parking_place(struct Node *tree, struct Parking *parking_places,
                           int32_t taking_place) {
    if (parking_places[taking_place - 1].value != -1) {
        parking_places[taking_place - 1].value = -1;
        update_tree(tree, parking_places[taking_place - 1].leaf_ptr, -1);
        return taking_place;
    }

    if (tree[ROOT_PTR].value != -1) {
        int32_t free_place = 
        search_parking_place(tree, parking_places[taking_place - 1].leaf_ptr);
        parking_places[free_place - 1].value = -1;
        update_tree(tree, parking_places[free_place - 1].leaf_ptr, -1);
        return free_place;
    }
    else return -1;
}

void build_tree(struct Node *tree, struct Parking *parking_places,
                size_t vertex, size_t tl, size_t tr, int32_t cur_orientation) {
    tree[vertex].orientation = cur_orientation;

    if (cur_orientation == LEFT) {
        tree[vertex].ancestor_ptr = vertex / 2;
    } else if (cur_orientation == RIGHT) {
        tree[vertex].ancestor_ptr = (vertex - 1) / 2;
    } else {
        tree[vertex].ancestor_ptr = NOT_EXIST;
    }

    if (tl == tr) {
        tree[vertex].value = parking_places[tl].value;
        parking_places[tl].leaf_ptr = vertex;
        tree[vertex].l_son_ptr = NOT_EXIST;
        tree[vertex].r_son_ptr = NOT_EXIST;
    }
    else {
        size_t tm = (tl + tr) / 2;
        build_tree (tree, parking_places, vertex * 2, tl, tm, LEFT);
        build_tree (tree, parking_places, vertex * 2 + 1, tm + 1, tr, RIGHT);
        tree[vertex].value = min (tree[vertex * 2].value, tree[vertex * 2 + 1].value);
        tree[vertex].orientation = cur_orientation;
        tree[vertex].l_son_ptr = vertex * 2;
        tree[vertex].r_son_ptr = vertex * 2 + 1;
    }
}

void generate_parking(struct Parking *parking_places, size_t places_number) {
    int32_t counter = 1;
    for(size_t idx = 0; idx != places_number; ++idx)
        parking_places[idx].value = counter++;
}

void problems_with_allocate() {
    fprintf(stderr, "failed to allocate memory\n");
    exit(1);
}

int main(void) {
    int32_t parking_place;
    size_t places_number, actions_number;
    char input_char;
    scanf("%zu %zu", &places_number, &actions_number);

    struct Parking* parking_places = malloc(places_number * sizeof(struct Parking));
    if (parking_places == NULL)
        problems_with_allocate();

    struct Node* tree = malloc(4 * places_number * sizeof(struct Node));
    if (tree == NULL) {
        free(parking_places);
        problems_with_allocate();
    }

    generate_parking(parking_places, places_number);
    build_tree(tree, parking_places, 1, 0, places_number - 1, ROOT);

    for (size_t idx = 0; idx != actions_number; ++idx) {
        scanf(" %c%"PRId32"", &input_char, &parking_place);
        if (input_char == '+')
            printf("%"PRId32"\n", take_parking_place(tree, parking_places,
                                                     parking_place));
        if (input_char == '-')
            printf("%"PRId32"\n", leave_parking_place(tree, parking_places,
                                                      parking_place));
    }

    free(tree);
    free(parking_places);
    return 0;
}
