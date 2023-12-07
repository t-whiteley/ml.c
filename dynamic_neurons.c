#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "neuron_structure.h"




typedef struct {
    TreeNode* struct_start;
    TreeNode** inputs;
    int inputs_len;
    TreeNode** layer1;
    int layer1_len;
    double eps;
    double rate;
} Model;



double rand_implementation(int nm) {
    double rd = ((double)rand() / RAND_MAX);
    return (double) nm * rd;
}



void rec_find_struct(TreeNode* root, TreeNode* prev_root, TreeNode*** arri, TreeNode*** arrl1, int* len) {
    if (!(root->num_children)) {
        *arri = realloc(*arri, *len * sizeof(TreeNode*));
        (*arri)[*len-1] = root;
        
        *arrl1 = realloc(*arrl1, *len * sizeof(TreeNode*));
        (*arrl1)[*len-1] = prev_root;
        
        (*len)++;
    } else {
        for (int i = 0; i < root->num_children; i++) {
            rec_find_struct(root->children_nodes[i], root, arri, arrl1, len);
        }
    }
}

void rec_find_layer1(TreeNode* root, TreeNode*** arri, int* len_arri) {
    
}

struct Model* init_model(TreeNode* neuron_config, double eps, double rate) {
    int len = 0;
    TreeNode** inputs_arr = (TreeNode**) malloc(sizeof(TreeNode*));
    TreeNode** layer1_arr = (TreeNode**) malloc(sizeof(TreeNode*));
    rec_find_struct(neuron_config, NULL, &inputs_arr, &layer1_arr, &len);
    len -= 1;


    printf("--------------\n");
    printf("%d\n", len);
    for (int j = 0; j < len; j++) {
        inputs_arr[j]->data = rand_implementation(10);
        inputs_arr[j]->weight = rand_implementation(10);
        inputs_arr[j]->bias = rand_implementation(10);
        printf("%f, ", inputs_arr[j]->data);
    }

    Model* m;
    m->struct_start = neuron_config;
    m->inputs = inputs_arr;
    m->inputs_len = len;
    m->layer1 = layer1_arr;
    m->layer1_len = len;
    m->eps = eps;
    m->rate = rate;
    return m;
}


double calculate_out(Model* m, double* data, int data_len) {
    if (data_len == m->inputs_len) {
        for (int i = 0; i < data_len; i++) {
            m->inputs[i]->data = data[i];
        }
    }
}


double cost(Model* m, double** data, int data_cols, int data_rows) {
    double result = 0;
    for (int i = 0; i < data_rows; i++) {
        double out = calculate_out(m, data[i], data_rows-1);
        double diff = out - data[i][data_rows-1];
        result += diff * diff;
    }
    return result /= data_rows;
}








int main() {
    // makes different rand values each time
    srand(time(NULL));

    // define training data
    int cols = 3;
    int rows = 4;
    int data[][3] = {
        {0, 0, 0},
        {0, 1, 1},
        {1, 0, 1},
        {1, 1, 0}};

    // define the structure of neural network
    TreeNode* out = output_node();
    TreeNode** pl1 = out;
        TreeNode* sub1 = middle_node(pl1, 1);
        TreeNode* sub2 = middle_node(pl1, 1);
        
        TreeNode* pl2[] = {&sub1, &sub2};
            TreeNode* in1 = input_node(pl2, 2);
            TreeNode* in2 = input_node(pl2, 2);
    
    print_structure(out);
    printf("---------\n");




    // always free structure by the root -> output
    free_structure(out);
    return 0;
}