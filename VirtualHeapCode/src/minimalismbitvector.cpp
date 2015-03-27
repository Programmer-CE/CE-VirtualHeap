#include "minimalismbitvector.h"
#include <iostream>

//####################################################
// Constructor y Destructor
//####################################################

MinimalismBitVector::MinimalismBitVector()
{
    _Offset = new int(-1);
    _Weight = 0;
}

MinimalismBitVector::MinimalismBitVector(int pOffset, int pWeight, string pType)
{
    _Offset = new int(pOffset);
    std::cout << "la direccion de &offset es: " << &_Offset << endl;
    _Weight = pWeight;
    _Type = pType;
}


MinimalismBitVector::~MinimalismBitVector()
{

}



//####################################################
// Final de Constructor y Destructor
//####################################################





//####################################################
// Getters
//####################################################

int MinimalismBitVector::getWeight() const
{
    return _Weight;
}

string MinimalismBitVector::getType() const
{
    return _Type;
}

int *MinimalismBitVector::getOffset() const
{
    return _Offset;
}



int MinimalismBitVector::ReferenceCounter() const
{
    return _ReferenceCounter;
}



//####################################################
// Final de getters
//####################################################







//####################################################
// Setters
//####################################################

void MinimalismBitVector::setWeight(int pWeight)
{
    _Weight = pWeight;
}
void MinimalismBitVector::setType(const string &pType)
{
    _Type = pType;
}


void MinimalismBitVector::setOffset(int pOffset)
{
    delete _Offset;
    _Offset = new int(pOffset);
}


void MinimalismBitVector::setReferenceCounter(int ReferenceCounter)
{
    _ReferenceCounter = ReferenceCounter;
}

bool MinimalismBitVector::operator <(const MinimalismBitVector &otherBitVector)
{
    return _Offset < otherBitVector._Offset;
}

bool MinimalismBitVector::operator >(const MinimalismBitVector &otherBitVector)
{
    return _Offset > otherBitVector._Offset;
}

bool MinimalismBitVector::operator !=(const MinimalismBitVector &otherBitVector)
{
    return _Offset != otherBitVector._Offset;
}


void MinimalismBitVector::setOnUse(bool pOnUse)
{
    _InUseFlag = pOnUse;
}

void MinimalismBitVector::setOnMemoryLocation(bool pMemoryLocationFlag)
{
    _OnMemoryFlag = pMemoryLocationFlag;
}

//####################################################
// Final de Setters
//####################################################





//####################################################
// Metodos de clase
//####################################################



void MinimalismBitVector::addReference()
{
    _ReferenceCounter++;
}

void MinimalismBitVector::removeReference()
{
    _ReferenceCounter--;
}

bool MinimalismBitVector::isChargedOnMemory()
{
    return _OnMemoryFlag;
}

bool MinimalismBitVector::isOnUse()
{
    return _InUseFlag;
}


//####################################################
// Final de Metodos de clase
//####################################################


