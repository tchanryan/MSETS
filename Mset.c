// COMP2521 24T3 - Assignment 1
// Implementation of the Multiset ADT

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Mset.h"
#include "MsetStructs.h"


////////////////////////////////////////////// HELPER FUNCTIONS ///////////////////////////////////////
static void MsetFreeHelper(Mset s, struct node *tree);
static struct node *MsetInsertHelper(Mset s, int item, struct node *t);
static struct node *newNode(Mset s, int val);
static struct node *MsetDeleteHelper(Mset s, int item, struct node *tree);
static struct node *MsetDeleteParent(Mset s, int item, struct node *tree);
static struct node *findSuccessor(struct node *tree);
static struct node *MsetGetCountHelper(Mset s, int item, struct node *tree);
static void bstInOrder(struct node *tree, SetCounts *results, int *index);
static void MsetInsertFromTree(struct node *tree, Mset result);
static int maxHelper(int count1, int count2);
static int nodeHeight(struct node *tree);
static int updateHeight(struct node *tree);
static struct node *rotateRight(struct node *tree);
static struct node *rotateLeft(struct node *tree);
static int getBalance(struct node *tree);
static struct node *balanceTree(Mset s, struct node *tree, int item);
static struct node *balanceDeleteTree(Mset s, struct node *tree, int item);
static void MsetInsertIfInSets(struct node *tree, Mset s2, Mset result);
static int minHelper(int count1, int count2);
static bool MsetCheckIncluded(struct node *tree, Mset s2);
static void sortArray(SetCounts results[], int size);
////////////////////////////////////////////////////////////////////////
// Basic Operations

/**
 * Creates a new empty multiset.
 */
Mset MsetNew(void) {
	// TODO
	Mset s = malloc(sizeof(*s));
	if (s == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}

	s->tree = NULL;
	s->size = 0;
	s->totalCount = 0;

	return s;
}

/**
 * Frees all memory allocated to the multiset.
 */
void MsetFree(Mset s) {
	// TODO
	if (s == NULL) {
		return; 
	}

	MsetFreeHelper(s, s->tree);
	free(s);
}

/**
 * Inserts one of an item into the multiset. Does nothing if the item is
 * equal to UNDEFINED.
 */
void MsetInsert(Mset s, int item) {
	// TODO
	if (s == NULL || item == UNDEFINED) {
		return;
	}

	s->tree = MsetInsertHelper(s, item, s->tree);
}

/**
 * Inserts the given amount of an item into the multiset. Does nothing
 * if the item is equal to UNDEFINED or the given amount is 0 or less.
 */
void MsetInsertMany(Mset s, int item, int amount) {
	// TODO
	if (s == NULL || item == UNDEFINED || amount <= 0) {
		return;
	}

	int i = 0;
	while (i < amount) {
		MsetInsert(s, item);
		i++;
	}
}

/**
 * Deletes one of an item from the multiset.
 */
void MsetDelete(Mset s, int item) {
	// TODO
	if (s == NULL) {
		return;
	}

	s->tree = MsetDeleteHelper(s, item, s->tree);
}

/**
 * Deletes the given amount of an item from the multiset.
 */
void MsetDeleteMany(Mset s, int item, int amount) {
	// TODO
	if (s == NULL || item == UNDEFINED || amount <= 0) {
		return;
	}

	int i = 0;
	while (i < amount) {
		MsetDelete(s, item);
		i++;
	}
}

/**
 * Returns the number of distinct elements in the multiset.
 */
int MsetSize(Mset s) {
	// TODO
	if (s == NULL) {
		return 0;
	}

	return s->size;
}

/**
 * Returns the sum of counts of all elements in the multiset.
 */
int MsetTotalCount(Mset s) {
	// TODO
	if (s == NULL) {
		return 0;
	}

	return s->totalCount;
}

/**
 * Returns the count of an item in the multiset, or 0 if it doesn't
 * occur in the multiset.
 */
