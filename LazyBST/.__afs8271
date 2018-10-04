// File: LazyBST.h
//
// CMSC 341 Spring 2017 Project 3
//
// Class declarations for Lazy BST interpreter
//

#include "LazyBST.h"
#include <iostream>

using namespace std;

// The default constructor must create a LazyBST object that is ready
// to have its member function invoked without any further processing.
LazyBST::LazyBST(){
    
  current = NULL;
  height = 0;
  size = 1;
}

// The copy constructor must make a deep copy and create a new object
// that has its own allocated memory.
LazyBST::LazyBST(const LazyBST& other){

  // empty tree
  if(other.current == NULL){
   current = NULL;
  }
  else
    // call helper function 
    current = copyTree(other.current);
  
    // helper function dont copy the height and size of the root
    height = other.height;
    size = other.size;
}


// a help function that 
LazyBST::node* LazyBST::copyTree(const node* other){
   
  if (other == NULL)
		return NULL;

  node *copyNode = new node;
	copyNode->key = other->key;
  
  // travel to the left 
  if(other->leftChild != NULL){
    copyNode->leftChild = new LazyBST();
    copyNode->leftChild->height = other->leftChild->height;
    copyNode->leftChild->size = other->leftChild->size;

    copyNode->leftChild->current = copyTree(other->leftChild->current);
  }
  if(other->rightChild != NULL){
     copyNode->rightChild = new LazyBST();
     copyNode->rightChild->height = other->rightChild->height;
     copyNode->rightChild->size = other->rightChild->size;
	copyNode->rightChild->current = copyTree(other->rightChild->current);
  }
	return copyNode;

}

// The destructor must completely free all memory allocated for the
// object. (Use valgrind on GL to check for memory leaks.)
LazyBST::~LazyBST(){
    
  makeEmpty(current);
}

void LazyBST::makeEmpty(node *&t){
 
  if(t != NULL){
    if(t->leftChild != NULL)
      t->leftChild->makeEmpty(t->leftChild->current);
    
    t->leftChild = NULL;
    t->rightChild = NULL;
    
  if(t->rightChild != NULL)
    t->leftChild->makeEmpty(t->rightChild->current);
  }
}

// The assignment operator must deallocate memory used by the host
// object and then make deep copy of rhs.
const LazyBST& LazyBST::operator=(const LazyBST& rhs){
  
  makeEmpty(current);
  
  current = copyTree(rhs.current);
  height = rhs.height;
  size = rhs.size;
}

// The insert() function must run in time proportional to the height
// of the Lazy BST (not counting time for rebalancing). Your LazyBST
// implementation must not allow duplicates. If the insert() function
// is invoked with a key value that already stored in the Lazy BST,
// your insert() function should do nothing, except that it may rebalance
// the tree if an imbalance is detected.
void LazyBST::insert (int key){
   
    // if key is already in the tree
  if(find(key)){
    return;
  }
  
  // if tree is empty, insert the key value
  else if(current == NULL){
    current = new node;
    current->leftChild = NULL;
    current->rightChild = NULL;
    current->key = key;
  }
  
  // if tree is not empty
  else{ 
    
    // create a new LazyBST tree and insert the value
    if(current->key > key && current->leftChild == NULL){  
      
      current->leftChild = new LazyBST();
      current->leftChild->current = new node;
      current->leftChild->current->leftChild = NULL;
      current->leftChild->current->key = key;
    }
    
    // travel to rightChild of the tree and call the insert() in that LazyBST
    else if(current->key > key && current->leftChild != NULL){ 
      current->leftChild->insert(key);
    }  
    
    // create a new LazyBST tree and insert the value
    else if(current->key < key && current->rightChild == NULL){
     
      current->rightChild = new LazyBST();    
      current->rightChild->current = new node;
      current->rightChild->current->leftChild = NULL;
      current->rightChild->current->key = key; 
    }
    
    // travel to rightChild of the tree and call the insert() in that LazyBST 
    else if(current->key < key && current->rightChild != NULL){  
      
   //   cout << "TR in insert" << endl;  
      current->rightChild->insert(key);
    }
   
    else{
      cout << "Error in insert()" << endl;
    }
  }
  update();
}

