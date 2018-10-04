// File: test1.cpp
//
// CMSC 341 Spring 2017
// Project 1 
//
// Basic testing of Graph class
//

#include <iostream>

// need this for pair template
//
#include <utility>
using namespace std ;

#include "Graph.h"

int main() {
  
  Graph* Gptr = new Graph(6) ;

  // add some edges
  Gptr->addEdge(3,4) ;
  Gptr->addEdge(2,4) ;
  Gptr->addEdge(2,3) ;
  Gptr->addEdge(4,5) ;
  Gptr->addEdge(2,5) ;
  Gptr->addEdge(3,5) ;
  Gptr->addEdge(0,4) ;
  Gptr->addEdge(1,3) ;


  // dump out data structure
  Gptr->dump() ;

  // make a copy using copy constructor
  Graph* Gptr2 = new Graph(*Gptr) ;
  Gptr2->dump() ;

  // get rid off original graph
  // check if new graph is still there
  delete Gptr ;
  Gptr2->dump() ;

  // Make another graph and dump
  Graph G3(4) ;
  G3.addEdge(0,1) ;
  G3.addEdge(3,2) ;
  G3.addEdge(1,2) ;
  G3.addEdge(0,2) ;
  G3.dump() ;

  // Copy over G3 and dump
  G3 = *Gptr2 ;
  G3.dump() ;

  delete Gptr2 ;

  // G has 5 vertices numbered 0 thru 4
  Graph G(7) ;

  // add some edges
  G.addEdge(1,3) ;
  G.addEdge(3,4) ;
  G.addEdge(0,3) ;
  G.addEdge(1,4) ;
  G.addEdge(0,1) ;
  G.addEdge(1,2) ;
  G.addEdge(6,2) ;
  G.addEdge(0,4) ;
  G.addEdge(4,2) ;


  // dump out data structure
  G.dump() ;

  // Test neighbor iterator
  //
  Graph::NbIterator nit ;

  cout << "\nThe neighbors of vertex 1 are:\n" ;
  for (nit = G.nbBegin(1); nit != G.nbEnd(1) ; nit++) {
    cout << *nit << " " ;
  }
  cout << endl ;


 // Test edge iterator
 //
 Graph::EgIterator eit ;
 pair<int,int> edge ;

 cout << "\nThe edges in the graph are:\n" ;
 for (eit = G.egBegin() ; eit != G.egEnd() ; eit++) {

    edge = *eit ;   // get current edge

    // the two data members of a pair are first and second
    //
    cout << "(" << edge.first << ", " << edge.second << ") " ;

 }

 cout << endl ;
}