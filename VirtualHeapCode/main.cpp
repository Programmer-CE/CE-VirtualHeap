#include "src/minimalismbitvector.h"
#include "src/vint.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <typeinfo>
#include "src/vref.h"
#include "src/vheap.h"
using namespace std;

void test(){
    int t = 2;
    MinimalismBitVector m(0,sizeof(t),typeid(t).name());

    cout << "m en bitvectors es: " << endl;
    cout << "offset: " <<m.Offset() << endl;
    cout << "peso: "<< m.getWeight() << endl;
    cout << "tipo: " << m.getType() << endl;

    cout << "direccion del offset: " << (int)m.Offset() << endl;

    int g = (m.Offset());

    cout << "direccion de g: " << g << endl;

    if (g == m.Offset()) cout << "IM A FUKING GENIUS!" << endl;
    //cout << "direccion de *g: " << *g << endl;
}



int main(int argc, char *argv[])
{
    //vInt * a = new (typeid(vInt).name()) vInt(5);
    vInt data = vInt(24);
    vRef myref = vHeap::getInstance()->vMalloc(sizeof(data),data.getType());
    myref = &data;
    vRef myref2 = vRef::assing(sizeof(data),&data);
    myref2 = &data;

    //vRef myref = vRef::assing(&data);
    vInt data2 = *(vInt*)(*myref);
    cout << data2._Int << endl;
    //cout << data3._Int << endl;
    //data = vInt(88);
    //myref = ((vObject*)&data);

    vRef copia = myref;

    vHeap::getInstance()->vFree(&myref);
    //test();
    return 0;
}
