// File: MinMaxHeap.cpp
//
// CMSC 341 Spring 2017 Project 4
//
#ifndef MinMaxHeap_cpp
#define MinMaxHeap_cpp

#include "MinMaxHeap.h"
#include <iostream>
#include <stdexcept>

using namespace std;

// The constructor must create a MinMaxHeap object capable of holding capacity
// items. The object created by the constructor must be able to accept
// insertion and deletion calls right after the constructor is done.
template <typename T>
MinMaxHeap<T>::MinMaxHeap(int capacity){
    
    // setting up function pointer
    bool (*lessPtr)(T,T);
    bool (*greaterPtr)(T,T);
    bool (*equalToPtr)(T,T);
    bool (*notEqualToPtr)(T,T);
    bool (*greaterEqualToPtr)(T,T);
    bool (*lessEqualToPtr)(T,T);
    
    // initialing function pointer
    lessPtr = &lessThan;
    greaterPtr = &greaterThan;
    equalToPtr = &equalTo;
    notEqualToPtr = &notEqualTo;
    greaterEqualToPtr = &greaterThanEqualTo;
    lessEqualToPtr = &lessThanEqualTo;
    
    // create a heap object
    minHeap = Heap<T>(capacity, lessEqualToPtr);
    maxHeap = Heap<T>(capacity, greaterEqualToPtr);
    
    minHeap.otherHeap = maxHeap.heap;       // a pointer to maxHeap from minHeap
    maxHeap.otherHeap = minHeap.heap;       // a pointer to minHeap from maxHeap
}

// The constructor must create a copy of the MinMaxHeap object given in the
// parameter. The copied object must have its own allocated memory.
template <typename T>
MinMaxHeap<T>::MinMaxHeap(const MinMaxHeap<T>& other){
    
    minHeap.size = other.minHeap.size;
    maxHeap.size = other.maxHeap.size;
    minHeap.heapSize = other.minHeap.heapSize;
    maxHeap.heapSize = other.maxHeap.heapSize;
    minHeap.index = other.minHeap.index;
    maxHeap.index = other.maxHeap.index;
  
    minHeap.funcPtr1 = other.minHeap.funcPtr1;
    maxHeap.funcPtr1 = other.maxHeap.funcPtr1;
  
    minHeap.heap = new pair<T, int>[minHeap.size + 1];
    maxHeap.heap = new pair<T, int>[maxHeap.size + 1];
  
    // for loop that copy all information
    for(int i = 0; i <= other.minHeap.heapSize; i++){
        minHeap.heap[i] = other.minHeap.heap[i];
        minHeap.heap[i].second = other.minHeap.heap[i].second;
        maxHeap.heap[i].first = other.maxHeap.heap[i].first;
        maxHeap.heap[i] = other.maxHeap.heap[i];
    }
  
   minHeap.otherHeap = maxHeap.heap;       // a pointer to maxHeap from minHeap
   maxHeap.otherHeap = minHeap.heap;       // a pointer to minHeap from maxHeap
}

//The destructor must deallocate any dynamically allocated memory.
template <typename T>
MinMaxHeap<T>::~MinMaxHeap(){
     
    delete[] minHeap.heap;
    delete[] maxHeap.heap;
  
    minHeap.heap = NULL;
    maxHeap.heap = NULL;
}

