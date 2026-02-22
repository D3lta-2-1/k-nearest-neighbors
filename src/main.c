
#include "image.h"
#include "heap.h"
#include "d-tree.h"
#include <stdio.h>
#include <time.h>

int count_label(BinaryHeap* heap, int label) {
    int acc = 0;
    for (int i = 0; i < heap->size; i++) {
        if (heap->elements[i].value->label == label) {
            acc++;
        }
    }
    return acc;
}

int most_present_value(BinaryHeap* heap) {
    int best = 0;
    int best_count = 0;
    for (int i = 0; i < 10; i++) {
        int count = count_label(heap, i);
        if (count > best_count) {
            best = i;
            best_count = count;
        }
    }
    return best;
}

int main() {
    int nb_training;

    Image* images_training = read_images("images_entr", "etiquettes_entr", &nb_training);
    if (!images_training) {
        printf("Erreur de lecture MNIST.\n");
        return 1;
    }

    int nb_test;

    Image* images_test = read_images("images_test", "etiquettes_test", &nb_test);
    if (!images_test) {
        printf("Erreur de lecture MNIST.\n");
        return 1;
    }

    printf("%i images tests trouvees\n", nb_test);
    printf("la premiere entree est: %i\n", images_training[0].label);
    print_image_threshold(&images_training[0], 127);

    int result[10][10];
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            result[i][j] = 0;
        }
    }


    clock_t building_start = clock();
    DTRee tree = new_tree(images_training, nb_training);
    float building_time = (float)(clock() - building_start) / CLOCKS_PER_SEC;
    printf("building tree took %f s\n", building_time);
    
 
    clock_t start = clock();
    BinaryHeap heap = new_binary_heap(5);
    for (int i = 0; i < nb_test; i++) {
        dtree_find_closest(&tree, &heap, &images_test[i]);
        int recognized = most_present_value(&heap);
        result[images_test[i].label][recognized]++;
    }

    float second = (float)(clock() - start) / CLOCKS_PER_SEC;
    printf("image recognized in %f s\n", second);

    for (int j = 0; j < 10; j++) {
        for (int i = 0; i < 10; i++) {
            int v = result[i][j];
            if (v < 10) printf(" ");
            if (v < 100) printf(" ");
            printf("%i ", v);
        }
        printf("\n");
    }

    return 0;
}
