//
//  HashTable.cpp
//  CMSC341 Project5
//
//  Created by Zhaojie Jiang on 5/3/17.
//  Copyright © 2017 Zhaojie Jiang. All rights reserved.
//

#include "HashTable.h"
#include "primes.h"

#include <iostream>
#include <string.h>
#include <stdexcept>
#include <cstdlib>

using namespace std;

char * const HashTable::DELETED  = (char *) 1;

double REHASH_PERCENT = 0.03;

// This is the default constructor for the HashTable class. The parameter n is the
// requested size of the hash table. If no size is given or if n is less than 101,
// then use the prime number 101 for the table size. (I.e., the minimum table size is 101.)
HashTable::HashTable(int n){
  
 	Size1 = 0;
	reHash = false;
	// setting up Capacity for Table1
	setCapacity(0, n);
	
	// create a table1
  table1 = (char**)malloc(Capacity1 * sizeof(char*));
	
	// set all array index to NULL
	for(int i = 0; i < Capacity1; i++){
		table1[i] = NULL;
	}
}

// This is the destructor. Make sure you deallocate all memory for this object.
// The strings in the hash table must be deallocated using free() since they are C strings
// (i.e., don't use remove for C strings).
HashTable::~HashTable(){
  
	for(int i = 0; i < Capacity1; i++){
		if(table1[i] == DELETED)
			table1[i] = NULL;
		free(table1[i]);
	}
	free(table1);
	
	if(reHash == true){
		for(int i = 0; i < Capacity2; i++){
			if(table2[i] == DELETED)
				table2[i] = NULL;
			free(table2[i]);
		}
	free(table2);
	}
	
}

// This is the copy constructor for the HashTable class. If the other hash table is not in
// incremental rehash, then copying the hash table is very straightforward. Just make sure
// you allocate memory for the new hash table and use strdup() to copy the strings.
HashTable::HashTable(HashTable& other) {
	
	if(other.reHash == true){
		other.oneTable();
	}
	Size1 = other.Size1;
	Capacity1 = other.Capacity1;
	reHash = other.reHash;
	
	table1 = (char**)malloc(Capacity1 * sizeof(char*));

  for(int i = 0; i < Capacity1; i++){
		table1[i] = NULL;
	}
	
	for(int i = 0; i < Capacity1; i++){
		
		if(other.table1[i] != NULL){
			if(other.table1[i] == DELETED)
				table1[i] = DELETED;
			else
				table1[i] = strdup(other.table1[i]);
		}
	}
}

// This is the overloaded assignment operator for the HashTable class. As with the copy constructor, 
// the process is fairly standard if the rhs does not have an ongoing incremental rehash.
// If there is an ongoing rehash, then force the rhs to finish its rehashing and copy over the 
// resulting single hash table.
const HashTable& HashTable::operator= (HashTable& rhs){
	
	for(int i = 0; i < Capacity1; i++){
		if(table1[i] == DELETED)
			table1[i] = NULL;
		free(table1[i]);
	}
	free(table1);
		
	if(reHash == true){
		for(int i = 0; i < Capacity2; i++){
			if(table2[i] == DELETED)
				table2[i] = NULL;
			free(table2[i]);
		}
	free(table2);
	}
	
	
	if(rhs.reHash == true){
		rhs.oneTable();
	}
	Size1 = rhs.Size1;
	Capacity1 = rhs.Capacity1;
	reHash = rhs.reHash;
	
	table1 = (char**)malloc(Capacity1 * sizeof(char*));

  for(int i = 0; i < Capacity1; i++){
		table1[i] = NULL;
	}
	
	for(int i = 0; i < Capacity1; i++){
		
		if(rhs.table1[i] != NULL){
			if(rhs.table1[i] == DELETED)
				table1[i] = DELETED;
			else
				table1[i] = strdup(rhs.table1[i]);
		}
	}
	return *this;
}