int MsetGetCount(Mset s, int item) {
	// TODO
	if (s == NULL) {
		return 0;
	}

	struct node *itemNode = MsetGetCountHelper(s, item, s->tree);
	if (itemNode == NULL) {
		return 0;
	}
	
	return itemNode->count;
}

/**
 * Prints the multiset to a file.
 * The elements of the multiset should be printed in ascending order
 * inside a pair of curly braces, with elements separated by a comma
 * and space. Each element should be printed inside a pair of
 * parentheses with its count, separated by a comma and space.
 */
void MsetPrint(Mset s, FILE *file) {
	// TODO
	if (file == NULL) {
		fprintf(stderr, "error: couldn't allocate Mset node\n");
		exit(EXIT_FAILURE);
	}

	if (s == NULL) {
		fprintf(file, "Multiset is NULL");
		return;
	}

	SetCounts *results = malloc(s->size * sizeof(SetCounts));
	if (results == NULL) {
		fprintf(stderr, "error: couldn't allocate memory to results\n");
		exit(EXIT_FAILURE);
	}
	
	int index = 0;
	bstInOrder(s->tree, results, &index);

	fprintf(file, "{");
	for (int i = 0; i < s->size; i++) {
		fprintf(file, "(%d, %d)", results[i].elem, results[i].count);
		if (i < s->size - 1) {
			fprintf(file, ", ");
		}
	}
	fprintf(file, "}");

	free(results);
}

////////////////////////////////////////////////////////////////////////
// Advanced Operations

/**
 * Returns a new multiset representing the union of the two given
 * multisets.
 */
Mset MsetUnion(Mset s1, Mset s2) {
	// TODO
	if (s1 == NULL && s2 == NULL) {
		return NULL;
	}

	Mset result = MsetNew();

	if (s1 != NULL) {
		MsetInsertFromTree(s1->tree, result);
	}

	if (s2 != NULL) {
		MsetInsertFromTree(s2->tree, result);
	}

	return result;
}

/**
 * Returns a new multiset representing the intersection of the two
 * given multisets.
 */
Mset MsetIntersection(Mset s1, Mset s2) {
	// TODO
	if (s1 == NULL || s2 == NULL) {
		return NULL;
	}

	Mset result = MsetNew();
	MsetInsertIfInSets(s1->tree, s2, result);

	return result;
}

/**
 * Returns true if the multiset s1 is included in the multiset s2, and
 * false otherwise.
 */
bool MsetIncluded(Mset s1, Mset s2) {
	// TODO
	if (s1 == NULL || s2 == NULL) {
		return NULL;
	}

	bool result = MsetCheckIncluded(s1->tree, s2);

	return result;
}

/**
 * Returns true if the two given multisets are equal, and false
 * otherwise.
 */
bool MsetEquals(Mset s1, Mset s2) {
	// TODO
	if (s1 == NULL && s2 == NULL) {
		return true;
	}	
	
	if (s1 == NULL || s2 == NULL) {
		return false;
	}

	bool aInb = MsetCheckIncluded(s1->tree, s2);
	bool bIna = MsetCheckIncluded(s2->tree, s1);

	return aInb && bIna;
}

/**
 * Stores the k most common elements in the multiset into the given
 * items array in decreasing order of count and returns the number of
 * elements stored. Elements with the same count should be stored in
 * increasing order. Assumes that the items array has size k.
 */
int MsetMostCommon(Mset s, int k, struct item items[]) {
	// TODO
	if (s == NULL || k <= 0) {
		return 0;
	}

	SetCounts *results = malloc(s->size * sizeof(SetCounts));
	if (results == NULL) {
		fprintf(stderr, "error: couldn't allocate memory to results\n");
		exit(EXIT_FAILURE);
	}
	
	int index = 0;
	bstInOrder(s->tree, results, &index);
	sortArray(results, index);

	for (int found = 0; found < k && found < index; found++) {
		items[found].elem = results[found].elem;
		items[found].count = results[found].count;
	}
	
	free(results);
	if (index < k) {
		return index;
	}

	return k;
}

