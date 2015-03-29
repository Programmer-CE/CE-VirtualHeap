#ifndef VHEAPEXCEPTION_H
#define VHEAPEXCEPTION_H

class VHeapException
{
public:
    virtual const char* what(){
        return "Ha ocurrido un error en el vHeap";
    }
};

#endif // VHEAPEXCEPTION_H
