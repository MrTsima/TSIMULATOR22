//

//

#ifndef TSIMULATOR_EMULATOR_H
#define TSIMULATOR_EMULATOR_H
#include<bits/stdc++.h>
using namespace std;
enum instructions{
    ALU=0,
    LOAD=1,
    STORE=2,
    DEFINE=3,
    BRANCH=4,
    JUMP=5,
    CALL=6,
    RET=7,
    END_DEF=8,
    FUNCTION=9,
    RESET=10,
    PRINT=11,
    ASSIGNMENT=12
};

extern bool file_processing(); // function to open and read the input file
extern bool parse_file(ifstream &file); // function to parse the input file line by line
extern bool check_eol(string &line); // helper function to check if a line ends with a valid end of line character
extern bool workline(string &str); // helper function to process a line of assembly code
extern bool isALUer(const string& s); // helper function to check if a given instruction is an arithmetic/logic operation
extern bool is_constant(const string& s); // helper function to check if a given token is a constant value
extern bool loadStore(string s); // helper function to check if a given instruction is a memory load/store operation
extern bool isBranch(string s); // helper function to check if a given instruction is a branch/jump/call operation
extern bool isBranchHelper(const string& prefix); // helper function to check if a given prefix is a valid branch instruction
extern bool pcRelative(string s); // helper function to check if a given instruction is a PC-relative memory access operation
extern bool is_register(string reg); // helper function to check if a given string is a valid register name
extern bool funCall(string s); // helper function to check if a given instruction is a function call
extern bool isLegal(string s); // helper function to check if a given string is a valid instruction
extern bool isJMP(string s); // helper function to check if a given instruction is a jump instruction
extern bool isBranchPassed(string s, int x, int y); // helper function to check if a branch condition is satisfied
extern void reseter(); // helper function to reset the emulator state
extern void ALUer(const string& s); // function to perform an arithmetic/logic operation
extern void loader(string s); // function to load data from memory to a register
extern void storer(const string& s); // function to store data from a register to memory
extern void assigner(string s); // function to assign a value to a register
extern void brancher(const string& s); // function to perform a branch operation
extern void jumper(const string& s); // function to perform a jump operation
extern void caller(const string& s); // function to perform a function call operation
extern void definer(string s); // function to define a new function
extern void printer(const string& s); // function to print a value to the console
extern void reter(); // function to perform a return operation
extern int checker(const string& left, string right, const string& line); // helper function to check if a given line of code is well-formed
extern int assi(string s); // helper function to parse and evaluate an assignment statement
extern int containsAluOper(const string& s); // helper function to find and return the index of an arithmetic/logic operator in a given string
extern int what_copy(string s); // helper function to parse and evaluate a PC-relative memory access operation
extern int addresser(string x); // helper function to parse and evaluate a memory address
extern int caster(int gela,int x); // helper function to cast an integer value to a char value
extern int addressHelper(char a,int b, int c); // helper function to construct a memory address from its component parts
extern int doALU(int x,int y, char z); // helper function to perform an arithmetic/logic operation on two values, based on a given operator

extern char STACK[8192];// This line declares a character array named STACK with a size of 8192.
// This array is used as a stack in the emulator program to keep track of function calls and returns.

extern unordered_map<string, int> registers;// This line declares an unordered map named registers with keys of type string and values of type int.
// This map is used to store and manipulate the values of various registers in the emulator program

extern unordered_map<string, int> functions;// This line declares another unordered map named functions with keys of type string and values of type int.
// This map is used to store the addresses of various functions in the emulator program.

extern vector<string> todo;// This line declares a vector named to do with elements of type string.
// This vector is used to store the names of all the functions that have been defined in the input file, so that they can be called later on in the program.

#endif //TSIMULATOR_EMULATOR_H


