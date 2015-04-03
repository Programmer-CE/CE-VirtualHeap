#include "fheap.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include "src/vobject.h"
#include "src/vref.h"


fHeap::fHeap(int overWeight){
    _isPage=false;
    _memoryPage=overWeight;
}

void fHeap::addToPage(vRef newvRef){
    if (!_isPage){
      this->deletePage();
      _isPage=true;
    }

    if (_totalDataMemory+newvRef.getWeight()<=_memoryPage){
        std::ofstream fileOut("/home/fran/Escritorio/proof.bin",
                              std::ios::out | std::ios::binary | std::ios::app);

        fileOut.write(reinterpret_cast<char *>(&newvRef),sizeof(newvRef.getWeight()));

        fileOut.close();
        _totalData++;
        _totalDataMemory+=newvRef.getWeight();
        _weightList.add(newvRef.getWeight());
    }
}

vRef fHeap::searchInPage(int id){

    vRef refTmp;//temporal
    vRef returnableRef;//para retornar
    std::ofstream fileTmp("/home/fran/Escritorio/tmp.bin",std::ios::out | std::ios::binary |std::ios::app);

    std::ifstream fileIn("/home/fran/Escritorio/proof.bin",std::ios::in | std::ios::binary);

    int decreaseMemory=0;
    int deletableItemWeight=-1;
    for(int i=0;i<_totalData;i++){
        char chain[_weightList.get(i)];//cambair weight por el de lista..

        fileIn.read(reinterpret_cast<char *>(&chain),_weightList.get(i));//cambiar size!!
        for(int j=0;j<_weightList.get(i);j++){
            *((char*)((void*)(&refTmp))+ j)=chain[j];
        }

        if(refTmp._Id==id){            //modificar condicion de comparacion!!!
            decreaseMemory=-1;//decremento de todal data
            _totalDataMemory-=_weightList.get(i);
            returnableRef=refTmp;
            deletableItemWeight=i;
        }
        else{
            fileTmp.write(reinterpret_cast<char *>(&refTmp),sizeof(_weightList.get(i)));//cambiar size segun lista!
            decreaseMemory=0;
        }
    }

    fileIn.close();
    fileTmp.close();
    if(deletableItemWeight!=-1)_weightList.remove(deletableItemWeight);
    remove("/home/fran/Escritorio/proof.bin");
    rename("/home/fran/Escritorio/tmp.bin","/home/fran/Escritorio/proof.bin");
    _totalData+=decreaseMemory;
    return returnableRef;

}

void fHeap::deletePage(){
    remove("/home/fran/Escritorio/proof.bin");
}

fHeap::~fHeap(){}