// This function inserts a copy of the C string str into the hash table. It has no
// return value. (Note: use strdup() to copy C strings.)
// The insert() function should insert in the new table if there is an ongoing incremental rehash.
// Calling insert() with a string that is already in the hash table should have no effect.
// (I.e., do not insert a second copy of the same value.) Make sure you don't have a copy of a
// string that you didn't insert floating around. That's a memory leak.
void HashTable::insert(const char *str){
 
    if(!find(str)){
     
			unsigned int index = hashCode(str);
			
			// insert to table1 when reHash is false
			if(reHash == false){
				index = index % Capacity1;
			
				while(table1[index] != NULL && table1[index] != DELETED){
					index = (index + 1) % Capacity1;
				}		
				table1[index] = strdup(str);
				Size1++;
			}
		
			// insert to table2 when reHash is true
			else{
				index = index % Capacity2;
			
				while(table2[index] != NULL && table2[index] != DELETED){
					index = (index + 1) % Capacity2;
				}
				table2[index] = strdup(str);
				Size2++;	
			}
		}
}

// The find() function looks for str in the hash table. The function returns true if found, false
// otherwise. The find() function look in both the old and the new hash tables if there is an
// ongoing incremental rehashing.
// The find() function should trigger incremental rehashing when appropriate as described above.
// The find() operation should also wrap up the incremental rehashing if the number of items in
// the old table drops below 3%.
bool HashTable::find(const char *str) {
 
	bool isFind = false;		// if item is find or not.
	int count = 1;					// number of onging incremental
	
	unsigned int index = hashCode(str);		// get the index of str from hashCode function 
	index = index % Capacity1;						// index of str in table1
	unsigned int tempIndex = index;				// copy of index of str in table1
	while(table1[index] != NULL){
	
		if(table1[index] != DELETED && strcmp(table1[index], str) == 0){
			isFind = true;
			
			
			// move items form table1 to table2 in the group of index
			if(reHash == true){
			
				insertStr(table1, table2, index, 0);

				insertStr(table1, table2, index - 1, 1);
			
				// make it to one take when take 1 is less than three percent
				if(double(Size1) / Capacity1 <= REHASH_PERCENT){	
						for(int i = 0; i < Capacity1; i++){

						if(table1[i] != NULL && table1[i] != DELETED){
						
							int index3 = hashCode(table1[i]);
							index3 = index3 % Capacity2;

							while(table2[index3] != NULL && table2[index3] != DELETED){
								index3 = (index3 + 1) % Capacity2;
							}
							table2[index3] = strdup(table1[i]);
							Size2++;
						}
						free(table1[i]);
						table1[i] = NULL;
						Size1--;
					}	
					free(table1);
					table1 = table2;
					Size1 = Size2;
					Capacity1 = Capacity2;
					table2 = NULL;
					reHash = false;
				}
			}
			break;
		}		
		else{
			index = (index + 1) % Capacity1;
			count++;
		}
		
		// when increment more than 10 time and rehash is false
		// create a new table and move that 9 items from take on to take 2
		if(count >= 10 && reHash == false){

			int n = Size1 * 4;
			Size2 = 0;
			setCapacity(1, n);
			table2 = (char**)malloc(Capacity2 * sizeof(char*));

			for(int i = 0; i < Capacity2; i++){
				table2[i] = NULL;
			}
		
			insertStr(table1, table2, index, 0);
			insertStr(table1, table2, index - 1, 1);

			reHash = true;
			break;	
		}
	}
	
	// when item is not find, and there is item in that table, move that group of items to take2
	if(reHash == true && isFind == false && table1[tempIndex] != NULL){
		insertStr(table1, table2, index - 1, 1);
	}
	
	// when items is not find in first table 
	else if(reHash == true && isFind == false){
		unsigned int index = hashCode(str);	
		index = index % Capacity2;
	
		int count = 1;
		
		while(table2[index] != NULL){
		
			// check if item is find
			if(table2[index] != DELETED && strcmp(table2[index], str) == 0){
				isFind = true;
				break;	
			}	
			else{
				index = (index + 1) % Capacity2;
				count++;	
			}
			
			// rehashing is happing, make two tables to one
			if(count >= 10){
				
				int n = (Size1 + Size2 + 1) * 4;
			
				int tempCapacity = Capacity1;
				Size1 = 0;
				
				setCapacity(0, n);
				char** newTable = (char**)malloc(Capacity1 * sizeof(char*));
				for(int i = 0; i < Capacity1; i++){
					newTable[i] = NULL;
				}
			
				for(int i = 0; i < tempCapacity; i++){
					
					if(table1[i] != NULL && table1[i] != DELETED){
						unsigned int index3 = hashCode(table1[i]);
						index3 = index3 % Capacity1;
						while(newTable[index3] != NULL){
							index3 = (index3 + 1) % Capacity1;
						}
						newTable[index3] = strdup(table1[i]);
						Size1++;
						free(table1[i]);
						table1[i] = NULL;
					}
				}	
				free(table1);
				for(int i = 0; i < Capacity2; i++){

					if(table2[i] != NULL && table2[i] != DELETED){

						unsigned int index3 = hashCode(table2[i]);
						index3 = index3 % Capacity1;

						while(newTable[index3] != NULL){
							index3 = (index3 + 1) % Capacity1;
						}
						newTable[index3] = strdup(table2[i]);
						Size1++;
					}
					free(table2[i]);
					table2[i] = NULL;
				}	
				free(table2);
				table1 = newTable;
				reHash = false;
				
				break;	
			}
		}
	}
	
	if(reHash == true && double(Size2) / Capacity2 > .5){	
		oneTable();
	}
	
	return isFind;
}