////////////////////////////////////////////////////////////////////////
// Cursor Operations

/**
 * Creates a new cursor positioned at the start of the multiset.
 * (see spec for explanation of start and end)
 */
MsetCursor MsetCursorNew(Mset s) {
	// TODO
	return NULL;
}

/**
 * Frees all memory allocated to the given cursor.
 */
void MsetCursorFree(MsetCursor cur) {
	// TODO
}

/**
 * Returns the element at the cursor's position and its count, or
 * {UNDEFINED, 0} if the cursor is positioned at the start or end of
 * the multiset.
 */
struct item MsetCursorGet(MsetCursor cur) {
	// TODO
	return (struct item){UNDEFINED, 0};
}

/**
 * Moves the cursor to the next greatest element, or to the end of the
 * multiset if there is no next greatest element. Does not move the
 * cursor if it is already at the end. Returns false if the cursor is at
 * the end after this operation, and true otherwise.
 */
bool MsetCursorNext(MsetCursor cur) {
	// TODO
	return false;
}

/**
 * Moves the cursor to the next smallest element, or to the start of the
 * multiset if there is no next smallest element. Does not move the
 * cursor if it is already at the start. Returns false if the cursor is
 * at the start after this operation, and true otherwise.
 */
bool MsetCursorPrev(MsetCursor cur) {
	// TODO
	return false;
}

////////////////////////////////////////////////////////////////////////
/////////////////////////// HELPER FUNCTIONS ///////////////////////////
////////////////////////////////////////////////////////////////////////

////////////////////////////// PART 1 //////////////////////////////////

// Helper function that helps MsetFree acces the struct nodes in the set
static void MsetFreeHelper(Mset s, struct node *tree) {
	if (tree == NULL) {
		return;
	}

	MsetFreeHelper(s, tree->left);
	MsetFreeHelper(s, tree->right);
	free(tree);
}

// Helper function that helps MsetInsert access the struct nodes in the set
// and increases its total count.
// Updated to include height balancing.
static struct node *MsetInsertHelper(Mset s, int item, struct node *tree) {
	if (tree == NULL) {
		return newNode(s, item);
	} else if (item < tree->elem) {
		tree->left = MsetInsertHelper(s, item, tree->left);
	} else if (item > tree->elem) {
		tree->right = MsetInsertHelper(s, item, tree->right);
	} else {
		tree->count++;
		s->totalCount++;
		return tree;
	}

	tree->height = updateHeight(tree);
	return balanceTree(s, tree, item);
}

// Helper function for MsetInsert to create a new struct node for insertion and 
// increases total count and size of set
static struct node *newNode(Mset s, int val) {
	struct node *new = malloc(sizeof(*new));
	if (new == NULL) {
		fprintf(stderr, "error: couldn't allocate Mset node\n");
		exit(EXIT_FAILURE);
	}

	new->elem = val;
	new->count = 1;
	new->left = NULL;
	new->right = NULL;
	new->height = 1;

	s->size++;
	s->totalCount++;

	return new;
}

// Helper function for MsetDelete that helps access the struct nodes in the tree and 
// locate the item for deletion and update the Mset struct. 
// Updated to balance the AVL tree.
static struct node *MsetDeleteHelper(Mset s, int item, struct node *tree) {
	if (tree == NULL) {
		return NULL;
	} 
	
	if (item < tree->elem) {
		tree->left = MsetDeleteHelper(s, item, tree->left);
	} else if (item > tree->elem) {
		tree->right = MsetDeleteHelper(s, item, tree->right);
	} else {
		s->totalCount--;
		if (tree->count > 1) {
			tree->count--;
		} else if (tree->count <= 1) {
			struct node *temp = MsetDeleteParent(s, item, tree);
			s->size--;
			free(tree);
			return temp;
		}
	}

