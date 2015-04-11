#include "ordinateList/doublelist.h"
#include "src/vint.h"
#include "src/vchar.h"
#include "src/vlong.h"
#include "src/vfloat.h"
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
#include "src/varray.h"
#include "rwpages/fheap.h"
#include <stdio.h>
#include <string.h>
using namespace std;

void perderRef(int pdata){
    vInt data = vInt(pdata);
    vRef myref = vRef::assing(sizeof(data),&data);
    cout << ((vInt*)*myref)->getInt()<< endl;
}


int main()
{
    /**
    vRef z;
    vArray<vInt, 8> *y = new (&z) vArray<vInt,8>();
    (*y)[2] = vInt(878551111);
    cout << "tipo de arreglo 1: " << y->getType() <<"y y[2] es:" << ((*y)[2]).getInt()<< endl;
    vArray<vChar, 8> *x = new (&z) vArray<vChar,8>();


    cout << "tipo de arreglo 2: " << x->getType() << endl;
    vRef c;
    new (&c) vInt (795);
    (*y)[5] = vInt(76458);
    for(int mm = 0; mm < 8; mm++) cout <<"y["<<mm<<"] es:" << ((*y)[mm]).getInt()<< endl;
    cout << "size of vInt[]: " << c.getWeight() << endl;
    cout << "size of vChar: " << sizeof(vChar) << endl;
    cout << "size of vLong: " << sizeof(vLong) << endl;
    cout << "size of vFloat: " << sizeof(vFloat) << endl;

    vRef d = c;

    vChar * f = new (&c) vChar('i');

    f->setChar('m');

    vInt gg = vInt(5);

    d = &gg;

    cout << "el dato int es: "<<(((vInt*)*d)[0]).getInt() << endl;
    cout << "el dato char es: "<<((vChar*)*c)->getChar()<< endl;
    vHeap::getInstance()->print();
    */

    fHeap *fh = new fHeap(800,"/home/cristianfernando/archa.bin");
    vInt a(512);
    char  s[sizeof(a)];
    for (int x = 0; x < sizeof(a);x++){
        s[x] = *((char*) (&a) + x);
    }
    fh->paginate(s, sizeof(a));
    char  d[37];
    strcpy(d, "esto es otra cosa que quiero paginar");
    fh->paginate(d,37);

    fh->getPage(0,s);
    cout << "despaginando la primera:" << endl << ((vInt*)s)->getInt() << endl;
    cout << "despaginando la segunda:" << endl << fh->getPage(0,d) << endl;
    //fh->deletePage();
    return 0;
}

