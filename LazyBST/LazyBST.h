// File: LazyBST.h
//
// CMSC 341 Spring 2017 Project 3
//
// Class declarations for Lazy BST interpreter
//

#ifndef _LAZYBST_
#define _LAZYBST_

class LazyBST{
  
private:  
  // node class
  struct node{
       
    int key;
    LazyBST *leftChild;
    LazyBST *rightChild;
  };

  node *current;
  int height;
  int size;
  
public:
  
  // The default constructor must create a LazyBST object that is ready 
  // to have its member function invoked without any further processing.
  LazyBST();
  
  // The copy constructor must make a deep copy and create a new object 
  // that has its own allocated memory.
  LazyBST(const LazyBST& other);
  
  // The destructor must completely free all memory allocated for the 
  // object. (Use valgrind on GL to check for memory leaks.)
  ~LazyBST();
  
  // The assignment operator must deallocate memory used by the host 
  // object and then make deep copy of rhs.
  const LazyBST& operator=(const LazyBST& rhs);
  
  //  The insert() function must run in time proportional to the height 
  // of the Lazy BST (not counting time for rebalancing). Your LazyBST 
  // implementation must not allow duplicates. If the insert() function 
  // is invoked with a key value that already stored in the Lazy BST, 
  // your insert() function should do nothing, except that it may rebalance 
  // the tree if an imbalance is detected.
  void insert (int key);
  
  // A remove() member function that finds and removes an item with the given 
  // key value. The remove() function should return a boolean value that 
  // indicates whether the key was found. Your remove() function should not 
  // abort or throw an exception when the key is not stored in the BST.
  bool remove(int key);
  
  // A find() function that reports whether the given key is stored in the tree. 
  bool find(int key);
  
  // A member function rebalance() that rebalances a subtree of the Lazy BST. 
  // The running time of rebalance() must be proportional to the number of nodes 
  // in the subtree being rebalanced. Note that a proper implementation 
  // would require you the keep track of the size and height of the subtree.
  void rebalance();
  
  // A member function inorder() that performs an inorder walk of the LazyBST and at 
  // each node, prints out the key followed by a : followed by the height of the node 
  // followed by another : followed by the size of the subtree rooted at that node.
  // Furthermore, inorder() should print an open parenthesis before visiting the left 
  // subtree and a close parenthesis after visiting the right subtree. 
  // Nothing should be printed when inorder() is called on an empty tree, not even parentheses.
  void inorder();
  
  // A function locate() that returns whether there is a node in a position of the LazyBST and 
  // stores the key in the reference parameter. The position is given by a constant C string,
  // where a character 'L' indicates left and a character 'R' indicates right. 
  bool locate(const char *position, int& key);
  
  // same as locate() but it increase index each time it recall itself
  bool locate2(const char *position, int& key, int index);
  
  // update height and size of the tree.
  void update();
  
  // find the largest key in leftChild
  int leftMax(int key);
  
  void makeEmpty(node *& t);  
  
  node* copyTree(const node* other);
  
  int arr(node *root, int array[], int pos);
  
  void newTree(int array[], int start, int end);
  
  //void newTree(int array[], int start, int end, int index, int newArray[]);
    
  
};

#endif