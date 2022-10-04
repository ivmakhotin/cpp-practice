#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

const float EPS = 10e-7;
const int START = 1;
const int FINISH = 0;
const int NOT_DEFINED = -1;

struct Point {
    float coord_x;
    float coord_y;
};

struct Segment {
    int entry;
    float bound;
};

void add_segment(struct Point* points, struct Segment* segments, float radius, size_t idx) {
    float hypotinuse = sqrt(radius * radius + EPS - points[idx].coord_y * points[idx].coord_y);
    segments[2 * idx].bound = points[idx].coord_x - hypotinuse;
    segments[2 * idx + 1].bound = points[idx].coord_x + hypotinuse;

    segments[2 * idx].entry = 1;
    segments[2 * idx + 1].entry = 0;
}

void make_segment(struct Point* points, struct Segment* segments,
                   float radius, size_t number_of_points) {
    for (size_t i = 0; i < number_of_points; ++i) {
        if (radius * radius + EPS >= points[i].coord_y * points[i].coord_y) {
            add_segment(points, segments, radius, i);
        }
        else {
            segments[2 * i].entry = -1;
            segments[2 * i + 1].entry = -1;
        }
    }
}

void sort_segment(struct Segment* segments, size_t first, size_t last) {
    size_t left = first;
    size_t right = last; 
    float pivot = segments[left + (right - left) / 2].bound;

    while (left <= right) {
        while (segments[left].bound < pivot) left++;
        while (segments[right].bound > pivot && right > 0) right--;
        if (left <= right) {
            struct Segment temporary;
            temporary = segments[left];
            segments[left] = segments[right];
            segments[right] = temporary;
            left++;
            if (right > 0) right--;
        }
    }
    if (left < last)
        sort_segment(segments, left, last);
    if (first < right)
        sort_segment(segments, first, right);
}

bool predicat(size_t number_of_points_to_cover, struct Segment* segments, size_t len) {
    int max_enters = 0, cur_enters = 0;

    for (size_t i = 0; i < len; ++i) {
        if (segments[i].entry == START) {
                cur_enters++;
                if (max_enters < cur_enters) max_enters = cur_enters;
        }
        else 
        if (segments[i].entry == FINISH) cur_enters--;
    }
    return number_of_points_to_cover <= max_enters;
}

float rad_bin_search(struct Point* points, struct Segment* segments,
                     size_t number_of_points, size_t number_of_points_to_cover) {
    float mid, left = 0, right = 10000;
    for (size_t i = 0; i < 32; ++i) {
        mid = (left + right) / 2.0f;
        make_segment (points, segments, mid, number_of_points);
        sort_segment (segments, 0, 2 * number_of_points - 1);
        if (predicat (number_of_points_to_cover, segments, 2 * number_of_points)) right = mid;
        else  left = mid;
    }
    return mid;
}

int main(void) {
    int number_of_points, number_of_points_to_cover;
    struct Point* points = malloc(number_of_points * (sizeof(struct Point)));
    if (points == NULL) {
        fprintf(stderr, "failed to allocate memory\n");
        exit(1);
    }

    struct Segment* segments = malloc(2 * number_of_points * (sizeof(struct Segment)));
    
    if (segments == NULL) {
        free(points);
        fprintf(stderr, "failed to allocate memory\n");
        exit(1);
    }

    scanf("%d%d", &number_of_points, &number_of_points_to_cover);
    for (size_t i = 0; i < number_of_points; ++i) {
        scanf("%f%f", &points[i].coord_x, &points[i].coord_y);
    }
    printf("%.6f", rad_bin_search(points, segments, number_of_points, number_of_points_to_cover));

    free(points);
    free(segments);
    return 0;
}
