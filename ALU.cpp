//

//

#include "ALU.h"
#include "Memory.h"
#include "emulator.h"

// This function performs arithmetic operations on two integers based on a given operator
int ALU::doALU(int x, int y, char z) {
    switch (z) {
        case '+':
            return add(x, y);
        case '-':
            return subtract(x, y);
        case '*':
            return multiply(x, y);
        case '/':
            return divide(x, y);
        default:
            printf("Operation ALU has failed...\n");
            return -1;
    }
}
// This function performs an arithmetic operation (addition or subtraction) on the values of two operands
int ALU::addressHelper(char i,int a,int b){
    switch (i)
    {
        case '+':
            return a+b;
            break;
        case '-':
            return a-b;
            break;
        default:
            printf("Operation LOADING failed\n" );
            break;
    }
    return -1;
}
/**

@brief Checks if the given string is a constant number.
@param s the string to check.
@return true if the string is a constant number, false otherwise.
*/
bool ALU::is_constant(const string& s){
    return std::all_of(s.begin(), s.end(), [](char c) {
        return c >= '0' && c <= '9';
    });
}


/**
 * Check if the given string is a valid register name.
 *
 * @param reg The register name to be checked.
 * @return true if the register name is valid, false otherwise.
 */
bool ALU::is_register(string reg) {
    // Check if the register is one of the special registers
    if (reg == "SP" || reg == "PC" || reg == "RV")
        return true;

    // Check if the register name is valid
    if (reg.size() < 2) // Register name should be at least two characters long
        return false;

    // Check if the first character is 'R' and the remaining characters are a valid constant
    if (reg[0] == 'R') {
        string s = reg.substr(1);
        if (s.empty()) // Register name should have a non-empty number after 'R'
            return false;
        return ALU::is_constant(s);
    }
    return false;
}

/**

Determines whether a given string is an arithmetic operation that involves at least one register and one constant or two registers.
@param str The input string to check.
@return True if the input string is an arithmetic operation that involves at least one register and one constant or two registers, false otherwise.
*/
bool ALU::isALUer(const string& str){
    string lf,rg;
    lf=rg="";

    // Find the index of the first arithmetic operator in the string
    int x = ALU::containsAluOper(str);
    if(x <= 0)
        return false;

    // Extract the left and right operands of the operation based on the operator's position
    rg = str.substr(x+1, str.size()-x-1);
    lf = str.substr(0, x);

    // Check if the operands are valid (registers, constants or a combination of both) for the given operation
    return (ALU::is_register(rg) && ALU::is_constant(lf))||
           (ALU::is_constant(lf)&&ALU::is_constant(rg)) ||
           (ALU::is_register(lf) && ALU::is_constant(rg)) ||
           (ALU::is_register(lf) && ALU::is_register(rg));
}

// Returns the index of the first arithmetic operator (+, -, *, /) found in the given string,
// or -1 if none is found
int ALU::containsAluOper(const string& s){
    if(s.find('+') != -1) // Check for '+' symbol
        return s.find('+');
    if(s.find('-') != -1) // Check for '-' symbol
        return s.find('-');
    if(s.find('*') != -1) // Check for '' symbol
        return s.find('*');
    if(s.find('/') != -1) // Check for '/' symbol
        return s.find('/');
    return -1; // If none of the symbols are present, return -1
}

// This function performs arithmetic operations based on the input string
void ALU::ALUer(const string& str){
    string lft,rg;
    lft=rg="";
    int i = assi(str);
    lft = str.substr(0, i);
    rg = str.substr(i+1, str.size()-i-1 );

    // If the right side is not a constant, then perform an arithmetic operation
    if(!ALU::is_constant(rg)){
        string lef,rig;
        lef=rig="";
        int x,y,j = containsAluOper(rg);

        // Split the right side string into left and right operands
        lef = rg.substr(0, j);
        rig = rg.substr(j+1, rg.size()-j-1);

        // Perform the operation based on the operands and operator
        if(ALU::is_constant(lef)&&ALU::is_constant(rig)){
            x = Memory::inter(lef);
            y = Memory::inter(rig);
        }else if(ALU::is_register(lef)&&ALU::is_register(rig)){
            x = registers[lef];
            y = registers[rig];
        }else if(ALU::is_register(lef)&&ALU::is_constant(rig)){
            x = registers[lef];
            y = Memory::inter(rig);
        }else if(ALU::is_register(rig)&&ALU::is_constant(lef)){
            x = Memory::inter(lef);
            y = registers[rig];
        }else{
            printf("BUG\n");
            return ;
        }
        registers[lft]=ALU::doALU(x,y,rg[j]); // Store the result on the left side register
        return ;
    }
    registers[lft] = Memory::inter(rg); // If the right side is a constant, store it on the left side register
}

int ALU::add(int x, int y) {
    return x + y;
}

int ALU::subtract(int x, int y) {
    return x - y;
}

int ALU::multiply(int x, int y) {
    return x * y;
}

int ALU::divide(int x, int y) {
    return x / y;
}