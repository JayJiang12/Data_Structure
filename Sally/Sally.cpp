// File: Sally.cpp
//
// CMSC 341 Spring 2017 Project 2
//
// Implementation of member functions of Sally Forth interpreter
//

#include <iostream>
#include <string>
#include <list>
#include <stack>
#include <stdexcept>
#include <cstdlib>
using namespace std ;

#include "Sally.h"

bool loop = false;

// Basic Token constructor. Just assigns values.
//
Token::Token(TokenKind kind, int val, string txt) {
   m_kind = kind ;
   m_value = val ;
   m_text = txt ;
}


// Basic SymTabEntry constructor. Just assigns values.
//
SymTabEntry::SymTabEntry(TokenKind kind, int val, operation_t fptr) {
   m_kind = kind ;
   m_value = val ;
   m_dothis = fptr ;
}


// Constructor for Sally Forth interpreter.
// Adds built-in functions to the symbol table.
//
Sally::Sally(istream& input_stream) :
  istrm(input_stream)  // use member initializer to bind reference
{  
  symtab["DUMP"]    =  SymTabEntry(KEYWORD,0,&doDUMP) ;

  symtab["+"]    =  SymTabEntry(KEYWORD,0,&doPlus) ;
  symtab["-"]    =  SymTabEntry(KEYWORD,0,&doMinus) ;
  symtab["*"]    =  SymTabEntry(KEYWORD,0,&doTimes) ;
  symtab["/"]    =  SymTabEntry(KEYWORD,0,&doDivide) ;
  symtab["%"]    =  SymTabEntry(KEYWORD,0,&doMod) ;
  symtab["NEG"]  =  SymTabEntry(KEYWORD,0,&doNEG) ;

  symtab["."]    =  SymTabEntry(KEYWORD,0,&doDot) ;
  symtab["SP"]   =  SymTabEntry(KEYWORD,0,&doSP) ;
  symtab["CR"]   =  SymTabEntry(KEYWORD,0,&doCR) ;
  
  //comparison operations
  symtab["<"]    =  SymTabEntry(KEYWORD,0,&doLess) ;
  symtab[">"]    =  SymTabEntry(KEYWORD,0,&doGreater) ;
  symtab["<="]    =  SymTabEntry(KEYWORD,0,&doLessEqual) ;
  symtab[">="]    =  SymTabEntry(KEYWORD,0,&doGreaterEqual) ;
  symtab["=="]    =  SymTabEntry(KEYWORD,0,&doEqual) ;
  symtab["!="]    =  SymTabEntry(KEYWORD,0,&doNotEqual) ;
  
  // Stack operations
  symtab["DUP"]    =  SymTabEntry(KEYWORD,0,&doDUP) ;
  symtab["DROP"]    =  SymTabEntry(KEYWORD,0,&doDROP) ;
  symtab["SWAP"]    =  SymTabEntry(KEYWORD,0,&doSWAP) ;
  symtab["ROT"]    =  SymTabEntry(KEYWORD,0,&doROT) ;
  
  // Variable operations
  symtab["SET"]    =  SymTabEntry(KEYWORD,0,&doSET) ;
  symtab["@"]    =  SymTabEntry(KEYWORD,0,&doAt) ;
  symtab["!"]    =  SymTabEntry(KEYWORD,0,&doStore) ;
  
  // Logic operations
  symtab["AND"]    =  SymTabEntry(KEYWORD,0,&doAND) ;
  symtab["OR"]    =  SymTabEntry(KEYWORD,0,&doOR) ;
  symtab["NOT"]    =  SymTabEntry(KEYWORD,0,&doNOT) ;
  
  // If statement
  symtab["IFTHEN"]    =  SymTabEntry(KEYWORD,0,&doIFTHEN) ;
  symtab["ELSE"]    =  SymTabEntry(KEYWORD,0,&doELSE) ;
  symtab["ENDIF"]    =  SymTabEntry(KEYWORD,0,&doENDIF) ;
  
  // Loop construct
  symtab["DO"]    =  SymTabEntry(KEYWORD,0,&doDO) ;
  symtab["UNTIL"]    =  SymTabEntry(KEYWORD,0,&doUNTIL) ;
  
}


