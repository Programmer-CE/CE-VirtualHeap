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

    vRef a;
    vRef b;
    vRef c;
    vRef d;
    vRef e;
    vRef f;

    new (&a) vInt(3);
    vArray<vFloat,19> * y = new (&b) vArray<vFloat,19>();
    (*y)[1] = vFloat(54);
    new (&c) vArray<vFloat,23>();

    new (&d) vArray<vFloat,19>();

    new (&e) vArray<vLong,6>();


    new (&f) vArray<vFloat,19>();




    //vArray<vLong,6> *y = new (&d) vArray<vLong,6>();
    //(*y)[5] = vLong(84);

    //vArray<vLong,6> *m = (vArray<vLong,6>*)*d;

    //cout << "el dato es: " << (*m)[5].getLong() << endl;

    vHeap::getInstance()->print();

    /**
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
    */
    return 0;
}

