// File: Graph.cpp
// Zhaojie Jiang
// CMSC 341 Spring 2017
//

#include <stdexcept>   // for throwing out_of_range exceptions
#include <utility>     // for pair template
#include "Graph.h"
#include <iostream>

using namespace std;

// Graph contructor, must give number of vertices
Graph::Graph(int n){
	
	m_size = n; //size of array
	
	// throw a out of range exception
	if (m_size <= 0)
		throw std::out_of_range ("Out of Range");

	// create a pointer to a array of linkedList of pointer.
	m_adjLists = new AdjListNode*[m_size];

	// For loop that create a array of linkedList pointer for each vertice
	for(int i = 0; i < m_size; i++){
		m_adjLists[i] = NULL;
	}
	
}

// Graph copy contructor
Graph::Graph(const Graph& G){

	// Check if the list is empty
	if(G.m_adjLists == NULL){

		cout << "empty list " << endl;
		return;
	}

	// Deep copy of size of array
	m_size = G.m_size;

	// Create a new pointer to a dynamically allocated
	// array of linked Lists of AdjListNodes
	m_adjLists = new AdjListNode*[m_size];

	// for loop go through each index of array
	for(int i = 0; i < m_size; i++){

		m_adjLists[i] = NULL;  // set the head of linked list to NULL

		AdjListNode* ptr = G.m_adjLists[i]; // set a pointer to the head of linked list
		AdjListNode* current = NULL; // create pointer

		// create a new node and with value
		current = new AdjListNode(ptr->m_vertex, NULL);

		m_adjLists[i] = current;  // add node to the linked list of i index of array

		// check if the next pointer is NULL
		// if not move to next node and add the value to new node
		// Then add new node to the linked List of i index of array
		while(ptr->next != NULL){

				ptr = ptr->next;
				current = new AdjListNode(ptr->m_vertex, NULL);
				current->next = m_adjLists[i];
				m_adjLists[i] = current;
		}
	}
}

// Graph assignment operator
const Graph& Graph::operator=(const Graph& rhs){

	// check for self assignment
	if(this == &rhs)
		return *this;
	
	else{

		// for loop through index of array
		for(int i = 0; i < m_size; i++){

			AdjListNode* deletePtr = m_adjLists[i];
			AdjListNode* temp = NULL;

			// while loop through each node in linked list
			while(deletePtr != NULL){

			temp = deletePtr->next;
				delete deletePtr;
				deletePtr = temp;
			}

			m_adjLists[i] = NULL;
		}
		
		delete m_adjLists; // delete the array of linkedList
		m_adjLists = NULL;
		
		// set current value in the Graph class equal to pass in Graph class

		// Check if the list is empty
		if(rhs.m_adjLists == NULL){

			cout << "empty list " << endl;
			return *this;
		}

		// Deep copy of size of array
		m_size = rhs.m_size;

		// Create a new pointer to a dynamically allocated
		// array of linked Lists of AdjListNodes
		m_adjLists = new AdjListNode*[m_size];

		// for loop go through each index of array
		for(int i = 0; i < m_size; i++){

			m_adjLists[i] = NULL;  // set the head of linked list to NULL

			AdjListNode* ptr = rhs.m_adjLists[i]; // set a pointer to the head of linked list
			AdjListNode* current = NULL; // create pointer

			// create a new node and with value
			current = new AdjListNode(ptr->m_vertex, NULL);

			m_adjLists[i] = current;  // add node to the linked list of i index of array

			// check if the next pointer is NULL
			// if not move to next node and add the value to new node
			// Then add new node to the linked List of i index of array
			while(ptr->next != NULL){

				ptr = ptr->next;
				current = new AdjListNode(ptr->m_vertex, NULL);
				current->next = m_adjLists[i];
				m_adjLists[i] = current;
			}
		}
		
		return *this;
	}
}

// Graph destructor
Graph::~Graph(){

	// for loop through index of array
	for(int i = 0; i < m_size; i++){

		AdjListNode* deletePtr = m_adjLists[i];
		AdjListNode* temp = NULL;

		// while loop through each node in linked list
		while(deletePtr != NULL){

			temp = deletePtr->next;
			delete deletePtr;
			deletePtr = temp;
		}

		m_adjLists[i] = NULL;
	}

//	m_adjLists = NULL;
	delete[] m_adjLists; // delete the array of linkedList
	m_adjLists = NULL;
}

// return number of vertices
int Graph::size(){

	return m_size;
}

// add edge between u and v
void Graph::addEdge(int u, int v){
	
	// check out of range for both u and v edge
	if((u < 0 || u >= m_size) || (v < 0 || v >= m_size)){

		throw std::out_of_range("Either u and/or v are out of range");
	}

	AdjListNode *headU, *headV;

	// create NULL header pointer to index of v and u of array of linkedList
	headU = m_adjLists[u];
	headV = m_adjLists[v];

	// check if the list is empty
	if(headU == NULL){

		m_adjLists[u] = new AdjListNode(v);
		m_adjLists[u]->next = NULL;
	}
	else{

		// add the edge at the beginning of list.
		m_adjLists[u] = new AdjListNode(v);
		m_adjLists[u]->next = headU;
	}

	// check if the list is empty
	if(headV == NULL){

		m_adjLists[v] = new AdjListNode(u);
		m_adjLists[v]->next = NULL;
	}
	
	else{

		// add the edge at the beginning of list.
		m_adjLists[v] = new AdjListNode(u);
		m_adjLists[v]->next = headV;
	}
}

