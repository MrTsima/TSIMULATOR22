/*
 * The program reads and parses a file containing a sequence of instructions
 * in assembly language, and then executes them. The program consists of several
 * functions that handle different types of instructions, such as ALU operations,
 * memory loading and storing, printing, and control flow instructions. The main function
 * continuously prompts the user for the path of the file containing the instructions,
 * parses it, and executes the instructions until the end of the file is reached.
 * The program also maintains a set of registers and memory locations to store and
 * manipulate data during the execution of the instructions.
 */

#include <bits/stdc++.h>
#include "emulator.h"
#include "ALU.h"
#include "Memory.h"
#include "Utils.h"

using namespace std;

int main(){
    while(true) {
        Memory::reseter();
        file_processing();
    }
    return 0;
}

// This function reads and parses a file
bool parse_file(ifstream &file){
    string s;
    while(getline(file, s)){
        // Remove spaces from the string
        string s1="";
        for(char i : s){
            if(i==' ') continue;
            s1+=i;
        }
        s=s1;
        // Convert all lowercase letters to uppercase
        for(char & i : s)
            if(i>='a'&&i<='z')
                i = i-'a'+'A';
        // Add the parsed line to the "todo" vector and check if it is an end-of-line
        todo.push_back(s);
        bool eol=check_eol(s);
        if(eol==0) return true;
    }
    return true;
}

// This function processes a file
bool file_processing(){
    ifstream ciner;
    bool processed=false;
    string s;
    // Ask the user for the file path
    printf("file path: ");
    while(true){
        // Read the input from the user
        getline(cin, s) ;
        // Open the file
        ciner.open(s.c_str());
        // If the file is successfully opened, parse it and close it
        if(ciner){
            processed = parse_file(ciner);
            ciner.close();
            break;
        }
        // If the file cannot be opened, ask the user to enter a valid path
        printf("Wtf, invalid path, try RIGHT one\n");
    }
    // If the file cannot be processed, return false
    if(processed==0){
        printf("Are you kidding? Who can understand this sh*t?\n");
        return 0;
    }
    // Process the instructions in the "todo" vector
    while(registers["PC"] < 4*(int)todo.size()){
        // If an error occurs while processing an instruction, return false
        if(!workline(todo[registers["PC"]/4])){
            printf( "What's wrong with you? Can't process file\n");
            return false;
        }
        // Move to the next instruction
        registers["PC"] += 4;
    }
    return true;
}

// This function checks if a line is an end-of-line
bool check_eol(string &line){
    // If the second-to-last character is not a semicolon, remove the last character and return false
    if(line[line.size()-2] != ';') {
        todo.back() = line = line.substr(0, line.size()-1);
        return 0;
    }
    // If the second-to-last character is a semicolon, remove the last two characters and return true
    todo.back() = line = line.substr(0, line.size()-2);
    return true;
}
bool workline(string &str){
    int what=-1;
    int i=assi(str);
    cout<<(registers["PC"]+4)/4<<": "<<str<<endl;

    // Check if the line is an assignment or not
    if(i==-1) what = Utils::checker("", "", str);
    else what = Utils::checker(str.substr(0, i), str.substr(i+1, str.size()-1 - i), str);

    // Execute the corresponding operation based on what the line is
    switch(what) {
        case ALU:
            ALU::ALUer(str);
            break;
        case LOAD:
            Memory::loader(str);
            break;
        case PRINT:
            printer(str);
            break;
        case RESET:
            Memory::reseter();
            break;
        case DEFINE:
            Memory::definer(str);
            break;
        case RET:
            Memory::reter();
            break;
        case CALL:
            caller(str);
            break;
        case JUMP:
            jumper(str);
            break;
        case BRANCH:
            brancher(str);
            break;
        case ASSIGNMENT:
            Memory::assigner(str);
            break;
        case STORE:
            Memory::storer(str);
            break;
        default:
            printf("not found\n");
            return 0;
    }
    return 1;
}

// This function returns the index of the character '=' in a given string, or -1 if it does not exist
int assi(string s){
    for(int i=0;i<s.size();i++)
        if(s[i]=='=')
            return i;
    return -1;
}

// This function returns 1, 2, or 4 depending on whether ".1", ".2", or neither appears in the string
int what_copy(string s){
    if(s.find(".1")!=-1)
        return 1;
    if(s.find(".2")!=-1)
        return 2;
    return 4;
}

