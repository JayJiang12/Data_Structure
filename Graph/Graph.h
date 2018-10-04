// File: Graph.h
//
// CMSC 341 Spring 2017
//
// Header file for Graph class
//

#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdexcept>   // for throwing out_of_range exceptions

#include <utility>     // for pair template


class Graph {
    
  class AdjListNode;   // forward inner class declaration
    
    
 public:
    
  // Graph constructor, must give number of vertices
  Graph(int n);
    
  // Graph copy constructor
  Graph(const Graph& G);
    
  // Graph destructor
  ~Graph();
    
  // Graph assignment operator
  const Graph& operator= (const Graph& rhs);
    
  // return number of vertices
  int size();
    
  // add edge between u and v
  void addEdge(int u, int v) ;
    
  // print out data structure for debugging
  void dump() ;
    
    
  // Edge Iterator inner class
  class EgIterator {
        
  public:
    // Edge Iterator constructor, isEnd true to create end iterator
    EgIterator(Graph *Gptr = NULL, bool isEnd = false) ;
        
    // Compare iterators, only makes sense to compare with end iterator
    bool operator!= (const EgIterator& rhs) ;
        
    // Move iterator to next printable edge
    void operator++(int dummy) ;   // post increment
        
    // return edge at iterator location
    std::pair<int,int> operator*() ;
        
  private:
    Graph *m_Gptr ;         // pointer to Graph using this iterator
    int m_source ;          // source vertex of current location
    AdjListNode *m_where ;  // location in linked list of iterator
  } ;
    
    
  // Make an initial edge Iterator
  EgIterator egBegin() ;
    
  // Make an end iterator for edge iterator
  EgIterator egEnd() ;
    
    
  // Neighbor Iterator inner class
  class NbIterator {
        
  public:
    // Constructor for iterator for vertices adjacent to vertex v
    NbIterator(Graph *Gptr = NULL, int v = -1, bool isEnd = false) ;
        
    // Compare iterators, only makes sense to compare with end iterator
    bool operator!= (const NbIterator& rhs) ;
        
    // Move iterator to next neighbor
    void operator++(int dummy) ;
        
    // Return neighbor at current iterator position
    int operator*() ;
        
  private:
    Graph *m_Gptr ;          // which graph?
    int m_source ;           // neighbor of which vertex?
    AdjListNode *m_where ;   // location in linked list
  } ;
    
  // Make an initial neighbor iterator
  NbIterator nbBegin(int v) ;
    
  // Make an end neighbor iterator
  NbIterator nbEnd(int v) ;
    
    
 private:
    
  // Private inner Node class for linked list for
  // adjacency lists
  //
  class AdjListNode {
        
    // let all Graph class have access
    friend class Graph ;
    friend class EgIterator ;
    friend class NbIterator ;
        
  private:
    // construct a node
    AdjListNode(int v=-1, AdjListNode *ptr=NULL) ;
        
    int m_vertex ;        // holds one neighbor vertex
    AdjListNode * next ;  // next node
  } ;
    
    
  // pointer to a dynamically allocated array of linked lists
  // of AdjListNodes
  AdjListNode ** m_adjLists ;
    
  // number of vertices in this graph
  int m_size ;
    
} ;



#endif
