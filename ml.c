#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "neuron_structure.h"




typedef struct Model {
    TreeNode* struct_start;
    TreeNode** inputs;
    int inputs_len;
    TreeNode** middles;
    int middles_len;
    double eps;
    double rate;
} Model;



double rand_implementation(int nm) {
    double rd = ((double)rand() / RAND_MAX);
    return nm * rd;
}


int remove_dupe_nodes(TreeNode** arr, int len_arr) {
    int result = len_arr;
    for (int i = 0; i < len_arr; i++) {
        if (i < len_arr-1) {
            for (int j = i+1; j < len_arr; j++) {
                if (arr[i] == arr[j]) {
                    for (int k = j; k < len_arr-1; k++) {
                        arr[k] = arr[k+1];
                    }
                    result--;
                    j--;
                }
            }
        }
    }
    return result;
}


void rec_find_struct(TreeNode* root, TreeNode*** arri, TreeNode*** arrm, int* leni, int* lenm) {
    root->data = rand_implementation(10);
    root->weight = rand_implementation(10);
    root->bias = rand_implementation(10);

    if (root->id == 2) {
        (*lenm)++;
        *arrm = realloc(*arrm, *lenm * sizeof(TreeNode*));
        (*arrm)[*lenm-1] = root;
    }

    if (!(root->num_children)) {
        (*leni)++;
        *arri = realloc(*arri, *leni * sizeof(TreeNode*));
        (*arri)[*leni-1] = root;
    } else {
        for (int i = 0; i < root->num_children; i++) {
            rec_find_struct(root->children_nodes[i], arri, arrm, leni, lenm);
        }
    }
}


struct Model* init_model(TreeNode* neuron_config, double eps, double rate) {
    int len_i = 0;
    int len_m = 0;
    TreeNode** inputs_arr = (TreeNode**) malloc(sizeof(TreeNode*));
    TreeNode** middle_arr = (TreeNode**) malloc(sizeof(TreeNode*));
    rec_find_struct(neuron_config, &inputs_arr, &middle_arr, &len_i, &len_m);

    int in_len = remove_dupe_nodes(inputs_arr, len_i - 2);
    int m_len = remove_dupe_nodes(middle_arr, len_m);

    Model* m = malloc(sizeof(Model));
    m->struct_start = neuron_config;
    m->inputs = inputs_arr;
    m->inputs_len = in_len;
    m->middles = middle_arr;
    m->middles_len = m_len;
    m->eps = eps;
    m->rate = rate;
    return m;
}


double rec_find_output(TreeNode* root) {
    if (!(root->num_children)) {
        return root->data;
    }
    double output = root->bias;
    for (int i = 0; i < root->num_children; i++) {
        double x = rec_find_output(root->children_nodes[i]);
        double w = root->children_nodes[i]->weight;
        output += x*w;
    }
    return output;
}



double calculate_out(Model* m, double* data, int data_len) {
    if (data_len == m->inputs_len) {
        // here i set the input nodes value to the inputs
        for (int i = 0; i < data_len; i++) {
            m->inputs[i]->data = data[i];
            // printf("i: %d, in: %f, value: %f\n", i, data[i], m->inputs[i]->data);
        }
        return rec_find_output(m->struct_start);
    }
    return 0;
}


double cost(Model* m, double** data, int data_cols, int data_rows) {
    double result = 0;
    for (int i = 0; i < data_rows; i++) {
        double out = calculate_out(m, data[i], data_cols-1);
        double diff = out - data[i][data_cols-1];
        // printf("Data %d, out %f\n", i, out);
        result += diff * diff;
    }
    result /= data_rows;
    // printf("cost: %f\n", result);
    return result;
}

double test_cost(Model* m, double** data, int data_cols, int data_rows) {
    printf("=====================\n");
    printf("training iter\n");
    double result = 0;
    for (int i = 0; i < data_rows; i++) {
        double out = calculate_out(m, data[i], data_cols-1);
        double diff = out - data[i][data_cols-1];
        
        printf("Data: [");
        for (int j = 0; j < data_cols - 2; j++) {
            printf("%f, ", data[i][j]);
        }
        printf("%f", data[i][data_cols - 2]);
        printf("] ");

        printf("out: %f\n", out);
        result += diff * diff;
    }
    printf("=====================\n");
    result /= data_rows;
    return result;
}

void wiggle(Model* m, TreeNode* root, double** data, int data_cols, int data_rows) {
    // bias of current node
    double c1 = cost(m, data, data_cols, data_rows);
    root->bias += m->eps;
    double c2 = cost(m, data, data_cols, data_rows);
    root->bias -= m->eps;

    double dc = (c2 - c1) / m->eps;
    root->bias -= m->rate * dc;
    
    // weights of sub nodes
    for (int k = 0; k < root->num_children; k++) {
        double k1 = cost(m, data, data_cols, data_rows);
        root->children_nodes[k]->weight += m->eps;
        double k2 = cost(m, data, data_cols, data_rows);
        root->children_nodes[k]->weight -= m->eps;

        double dk = (k2 - k1) / m->eps;
        root->children_nodes[k]->weight -= m->rate * dk;
    }
}


void train_model(Model* m, int iters, double** data, int data_cols, int data_rows) {
    for (int i = 0; i < iters; i++) {
        // wiggle the top node
        wiggle(m, m->struct_start, data, data_cols, data_rows);
        // wiggle the middle nodes
        for (int j = 0; j < m->middles_len; j++) {
            wiggle(m, m->middles[j], data, data_cols, data_rows);
        }
    }
}






int main() {
    // makes different rand values each time
    srand(time(NULL));


    // define the structure of neural network
    TreeNode* out = output_node();
    TreeNode* pl1 = out;
        TreeNode* sub1 = middle_node(&pl1, 1);
        TreeNode* sub2 = middle_node(&pl1, 1);

        TreeNode* pl2[] = {sub1, sub2};
            TreeNode* in1 = input_node(pl2, 2);
            TreeNode* in2 = input_node(pl2, 2);
    
    Model* m = init_model(out, 0.0001, 0.0001);
    print_structure(m->struct_start);



    // define training data, the last col is output
    double data[][3] = {
        {0, 0, 0},
        {0, 1, 1},
        {1, 0, 1},
        {1, 1, 0}};

    int rows = sizeof(data) / sizeof(data[0]);
    int cols = sizeof(data[0]) / sizeof(data[0][0]);



    double **dynamic_data = (double **)malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++) {
        dynamic_data[i] = (double *)malloc(cols * sizeof(double));
        for (int j = 0; j < cols; j++) {
            dynamic_data[i][j] = data[i][j];
        }
    }


    test_cost(m, dynamic_data, cols, rows);

    int training_iterations = 10000;
    train_model(m, training_iterations, dynamic_data, cols, rows);

    test_cost(m, dynamic_data, cols, rows);
    print_structure(m->struct_start);






    // free memory
    for (int i = 0; i < rows; i++) {
        free(dynamic_data[i]);
    }
    free(dynamic_data);
    free(m);
    free_structure(out);
    return 0;
}