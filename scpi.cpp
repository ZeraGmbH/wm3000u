// implementation scpi

#include <qstring.h>

#include "scpi.h"
#include "parse.h"

bool cSCPIString::operator == (const QString& s) {
    QString tmp=*this; // mach einen qstring daraus
    QString atmp=ScpiAbreviation(tmp); // scpi konforme abkürzung davon
    QString S=s.upper(); // das ganze in gross
    bool b=( (tmp==s) || (tmp == S) || (atmp==s) || (atmp == S) );
    return (b);
}


cSCPIString& cSCPIString::operator = (const QString& s) {
    *this=(cSCPIString) s;
    return (*this);
}

cSCPIString& cSCPIString::operator = (const char* s) {
    *this=(cSCPIString) s;
    return (*this);
}

const QString cSCPIString::ScpiAbreviation (const QString& s) {
    if (s.length() <= 4) return s; // wenn lange kommando form <= 4 zeichen -> kurzes kommando = langes kommando
    QString t=s.left(4); // kurzes kommando ist erste 4 zeichen
    if ( QString("AEIOU").contains(t.right(1),false) ) t=t.left(3); // ist das 4. zeichen ein vokal -> kurzes kommando ist erste 3 zeichen
    return(t);
}
    
// konstruktor, nNodedef, pNextNode, pNewLevelNode, m_pCmd, m_pQuery
cNode::cNode(int ns,cNode* n1,cNode* n2,int cmd,int query) {
    nNodeDef = ns;
    pNextNode = n1;
    pNewLevelNode = n2;
    m_nCmd = cmd;
    m_nQuery = query;
}

// konstruktor, sNodeName, nNodedef, pNextNode, pNewLevelNode, m_pCmd, m_pQuery
cNodeSCPI::cNodeSCPI (const char* s,int ns,cNode* n1,cNode* n2,int cmd,int query)
    :cNode(ns,n1,n2,cmd,query)	{
    
    sNodeName=s;
}


cNode* cNodeSCPI::TestNode(cCmdInterpreter* ci,char** inp)
{
    char* tinp=*inp; // zeiger kopieren
    QString stmp=ci->m_pParser->GetKeyword(&tinp); // lässt das nächste schlüsselwort vom parser lesen ( der ist static)
    m_nNodeStat=isUnknown; // erst mal unbekannt
    if (sNodeName != stmp) return (pNextNode); // zum prüfen des nächsten knoten ( solange != NULL gehts weiter )
    m_nNodeStat |= isKnown;
    *inp=tinp; // hinter dem schlüsselwort gehts weiter
    
    char c=ci->m_pParser->GetChar(inp);
    switch (c) {
    case ':' 	: // es ist ein knoten
	if (nNodeDef & isNode) { 
	m_nNodeStat |= isNode; // es darf einer sein
	return (pNewLevelNode); // dann return nächsten level ( d.h. es geht weiter )
              }
	else return (NULL); // es darf keiner sein -> fertig !!!
    case '?'	: // es ist eine query
	m_nNodeStat |= (nNodeDef & isQuery); // gesetzt wenn es eine sein darf
	return (NULL); // -> fertig !!!
    default		: // es ist ein command
	 m_nNodeStat |= (nNodeDef & isCommand); // gesetzt wenn es eines sein darf
               *inp=tinp; // lass dem Kommando das zeichen (reparsen)
	 return (NULL);
     }
}


cNodeSCPIVar::cNodeSCPIVar(QStringList* sl,int ns,cNode* n1,cNode* n2,int ct,int qt)
    :cNode(ns,n1,n2,ct,qt)	{
    
    sNodeNames=sl;
}


cNode* cNodeSCPIVar::TestNode(cCmdInterpreter* ci, char** inp) {
    char* tinp=*inp;
    QString stmp;
    stmp=ci->m_pParser->GetKeyword(&tinp); // lässt das nächste schlüsselwort vom parser lesen 
    QString STMP=stmp.upper();
    m_nNodeStat=isUnknown; // erst mal unbekannt
    for ( QStringList::iterator it = sNodeNames->begin(); it != sNodeNames->end(); ++it ) {
	if ( ((*it) == stmp)  || ((*it) == STMP)) {
	    //*psNode2Set=*it; // speichere knotenname an der vorgesehen stelle
	    ci->dedicatedList.append(*it); // in die liste der gefundenen schlüsselworte 
	    m_nNodeStat |= isKnown;
	    break;
	}
    }
        
    if (m_nNodeStat == isUnknown) return (pNextNode); // zum prüfen des nächsten knoten ( solange != NULL gehts weiter )
    *inp=tinp; // hinter dem schlüsselwort gehts weiter
    
    char c=ci->m_pParser->GetChar(inp);
    switch (c) {
    case ':' 	: // es ist ein knoten
	if (nNodeDef & isNode) { 
	m_nNodeStat |= isNode; // es darf einer sein
	if (m_nQuery) ci->m_pcbIFace->SCPIQuery(m_nQuery, tinp); 
	// knoten haben normalerweise keine funktionen
	// ausnahme: in abhängigkeit vom knoten muss noch was passieren (z.b. einen nachfolgenden knoten umbauen) 
	return (pNewLevelNode); // dann return nächsten level ( d.h. es geht weiter )
              }
	else return (NULL); // es darf keiner sein -> fertig !!! 
    case '?'	: // es ist eine query
	m_nNodeStat |= (nNodeDef & isQuery); // gesetzt wenn es eine sein darf
	return (NULL); // -> fertig !!!
    default		: // es ist ein command
	 m_nNodeStat |= (nNodeDef & isCommand); // gesetzt wenn es eines sein darf
               *inp=tinp; // lass dem Kommando das zeichen (reparsen)
	 return (NULL);
     }
}

void cNodeSCPIVar::SetNodeNameList(QStringList* sl) { // umschreiben der liste der knotennamen
    sNodeNames=sl;
}


