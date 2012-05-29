// header datei für kommando interpreter

#ifndef KINTERPRETER_H
#define KINTERPRETER_H

#include <stdlib.h>
#include <qstring.h>
#include <qstringlist.h>
#include "cbiface.h"

class cNode;
class cbIFace;
class cParse;

class cCmdInterpreter { // interpretiert einen inputstring und führt kommando aus (scpi)
public:
    cCmdInterpreter(cParse* p) {
	m_pParser = p; };
    ~cCmdInterpreter() {};  
    cParse* m_pParser;
    void SetParser(cParse*); // wir können ihn umschreiben
    bool CmdExecute(cNode*,cbIFace*, char*, char*&); // return ja/nein , par die befehlsliste, das callback interface, input , output
    cbIFace* m_pcbIFace; // pointer auf call back interface
    QStringList dedicatedList; // liste gefundener schlüsselwörter
};

#endif

