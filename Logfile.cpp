// LogFile nutzt immer die gleiche datei und subdirectory.
// LogFile hat maximale länge default 8192 (8K). 
// wenn die länge dann später neu gesetzt wird, wird das u.U.
// zuviel gelesene verworfen. Logfile verhält sich wie
// ein Fifo mit der Länge MaxLen. 

#include "logfile.h"

#include <qobject.h>
#include <qstring.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qtextedit.h>
#include <qdialog.h>
#include <qwidget.h>
#include <qevent.h>


CLogFile::CLogFile(const QString FileName,const long flen) 
{
    m_sFileName=FileName;
    QFile file( FileName );
    m_nActFileLen = 0;
    if ( file.open( IO_ReadOnly ) ) // beim öffnen stringliste lesen und aktuelle länge bestimmen
    {
        QTextStream stream( &file );
        QString line;
	while ( !stream.atEnd() ) {
	    line = stream.readLine(); // line of text excluding '\n'
	    m_sLogLinesList+=line;
	    m_nActFileLen+=line.length();
   	}
	file.close();
    }
    SetFileSizeSlot(flen); // event. nur kürzen, kann vorkommen wenn wm später mittels xml datei konfiguriert wird
}    

CLogFile::~CLogFile() 
{
    QFile file(m_sFileName );	
    file.remove();
    if ( file.open( IO_WriteOnly ) ) {
	QTextStream stream( &file );
	for ( QStringList::Iterator it = m_sLogLinesList.begin(); it != m_sLogLinesList.end(); ++it ) 
	    stream << *it << "\n";
	file.close();
    }
}


void CLogFile::SetFileSizeSlot(const long fs)
{
    QStringList::Iterator it=m_sLogLinesList.begin();
    m_nMaxFileLen=fs;
    // falls länger als konfiguriert zeilen rauswerfen
    while (m_nMaxFileLen<m_nActFileLen) {
	m_nActFileLen-=QString(*it).length();
	it=m_sLogLinesList.remove(it);
    }
}


void CLogFile::AddLogTextSlot(const QString& logtext) // neuer input für logfile
{
    QString s = logtext.stripWhiteSpace();
    m_sLogLinesList.append(s); // an die liste hängen
    m_nActFileLen+=s.length(); // länge aktualisieren
    emit SendLogDataSignal(logtext); // daten weitergeben
    SetFileSizeSlot(m_nMaxFileLen); // tu so als ob wir die grösse neu gesetzt hätten
}

void CLogFile::SendLogSlot() // zu initialisierungs zwecken
{
    QString s;
    QTextStream ts( &s, IO_WriteOnly );
    int n;
    if ( (n = m_sLogLinesList.count()) > 0 ) {
	int i;
	for (i = 0; i < n-1; i++)
	    ts << m_sLogLinesList[i] << "\n";
	ts << m_sLogLinesList[i];
    }
    emit SendLogDataSignal(s); // daten weitergeben
}