// This function should be called when tkBuffer is empty.
// It adds tokens to tkBuffer.
//
// This function returns when an empty line was entered 
// or if the end-of-file has been reached.
//
// This function returns false when the end-of-file was encountered.
// 
// Processing done by fillBuffer()
//   - detects and ignores comments.
//   - detects string literals and combines as 1 token
//   - detetcs base 10 numbers
// 
//
bool Sally::fillBuffer() {
  string line ;     // single line of input
  int pos ;         // current position in the line
  int len ;         // # of char in current token
  long int n ;      // int value of token
  char *endPtr ;    // used with strtol()


  while(true) {    // keep reading until empty line read or eof

    // get one line from standard in
    //
    getline(istrm, line) ;   

    // if "normal" empty line encountered, return to mainLoop
    //
    if ( line.empty() && !istrm.eof() ) {
       return true ;
    }

    // if eof encountered, return to mainLoop, but say no more
    // input available
    //
    if ( istrm.eof() )  {
      return false ;
    }


    // Process line read

    pos = 0 ;                      // start from the beginning

    // skip over initial spaces & tab
    //
    while( line[pos] != '\0' && (line[pos] == ' ' || line[pos] == '\t') ) {
      pos++ ; 
    }

    // Keep going until end of line
    //
    while (line[pos] != '\0') {

      // is it a comment?? skip rest of line.
      //
      if (line[pos] == '/' && line[pos+1] == '/') break ;

      // is it a string literal? 
      //
      if (line[pos] == '.' && line[pos+1] == '"') {

        pos += 2 ;  // skip over the ."
        len = 0 ;   // track length of literal

        // look for matching quote or end of line
        //
        while(line[pos+len] != '\0' && line[pos+len] != '"') {
          len++ ;
        }

        // make new string with characters from
        // line[pos] to line[pos+len-1]
        string literal(line,pos,len) ;  // copy from pos for len chars

        // Add to token list
        //
        tkBuffer.push_back( Token(STRING,0,literal) ) ;  

        // Different update if end reached or " found
        //
        if (line[pos+len] == '\0') {
           pos = pos + len ;
        } else {
           pos = pos + len + 1 ;
        }
      
      } else {  // otherwise "normal" token

        len = 0 ;  // track length of token

        // line[pos] should be an non-white space character
        // look for end of line or space or tab
        //
        while(line[pos+len] != '\0' && line[pos+len] != ' ' && line[pos+len] != '\t') {
           len++ ;
        }

        string literal(line,pos,len) ;   // copy form pos for len chars
        pos = pos + len ;

        // Try to convert to a number
        //
        n = strtol(literal.c_str(), &endPtr, 10) ;

        if (*endPtr == '\0') {
          tkBuffer.push_back( Token(INTEGER,n,literal) ) ;
        } else {
          tkBuffer.push_back( Token(UNKNOWN,0,literal) ) ;
        }
      }

      // skip over trailing spaces & tabs
      //
      while( line[pos] != '\0' && (line[pos] == ' ' || line[pos] == '\t') ) {
        pos++ ; 
      }
    }
  }
}



// Return next token from tkBuffer.
// Call fillBuffer() if needed.
// Checks for end-of-file and throws exception 
//
Token Sally::nextToken() {
  Token tk ;
  bool more = true ;

  while(more && tkBuffer.empty() ) {
    more = fillBuffer() ;
  }

  if ( !more && tkBuffer.empty() ) {      
    throw EOProgram("End of Program") ;
  }
  
  tk = tkBuffer.front() ;
  tkBuffer.pop_front() ;
  
  if(loop == true) 
   listLoop.push_back(tk) ;

  return tk ;
}


// The main interpreter loop of the Sally Forth interpreter.
// It gets a token and either push the token onto the parameter
// stack or looks for it in the symbol table.
//
//
void Sally::mainLoop() {

  Token tk ;
  map<string,SymTabEntry>::iterator it ;

  try {
    while( 1 ) {
      tk = nextToken() ;

      if (tk.m_kind == INTEGER || tk.m_kind == STRING) {

        // if INTEGER or STRING just push onto stack
        params.push(tk) ;

      } else {
      
        it = symtab.find(tk.m_text) ;

        if ( it == symtab.end() )  {   // not in symtab
          
          params.push(tk) ;

        } else if (it->second.m_kind == KEYWORD)  {

          // invoke the function for this operation
          //
          it->second.m_dothis(this) ;   
        
        } else if (it->second.m_kind == VARIABLE) {

          // variables are pushed as tokens
          //
          tk.m_kind = VARIABLE ;
          params.push(tk) ;

        } else {
          
          // default action
          //
          params.push(tk) ;
        }
      }  
    }
    
  } catch (EOProgram& e) {
    
    cerr << "End of Program\n" ;
    if ( params.size() == 0 ) {
      
      cerr << "Parameter stack empty.\n" ;
      
    } else {
      
      cerr << "Parameter stack has " << params.size() << " token(s).\n" ;  
    }
   
  } catch (out_of_range& e) {
 
    cerr << "Parameter stack underflow??\n" ;
  
  } catch (...) {

    cerr << "Unexpected exception caught\n" ;
  }
}

