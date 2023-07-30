//
// Created by mrtsima on 4/21/23.
//

#include "Utils.h"
#include <string>
#include "ALU.h"
#include "Memory.h"
#include "emulator.h"


/**
 * @brief The function isLegal takes a string s as input and checks if it's a legal function call.
 * @param s the string to check
 * @return If s is a legal function call, it returns true, otherwise it prints a message
 * and returns false.
 */
bool Utils::isLegal(string s) {
    // Check if the function name is in the set of valid functions
    if (functions.find(s.substr(1, s.size() - 2)) != functions.end()) {
        // Check if the function call is in the form <function_name>
        return (s[0] == '<' && s.back() == '>');
    }
    // If the function call is not legal, print an error message and return false
    printf("No such function (: ");
    return 0;
}


// Checks if the given string is a function call statement
bool Utils::funCall(string s){
    // Check if the first four characters of the string are "CALL"
    // and the remaining characters are legal
    return (s.substr(0, 4) == "CALL"&&(isLegal(s.substr(4, s.length()-4))));
}



// Check if the instruction is a JMP
bool Utils::isJMP(string jmp){
    // Check if the instruction starts with "JMP"
    if(jmp.substr(0, 3) != "JMP")
        return false;
    // Remove "JMP" from the instruction
    jmp = jmp.substr(3, jmp.size()-3);
    // Check if the remaining part is a constant or a pc-relative address
    return (ALU::is_constant(jmp) || pcRelative(jmp));
}


// This function checks if the given string prefix is a valid branch instruction
bool Utils::isBranchHelper(const string& prefix){
    return (prefix == "BGT" || // greater than
            prefix == "BEQ" || // equal
            prefix == "BNE" || // not equal
            prefix == "BLT" || // less than
            prefix == "BLE" || // less than or equal
            prefix == "BGE" // greater than or equal
    );
}

/**
 * Checks if the given string is a PC-relative branch.
 * A PC-relative branch can be a PC register or a constant added or subtracted to the PC register.
 *
 * @param s The string to be checked.
 * @return True if s is a PC-relative branch, false otherwise.
 */
bool Utils::pcRelative(string s){
    if(s == "PC") // If s is the PC register, it is a PC-relative branch
        return 1;
    if(s.substr(0, 2) == "PC" && (s[2] == '+'|| s[2] == '-')) // If s starts with PC and has '+' or '-' after it, check the remaining string for a valid constant
        return ALU::is_constant(s.substr(3, s.size()-3));
    return 0; // If none of the above conditions are satisfied, s is not a PC-relative branch
}



/**
This function checks the type of operation being performed in the given line of code based on the contents of the left and right operands.

@param left: A constant reference to the left operand.
@param right: A string containing the right operand.
@param line: A string containing the entire line of code.

@return: An integer representing the type of operation being performed. Possible values are:

BRANCH: if the line is a branch instruction.
JUMP: if the line is a jump instruction.
CALL: if the line is a function call instruction.
RET: if the line is a return instruction.
RESET: if the line is a reset instruction.
PRINT: if the line is a print instruction.
DEFINE: if the line is a define instruction.
LOAD: if the operation being performed is loading data from memory.
ALU: if the operation being performed is an arithmetic or logic operation.
ASSIGNMENT: if the operation being performed is a simple assignment.
STORE: if the operation being performed is storing data to memory.
-1: if the operation cannot be determined.
*/
int Utils::checker(const string& left, string right, const string& line){
    // Check if either left or right operand is empty, in which case the operation type is
    // determined based on the entire line of code.
    if(left.empty() || right.empty()){
        if(isBranch(line))
            return BRANCH;
        if(isJMP(line))
            return JUMP;
        if(funCall(line))
            return CALL;
        if(line == "RET")
            return RET;
        if(line == "RESET")
            return RESET;

        if(line.substr(0, 5) == "PRINT" ){
            return PRINT;
        }
        if(line.substr(0, 6) == "DEFINE")
            return DEFINE;


        return -1;
    }
    // If left operand is a register, determine the operation type based on the right operand.
    if(ALU::is_register(left)){
        if(what_copy(line) != 4)
            right = right.substr(2, right.size()-2);

        if(Memory::loadStore(right))
            return LOAD;
        if(ALU::isALUer(right))
            return ALU;
        if(right[0] == '-')
            right = right.substr(1, right.size()-1);
        if(ALU::is_constant(right) || ALU::is_register(right))
            return ASSIGNMENT;
    }
        // If left operand is a memory location, determine the operation type based on the right operand.
    else if(Memory::loadStore(left))
        if(ALU::is_constant(right) || ALU::is_register(right))
            return STORE;
    return -1;
}

/**

Checks if the given string represents a valid branch instruction with two valid registers/constants
and a valid constant or a pc-relative branch as the last part.
@param s The string to be checked for branch instruction
@return True if the given string is a valid branch instruction with two valid registers/constants and a
valid constant or a pc-relative branch as the last part, false otherwise.
*/
bool Utils::isBranch(const string& s){
// Check if the string is at least 3 characters long
    if(s.size() < 3){
        return false;
    }
// Check if the first three characters are a valid branch instruction
    if(!isBranchHelper(s.substr(0, 3))){
        return false;
    }
// Find the two registers or constants and check if they are valid
    string str = s.substr(3, s.size()-3);
    int fr = str.find(',');
    int sc = str.rfind(',');
    if(fr == -1 || sc == -1|| fr == sc){
        return false;
    }
    string s1 = str.substr(0, fr);
    string s2 = str.substr(fr+1, sc - fr - 1);
    if(!(ALU::is_register(s1) && ALU::is_register(s2))
       && !(ALU::is_constant(s1) && ALU::is_constant(s2))
       && !(ALU::is_register(s1) && ALU::is_constant(s2))
       && !(ALU::is_register(s2) && ALU::is_constant(s1))){
        return false;
    }
// Check if the last part is a constant or a pc-relative branch
    string lastPart = str.substr(sc+1, str.size()-sc-1);
    return ALU::is_constant(lastPart) || pcRelative(lastPart);
}
