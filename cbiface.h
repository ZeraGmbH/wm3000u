// header datei für callback interface

#ifndef CBIFACE_H
#define CBIFACE_H

class cbIFace { // call back interface 
public:    
    virtual ~cbIFace(){}; // rein virtuell
    virtual void SCPICmd( int, char*) = 0; 
    virtual char* SCPIQuery( int, char*) = 0;
};

#endif
