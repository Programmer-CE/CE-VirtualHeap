#include "fheap.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include "src/vobject.h"
#include "src/vref.h"
#include <string.h>

void fHeap::write(char *pToWrite, int pNumberOfCharacters)
{
}

fHeap::fHeap(int overWeight, char* pPath){
    _isPage=false;
    _memoryPage=overWeight;
    _path = pPath;
}

void fHeap::paginate(char * pToWrite,int pNumberOfCharacters){
    if (!_isPage){
      this->deletePage();
      _isPage=true;
    }

    if (_totalDataMemory + pNumberOfCharacters <=_memoryPage){
        //write(pToWrite,pNumberOfCharacters);
        std::ofstream fileOut(_path,std::ios::out | std::ios::binary | std::ios::app);
        fileOut.write(reinterpret_cast<char *>(pToWrite),pNumberOfCharacters);
        fileOut.close();
        _totalData++;
        _totalDataMemory+=pNumberOfCharacters;
        _weightList.add(pNumberOfCharacters);
    }
}

const char* fHeap::getPage(int pOffset, char * toCopy){

    std::ofstream fileTmp("tmp.bin",std::ios::out | std::ios::binary |std::ios::app);

    std::ifstream fileIn(_path,std::ios::in | std::ios::binary);

    int decreaseMemory=0;
    int deletableItemWeight=-1;
    int currentOffset = 0;
    int currentWeight;
    for (int currentWeightIndex = 0; currentWeightIndex < _weightList.getLenght();currentWeightIndex++){
        currentWeight = _weightList.get(currentWeightIndex);
        char chain[currentWeight];//setea el tamanio de la cadena por el peso del objeto guardado en el archivo
        fileIn.read(reinterpret_cast<char *>(&chain),currentWeight);//cambiar size!!


        if(currentOffset==pOffset){//modificar condicion de comparacion!!!
            decreaseMemory=-1;//decremento de todal data
            _totalDataMemory-= currentWeight;
            strcpy(toCopy,chain);
            deletableItemWeight=currentWeightIndex;
        }
        else{
            fileTmp.write(reinterpret_cast<char *>(&chain),currentWeight);//cambiar size segun lista!
            //decreaseMemory=0;
        }
        currentOffset += currentWeight;



    }

    fileIn.close();
    fileTmp.close();
    if(deletableItemWeight!=-1)_weightList.remove(deletableItemWeight);
    remove(_path);
    rename("tmp.bin",_path);
    _totalData+=decreaseMemory;
    return toCopy;

}

void fHeap::deletePage(){
    remove(_path);
}

fHeap::~fHeap(){deletePage();}