/**

Decreases the value of SP by 4 and stores the value of PC in the memory location pointed by SP.
Sets the value of PC to the address of the function being called, obtained from the function map using the function name in the input string.
@param str Input string containing the name of the function being called in the format "CALL function_name;"
*/
void caller(const string& str){
// Decrease SP by 4
    registers["SP"] -= 4;
// Store the value of PC in the memory location pointed by SP
    *((int*)(&STACK[registers["SP"]])) = registers["PC"];
// Set the value of PC to the address of the function being called
    registers["PC"] = functions[str.substr(5,str.size()- 6)];
}

void jumper(const string& str){
    // Extract the address from the input string
    string s = str.substr(3, str.size() - 3);

    // Check if the address is in the relative address format
    if(Utils::pcRelative(s)){
        int nish=1;
        // If the offset is negative, set nish to -1 to indicate negative offset
        if(s[2] == '-')
            nish *=-1;

        // Calculate the new value of "PC" based on the relative offset and set the register to the new value
        registers["PC"] += Memory::inter(s.substr(3, s.size()-3))*nish - 4;
        return;
    }

    // If the address is a constant, set the value of "PC" to the constant value
    if(ALU::is_constant(s))
        registers["PC"] = Memory::inter(s);
}


/**
 * @brief Branches program execution to a new location if a branch condition is met
 * @param str: string containing branch instruction and operands
 */
void brancher(const string& str){
    int frs,sec,frs1,sec1;
    string jmp,fr,se,s = str.substr(0,3); // get instruction and operands
    frs = str.find(',');
    sec = str.rfind(',');
    jmp = "JMP" + str.substr(sec+1, str.size()- sec -1); // get jump location
    fr = str.substr(3, frs- 3); // get first operand
    se = str.substr(frs +1, sec - frs -1); // get second operand

    if(ALU::is_register(se)) sec1 = registers[se]; // get value of second operand
    else if(ALU::is_constant(se)) sec1 = Memory::inter(se);

    if(ALU::is_register(fr)) frs1 = registers[fr]; // get value of first operand
    else if(ALU::is_constant(fr)) frs1 = Memory::inter(fr);

    if(isBranchPassed(s, frs1, sec1)){ // check if branch condition is met
        jumper(jmp); // jump to new location if condition is met
        return ;
    }
    printf("Branch failed :( \n"); // if branch condition is not met, print error message
}


// This function determines if a branch instruction has been passed by comparing two values
// based on the specified operation (s). It returns true if the branch condition is met,
// and false otherwise.
bool isBranchPassed(string s, int x, int y){
    return ( (s == "BGE" && x >= y) || // if s is "BGE" and x is greater than or equal to y, return true
             (s == "BLE" && x <= y) || // if s is "BLE" and x is less than or equal to y, return true
             (s == "BLT" && x < y) || // if s is "BLT" and x is less than y, return true
             (s == "BNE" && x != y) || // if s is "BNE" and x is not equal to y, return true
             (s == "BEQ" && x == y) || // if s is "BEQ" and x is equal to y, return true
             (s == "BGT" && x > y)); // if s is "BGT" and x is greater than y, return true
}

/**

@brief This function prints the value of a given string that represents a constant, register or memory address.
@param str The input string.
The function first extracts the relevant part of the string by removing the surrounding characters.
If the string is a constant, it converts the string to an integer using the "Memory::inter" function.
If the string is a register, it retrieves its value from the "registers" map.
If the string is a memory address, it uses the "addresser" function to calculate the address and casts it to an int poMemory::inter.
If the string is none of the above, it outputs an error message and returns.
The function then determines the byte size of the value based on the "what_copy" function's result.
Finally, the function uses a switch statement to print the value of the string based on its byte size.
*/
void printer(const string& str){
    // Extract relevant part of the string
    string s = str.substr(6,str.size()- 7);
    int y,x;
    // Determine the byte size of the value
    y = what_copy(s);
    if(y != 4)
        s = s.substr(2, s.size()-2);
    // Check if the string is a constant or register
    if(ALU::is_constant(s))
        x = Memory::inter(s);
    else if(ALU::is_register(s))
        x = registers[s];
    // Check if the string is a memory address
    else if(Memory::loadStore(s))
        x = *((int*)(&STACK[Memory::addresser(s.substr(2, s.size()-3))]));
    else{
        // The string is none of the above, output an error message and return
        printf("Bro, do you even code? print FAILED :) \n" );
        return;
    }
    // Print the value of the string based on its byte size
    switch (y)
    {
        case 1:
            cout<<*((char*)(&x))<<endl;
            break;
        case 2:
            cout<<*((short*)(&x))<<endl;
            break;
        case 4:
            cout<<*((int*)(&x))<<endl;
            break;
        default:
            break;
    }
}