// The assignment operator must deallocate memory in the host object and copy
// the rhs MinMaxHeap into the host.
template <typename T>
const MinMaxHeap<T>& MinMaxHeap<T>::operator=(const MinMaxHeap<T>& rhs){
    
    delete[] minHeap.heap;
    delete[] maxHeap.heap;

    minHeap.heap = NULL;
    maxHeap.heap = NULL;
    
    minHeap.size = rhs.minHeap.size;
    maxHeap.size = rhs.maxHeap.size;
    minHeap.heapSize = rhs.minHeap.heapSize;
    maxHeap.heapSize = rhs.maxHeap.heapSize;
    minHeap.index = rhs.minHeap.index;
    maxHeap.index = rhs.maxHeap.index;
  
   // setting up function pointer
    bool (*lessPtr)(T,T);
    bool (*greaterPtr)(T,T);
    bool (*equalToPtr)(T,T);
    bool (*notEqualToPtr)(T,T);
    bool (*greaterEqualToPtr)(T,T);
    bool (*lessEqualToPtr)(T,T);
    
    // initialing function pointer
    lessPtr = &lessThan;
    greaterPtr = &greaterThan;
    equalToPtr = &equalTo;
    notEqualToPtr = &notEqualTo;
    greaterEqualToPtr = &greaterThanEqualTo;
    lessEqualToPtr = &lessThanEqualTo;

    minHeap.funcPtr1 = rhs.minHeap.funcPtr1;
    maxHeap.funcPtr1 = rhs.maxHeap.funcPtr1;
 
    minHeap.heap = new pair<T, int>[minHeap.size + 1];
    maxHeap.heap = new pair<T, int>[maxHeap.size + 1];
   
    
    // for loop that copy all information
    for(int i = 0; i <= rhs.minHeap.heapSize; i++){
        minHeap.heap[i].first = rhs.minHeap.heap[i].first;
        minHeap.heap[i].second = rhs.minHeap.heap[i].second;
        maxHeap.heap[i].first = rhs.maxHeap.heap[i].first;
        maxHeap.heap[i].second = rhs.maxHeap.heap[i].second;
    }
  
    minHeap.otherHeap = maxHeap.heap;       // a pointer to maxHeap from minHeap
    maxHeap.otherHeap = minHeap.heap;       // a pointer to minHeap from maxHeap

  return rhs;
}

// The size() function must run constant time to receive full credit.
template <typename T>
int MinMaxHeap<T>::size(){
    
    return minHeap.heapSize;
    
}


// If min-max heap cannot hold anymore items (i.e., it has reached capacity),
// then insert() should throw a out_of_range exception. The insert() function
// must run in O(log n ) time
template <typename T>
void MinMaxHeap<T>::insert(const T& data){
    
 
    if(minHeap.heapSize >= minHeap.size) throw out_of_range("out_of_range exception");
    
    else{
      
        // make pair for both min and max heap
        minHeap.heap[minHeap.index] = make_pair(data, maxHeap.index);
        maxHeap.heap[maxHeap.index] = make_pair(data, minHeap.index);
        
        // call bubbleUp for minHeap and maxHeap
        minHeap.bubbleUp(minHeap.heap[minHeap.index]);
        maxHeap.bubbleUp(maxHeap.heap[maxHeap.index]);
        
        // increase size of index and heapSize by one
        minHeap.index++;
        maxHeap.index++;
        minHeap.heapSize++;
        maxHeap.heapSize++;
    }
}

// If the heap is empty, the deleteMin() function should throw a out_of_range
// exception. The deleteMin() function must run in O(log n ) time.
template <typename T>
T MinMaxHeap<T>::deleteMin(){
    
    if(minHeap.heapSize == 0) throw out_of_range("out_of_range exception");
    
    // if heap size is one decrease heapSize by one for both heap
    else if(minHeap.heapSize == 1){
      
      T temp = minHeap.heap[1].first;
      
      minHeap.heapSize--;
      maxHeap.heapSize--;
      return temp;
    }
    
    // if heap size is more than two
    else{
        T temp = minHeap.heap[1].first;
        int index = minHeap.heap[1].second;
   
        minHeap.heapSize--;     // decreaes the heapSize
        minHeap.bubbleDown(1);  // call bubbleDown
        
        
        maxHeap.heap[index] = maxHeap.heap[maxHeap.heapSize];
        maxHeap.heapSize--;
       
        if(greaterThan(maxHeap.heap[index].first, maxHeap.heap[index/2].first)){
    
          if(index/2 > 0){
          maxHeap.bubbleUp(maxHeap.heap[index]);
          }
        }
        else{
          if(index <= minHeap.heapSize){
            maxHeap.bubbleDown(index);
          }
        }
        return temp;
    }    
}


// If the heap is empty, the deleteMax() function should throw a out_of_range
// exception.The deleteMax() function must run in O( log n ) time
template <typename T>
T MinMaxHeap<T>::deleteMax(){
    
    if(maxHeap.heapSize == 0) throw out_of_range("out_of_range exception");
    
    // if heap size is one 
    else if(maxHeap.heapSize == 1){
         
      T temp = maxHeap.heap[1].first;
      
      minHeap.heapSize--;
      maxHeap.heapSize--;
      return temp;
    }
    
    else{
        
        T temp = maxHeap.heap[1].first;     
        int index = maxHeap.heap[1].second;
        
        maxHeap.heapSize--;
        maxHeap.bubbleDown(1);
      
        minHeap.heap[index] = minHeap.heap[minHeap.heapSize]; 
        minHeap.heapSize--;
      
      // check to see if you want bubble up or bubble down
       if(lessThan(minHeap.heap[index].first, minHeap.heap[index/2].first)){
          if(index/2 > 0){
             minHeap.bubbleUp(minHeap.heap[index]);
          }
        }
        else{
          if(index <= minHeap.heapSize){
          minHeap.bubbleDown(index);
          }
        }        
        return temp;
    }
    
}