// The remove() function removes str from the hash table and returns the pointer.
// If str is not in the hash table, remove() returns NULL.
char * HashTable::remove(const char *str) {
	
	if(find(str)){
		// if reHash is false, remove item from table1
		if(reHash == false){
			
			unsigned int index = hashCode(str);
			index = index % Capacity1;
		
			while(table1[index] != NULL){
				
				if(table1[index] != DELETED && strcmp(table1[index], str) == 0){
					
					char *temp = strdup(table1[index]);
					free(table1[index]);
					table1[index] = DELETED;
					Size1--;
					return temp;
				}
				else{
					index = (index + 1) % Capacity1;
				}
			}
			return NULL;
		}
		
		// if reHash is true, find function will copy all items from table1 to table2, 
		// so no need to check for table1.
		else{
			
			unsigned int index = hashCode(str);
			index = index % Capacity2;
		
			while(table2[index] != NULL){
				
				if(table2[index] != DELETED && strcmp(table2[index], str) == 0){
				
					char *temp = strdup(table2[index]);
				
					free(table2[index]);
					
					table2[index] = DELETED;
			
					table2--;
					return temp;
				}
				else{
					index = (index + 1) % Capacity2;
				}
			}
			return NULL;
		}
	}
	
	else
		return NULL;
}


// The isRehashing() function returns true if there is an ongoing incremental rehash.
bool HashTable::isRehashing() {
  return reHash;
}

// tableSize(0) should return the size of the old table and tableSize(1)
// should return the size of the new table
int HashTable::tableSize(int table) {
 
	if(table == 0)
		return Capacity1;
	else if(table == 1)
  	return Capacity2;
	else
		throw out_of_range("out of range");
}

// size() returns the number of items currently in the table(s)
int HashTable::size(int table) {
  	
	if(table == 0)
		return Size1;
	else if(table == 1)
  	return Size2;
	else
		throw out_of_range("out of range");
}

// The at() function returns a pointer to the string stored at the index slot of the
// hash table specified by table.
// table = 0, check for table1; table = 1, check for table2
const char * HashTable::at(int index, int table) {
  
	if(table == 0)
		return table1[index];
	else if(table == 1)
  	return table2[index];
	else
		throw out_of_range("out of range");
}

// Dump should print some vital statistics and the contents of the hash table(s) to stdout.
// You should include the table size and number of items in the hash table(s).
void HashTable::dump() {
    
   
	cout << "HashTable #1: " << "size: " << Size1 << " Table Size: " <<  Capacity1 << endl;
  
	// for loop the print of all items in table1
	for(int i = 0; i < Capacity1; i++){
    if(table1[i] == NULL){
			cout << "H1[" << i << "] = " << endl; 
		}
		else if(table1[i] == DELETED){
			cout << "H1[" << i << "] = " << "DELETED "<< endl; 
		}
		else{
			cout << "H1[" << i << "] = " << table1[i] << endl;
		}
	}
	
	// for loop the print of all items in table2 if reHash is true
	if(reHash == true){
		cout << "\n" << endl;
		cout << "HashTable #2: " << "size: " << Size2 << " Table Size: " <<  Capacity2 << endl;
		
		for(int i = 0; i < Capacity2; i++){
			if(table2[i] == NULL){
				cout << "H1[" << i << "] = " << endl; 
			}
			else if(table2[i] == DELETED){
				cout << "H1[" << i << "] = " << "DELETED "<< endl; 
			}
			else{
				cout << "H1[" << i << "] = " << table2[i] << endl;
			}
		}
	} 
}

