//
//  HashTable.h
//  CMSC341 Project5
//
//  Created by Zhaojie Jiang on 5/3/17.
//  Copyright © 2017 Zhaojie Jiang. All rights reserved.
//

#ifndef HashTable_h
#define HashTable_h

#include <string>

using namespace std;

class HashTable{

public:
  // This is the default constructor for the HashTable class. The parameter n is the
  // requested size of the hash table. If no size is given or if n is less than 101,
  // then use the prime number 101 for the table size. (I.e., the minimum table size is 101.)
  HashTable(int n=101);

  // This is the destructor. Make sure you deallocate all memory for this object.
  // The strings in the hash table must be deallocated using free() since they are C strings
  // (i.e., don't use remove for C strings).
  ~HashTable();
    
  // This is the copy constructor for the HashTable class. If the other hash table is not in
  // incremental rehash, then copying the hash table is very straightforward. Just make sure
  // you allocate memory for the new hash table and use strdup() to copy the strings.
  HashTable (HashTable& other) ;
  
  // This is the overloaded assignment operator for the HashTable class. As with the copy constructor, 
  // the process is fairly standard if the rhs does not have an ongoing incremental rehash.
  // If there is an ongoing rehash, then force the rhs to finish its rehashing and copy over the 
  // resulting single hash table.
  const HashTable& operator= (HashTable& rhs) ;
       
  // This function inserts a copy of the C string str into the hash table. It has no
  // return value. (Note: use strdup() to copy C strings.)
  // The insert() function should insert in the new table if there is an ongoing incremental rehash.
  // Calling insert() with a string that is already in the hash table should have no effect.
  // (I.e., do not insert a second copy of the same value.) Make sure you don't have a copy of a
  // string that you didn't insert floating around. That's a memory leak.
  void insert(const char *str);
    
  // The find() function looks for str in the hash table. The function returns true if found, false
  // otherwise. The find() function look in both the old and the new hash tables if there is an
  // ongoing incremental rehashing.
        
  // The find() function should trigger incremental rehashing when appropriate as described above.
  // The find() operation should also wrap up the incremental rehashing if the number of items in
  // the old table drops below 3%.
  bool find(const char *str) ;
    
  // The remove() function removes str from the hash table and returns the pointer.
  // If str is not in the hash table, remove() returns NULL.
  char * remove(const char *str) ;
        
  // The isRehashing() function returns true if there is an ongoing incremental rehash.
  bool isRehashing() ;
    
  // tableSize(0) should return the size of the old table and tableSize(1)
  // should return the size of the new table
  int tableSize(int table=0) ;
    
  // size() returns the number of items currently in the table(s)
  int size(int table=0) ;
    
  // The at() function returns a pointer to the string stored at the index slot of the
  // hash table specified by table.
  const char * at(int index, int table=0) ;
    
  // Dump should print some vital statistics and the contents of the hash table(s) to stdout.
  // You should include the table size and number of items in the hash table(s).
  void dump() ;
    
  unsigned int hashCode(const char *str);
  
  // copying string from one table to another table
  void insertStr(char** fromTable, char** toTable, int index, int num);

  // sett he capacity for the table
  void setCapacity(int num, int n);
  
  // final rehash to on table
  void oneTable();
   
 private:
  
  char **table1;    // table that being use when there is only one table (no reHashing)
                    // or old table when they are two table use or reHashing 
  char **table2;    // new table when reHashing happened

  int Capacity1;    // size of table1
  int Capacity2;    // size of table2
  
  int Size1;        // number of items in table1
  int Size2;        // number of items in table2
    
  bool reHash;      // true when reHashing, false when there is not rehashing
  
  static char * const DELETED;  // replace delete when you remove a item from the arrays
};

#endif /* HashTable_hpp */
