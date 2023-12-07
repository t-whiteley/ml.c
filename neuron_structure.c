#include <stdio.h>
#include <stdlib.h>


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


TreeNode* output_node() {
    TreeNode* n = (TreeNode*)malloc(sizeof(TreeNode));
    if (n == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    n->data = 0.;
    n->weight = 0.;
    n->bias = 0.;
    n->parents_nodes = (TreeNode**) malloc(1 * sizeof(TreeNode*));
    n->children_nodes = (TreeNode**) malloc(1 * sizeof(TreeNode*));
    n->num_children = 0;
    n->num_parents = 0;
    n->id = 1;
    return n;
}


TreeNode* input_node(TreeNode** parents, int num_parents) {
    TreeNode* n = (TreeNode*)malloc(sizeof(TreeNode));
    if (n == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < num_parents; i++) {
        parents[i]->children_nodes = (TreeNode**) realloc(parents[i]->children_nodes, parents[i]->num_children * sizeof(TreeNode*));
        parents[i]->children_nodes[parents[i]->num_children] = n;
        parents[i]->num_children++;
    }
    n->data = 0.;
    n->weight = 0.;
    n->bias = 0.;
    n->parents_nodes = parents;
    n->num_parents = num_parents;
    n->children_nodes = NULL;
    n->num_children = 0;
    n->id = 3;
    return n;
}


TreeNode* middle_node(TreeNode** parents, int num_parents) {
    TreeNode* n = (TreeNode*)malloc(sizeof(TreeNode));
    if (n == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < num_parents; i++) {
        parents[i]->children_nodes = (TreeNode**) realloc(parents[i]->children_nodes, parents[i]->num_children * sizeof(TreeNode*));
        parents[i]->children_nodes[parents[i]->num_children] = n;
        parents[i]->num_children++;
    }
    n->data = 0.;
    n->weight = 0.;
    n->bias = 0.;
    n->parents_nodes = parents;
    n->num_parents = num_parents;
    n->children_nodes = NULL;
    n->num_children = 0;
    n->id = 2;
    return n;
}


void free_structure(TreeNode* root) {
    if (root == NULL) {
        return;
    }
    for (int i = 0; i < root->num_children; i++) {
        free_structure(root->children_nodes[i]);
    }
    if (!root) {
        free(root->children_nodes);
        free(root->parents_nodes);
        free(root);
        root = NULL;
    }
}


void rec_print_structure(TreeNode* root, int depth) {
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    printf("%d\n", root->id);
    if (!(root->num_children)) {
        return;
    }
    for (int i = 0; i < root->num_children; i++) {
        rec_print_structure(root->children_nodes[i], depth + 1);
    }
}
void print_structure(TreeNode* root) {
    rec_print_structure(root, 0);
}





// int main() {
//     TreeNode* root = output_node();

//     TreeNode* parents_ls1[] = {root};

//     TreeNode* sub1 = middle_node(parents_ls1, 1);
//     TreeNode* sub2 = middle_node(parents_ls1, 1);


//     TreeNode* parents_ls2[] = {sub1};
//     TreeNode* sub11 = middle_node(parents_ls2, 1);


//     TreeNode* parents_ls3[] = {sub2, sub11};
//     TreeNode* parents_ls4[] = {sub2, sub11};
//     TreeNode* input1 = input_node(parents_ls3, 2);
//     TreeNode* input2 = input_node(parents_ls4, 2);

//     print_structure(root);
//     free_structure(root);

//     return 0;
// }