// -------------------------------------------------------


void Sally::doPlus(Sally *Sptr) {
  Token p1, p2 ;

  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for +.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  int answer = p2.m_value + p1.m_value ;
  Sptr->params.push( Token(INTEGER, answer, "") ) ;
}


void Sally::doMinus(Sally *Sptr) {
  Token p1, p2 ;

  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for -.") ; 
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  int answer = p2.m_value - p1.m_value ;
  Sptr->params.push( Token(INTEGER, answer, "") ) ;
}


void Sally::doTimes(Sally *Sptr) {
  Token p1, p2 ;

  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for *.") ; 
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  int answer = p2.m_value * p1.m_value ;
  Sptr->params.push( Token(INTEGER, answer, "") ) ;
}


void Sally::doDivide(Sally *Sptr) {
  Token p1, p2 ;
 
  if ( Sptr->params.size() < 2 ) { 
    throw out_of_range("Need two parameters for /.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  int answer = p2.m_value / p1.m_value ;
  Sptr->params.push( Token(INTEGER, answer, "") ) ;
}


void Sally::doMod(Sally *Sptr) {
  Token p1, p2 ;
  
  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for %.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  int answer = p2.m_value % p1.m_value ;
  Sptr->params.push( Token(INTEGER, answer, "") ) ;
}


void Sally::doNEG(Sally *Sptr) {
  Token p ;

  if ( Sptr->params.size() < 1 ) {
    throw out_of_range("Need one parameter for NEG.") ;
  }
  p = Sptr->params.top() ;
  Sptr->params.pop() ;
  Sptr->params.push( Token(INTEGER, -p.m_value, "") ) ;
}


void Sally::doDot(Sally *Sptr) { 
  Token p ;
  
  if ( Sptr->params.size() < 1 ) {      
    throw out_of_range("Need one parameter for .") ;
  }

  p = Sptr->params.top() ;
  Sptr->params.pop() ;

  if (p.m_kind == INTEGER) {
    cout << p.m_value ;
  } else {  
    cout << p.m_text ;
  }
}


void Sally::doSP(Sally *Sptr) {
  cout << " " ;
}


void Sally::doCR(Sally *Sptr) {
  cout << endl ;
}

void Sally::doDUMP(Sally *Sptr) {
   // do whatever for debugging
}

// Comparison operations
//

// less than
void Sally::doLess(Sally *Sptr) {
  Token p1, p2 ;
  
  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for <.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  
  // check if it is less than 
  if(p2.m_value < p1.m_value)
    Sptr->params.push( Token(INTEGER, 1, "") ) ;
  else
    Sptr->params.push( Token(INTEGER, 0, "false") ) ;
}

// greater than
void Sally::doGreater(Sally *Sptr) {
  Token p1, p2 ;
  
  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for >.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
    
  // check if it is greate than
  if(p2.m_value > p1.m_value)
    Sptr->params.push( Token(INTEGER, 1, "") ) ;
  else
    Sptr->params.push( Token(INTEGER, 0, "") ) ;
}


// less than and equal to 
void Sally::doLessEqual(Sally *Sptr) {
  Token p1, p2 ;
  
  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for <=.") ;
  }
  
  // pop off the stack
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  
  // check if it is less than and equal to 
  if(p2.m_value <= p1.m_value)
    Sptr->params.push( Token(INTEGER, 1, "") ) ;
  else
    Sptr->params.push( Token(INTEGER, 0, "") ) ;
}

// greater than and equal to 
void Sally::doGreaterEqual(Sally *Sptr) {
  Token p1, p2 ;
  
  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for >=.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  
  // check if it is greater than and equal to 
  if(p2.m_value >= p1.m_value)
    Sptr->params.push( Token(INTEGER, 1, "") ) ;
  else
    Sptr->params.push( Token(INTEGER, 0, "") ) ;
}

// equal to 
void Sally::doEqual(Sally *Sptr) {
  Token p1, p2 ;
  
  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for ==.") ;
  }
  
  // pop off the stack
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  
  // check if it is equal to 
  if(p2.m_value == p1.m_value)
    Sptr->params.push( Token(INTEGER, 1, "") ) ;
  else
    Sptr->params.push( Token(INTEGER, 0, "") ) ;
}

