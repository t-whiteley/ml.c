#ifndef NEURON_STRUCTURE_H
#define NEURON_STRUCTURE_H

typedef struct TreeNode {
    double data;
    double weight;
    double bias;
    struct TreeNode** parents_nodes;
    struct TreeNode** children_nodes;
    int num_parents;
    int num_children;
    int id;
} TreeNode;


TreeNode* output_node();
TreeNode* input_node(TreeNode** parents, int num_parents);
TreeNode* middle_node(TreeNode** parents, int num_parents);
void free_structure(TreeNode* root);
void print_structure(TreeNode* root);

#endif