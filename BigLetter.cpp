// BigLetter implementation

#include "bigletter.h"
#include <qpainter.h>
#include <qsize.h>
#include <qfontmetrics.h>

CBigLetter::CBigLetter(int s,QString d,QWidget *parent,const char* name,WFlags f)
    :QFrame(parent,name,f)
{
    m_nStartSize=s;
    actFontSize(m_nStartSize);
    m_sDisplay=d;
    setFrame(m_nFrame);
}


CBigLetter::CBigLetter(QWidget *parent,const char* name,WFlags f)
    :QFrame(parent,name,f)
{
}

void CBigLetter::drawContents(QPainter* p)
{
    int lFlags=AlignVCenter;
    if (m_nAlign==left)
	lFlags=AlignLeft;
    else
	lFlags|=AlignRight;
    p->drawText(rect(),lFlags,m_sDisplay);
}    


void CBigLetter::display(QString s)
{
    m_sDisplay=s;
    this->repaint();
}


QString CBigLetter::isDisplay() const
{
    return(m_sDisplay);
}


void CBigLetter::setStartSize(int s)
{
    m_nStartSize = s;
    actFontSize(m_nStartSize);
}    
 

int CBigLetter::getStartSize() const
{
    return(m_nStartSize);
}


void CBigLetter::setAlignment(cAlign a)
{
     m_nAlign=a;
}     


cAlign CBigLetter::getAlignment() const
{
    return(m_nAlign);
}


void CBigLetter::setFrame(cFrame f)
{
    m_nFrame=f;
    if (m_nFrame==frame)
	setFrameStyle(QFrame::Box | QFrame::Plain);
    else 
	setFrameStyle(/*QFrame::MenuBarPanel |*/ QFrame::Plain); 
}    


cFrame CBigLetter::getFrame() const
{
    return(m_nFrame);
}


void CBigLetter::resizeEvent ( QResizeEvent* r)
{
    QSize ls=size();
    actFontSize((int) ((1.0*ls.height())/1.6));
    this->QFrame::resizeEvent(r);
}


QSize CBigLetter::sizeHint() const 
{
   QFont fo(font());
    fo.setPointSize(m_nStartSize); // empfohlene grösse ist immer die startgrösse
    QFontMetrics tfm(fo);
    return(tfm.size(SingleLine,m_sDisplay));
}


QSize CBigLetter::minimumSizeHint() const
{
    QFont fo(font());
    fo.setPointSize(m_nStartSize);
    QFontMetrics tfm(fo);
    return(tfm.size(SingleLine,m_sDisplay));
}


void CBigLetter::actFontSize(int fs)
{	
    QFont fo( font());
    fo.setPointSize(fs);
    setFont(fo);
}    
