//

//

#include "Memory.h"
#include "ALU.h"
#include "emulator.h"



// This function takes a string and returns its integer value if it represents a constant,
// or the value in the corresponding register if it is a register name
// If the string contains an arithmetic operator (+ or -), it returns the result of performing
// the operation on the values of the two operands (one register and one constant)
int Memory::addresser(const string& x){
    if(ALU::is_constant(x))
        return inter(x);
    else if(ALU::is_register(x))
        return registers[x];
    string lft,rig;
    lft=rig="";
    int a,b,i = ALU::containsAluOper(x);
    lft = x.substr(0, i);
    rig = x.substr(i+1, x.length()-i-1);
    if(ALU::is_register(lft) && ALU::is_constant(rig)){
        a = registers[lft];
        b = inter(rig);
    }else if((ALU::is_register(rig) && ALU::is_constant(lft))){
        a = inter(lft);
        b = registers[rig];
    }
    return ALU::addressHelper(x[i],a,b);
}
// This function converts a string to an integer
int Memory::inter(const string& s){
    int x=0;
    for(char i : s){
        x*=10;
        x+=i-'0';
    }
    return x;
}

// This function casts a value in memory to a different size and returns it as an integer
int Memory::caster(int gela, int x){
    switch (gela)
    {
        case 4:
            return *((int*)(&STACK[x]));  // If the data size is 4 bytes, return the integer value at the memory address
            break;
        case 1:
            return *((unsigned char*)(&STACK[x]));  // If the data size is 1 byte, return the unsigned char value at the memory address
            cout<<1<<endl;
            break;
        case 2:
            return *((unsigned short*)(&STACK[x]));  // If the data size is 2 bytes, return the unsigned short value at the memory address
            cout<<2<<endl;
            break;
        default:
            printf("Operation casting failed...\n" );
            return -1;
    }
    return -1;
}

// This function loads a value from memory into a register
void Memory::loader(const string& str){
    string rg,st;
    rg=st="";
    int x,y,gela,i = assi(str);
    rg = str.substr(0, i);  // The register name is the substring before the '=' character
    st = str.substr(i+3,str.size()- i-4);  // The memory address is the substring between ']' and the '=' character
    gela = what_copy(str);  // This function returns the size of the data to copy from memory to the register
    if(gela != 4){
        st = st.substr(2, st.size()-2);  // If the data size is not 4, remove the '0x' prefix from the memory address
    }
    x = Memory::addresser(st);  // Get the memory address to copy from
    registers[rg] = Memory::caster(gela,x);  // Copy the data from memory to the register
}

// This function clears the register map, to-do list, and function map, sets the values of SP and PC, and resets the program
void Memory::reseter(){
    registers.clear();
    todo.clear();
    functions.clear();
    registers.insert({"SP", 8192});
    registers.insert({"PC", 0});
}

// This function adds a function name and its corresponding address to the function map
void Memory::definer(const string& str){
    functions[str.substr(7,str.size() - 8)] = registers["PC"];
    while(todo[registers["PC"]/4] != "END_DEF")
        registers["PC"] += 4;
    printf("Definition succesful...\n");
}
// This function sets the PC to the value at the top of the stack, increments the value of SP by 4, and returns control to the calling function
void Memory::reter(){
    if(registers["PC"]/4 >= todo.size()-1){
        printf("Program finished\n");
        return;
    }
    registers["PC"]=*((int*)(&STACK[registers["SP"]]));
    registers["SP"]+= 4;
}

/**

Checks if the given instruction is a load/store instruction and the operand is valid.
@param s The instruction string to be checked.
@return True if the instruction is a load/store instruction with a valid operand, false otherwise.
*/
bool Memory::loadStore(string s) {
    // Check if the instruction has the minimum required length and starts with 'M' and has opening '[' and closing ']'
    if (s.size() < 4 || s[0] != 'M' || s[1] != '[' || s.back() != ']')
        return false;

    // Get the content inside the brackets
    string x = s.substr(2, s.size() - 3);

    // Check if there is a '+' symbol present in the content
    size_t pluspos = x.find('+');
    if (pluspos != string::npos) {
        // If there is a '+' symbol, then check if it is between a valid register and a constant
        string s1 = x.substr(0, pluspos);
        string s2 = x.substr(pluspos + 1);
        return (ALU::is_register(s1) && ALU::is_constant(s2)) || (ALU::is_register(s2) && ALU::is_constant(s1));
    }
    // If there is no '+' symbol, then check if the content is a valid register or a constant
    return (ALU::is_register(x) || ALU::is_constant(x));
}

/**
 * The function storer takes a string str as input, extracts the register
 * and value from the string, and stores the value in memory at the given address with the given size.
 * @param str
 */
void Memory::storer(const string& str) {
    string rg,s;
    rg=s="";
    int b,y,x,i = assi(str); // Get index of the assignment operator
    rg = str.substr(i+1, str.size() - i - 1); // Get register from string
    s = str.substr(2, str.size()- rg.size()-4); // Get memory address from string
    y = what_copy(str); // Get size of data to store
    if(y != 4)
        s = rg.substr(2, rg.size()-2); // If the size is not 4 bytes, adjust the memory address accordingly
    x = Memory::addresser(s); // Get integer value of memory address
    if(ALU::is_register(rg))
        b = registers[rg]; // If the value is a register, get its value from registers map
    else if(ALU::is_constant(rg))
        b = Memory::inter(rg); // If the value is a constant, convert it to an integer
    switch (y)
    {
        case 1:
            *((unsigned char*)(&STACK[x])) = (char)b; // Cast the value to char and store it in memory
            break;
        case 2:
            *((unsigned short*)(&STACK[x])) = (short)b; // Cast the value to short and store it in memory
            break;
        case 4:
            *((int*)(&STACK[x])) = (int)b; // Cast the value to int and store it in memory
            break;
        default:
            printf("cast failed :/ " ); // If the size is not 1, 2, or 4 bytes, return an error message
            return;
            break;
    }
}

/**
 * @brief:This function takes a string str as input, which is expected to be in the format of
 * a register assignment instruction. It extracts the register name and the value to be assigned
 * from the input string, and assigns the value to the register. The function handles values of
 * size 1, 2, or 4 bytes, and can handle negative values as well.
 * @param str: a string in the format of a register assignment instruction.
 */
void Memory::assigner(const string& str){
    string s,x;
    int nish=1,y,a,i = assi(str); // initialize variables
    x = str.substr(i+1, str.size()- i-1); // get value string from input
    s = str.substr(0, i); // get register name from input
    y=what_copy(str); // determine size of value
    if(y != 4)
        x = x.substr(2, x.size()-2); // remove leading "0x" if value is not 4 bytes
    if(x[0]=='-'){
        x = x.substr(1, x.size()-1); // remove "-" character if present
        nish = -1; // set nish to -1 to indicate negative value
    }
    if(ALU::is_constant(x))
        a = Memory::inter(x); // get value from constant
    else if(ALU::is_register(x))
        a = registers[x]; // get value from register

    switch (y) // assign value to register based on size
    {
        case 1:
            *((unsigned char*)(&registers[s])) = (unsigned char)a;
            break;
        case 2:
            *((unsigned short*)(&registers[s])) = (unsigned short)a;
            break;
        case 4:
            a*=nish; // multiply by -1 if value is negative
            *((int*)(&registers[s])) = (int)a;
            break;
        default:
            printf("cast failed \n" ); // if size is not 1, 2, or 4, print error message
            return;
            break;
    }
}