// rehash function
unsigned int HashTable::hashCode(const char *str) {
    
    unsigned int val = 0 ;
    const unsigned int thirtyThree = 33 ;  // magic number from textbook
    
    int i = 0 ;
    while (str[i] != '\0') {
        val = val * thirtyThree + str[i] ;
        i++ ;
    }
    return val ;
}

// copying string from one table to another table
// num = 0, index increase
// num = 1, index decrease
void HashTable::insertStr(char** table1, char** table2, int index, int num){
	while(table1[index] != NULL){
		if(table1[index] != DELETED){
			unsigned int newIndex = hashCode(table1[index]);
			newIndex = newIndex % Capacity2;

			while(table2[newIndex] != NULL){
			
					// mod will let array going around with out hit out of range
					newIndex = (newIndex + 1) % Capacity2;
				}
	
			table2[newIndex] = strdup(table1[index]);

			Size2++;
			free(table1[index]);
			table1[index] = NULL;
			Size1--;
		}
		if(num == 0)
			index = (index + 1) % Capacity1;	
		
		else{
			index = (index - 1) % Capacity1;	
			// in c++ mod a negative number will give you a negative number 
			// which will be out of range. add Capacity to let it out to last index of array
			if(index < 0)			
				index = index + Capacity1;
		}
	}
}

// using binarySearch to find the next primes number of table Capacity
// mum = 0 is table1 and num = 1 is table2
void HashTable::setCapacity(int num, int n){
	
	if(n < 101){
    Capacity1 = 101;
  }
  else if(n > 199999){
    throw out_of_range("out of range");
  }
	
	// using binarySearch to find the primes number
	else{

		bool isTrue = false;
	
		int low = 0;
		int hight = numPrimes - 1;
		int mid = (low + hight) / 2;


		while(low <= hight){

			if(primes[mid] < n){

				low = mid + 1;
			}
			
			// if items is equal to mid index of array
			else if(primes[mid] == n){

				// check to see which table we and to set the Capacity
				if(num == 0)
					Capacity1 = primes[mid];
				else
					Capacity2 = primes[mid];
				isTrue = true;
				break;
			}
			else{
				hight = mid - 1;
			}
			mid = (low + hight) / 2;
		}
		
		// if none of the mid index is equal to give number
		// set Capacity to low
		if(!isTrue){
			if(num == 0)
				Capacity1 = primes[low];	
			else{
				Capacity2 = primes[low];	
				if(Capacity2 == Capacity1)
						Capacity2 = primes[low+1];
			}
		}
	}
}

void HashTable::oneTable(){
		int n = (Size1 + Size2 + 1) * 4;
					
		int tempCapacity = Capacity1;
		Size1 = 0;
				
				setCapacity(0, n);

				char** newTable = (char**)malloc(Capacity1 * sizeof(char*));
				for(int i = 0; i < Capacity1; i++){
					newTable[i] = NULL;
				}				
		
				for(int i = 0; i < tempCapacity; i++){

					if(table1[i] != NULL && table1[i] != DELETED){

						unsigned int index3 = hashCode(table1[i]);
						index3 = index3 % Capacity1;
						while(newTable[index3] != NULL){
							index3 = (index3 + 1) % Capacity1;
						}
						newTable[index3] = strdup(table1[i]);
						Size1++;
				//		cout << "Size: " << Size1 << "Index: " << index3 <<  " : " << table1[i] << endl;
					}
					free(table1[i]);
					table1[i] = NULL;
				}
				free(table1);
				
				for(int i = 0; i < Capacity2; i++){

					if(table2[i] != NULL && table2[i] != DELETED){

						unsigned int index3 = hashCode(table2[i]);
						index3 = index3 % Capacity1;

						while(newTable[index3] != NULL){
							index3 = (index3 + 1) % Capacity1;
						}
						newTable[index3] = strdup(table2[i]);
						Size1++;
					}
					free(table2[i]);
					table2[i] = NULL;
				}	
				free(table2);
				table1 = newTable;
				reHash = false;
}
