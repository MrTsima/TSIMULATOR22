//

//

#ifndef TSIMULATOR_ALU_H
#define TSIMULATOR_ALU_H
#include<bits/stdc++.h>
using namespace std;

class ALU {
public:
    static int doALU(int x, int y, char z);
    static int addressHelper(char i,int a,int b);
    static bool is_constant(const string& s);
    static bool is_register(string reg);
    static bool isALUer(const string& str);
    static int containsAluOper(const string& s);
    static void ALUer(const string& str);
private:
    // Functions for doALU
    static int add(int x, int y);
    static int subtract(int x, int y);
    static int multiply(int x, int y);
    static int divide(int x, int y);
};


#endif //TSIMULATOR_ALU_H
