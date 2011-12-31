#include "bst.h"
#include <stdio.h>
#include <stdlib.h>

#define RIGHT p_Node_right
#define LEFT p_Node_left

#ifdef DEBUG
	void
	int_print(void *p_key);
#endif 

int
get_unbalance_factor (Node *p_Node_root) {
	return get_height(p_Node_root->RIGHT) 
		- get_height(p_Node_root->LEFT);
}

int
is_unbalanced (Node *p_Node_root) {
	if (!p_Node_root)
		return F;
	
	int balance_factor = get_unbalance_factor(p_Node_root);
		
	if (balance_factor != 1 && balance_factor != -1 && balance_factor != 0)
		return T;
	
	return F;
}

static enum {
	LL = 1,
	LR = 2,
	RR = 4,
	RL = 8,
} unbalance_direction;

/*
 updates the 4 rotation pointers and determines type of unbalance (RR, RL, LR, LL)
*/
static void
update_rotate_pointers(Node *p_Node_root, Node *p_Node_parent, Node *a_p_Node_rotate[]) {
	a_p_Node_rotate[0] = p_Node_parent;
	a_p_Node_rotate[1] = p_Node_root;
	
	int unbalance_factor = get_unbalance_factor (p_Node_root);

	if (unbalance_factor > 1) {
		a_p_Node_rotate[2] = p_Node_root->RIGHT;
		unbalance_factor = get_unbalance_factor (p_Node_root->RIGHT);
		
		if (unbalance_factor > 1) {
			a_p_Node_rotate[3] = p_Node_root->RIGHT->RIGHT;
			unbalance_direction = RR;
		}
		else {
			a_p_Node_rotate[3] = p_Node_root->RIGHT->LEFT;
			unbalance_direction = RL;	
		}
	}
	else if (unbalance_factor < -1) {
		a_p_Node_rotate[2] = p_Node_root->LEFT;
		unbalance_factor = get_unbalance_factor (p_Node_root->LEFT);
		
		if (unbalance_factor > 1) {
			a_p_Node_rotate[3] = p_Node_root->LEFT->RIGHT;
			unbalance_direction = LR;
		}
		else {
			a_p_Node_rotate[3] = p_Node_root->LEFT->LEFT;
			unbalance_direction = LL;	
		}
	}
	
	
}

/* Only balances ancestors of recently added/removed element (specified in p_key)*/
static void
balance_avl_limited(Node **pp_Node_root, void *p_key, int (*p_f_comp)(void *, void *)) {
	/* rotate pointers used in the balancing algorithm */
	/* A is the unbalanced node */
	Node *a_p_Node_rotate[4] = {NULL, NULL, NULL, NULL}; /* Parent, A, B, C */
	
	/* pointers to sweep over ancestors of p_key in the tree */
	Node *p_Node_sweeper = *pp_Node_root;
	Node *p_Node_parent_sweeper = NULL;
	
	int i_comparison;
	
	/* sweep the ancestors of the p_key element and save the deepest unbalanced node */
	while (p_Node_sweeper && p_Node_sweeper->p_key != p_key) {
		#ifdef DEBUG
			//printf("\tcurrently checking %d unbalanced?\n",*(int *)p_Node_sweeper->p_key);
		#endif
		/* if the node is unbalanced, update P, A, B, C pointers */
		if (is_unbalanced(p_Node_sweeper)) 
			update_rotate_pointers(p_Node_sweeper, p_Node_parent_sweeper, a_p_Node_rotate);
		
		/* update sweeper and parent_sweeper for next node to examine */
		p_Node_parent_sweeper = p_Node_sweeper; /* update parent */
		
		i_comparison = p_f_comp(p_Node_sweeper->p_key, p_key);
		
		if (i_comparison > 0) 
			p_Node_sweeper = p_Node_sweeper->LEFT;
		else if (i_comparison < 0)
			p_Node_sweeper = p_Node_sweeper->RIGHT;
		else
			break;		
	}
	
	if (a_p_Node_rotate[1]) {
		Node *P = a_p_Node_rotate[0];
		Node *A = a_p_Node_rotate[1];
		Node *B = a_p_Node_rotate[2];
		Node *C = a_p_Node_rotate[3];
		
		#ifdef DEBUG
			printf("balancing tree\n");
			printf("\tP = %d\n", P ? *(int *)P->p_key:-20000);
			printf("\tA = %d\n", A ? *(int *)A->p_key:-20000);
			printf("\tB = %d\n", B ? *(int *)B->p_key:-20000);
			printf("\tC = %d\n", C ? *(int *)C->p_key:-20000);
		#endif 
		
		switch (unbalance_direction) {
			case RL :
				B->LEFT = C->RIGHT;
				A->RIGHT = C->LEFT;
				C->RIGHT = B;
				C->LEFT = A;
				/* update parent */
				if (!P) *pp_Node_root = C;
				else if (P->RIGHT == A) P->RIGHT = C;
				else P->LEFT = C;
				break;
			case LR :
				A->LEFT = C->RIGHT;
				B->RIGHT = C->LEFT;
				C->LEFT = B;
				C->RIGHT = A;
				/* update parent */
				if (!P) *pp_Node_root = C;
				else if (P->RIGHT == A) P->RIGHT = C;
				else P->LEFT = C;
				break;

			case RR :
				A->RIGHT = B->LEFT;
				B->LEFT = A;
				/* update parent */
				if (!P) *pp_Node_root = B;
				else if (P->RIGHT == A) P->RIGHT = B;
				else P->LEFT = B;			
				break;
			case LL :
				A->LEFT = B->RIGHT;
				B->RIGHT = A;
				/* update parent */
				if (!P) *pp_Node_root = B;
				else if (P->RIGHT == A) P->RIGHT = B;
				else P->LEFT = B;			
				break;
		}
	}
}

void
insert_avl(Node **pp_Node_root, void *p_key, int (*p_f_comp)(void *, void *)) {
	#ifdef DEBUG
		printf("inserting %d\n", *(int *)p_key);
	#endif 
	
	insert(pp_Node_root, p_key, p_f_comp);
	balance_avl_limited(pp_Node_root, p_key, p_f_comp);
	
	#ifdef DEBUG
		printf("after insertion\n");
		print_tree(*pp_Node_root, 0, &int_print);
		printf("\n");
	#endif
}

void
delete_avl(Node **pp_Node_root, void *p_key, int (*p_f_comp)(void *, void *)) {
	#ifdef DEBUG
		printf("deleting %d\n", *(int *)p_key);
	#endif 
	
	delete(pp_Node_root, p_key, p_f_comp);
	balance_avl_limited(pp_Node_root, p_key, p_f_comp);
	
	#ifdef DEBUG
		printf("after deletion\n");
		print_tree(*pp_Node_root, 0, &int_print);
		printf("\n");
	#endif
}