// A remove() member function that finds and removes an item with the given
// key value. The remove() function should return a boolean value that
// indicates whether the key was found. Your remove() function should not
// abort or throw an exception when the key is not stored in the BST.
bool LazyBST::remove(int key){
  
//  cout << "remove()" << endl;
   // if tree is empty
  if(current == NULL){
    return false;
  }
   
  // if root or parent of that tree's value is not equal to key 
  else if(current->key != key){
    
    // if key is smaller than the key of current tree
    if(current->key > key){
      
       // if leftChild is empty, no key find
      if(current->leftChild == NULL){
          return false;
      }
      
      // if leftChild is not empty 
      else{
        if(current->leftChild->current->key != key){
          current->leftChild->remove(key);
        }
      }
    }

    // if key is larger than the key of current tree
    else if(current->key < key){
      
      // if rightChild is empty, no key find
      if(current->rightChild == NULL){
          return false;
      }
      
      // if rightChild is not empty 
      else{
        
        // travel to rightChild of the tree if key is not equal to key
        // otherwise key find
        if(current->rightChild->current->key != key){
          current->rightChild->remove(key);
        }
      }
    }
  }
  
  // root of the tree
  if(current->key == key){
    
    // case 1:  no child  
    if(current->leftChild == NULL && current->rightChild == NULL){
   
      delete current;
      current = NULL;
      update();
    }
      
    // case 2: only one child
    else if(current->leftChild == NULL){
      struct node *temp;

      temp = current;
      current = current->rightChild->current;
      delete temp;      
      update();      
    }
    else if(current->rightChild == NULL){
      struct node *temp;

      temp = current;
      current = current->leftChild->current;
      delete temp;   
      update();
    }

    // case 3: two child 
    else{
      
      int temp; 
     
      temp = current->leftChild->leftMax(key);
     
      remove(temp);
      current->key = temp;
      update();
      }
  }
    
  if(current != NULL){
    if(current->leftChild != NULL){
      if(current->leftChild->current->key == key){

       // case 1:  no child
        if(current->leftChild->current->leftChild == NULL 
           && current->leftChild->current->rightChild == NULL){
          delete current->leftChild;
          current->leftChild = NULL;

          update();
        }

        // case 2: only one child
        else if(current->leftChild->current->leftChild == NULL){
          struct LazyBST *temp;

          temp = current->leftChild;
          current->leftChild = current->leftChild->current->rightChild;
          delete temp;
          update();
        }
        else if(current->leftChild->current->rightChild == NULL){
          struct LazyBST *temp;

          temp = current->leftChild;
          current->leftChild = current->leftChild->current->leftChild;
          delete temp;
          update();
        }

        // case 3: two child 
        else{

          int temp = current->leftChild->current->leftChild->leftMax(key);

          current->leftChild->remove(temp);
          current->leftChild->current->key = temp;

          update();  
        }
      }
    }

    if(current->rightChild != NULL){    
      if(current->rightChild->current->key == key){

       // case 1:  no child
        if(current->rightChild->current->leftChild == NULL 
           && current->rightChild->current->rightChild == NULL){
          delete current->rightChild;
          current->rightChild = NULL;
          update();
        }

        // case 2: only one child
        else if(current->rightChild->current->leftChild == NULL){
          struct LazyBST *temp;

          temp = current->rightChild;
          current->rightChild = current->rightChild->current->rightChild;
          delete temp;
          update();
        }
        else if(current->rightChild->current->rightChild == NULL){
          struct LazyBST *temp;

          temp = current->rightChild;
          current->rightChild = current->rightChild->current->leftChild;
          delete temp;
          update();
        }

        // case 3: two child 
        else{

          int temp = current->rightChild->current->leftChild->leftMax(key);

          current->rightChild->remove(temp);
          current->rightChild->current->key = temp;

          update();
         }
      }
    }
  }
}

// find the largest number in leftChild 
int LazyBST::leftMax(int key){
  
  // if there is no more right child
  // return key of that tree
  if(current->rightChild == NULL){
    return current->key;
  }
  
  // keep going to right child until there is not more right child
  else{
    return current->rightChild->leftMax(key);
  }
}

// A find() function that reports whether the given key is stored in the tree.
bool LazyBST::find(int key){
  
  // if tree is empty
  if(current == NULL){
    return false;
  }
  
  else{  
  
    // if root or parent of that tree's value is equal to key 
    if(current->key == key){
    return true;
  }
  
    // if key is less than the key of current tree
    // if leftChild is empty, no key find
    // if leftChild is not empty, travel to leftChild of the tree 
    // and call the insert() in that LazyBST 
    else if(current->key > key){
      if(current->leftChild == NULL){
          return false;
      }
      else{
          current->leftChild->find(key);
      }
  }
    
    // if key is larger than the key of current tree
    // if rightChild is empty, no key find
    // if rightChild is not empty, travel to rightChild of the tree 
    // and call the insert() in that LazyBST 
    else if(current->key < key){
      if(current->rightChild == NULL){
          return false;
      }
      else{
          current->rightChild->find(key);
      } 
    }
  }
}

