// kommando interpreter für scpi kommandolisten

#include <stdlib.h>
#include <qstring.h>
#include <qstringlist.h>
#include "cmdinterpret.h"
#include "scpi.h"
#include "zeraglobal.h"

bool cCmdInterpreter::CmdExecute(cNode* RootCmd,cbIFace* callback, char* CmdString, char*  &outp) { // return ja/nein , par die befehlsliste, das callback interface, input , output ( append output)
    bool ret = false;
    m_pcbIFace = callback; // wir merken uns das callback
    dedicatedList.clear();
    if (*CmdString) { // leeres kommando ist nichts falsches -> also richtig
//	char* CmdString=s; // der input string
	cNode* actNode = RootCmd; // startknoten setzen
	cNode* prevNode;
	do {
	    prevNode = actNode;
	} while ( (actNode = actNode->TestNode(this,&CmdString)) );
	switch ( prevNode->m_nNodeStat ) {
/*	  case (isKnown) : 
	      ret = true; // es war ein bekanntes kommando    
	      break; */
	  case (isKnown | isCommand) : 
	      ret=true;
	      callback->SCPICmd(prevNode->m_nCmd,CmdString);
	      break;
	  case (isKnown | isQuery) : 
	      ret = true;
	      outp = callback->SCPIQuery(prevNode->m_nQuery,CmdString);
	      break; // nur queries setzen den output
	  };
    }
    
    return (ret);
}


void cCmdInterpreter::SetParser(cParse* p)
{
    m_pParser = p;
}