	//updateHeight(tree);
	return balanceDeleteTree(s, tree, item);
} 

// if the element is to be removed from the set after deletion then, find a successor
// node to take the place of the deleted item
static struct node *MsetDeleteParent(Mset s, int item, struct node *tree) {
	if (tree == NULL) {
		return NULL;
	} 

	if (tree->left == NULL) {
		struct node *temp = tree->right;
		return temp;
	}

	if (tree->right == NULL) {
		struct node *temp = tree->left;
		return temp;
	}

	struct node *successor = findSuccessor(tree);
	tree->elem = successor->elem;
	tree->count = successor->count;
	tree->right = MsetDeleteParent(s, successor->elem, tree->right);

	return tree;
}

// helper function that locates the successor of the deleted item
static struct node *findSuccessor(struct node *tree) {
	tree = tree->right;
	while (tree != NULL && tree->left != NULL) {
		tree = tree->left;
	}
	return tree;
}

// Helper function that lets MsetGetCount access the struct nodes of the set
static struct node *MsetGetCountHelper(Mset s, int item, struct node *tree) {
	if (tree == NULL) {
		return NULL;
	}
	
	if (item < tree->elem) {
		return MsetGetCountHelper(s, item, tree->left);
	} else if (item > tree->elem) {
		return MsetGetCountHelper(s, item, tree->right);
	} else {
		return tree;
	}
}

// traverses the BST in-order from L to R and places it into an array to help MsetPrint
static void bstInOrder(struct node *tree, SetCounts *results, int *index) {
	if (tree == NULL) {
		return;
	}

	bstInOrder(tree->left, results, index);

	results[*index].elem = tree->elem;
	results[*index].count = tree->count;
	(*index)++;

	bstInOrder(tree->right, results, index);
}

//////////////////////////////////////// PART 2 ///////////////////////////////////////

// Function that places items from a tree into a 'result' set and checks if the item is 
// already present. Updates count and total count if so.
static void MsetInsertFromTree(struct node *tree, Mset result) {
	if (tree == NULL) {
		return;
	}	

	MsetInsertFromTree(tree->left, result);
		
	int elementCount = MsetGetCount(result, tree->elem);

	if (elementCount == 0) {
		MsetInsert(result, tree->elem);
		struct node *resultNode = MsetGetCountHelper(result, tree->elem, result->tree);
		if (resultNode != NULL) {
			resultNode->count = tree->count;
			result->totalCount += tree->count - 1; //MsetInsert automatically increases totalCount by 1
		}
	} else {
		struct node *resultNode = MsetGetCountHelper(result, tree->elem, result->tree);
		if (resultNode != NULL) {
			int newCount = maxHelper(resultNode->count, tree->count);
			result->totalCount += abs(newCount - elementCount);
			resultNode->count = newCount;
		}
	}

	MsetInsertFromTree(tree->right, result);	
}

// Helper function that returns the compares two numbers and returns the maximal
static int maxHelper(int count1, int count2) {
	if (count1 > count2) {
		return count1;
	}

	return count2;
}

// Function that adds an item into the 'results' set if the item in the tree is 
// also present in a secondary set. Updates count and total count if so.
static void MsetInsertIfInSets(struct node *tree, Mset s2, Mset result) {
	if (tree == NULL) {
		return;
	}

	MsetInsertIfInSets(tree->left, s2, result);
	
	int elemCountInS2 = MsetGetCount(s2, tree->elem);

	if (elemCountInS2 > 0) {
		int min = minHelper(tree->count, elemCountInS2);
		MsetInsert(result, tree->elem);
		struct node *resultNode = MsetGetCountHelper(result, tree->elem, result->tree);
		if (resultNode != NULL) {
			resultNode->count = min;
			result->totalCount += min - 1;
		}
	} 
	MsetInsertIfInSets(tree->right, s2, result);
}