// print out data structure for debugging
void Graph::dump(){

	AdjListNode* list;

	cout << "Dump out graph (size = " << m_size << ")" << endl;

	// loop through array
	for (int i = 0; i < m_size; i++){
		
		list = m_adjLists[i];

		cout << "[" << i << "]: ";

		// loop through the linked list till NULL pointer is hit.
		while (list != NULL){

			cout << list->m_vertex << " ";
			list = list->next;
		}
		cout << endl;
	}
}

// Edge Iterator inner class

// Edge Iterator constructor, isEnd true to create end iterator
Graph::EgIterator::EgIterator(Graph *Gptr, bool isEnd){

	m_Gptr = Gptr;

	// if Graph is empty  
	if(m_Gptr == NULL){  
		return;
	}
	
	// not at the end of list  
	else if (isEnd == false){
		
		m_source = 0;
		m_where = m_Gptr->m_adjLists[m_source];
		
		// check if the first array index of linked list is empty
		while(m_where == NULL){
				m_source += 1;
				m_where = m_Gptr->m_adjLists[m_source];
			}
	}
	
	// at the end of the list
	else{
		m_source = m_Gptr->m_size;
		m_where = NULL;
	}
}

// Compare iterators, only makes sense to compare with end iterator
bool Graph::EgIterator::operator != (const EgIterator& rhs){

	return (m_Gptr != rhs.m_Gptr || m_where != NULL 
					|| m_source != rhs.m_Gptr->m_size-1);
}

// Move iterator to next printable edge
void Graph::EgIterator::operator++(int dummy){

	// check if pointer is at the end
	if(m_where->next == NULL)
	{
		// move to next index in the array
		if(m_source < m_Gptr->m_size-1){
			
			m_source += 1;
			m_where = m_Gptr->m_adjLists[m_source];
			while(m_where == NULL){
				m_source += 1;
				m_where = m_Gptr->m_adjLists[m_source];
			}
		}
	// end of the Graph
		else{
			m_source = m_Gptr->m_size;
			m_where = NULL;
		}        
	}

	// move to next position in the linked list 
	else
		m_where = m_where->next;

	// check if the next pair already showed  
	while(m_where != NULL && m_source > m_where->m_vertex){
		
		m_where = m_where->next; //move to next position

		// if it is at the end of linked list
		// increase index by on in array
		if(m_source != m_Gptr->m_size-1 && m_where == NULL){
			
			m_source += 1;
			m_where = m_Gptr->m_adjLists[m_source];
			
			while(m_where == NULL){
				m_source += 1;
				m_where = m_Gptr->m_adjLists[m_source];
			}
		}
	}
}

// return edge at iterator location
std::pair<int, int> Graph::EgIterator::operator*(){

	if (m_where == NULL) 
			throw out_of_range("EgIterator dereference error.") ;

	return std::pair<int,int>(m_source, m_where->m_vertex) ;
}

// Make an initial neigbor iterator
Graph::EgIterator Graph::egBegin(){

	return EgIterator(this, false);
}

// Make an end eighbor iterator
Graph::EgIterator Graph::egEnd(){

	return EgIterator(this, false);
}

// Neighbor Iterator inner class

// Constructor for iterator for vertices adjacent to vertex v
Graph::NbIterator::NbIterator(Graph *Gptr, int v, bool isEnd){

	m_Gptr = Gptr;

	// initial Graph
	if(v == -1){

	}

	// check if it is at the end of linked list
	else if(isEnd == false){
		m_source = v;
		m_where = m_Gptr->m_adjLists[m_source];
	}
	// at the end of the linked list
	else{
		
		m_where = NULL;
		m_source = m_Gptr->m_size;
	}
}

// Compare iterators, only makes sense to compare with end iterator
bool Graph::NbIterator::operator!= (const NbIterator& rhs){

	return (m_where != NULL || m_source != rhs.m_source|| m_Gptr != rhs.m_Gptr);

}

// Move iterator to next neighbor
void Graph::NbIterator::operator++ (int dummy){

	// move to next pointer 
	if (m_where != NULL) {

		m_where = m_where->next;
	}
}

// Return neighbor at current iterator position
int Graph::NbIterator::operator*(){

	if (m_where == NULL){

		throw out_of_range("NbIterator dereference error.");
	}

	return m_where->m_vertex;
}

// Make an initial neghbor iterator
// call the NbIterator constructor to create iterators that can be
// used in for loops to iterate through the neighbors of vertex v
Graph::NbIterator Graph::nbBegin(int v){

	return NbIterator(this, v);
}

// Make an end neighbor iterator
// call the NbIterator constructor to create iterators that can be
// used in for loops to iterate through the neighbors of vertex v
Graph::NbIterator Graph::nbEnd(int v){

	return NbIterator(this, v);
}

//Private inner Node Class for linked list for adjacency lists

//contruct a node
Graph::AdjListNode::AdjListNode(int v, AdjListNode *prt){

	m_vertex = v;
	next = prt;
}



