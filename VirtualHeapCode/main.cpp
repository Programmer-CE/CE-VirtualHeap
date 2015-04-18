#include "list/DoubleList.h"
#include "src/vint.h"
#include "src/vchar.h"
#include "src/vlong.h"
#include "src/vfloat.h"
#include "src/nullpointerexception.h"
#include "src/fullmemoryexception.h"
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
#include <cmath>
using namespace std;



int main()
{
    DoubleList<vRef> lista;
    bool notExit = true;
    int opcion = 0;
    int indexToSelect =0;
    vRef a;
    while(notExit){
        cout << "elija su opcion:" << endl;
        cout << "1)insertar un dato" << endl;
        cout << "2)eliminar un dato por vFree" << endl;
        cout << "3)perder una referencia" << endl;
        cout << "4)cerrar" << endl << ">>";
        cin >> opcion;
        switch (opcion) {
        case 1:
            new (&a) vInt(rand()%500+1);
            lista.add(a);
            a.leave();
            continue;
        case 2:
            cout << "elija el indice a borrar indice: " << endl;
            cin >> indexToSelect;
            if (indexToSelect < 0 || indexToSelect > lista.getLenght());
            else{
                a = lista.get(indexToSelect);
                vHeap::getInstance()->vFree(&a);
                lista.remove(indexToSelect);
            }
            continue;
        case 3:
            cout << "elija el indice a borrar indice: " << endl;
            cin >> indexToSelect;
            if (indexToSelect < 0 || indexToSelect > lista.getLenght());
            else lista.remove(indexToSelect);
            continue;
        case 4:
            notExit = false;
            continue;
        default:
            continue;
        }
    }
    return 0;
}