// not equal to 
void Sally::doNotEqual(Sally *Sptr) {
  Token p1, p2 ;
  
  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for !=.") ;
  }
  
  // pop off the stack
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  
  // check if it is not euqal to 
  if(p2.m_value != p1.m_value)
    Sptr->params.push( Token(INTEGER, 1, "") ) ;
  else
    Sptr->params.push( Token(INTEGER, 0, "") ) ;
}


// Stack operations
//

// make a copy of hte top item
void Sally::doDUP(Sally *Sptr) {
  Token p;
  
  if ( Sptr->params.size() < 1 ) {
    throw out_of_range("Need one parameters for DUP.") ;
  }
  
  // pop off the stack 
  p = Sptr->params.top() ;
  
  // check if input is integer of string
  if(p.m_kind == INTEGER)
    Sptr->params.push( Token(INTEGER, p.m_value, "") ) ; 
  else
    Sptr->params.push( Token(STRING, 0, p.m_text) ) ; 
}


// drop the top item of the parameter stack
void Sally::doDROP(Sally *Sptr) {
  
  if ( Sptr->params.size() < 1 ) {      
    throw out_of_range("Need one parameter for DROP") ;
  }
  
  // pop top off stack
  Sptr->params.pop() ;
}


// exchanges the op two items of the parameter stack
void Sally::doSWAP(Sally *Sptr) {
  
  Token p1, p2 ;
  
  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for SWAP.") ;
  }
  
  // pop two items of the stack
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  
  // swap two items and push back to stack
  if(p1.m_kind == INTEGER)
    Sptr->params.push( Token(INTEGER, p1.m_value, "") ) ; 
  else
    Sptr->params.push( Token(STRING, 0, p1.m_text) ) ;
  
  if(p2.m_kind == INTEGER)
    Sptr->params.push( Token(INTEGER, p2.m_value, "") ) ; 
  else
    Sptr->params.push( Token(STRING, 0, p2.m_text) ) ;
}


// rotates the top three items of the parameter stack
void Sally::doROT(Sally *Sptr) {
  
  Token p1, p2, p3 ;
  
  if ( Sptr->params.size() < 3 ) {
    throw out_of_range("Need three parameters for ROT.") ;
  }
  
  // pop three item of the stack
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p3 = Sptr->params.top() ;
  Sptr->params.pop() ;
  
  
  // rotates them and push the into the stack
  if(p2.m_kind == INTEGER)
    Sptr->params.push( Token(INTEGER, p2.m_value, "") ) ; 
  else
    Sptr->params.push( Token(STRING, 0, p2.m_text) ) ;
  
  if(p1.m_kind == INTEGER)
    Sptr->params.push( Token(INTEGER, p1.m_value, "") ) ; 
  else
    Sptr->params.push( Token(STRING, 0, p1.m_text) ) ;
  
  if(p3.m_kind == INTEGER)
    Sptr->params.push( Token(INTEGER, p3.m_value, "") ) ; 
  else
    Sptr->params.push( Token(STRING, 0, p3.m_text) ) ;
}
  

// Variable operations
//

// create a symbol entry fo ra variable 
// and initialize it with the value
void Sally::doSET(Sally *Sptr) {
 
  Token p1, p2 ;
  
  if ( Sptr->params.size() < 2) {
    throw out_of_range("Need two parameters for SET.") ;
  }
  
  // pop of the stack
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
 
  if (p2.m_kind != INTEGER){
    throw out_of_range("Second value in stack shoud be integer.") ;
  }

  // insert into symtab if it is not already there
  if(Sptr->symtab.count(p1.m_text) == 0)
  {
    Sptr->symtab.insert(map<string, SymTabEntry>::value_type(p1.m_text, SymTabEntry(UNKNOWN, p2.m_value, NULL))); 
  }
  else{
    cout << p1.m_text << " is already in the symtab" << endl;
  }
}

// The @ operation (pronounced "at" or "fetch") retrieves the value of a variable 
// and places that value on the parameter stack.
void Sally::doAt(Sally *Sptr) {

  Token p1;
  map<string, SymTabEntry>::iterator it;
  
  if ( Sptr->params.size() < 1) {
    throw out_of_range("Need one parameters for @.") ;
  }
  
  // pop off the stack
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;

  // push the value to the stack for the symtab
  if(Sptr->symtab.count(p1.m_text) != 0){
   
    Sptr->params.push( Token(INTEGER, Sptr->symtab.find(p1.m_text)->second.m_value, "")) ;
  }
  else{
     cout << p1.m_text << " is not in the symtab" << endl;
  }  
}