// Helper function that compares two numbers and return the minimal number
static int minHelper(int count1, int count2) {
	if (count1 < count2) {
		return count1;
	}

	return count2;
}

// Function that checks if an element is also included in a secondary set and 
// returns a boolean value
static bool MsetCheckIncluded(struct node *tree, Mset s2) {
	if (tree == NULL) {
		return true;
	}

	if (!MsetCheckIncluded(tree->left, s2)) {
		return false;
	}

	int elemCountInS2 = MsetGetCount(s2, tree->elem);

	if (tree->count > elemCountInS2) {
		return false;
	} 

	return MsetCheckIncluded(tree->right, s2);
}

// a helper function that sorts the results array in descending order of count and 
// ascending order for elements with the same count
static void sortArray(SetCounts results[], int size) {
	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; j++) {
			if (results[i].count < results[j].count || 
					(results[i].count == results[j].count && results[i].elem > results[j].elem)) {
				SetCounts temp = results[i];
				results[i] = results[j];
				results[j] = temp;
			}
		}
	}
}

//////////////////////////////////////// PART 3 ///////////////////////////////////////
// Return the height of the tree
static int nodeHeight(struct node *tree) {
	if (tree == NULL) {
		return -1;
	}

	return tree->height;
}

// Update the height values of the nodes
static int updateHeight(struct node *tree) {
	if (tree == NULL) {
		return -1;
	}

	int left = nodeHeight(tree->left);
	int right = nodeHeight(tree->right);

	tree->height = 1 + maxHelper(left, right);
	return tree->height;
}

// Rotate Right function adapted from Balancing BST lecture slides
static struct node *rotateRight(struct node *tree) {
	if (tree == NULL || tree->left == NULL) {
		return tree;
	}
	struct node *newRoot = tree->left;
	tree->left = newRoot->right;
	newRoot->right = tree;

	return newRoot;
}

// Rotate Left function adapted from Balancing BST lecture slides
static struct node *rotateLeft(struct node *tree) {
	if (tree == NULL || tree->right == NULL) {
		return tree;
	}
	struct node *newRoot = tree->right;
	tree->right = newRoot->left;
	newRoot->left = tree;

	return newRoot;
}

// return the height difference between L and R subtrees, balanced <= 1
// adapted from AVL lecture slides
static int getBalance(struct node *tree) {
	if (tree == NULL) {
		return -1;
	}

	return nodeHeight(tree->left) - nodeHeight(tree->right);
}

// perform L and R rotations to balance the tree
// adapted from AVL lecture slides
static struct node *balanceTree(Mset s, struct node *tree, int item) {
	if (tree == NULL) {
		return NULL;
	}

	int balance = getBalance(tree);

	if (balance > 1) {
		if (getBalance(tree->left) < 0) {
			tree->left = rotateLeft(tree->left);
		}
		tree = rotateRight(tree);
	} else if (balance < -1) {
		if (getBalance(tree->right) > 0) {
			tree->right = rotateRight(tree->right);
		}			
		tree = rotateLeft(tree);
	}

	return tree;
}

// balance the tree after deletion by checking if the root and L/R subtrees are currently
// balanced
static struct node *balanceDeleteTree(Mset s, struct node *tree, int item) {
	int balance = getBalance(tree);

	if (balance > 1 && getBalance(tree->left) >= 0) {
		return rotateRight(tree);
	} else if (balance < -1 && getBalance(tree->right) <= 0) {
		return rotateLeft(tree);
	} else if (balance > 1 && getBalance(tree->left) < 0) {
		tree->left = rotateLeft(tree->left);
		return rotateRight(tree);
	} else if (balance < -1 && getBalance(tree->right) > 0) {
		tree->right = rotateRight(tree->right);
		return rotateLeft(tree);
	}

	return tree;
}
