/*
 Binary Search Tree Functions
 - node definition
 - add node
 - remove node
 - find node
 - node height query
 - print tree
*/

#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define F 0
#define T 1
#define MAXHEIGHT 100
#define MAX(x,y) ((x) > (y) ? (x):(y))

typedef struct Node {
	struct Node * p_Node_left;
	struct Node * p_Node_right;
	void * p_data;
} Node;

/*
 If p_NodeRoot is null, then create tree
 @param p_Node_root: pointer to the root node of tree (or null pointer if new tree)
 @param p_data: pointer to data to be inserted
 @param p_f_comp: pointer to comparator function (returns +ve if 1st arg greater than 2nd)
 @return pointer to root node of modified tree
*/
void 
insert(Node **p_Node_root, void * p_data, 
		int (*p_f_comp)(void * p_data_1, void * p_data_2)) {
	
	if (!(*p_Node_root)) {
		(*p_Node_root) = (Node *) malloc(sizeof(Node));
		(*p_Node_root)->p_data = p_data;
		(*p_Node_root)->p_Node_left = NULL;
		(*p_Node_root)->p_Node_right = NULL;
	}
	else {
		int i_comparison = p_f_comp((*p_Node_root)->p_data, p_data);
		
		if (i_comparison < 0)
			insert(&((*p_Node_root)->p_Node_right), p_data, p_f_comp);
		else if (i_comparison > 0)
			insert(&((*p_Node_root)->p_Node_left), p_data, p_f_comp);
	}
}

int
recursive_height(Node * p_Node_root, int i_height) {
	if (!p_Node_root)
		return i_height - 1;
		
	return MAX(recursive_height(p_Node_root->p_Node_left, i_height + 1),
		recursive_height(p_Node_root->p_Node_right, i_height + 1));
}

int
get_height(Node * p_Node_root) {
	return recursive_height(p_Node_root, 0);
}

/* 
 Recursive printing of tree
 @parameter p_Node_root: root node of (sub)tree to be printed
 @parameter i_level: node's level in the tree being printed
 @parameter formatted_print: function pointer printing one data element
*/
void
print_tree(Node * p_Node_root, int i_level, void formatted_print(void *p_data)) {
	/* hot bit encoded buffer indicating if we are in a level's right branch */
	static unsigned char *p_c_right;
	static int i_height;
	
	/* initialize variables for print session at first call of print_tree */
	if (i_level == 0) {
		i_height = get_height(p_Node_root);
		p_c_right = (char *)malloc(i_height/(sizeof(char)*8) + 1);
	}
	
	/* print one line */
	int i;
	for (i = 0; i < i_level; i++) {
		if (i == (i_level - 1)) printf ("`---");
		else
			if (*(p_c_right + i/(sizeof(char)*8)) & (1 << (i % (sizeof(char)*8)))) 
			//if (*(p_c_right) & (1 << i))	
				printf ("|   ");
			else 
			printf ("    ");
	}
	
	if (!p_Node_root) {
		printf("\n");
		return;
	}
	else {
		formatted_print(p_Node_root->p_data);
		printf("\n");
	}
	
	/* make recursive calls to print_tree on children if at least one child */
	if (p_Node_root->p_Node_right || p_Node_root->p_Node_left) {
		*(p_c_right + i/(sizeof(char)*8)) |= (1 << (i_level % (sizeof(char)*8)));
			print_tree(p_Node_root->p_Node_right, i_level + 1, formatted_print);
		*(p_c_right + i/(sizeof(char)*8)) &= ~(1 << (i_level % (sizeof(char)*8)));		
		
			print_tree(p_Node_root->p_Node_left, i_level + 1, formatted_print);
	}
	
	/* free memory if end of tree */
	if (i_level == i_height - 1 && !(*p_c_right)) {
		free(p_c_right);
		p_c_right = NULL;
	}
}


/* Code below for integer BST tree */

void
int_print(void *p_data) {
	printf("(%d)",*(int *)p_data);
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
		insert(&p_Node_root, (void *) &i_a_data[size], &int_comparator);
	
	print_tree(p_Node_root, 0, &int_print);

	return 0;
}