// prints out the contents of the min-max heap including the positions of each
// key in the min-heap and the max-heap along with recorded position of it "twin".
template <typename T>
void MinMaxHeap<T>::dump(){
    
    cout <<"\n... MinMaxHeap::dump() ...\n" << endl;
    
    cout << "----------Min Heap----------" << endl;
    cout << "size = " << minHeap.heapSize << ", capacity = " << minHeap.size << endl;
    for(int i = 1; i <= minHeap.heapSize; i++){
        cout << "Heap[" << i << "] = (" << minHeap.heap[i].first << ", "
        << minHeap.heap[i].second << ")"  << endl;
    }
    
    cout << endl;
    
    cout << "----------Max Heap----------" << endl;
    cout << "size = " << maxHeap.heapSize << ", capacity = " << maxHeap.size << endl;
    for(int i = 1; i <= maxHeap.heapSize; i++){
        cout << "Heap[" << i << "] = (" << maxHeap.heap[i].first << ", "
        << maxHeap.heap[i].second << ")"  << endl;
    }
    
    cout << "----------------------------\n" << endl;
    
}

// The function locateMin has two reference parameters for "return values".
// The data parameter should hold the value of the item in position pos of
// the min-heap. The index parameter should hold the location of the item's
// "twin" in the other heap.
template <typename T>
void MinMaxHeap<T>::locateMin(int pos, T& data, int& index){
    
    data = minHeap.heap[pos].first;
    index = minHeap.heap[pos].second;
}

template <typename T>
void MinMaxHeap<T>::locateMax(int pos, T& data, int& index){
    
    data = maxHeap.heap[pos].first;
    index = maxHeap.heap[pos].second;
}

// bubbleUp() moving index of key up
template <typename T>
void Heap<T>::bubbleUp(pair<T, int> pairHeap){
    
    int hole = index;
    
    for( ; hole > 1 && funcPtr1(pairHeap.first, heap[hole/2].first); hole /= 2) {
        heap[hole] = heap[hole / 2];
        otherHeap[heap[hole].second].second = hole;
    }
    
    heap[hole] = pairHeap;
    otherHeap[heap[hole].second].second = hole;
}

// bubbleDown moving index of key down
template <typename T>
void Heap<T>::bubbleDown(int index){
   
  int hole, child;

  pair<T, int> tmp = heap[heapSize+1];
 // heap[index].second = heap[1].second;

  for (hole = index, child = index*2; child <= heapSize; hole = child, child *= 2) {

    /* find smaller of siblings (if there is one) */
    if (child < heapSize && funcPtr1(heap[child+1].first, heap[child].first)){
     
      child++;
    }
    
    if (funcPtr1(heap[child].first, tmp.first)){
      
      heap[hole] = heap[child];
      otherHeap[heap[hole].second].second = hole;   
    }  
    else
        break; 
  }
  
  heap[hole] = tmp;
  otherHeap[heap[hole].second].second = hole;
}


// lessThan for the minHeap, inside the insert
template <typename T>
bool lessThan(T parent, T child){
    return parent < child;
}

// greaterThan for the maxHeap, insdie the insert
template <typename T>
bool greaterThan(T parent, T child){
    return parent > child;
}

template <typename T>
static bool equalTo(T parent, T child){
    return parent == child;
}

template <typename T>
static bool notEqualTo(T parent, T child){
    return parent != child;
}

template <typename T>
static bool greaterThanEqualTo(T parent, T child){
    return parent >= child;
}
template <typename T>
static bool lessThanEqualTo(T parent, T child){
    return parent <= child;
}


// empty constructor for heap class
template<typename T>
Heap<T>::Heap(){
    
}

// The constructor for heap class that pass in capacity of heap and function pointer.
template<typename T>
Heap<T>::Heap(int capacity, bool (*funcPtr)(T, T)){
    
    heap = new pair<T, int>[capacity + 1];      // create an array of pair for minHeap
    size = capacity;
    heapSize = 0;
    index = 1;
  
    funcPtr1 = funcPtr;  
}

#endif
