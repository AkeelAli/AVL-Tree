/* Code below for integer BST tree */

#include "bst.h"
#include <stdio.h>
#include <stdlib.h>

void
int_print(Node *p_Node) {
	printf("(%d:%d)",*(int *)(p_Node->p_key),*(int *)(p_Node->p_data));
}

int 
int_comparator(void *p_k1, void *p_k2) {
	int k1 = * (int *)p_k1;
	int k2 = * (int *)p_k2;
	if (k1 > k2) return 1;
	if (k1 < k2) return -1;
	return 0;
}

int main(void) {
	int i_a_data[][2] = { {9, 3}, {7, 2}, {8, 9}, {5, 2}, {4, 10}, {6, 23}, {1, 39}, {2, -3}, {-3, 23}};
	Node * p_Node_root = NULL;
	
	int size = sizeof(i_a_data)/sizeof(i_a_data[0]);
	while (--size >= 0)
		insert(&p_Node_root, (void *) &i_a_data[size][0], (void *) &i_a_data[size][1], &int_comparator);
	
	
	print_tree(p_Node_root, 0, &int_print);
	
	delete(&p_Node_root, &i_a_data[4][0], &int_comparator);
	
	print_tree(p_Node_root, 0, &int_print);
	
	//printf("%d",*(int *)find(p_Node_root, &i_a_data[4], &int_comparator)->p_key);

	return 0;
}