//

//
#include <bits/stdc++.h>
using namespace std;


 char STACK[8192];// This line declares a character array named STACK with a size of 8192.
// This array is used as a stack in the emulator program to keep track of function calls and returns.

 unordered_map<string, int> registers;// This line declares an unordered map named registers with keys of type string and values of type int.
// This map is used to store and manipulate the values of various registers in the emulator program

 unordered_map<string, int> functions;// This line declares another unordered map named functions with keys of type string and values of type int.
// This map is used to store the addresses of various functions in the emulator program.

 vector<string> todo;// This line declares a vector named to do with elements of type string.
// This vector is used to store the names of all the functions that have been defined in the input file, so that they can be called later on in the program.


