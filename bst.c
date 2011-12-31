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

#include "bst.h"

/*
 Recursive function to find a node holding given data
 @parameter p_Node_root: root at which to start search
 @parameter p_key: pointer to data looked for
 @parameter comp: function pointer that performs comparison
 @return Node pointer that has the given data, or null if not found
*/
Node *
find(Node *p_Node_root, void * p_key, int (*comp)(void *, void *)) {
	if(p_Node_root) {
		int i_comparison = comp(p_Node_root->p_key, p_key);
		if (i_comparison > 0)
			return find(p_Node_root->p_Node_left, p_key, comp);
		if (i_comparison < 0)
			return find(p_Node_root->p_Node_right, p_key, comp);
	}
	
	return p_Node_root;	
}

/*
 Returns a void pointer to minimum data in tree
*/
void *
find_min(Node *p_Node_root, int (*comp)(void *, void *)) {
	if (!p_Node_root)
		return NULL;
	if (p_Node_root->p_Node_left)
		return find_min(p_Node_root->p_Node_left, comp);
	else
		return p_Node_root->p_key;
}

/*
 Returns a void pointer to maximum data in tree
*/
void *
find_max(Node *p_Node_root, int (*comp)(void *, void *)) {
	if (!p_Node_root)
		return NULL;
	if (p_Node_root->p_Node_right)
		return find_max(p_Node_root->p_Node_right, comp);
	else
		return p_Node_root->p_key;
}

/*
 If p_NodeRoot is null, then create tree
 @param pp_Node_root: double pointer to the root node of tree (or null pointer if new tree)
 @param p_key: pointer to data to be inserted
 @param p_f_comp: pointer to comparator function (returns +ve if 1st arg greater than 2nd)
 @return pointer to root node of modified tree
*/
void 
insert(Node **pp_Node_root, void * p_key, int (*p_f_comp)(void *, void *)) {
	
	if (!(*pp_Node_root)) {
		(*pp_Node_root) = (Node *) malloc(sizeof(Node));
		(*pp_Node_root)->p_key = p_key;
		(*pp_Node_root)->p_Node_left = NULL;
		(*pp_Node_root)->p_Node_right = NULL;
	}
	else {
		int i_comparison = p_f_comp((*pp_Node_root)->p_key, p_key);
		
		if (i_comparison < 0)
			insert(&((*pp_Node_root)->p_Node_right), p_key, p_f_comp);
		else if (i_comparison > 0)
			insert(&((*pp_Node_root)->p_Node_left), p_key, p_f_comp);
	}
}

/*
 Deletes the node containing given data in a tree
 @parameter pp_Node_root: double pointer to root of tree
 @parameter p_key: pointer to data to be deleted from tree
 @parameter p_f_comp: function pointer comparing tree data elements
*/
void
delete(Node **pp_Node_root, void * p_key, int (*p_f_comp)(void *, void *)) {
	Node * p_Node_delete = *pp_Node_root;
	Node * p_Node_parent = NULL;	/* root node considered to have null parent */ 
	enum {
		right,
		left,
	} direction;
	
	int i_comparison;
	int found = F;
	while(p_Node_delete && !found) {
		i_comparison = p_f_comp(p_Node_delete->p_key, p_key);
		if (i_comparison == 0)
			found = T;
		else {
			p_Node_parent = p_Node_delete;
			if (i_comparison > 0) {
				p_Node_delete = p_Node_delete->p_Node_left;
				direction = left;
			}
			else {
				p_Node_delete = p_Node_delete->p_Node_right;
				direction = right;
			}
		}
	}
	
	if (found) {
		if (!p_Node_parent) {
			/* delete the only node (root node) */
			*pp_Node_root = NULL;
		}
		else if (p_Node_delete->p_Node_left && p_Node_delete->p_Node_right) {
			/* node has 2 children */
			p_Node_delete->p_key = find_min(p_Node_delete->p_Node_right, p_f_comp);
			delete(&(p_Node_delete->p_Node_right), p_Node_delete->p_key, p_f_comp);
		}
		else if (p_Node_delete->p_Node_left || p_Node_delete->p_Node_right) {
			/* node has 1 child */
			if (direction == right)
				p_Node_parent->p_Node_right = p_Node_delete->p_Node_right ? 
					p_Node_delete->p_Node_right: p_Node_delete->p_Node_left;
			else
				p_Node_parent->p_Node_left = p_Node_delete->p_Node_right ? 
					p_Node_delete->p_Node_right: p_Node_delete->p_Node_left;			
		}
		else {
			/* node has no children */
			if (direction == right)
				p_Node_parent->p_Node_right = NULL;
			else
				p_Node_parent->p_Node_left = NULL;
		}
	}
}

static int
recursive_height(Node * p_Node_root, int i_height) {
	if (!p_Node_root)
		return i_height - 1;
		
	return MAX(recursive_height(p_Node_root->p_Node_left, i_height + 1),
		recursive_height(p_Node_root->p_Node_right, i_height + 1));
}

/*
 Returns the height of the passed node in its tree
 (-1 if null tree, and 0 if only 1 node)
*/
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
print_tree(Node * p_Node_root, int i_level, void formatted_print(void *p_key)) {
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
		formatted_print(p_Node_root->p_key);
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
