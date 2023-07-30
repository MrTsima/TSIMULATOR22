//

//

#ifndef TSIMULATOR_MEMORY_H
#define TSIMULATOR_MEMORY_H
#include<string>
using namespace std;

class Memory {
public:
    static int addresser(const string& x);
    static int caster(int gela, int x);
    static int inter(const string& s);
    static void loader(const string& str);
    static void reseter();
    static void definer(const string& str);
    static void reter();
    static bool loadStore(string s);
    static void storer(const string& str);
    static void assigner(const string& str);
};


#endif //TSIMULATOR_MEMORY_H