// The ! operation stores a value in a variable
void Sally::doStore(Sally *Sptr) {
  
  Token p1, p2 ;
  
  if ( Sptr->params.size() < 2) {
    throw out_of_range("Need two parameters for !.") ;
  }
  
  // pop off the stack 
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;

  // change the value in the symtab
  if(Sptr->symtab.count(p1.m_text) != 0){
    
    Sptr->symtab.erase(p1.m_text);
    Sptr->symtab.insert(map<string, SymTabEntry>::value_type(p1.m_text, SymTabEntry(UNKNOWN, p2.m_value, NULL)));
  }
  else{
     cout << p1.m_text << " is not in the symtab" << endl;
  }
}
  

// Logic operations
//

// AND operation
void Sally::doAND(Sally *Sptr) {
  
  Token p1, p2 ;
  
  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for AND.") ;
  }
  
  // get the value from the stack and pop it off
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  
  // check the it is true or false
  int answer = p2.m_value && p1.m_value;
    Sptr->params.push( Token(INTEGER, answer, "") ) ;
}

// OR operation
void Sally::doOR(Sally *Sptr) {
  
  Token p1, p2 ;
  
  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for OR.") ;
  }
  
  // get the value from the stack and pop it off
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  
  // check the it is true or false
  int answer = p2.m_value || p1.m_value;
    Sptr->params.push( Token(INTEGER, answer, "") ) ;
}

// not operation
void Sally::doNOT(Sally *Sptr) {
  
   Token p;
  
  if ( Sptr->params.size() < 1 ) {
    throw out_of_range("Need one parameters for NOT.") ;
  }
  
  // get the value from the stack and pop it off
  p = Sptr->params.top() ;
  Sptr->params.pop() ;
  
   // check the it is true or false
  int answer = !(p.m_value);
   Sptr->params.push( Token(INTEGER, answer, "") ) ;
}
  

// If statement
//
void Sally::doIFTHEN(Sally *Sptr) {

  Token p;
  
  if ( Sptr->params.size() < 1 ) {
    throw out_of_range("Need two parameters for OR.") ;
  }
  
  p = Sptr->params.top() ;
  
  if(p.m_value != 0){
    return;
  }
  
  else{
    // loop it hit ENDIF, ELSE and IFTHEN
    while(p.m_text != "ENDIF" && p.m_text != "ELSE" && p.m_text != "IFTHEN" )
    {
      p = Sptr->nextToken();
    }     
    
    // call function
    if(p.m_text == "IFTHEN")
    {
      doIFTHEN(Sptr);    
    }
    else if(p.m_text == "ELSE")
    {
      doELSE(Sptr);
    }
  }
}

// Else statement
void Sally::doELSE(Sally *Sptr) {
  Token p;
  
  if ( Sptr->params.size() < 1 ) {
    throw out_of_range("Need two parameters for OR.") ;
  }
  
  // save the value and pop off the value
  p = Sptr->params.top() ;
  Sptr->params.pop() ;
  
  if(p.m_value == 0){
    return;
  }
  else{
    // loop till it hit ENDIF
    while(p.m_text != "ENDIF"){
      
      p = Sptr->nextToken();
    }    
  }
}

// ENDIF function
void Sally::doENDIF(Sally *Sptr) {
  return;
}
 

// Loop construct
//
void Sally::doDO(Sally *Sptr) {
  
  loop = true;
}

// 
void Sally::doUNTIL(Sally *Sptr) {
  loop = false; 
  
  Token p;
  
  if ( Sptr->params.size() < 1) {
    throw out_of_range("Need one parameters.") ;
  }
  
  // get the value and pop it off from the stack
  p = Sptr->params.top() ;
  Sptr->params.pop() ;
  
  Token tk;
  
  // loop all value till it is true
  if(!p.m_value)
  {
    while(tk.m_text != "UNTIL" && tk.m_text != "DO" && tk.m_text != "IFTHEN" )
    {
      
      // reall the function 
      for (list<Token>::iterator it = Sptr->listLoop.begin(); it != Sptr->listLoop.end(); ++it)
      {
        tk = *it;
  
        Sptr->tkBuffer.push_back(tk) ;
      }
    }
  }
  else{
 
    Sptr->listLoop.clear();
  }
}