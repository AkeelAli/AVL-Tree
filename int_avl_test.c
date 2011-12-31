/* Code below for integer AVL tree */

#include "avl.c"
#include <stdio.h>
#include <stdlib.h>

void
int_print(void *p_key) {
	printf("(%d)",*(int *)p_key);
}

int 
int_comparator(void *p_v1, void *p_v2) {
	int v1 = * (int *)p_v1;
	int v2 = * (int *)p_v2;
	if (v1 > v2) return 1;
	if (v1 < v2) return -1;
	return 0;
}

int main(void) {
	int i_a_data[] = {1, 4, 10, 2, -3, -1, 10, 23, 200, 12, 32, 438, 538, 93, 439, -4, 3, 0, 10000, 2023023 };
	//int i_a_data[] = {9, 7, 8, 5, 4, 6, 1, 2, 3};
	Node * p_Node_root = NULL;
	
	int size = sizeof(i_a_data)/sizeof(int);
	while (--size >= 0)
		insert_avl(&p_Node_root, (void *) &i_a_data[size], &int_comparator);
	
	
	print_tree(p_Node_root, 0, &int_print);
	
	delete_avl(&p_Node_root, &i_a_data[1], &int_comparator);
	delete_avl(&p_Node_root, &i_a_data[2], &int_comparator);
	
	print_tree(p_Node_root, 0, &int_print);
	
	//printf("%d",*(int *)find(p_Node_root, &i_a_data[4], &int_comparator)->p_key);

	return 0;
}