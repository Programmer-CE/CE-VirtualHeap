#include "fheap.h"
#include <fstream>
#include <iostream>
#include <stdio.h>

registry::registry(int d,int a){
    id=d;
    id2=a;
}


fHeap::fHeap(int overWeight){
    _isPage=false;
    _memoryPage=overWeight;
}

void fHeap::addToPage(registry nregistry){
    if (!_isPage){
      this->deletePage();
      _isPage=true;
    }

    if (_totalDataMemory+sizeof(nregistry)<=_memoryPage){
        std::ofstream fileOut("/home/fran/Escritorio/proof.bin",std::ios::out | std::ios::binary | std::ios::app);

        fileOut.write(reinterpret_cast<char *>(&nregistry),sizeof(registry));

        fileOut.close();
        _totalData++;
        _totalDataMemory+=sizeof(nregistry);
    }
}


registry fHeap::searchInPage(int id, int weight){

    registry regTmp(-1,-1);
    registry returnableRegistry(-1,-1);
    std::ofstream fileTmp("/home/fran/Escritorio/tmp.bin",std::ios::out | std::ios::binary |std::ios::app);

    std::ifstream fileIn("/home/fran/Escritorio/proof.bin",std::ios::in | std::ios::binary);
    int x=0;
    for(int i=0;i<_totalData;i++){
        char chain[sizeof(registry)];

        fileIn.read(reinterpret_cast<char *>(&chain),sizeof(registry));
        for(int j=0;j<sizeof(registry);j++){
            *((char*)((void*)(&regTmp))+ j)=chain[j];
        }

        if(regTmp.id==id){
            x=-1;
            std::cout<<"entrada de if"<<std::endl;
            std::cout<<_totalData<<"  data  antes"<<std::endl;
            _totalDataMemory-=weight;
            returnableRegistry=regTmp;

        }
        else{
            fileTmp.write(reinterpret_cast<char *>(&regTmp),sizeof(registry));
            std::cout<<regTmp.id<<"   id agregado al doc nuevo"<<std::endl;
        }
    }

    fileIn.close();
    fileTmp.close();
    remove("/home/fran/Escritorio/proof.bin");
    rename("/home/fran/Escritorio/tmp.bin","/home/fran/Escritorio/proof.bin");
    _totalData+=x;
    std::cout<<_totalData<<"   data final"<<std::endl;
    return returnableRegistry;

}

void fHeap::deletePage(){
    _isPage=false;
    remove("/home/fran/Escritorio/proof.bin");
}

