Multisets
You are probably familiar with the idea of a set. In mathematics, a set is a collection of distinct elements.

A multiset is a collection of elements where duplicates are allowed. In this assignment, we are concerned with multisets of integers.

Since duplicates are allowed, each distinct element in a multiset has a count, which is the number of times it occurs in the multiset. For example, in the multiset 
{1,4,4,4,8,8}, the count of 1 is 1, the count of 4 is 3 and the count of 8 is 2. 

Since the counts of the elements in a multiset can be arbitrarily high, we will write a multiset by listing all the distinct elements with their counts between curly braces. For example, the multiset 
{1,4,4,4,8,8} is written as {(1,1),(4,3),(8,2)}. notation 

Let A be a multiset and x be an item. cA(x) is the count of x in A. If x does not occur in A, then cA(x)=0.

The empty multiset (i.e., the multiset containing no elements) is denoted by ∅.

Multiset ADT
A multiset is an example of an abstract data type: there is a collection of operations that can be performed on multisets, but the exact details of the implementation are unimportant, as long as they produce the desired behaviour from the user's perspective.

It is possible to implement the Multiset ADT in many different ways, such as using arrays or linked lists. Although these are relatively simple to implement, each of them are inefficient in some way:

An ordered array allows for binary search to be used, but requires elements to be shifted to maintain order when inserting
A linked list does not require shifting, but requires a linear traversal when inserting and searching for elements
Recently, we introduced binary search trees as a solution to both of these problems (shifting and linear search). A binary search tree (BST) is a linked data structure, and therefore does not require shifting, and the way searching works in a BST is similar to how binary search works. However, a BST is only guaranteed to be efficient if the tree is relatively balanced. Thus, your task in this assignment will be to implement a Multiset ADT using a balanced binary search tree.
