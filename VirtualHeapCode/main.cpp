#include "src/minimalismbitvector.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <typeinfo>
using namespace std;

void test(){
    int t = 2;
    MinimalismBitVector m(0,sizeof(t),typeid(t).name());

    cout << "m en bitvectors es: " << endl;
    cout << "offset: " <<*m.getOffset() << endl;
    cout << "peso: "<< m.getWeight() << endl;
    cout << "tipo: " << m.getType() << endl;

    cout << "direccion del offset: " << (int*)m.getOffset() << endl;

    int * g = (m.getOffset());

    cout << "direccion de g: " << g << endl;

    if (g == m.getOffset()) cout << "IM A FUKING GENIUS!" << endl;
    //cout << "direccion de *g: " << *g << endl;
}



int main(int argc, char *argv[])
{
    test();
    return 0;
}
