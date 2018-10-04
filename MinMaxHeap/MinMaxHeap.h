// File: MinMaxHeap.h
//
// CMSC 341 Spring 2017 Project 4
//
// Class declarations for Lazy BST interpreter
//

#ifndef MinMaxHeap_h
#define MinMaxHeap_h

#include <iostream>

using namespace std;

template <typename T>
class Heap;

template <typename T>
class MinMaxHeap{
    
public:
  
     // The constructor must create a MinMaxHeap object capable of holding capacity
     // items. The object created by the constructor must be able to accept
     // insertion and deletion calls right after the constructor is done.
     MinMaxHeap(int capacity);
    
     // The constructor must create a copy of the MinMaxHeap object given in the
     // parameter. The copied object must have its own allocated memory.
     MinMaxHeap(const MinMaxHeap<T>& other);
     
     //The destructor must deallocate any dynamically allocated memory.
     ~MinMaxHeap() ;
    
     // The assignment operator must deallocate memory in the host object and copy
     // the rhs MinMaxHeap into the host.
     const MinMaxHeap<T>& operator=(const MinMaxHeap<T>& rhs);
     
     // The size() function must run constant time to receive full credit.
     int size();
    
     // If min-max heap cannot hold anymore items (i.e., it has reached capacity),
     // then insert() should throw a out_of_range exception. The insert() function
     // must run in O(log n ) time
     void insert(const T& data);
    
     // If the heap is empty, the deleteMin() function should throw a out_of_range
     // exception. The deleteMin() function must run in O(log n ) time.
     T deleteMin() ;
    
     // If the heap is empty, the deleteMax() function should throw a out_of_range
     // exception.The deleteMax() function must run in O( log n ) time
     T deleteMax() ;

     // prints out the contents of the min-max heap including the positions of each
     // key in the min-heap and the max-heap along with recorded position of its "twin".
     void dump() ;
    
     // The function locateMin has two reference parameters for "return values".
     // The data parameter should hold the value of the item in position pos of
     // the min-heap. The index parameter should hold the location of the item's
     // "twin" in the other heap.
     void locateMin(int pos, T& data, int& index) ;
     void locateMax(int pos, T& data, int& index) ;
    
private:
    
     Heap<T> minHeap;    // a template heap obejct
     Heap<T> maxHeap;    // a template heap object
};

template <typename T>
class Heap{
    
public:
     pair<T, int> *heap;          // a pair of heap pointer
     pair<T, int> *otherHeap;     // a other heap pointer
     
    int size, heapSize, index;    // capacity of the heap, current size of heap and index of heap
  
     
     Heap();
     Heap(int heapSize,bool(*funcPtr)(T, T));
     
     void insert2(T data, int heapSize);
     
     void bubbleUp(pair<T, int> pairHea);       // move key up
     void bubbleDown(int index);     // move key down
     
     bool (*funcPtr1)(T,T);
};

// lessThan for the minHeap, inside the insert
template <typename T>
static bool lessThan(T parent, T child);

// greaterThan for the maxHeap, insdie the insert
template <typename T>
static bool greaterThan(T parent, T child);

template <typename T>
static bool equalTo(T parent, T child);

template <typename T>
static bool notEqualTo(T parent, T child);

template <typename T>
static bool greaterThanEqualTo(T parent, T child);

template <typename T>
static bool lessThanEqualTo(T parent, T child);

#include "MinMaxHeap.cpp" 
#endif
