#include "minimalismbitvector.h"
#include <iostream>

int MinimalismBitVector::Serial = 0;


//####################################################
// Constructor y Destructor
//####################################################

MinimalismBitVector::MinimalismBitVector()
{
    _Id = -1;
    _Weight = 0;
}

MinimalismBitVector::MinimalismBitVector(const MinimalismBitVector &othervariable)
{
    _Id = othervariable.getId();
    _Weight = othervariable.getWeight();
    _Type = othervariable.getType();
    _ReferenceCounter = othervariable.getReferenceCounter();
    _InUseFlag = othervariable.isOnUse();
    _OnMemoryFlag = othervariable.isChargedOnMemory();
    _Offset = othervariable.getOffset();
}

MinimalismBitVector::MinimalismBitVector(unsigned int pOffset, unsigned int pWeight, string pType)
{
    _Offset = pOffset;
    _Id = ++Serial;
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

unsigned int MinimalismBitVector::getWeight() const
{
    return _Weight;
}

string MinimalismBitVector::getType() const
{
    return _Type;
}

int MinimalismBitVector::getId() const
{
    return _Id;
}


void MinimalismBitVector::setOffset(unsigned int pOffset)
{
    _Offset = pOffset;
}

//####################################################
// Final de getters
//####################################################







//####################################################
// Setters
//####################################################

void MinimalismBitVector::setWeight(unsigned int pWeight)
{
    _Weight = pWeight;
}
void MinimalismBitVector::setType(const string &pType)
{
    _Type = pType;
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

unsigned int MinimalismBitVector::getOffset() const
{
    return _Offset;
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

bool MinimalismBitVector::isChargedOnMemory() const
{
    return _OnMemoryFlag;
}

bool MinimalismBitVector::isOnUse() const
{
    return _InUseFlag;
}

int MinimalismBitVector::getReferenceCounter() const
{
    return _ReferenceCounter;
}


//####################################################
// Final de Metodos de clase
//####################################################


