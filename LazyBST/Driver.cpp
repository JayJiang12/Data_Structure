// File: Driver
//
// Simple test of insertion
// Should see rebalance when inserting 33.

#include <iostream>
using namespace std ;

#include "LazyBST.h"

int main() {
    
    LazyBST T ;
    int n;
    cout << "\n\n***** Insert 80 *****\n" ;
    T.insert(80) ;
    T.inorder() ; cout << endl ;
    
    cout << "\n\n***** Insert 40 *****\n" ;
    T.insert(40) ;
    T.inorder() ; cout << endl ;
    
    cout << "\n\n***** Insert 41 *****\n" ;
    T.insert(41) ;
    T.inorder() ; cout << endl ;
    
    cout << "\n\n***** Insert 39 *****\n" ;
    T.insert(39) ;
    T.inorder() ; cout << endl ;
  
    n = 40; cout << "removing " << n << endl ; T.remove(n) ; T.inorder() ; cout << endl ; 
    n = 39; cout << "removing " << n << endl ; T.remove(n) ; T.inorder() ; cout << endl ; 
    
    cout << "\n\n***** Insert 10 *****\n" ;
    T.insert(10) ;
    T.inorder() ; cout << endl ;
    
    cout << "\n\n***** Insert 4 *****\n" ;
    T.insert(4) ;
    T.inorder() ; cout << endl ;
  
    n = 4 ; cout << "removing " << n << endl ; T.remove(n) ; T.inorder() ; cout << endl ; 
    
    cout << "\n\n***** Insert 11 *****\n" ;
  T.insert(11) ;   
  T.inorder() ; cout << endl ;
  
  cout << "\n\n***** Insert 11 *****\n" ;
    T.insert(11) ;
    T.inorder() ; cout << endl ;
  
  cout << "\n\n***** Insert 90 *****\n" ;
    T.insert(90) ;
    T.inorder() ; cout << endl ;
  
  cout << "\n\n***** Insert 110 *****\n" ;
    T.insert(110) ;
    T.inorder() ; cout << endl ;
  
  cout << "\n\n***** Insert 30 *****\n" ;
    T.insert(30) ;
    T.inorder() ; cout << endl ;
  
  cout << "\n\n***** Insert 99 *****\n" ;
    T.insert(99) ;
    T.inorder() ; cout << endl ;
  
   cout << "\n\n***** Insert 300 *****\n" ;
    T.insert(300) ;
    T.inorder() ; cout << endl ;
  
  
  

}