// A member function rebalance() that rebalances a subtree of the Lazy BST.
// The running time of rebalance() must be proportional to the number of nodes
// in the subtree being rebalanced. Note that a proper implementation
// would require you the keep track of the size and height of the subtree.
void LazyBST::rebalance(){
  
   cout << "Rebalance" << endl;
  cout << "current key: " << current->key << "size: " << size << endl;
  int arraySize = size;
  int array[arraySize];
  int index = 0;
  
  
  arr(current, array, index);
  
  cout << "after arr" << endl;
  
  newTree(array, 0, arraySize-1);
  update();
}

void LazyBST::newTree(int *array, int start, int end){

  if(start > end){
    return;
  }
  
  int mid; 
  mid = (start + end) / 2;
  current = new node;
  current->key = array[mid];
 
  current->leftChild = new LazyBST();
  current->leftChild->newTree(array, start, mid-1);
  current->rightChild = new LazyBST();
  current->rightChild->newTree(array, mid+1, end); 
}


int LazyBST::arr(node *root, int array[], int pos){
 
  if (root->leftChild != NULL) {
        pos = arr(root->leftChild->current, array, pos);
    }
    array[pos++] = root->key;
  
  delete root;
  if (root->rightChild != NULL) {
        pos = arr(root->rightChild->current, array, pos);
    }
    return pos; // return the last position
}


// A member function inorder() that performs an inorder walk of the LazyBST and at
// each node, prints out the key followed by a : followed by the height of the node
// followed by another : followed by the size of the subtree rooted at that node.
// Furthermore, inorder() should print an open parenthesis before visiting the left
// subtree and a close parenthesis after visiting the right subtree.
// Nothing should be printed when inorder() is called on an empty tree, not even parentheses.
void LazyBST::inorder(){
  
  // if tree is empty
  if (current == NULL){
    return;  
  }
  
  // tree is not empty
  else{
   cout << "(";
  
  // trave to the left child untill there is no more left child
  // then print out the current key at that tree or subtree
  if(current->leftChild != NULL){
    
    current->leftChild->inorder();  
  }
  
  // print when there is no more left child
  cout << current->key << ":" << height << ":" << size;
  
  // trave to the right child or next right child
 if(current->rightChild != NULL){
   
    current->rightChild->inorder();
  }
   cout << ")";
  }
}

// update the height and size of tree; 
void LazyBST::update(){
  
  // tree is empty
  if (current == NULL){
    return;
  }
 
  // tree is not empty
  else{
    
    // when tree do not have left and right child
    if(current->leftChild == NULL && current->rightChild == NULL){
      height = 0;
      size = 1;    
    }
    
    // when tree have both left and right child
    else if(current->leftChild != NULL && current->rightChild != NULL){
      
      // size is equal to total of leftchild side and right child size
      size = current->leftChild->size + current->rightChild->size + 1;
      
      // if left child's height is higher than right child's height
      if(current->leftChild->height < current->rightChild->height){
          height = current->rightChild->height + 1;    
      }
      else{
          height = current->leftChild->height + 1;
      }
      
      if(height >= 4 && (current->leftChild->size >= (2 * current->rightChild->size)
         || (2 * current->leftChild->size) <= current->rightChild->size)) {
        
          rebalance();
        }
    }
    
    // only have left child
    else if(current->leftChild != NULL){  
      current->leftChild->update();
    
      size = current->leftChild->size + 1;
      height = current->leftChild->height + 1;
        
      if(height >= 4 && current->leftChild->size >= 2){
       
          rebalance();
      }
    }
    
    // only have right child
    else if(current->rightChild != NULL){
       
      current->rightChild->update();
      size = current->rightChild->size + 1;
      height = current->rightChild->height + 1;
        
      if(height >= 4 && current->rightChild->size >= 2){
        
        rebalance(); 
      } 
    }
  }
}

// A function locate() that returns whether there is a node in a position of the LazyBST and
// stores the key in the reference parameter. The position is given by a constant C string,
// where a character 'L' indicates left and a character 'R' indicates right.
bool LazyBST::locate(const char *position, int& key){
  return locate2(position, key, 0);
}
 
bool LazyBST::locate2(const char *position, int& key, int index){
  if(position[index] == '\0'){
    if (key == current->key)
      return true;
    else
      return false;
  }
  
  else{
    
    if(position[0] == 'L') {
      current->leftChild->locate2(position, key, index+1);
    }
   
    else if(position[0] == 'R'){
       current->rightChild->locate2(position, key, index+1);
    }
    else{
      cout << "Error" << endl;
    }
  }
}