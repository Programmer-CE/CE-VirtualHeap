#include "ordinateList/doublelist.h"
#include "src/vint.h"
#include "src/nullpointerexception.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "src/minimalismbitvector.h"
#include <typeinfo>
#include "src/vref.h"
#include "src/vheap.h"
#include "cmath"
#include "unistd.h"
using namespace std;

void perderRef(int pdata){
    vInt data = vInt(pdata);
    vRef myref = vRef::assing(sizeof(data),&data);
    cout << ((vInt*)*myref)->getInt()<< endl;
}


int main()
{
    cout << "=== Fin PederRef ===" << endl;
    //vInt * a = new (typeid(vInt).name()) vInt(5);
    vInt data = vInt(24);
    vRef b = vRef::assing(sizeof(data),&data);
    vRef a = b;
    vRef c = b;
    vRef d = b;
    b = vRef::assing(sizeof(data),&data);
    b = vRef::assing(sizeof(data),&data);
    //vRef m = vHeap::getInstance()->vMalloc(sizeof(data),data.getType());
    vHeap::getInstance()->print();
    return 0;
}
