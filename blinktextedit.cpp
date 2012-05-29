// implementation cBlinkTextEdit

#include <qfont.h>
#include <qtextedit.h>
#include <qwidget.h>
#include "blinktextedit.h"

cBlinkTextEdit::cBlinkTextEdit(QWidget* parent,const char* name)
    :QTextEdit(parent,name) 
{
    this->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    this->setReadOnly(true);
    this->setVScrollBarMode(AlwaysOff);
    this->setHScrollBarMode(AlwaysOff);
    this->setWordWrap(QTextEdit::NoWrap);
    BlinkTimer = new QTimer(this);
    connect( BlinkTimer , SIGNAL(timeout()), SLOT(show()) );
    isBlink = isOn = false;
    TextSize = QSize(0,0);
}


cBlinkTextEdit::~cBlinkTextEdit(){}
    
void cBlinkTextEdit::setBlink(bool b)
{
    if (isBlink=b) 
	BlinkTimer->start(1000);
    else {
	BlinkTimer->stop();
	show(); // damit text nach abschalten des timers sichtbar wird
    }
}


QSize cBlinkTextEdit::sizeHint() const
{
    return TextSize+QSize(10,10);
}


void cBlinkTextEdit::append(const QString& s)
{
    QFont fo(font());
    QFontMetrics fm(fo);
    TextSize.setHeight(TextSize.height()+fm.height()); // eine zeile mehr 
    int pixelsWide=fm.width(s);
    if (pixelsWide > TextSize.width() ) TextSize.setWidth(pixelsWide);
    this->QTextEdit::append(s);
    saveText = this->text(); // holt immer den ganzen text
}


void cBlinkTextEdit::clear()
{
    TextSize = QSize(0,0);
    this->QTextEdit::clear();
}    
    
    
void cBlinkTextEdit::show()
{
    bool setON = (isBlink) ? !isOn : true;
    if (setON != isOn ) {
	isOn = setON;
	if (setON) 
	    this->QTextEdit::setText(saveText);
	else 
	    this->QTextEdit::clear(); // text löschen
    }
    this->QTextEdit::show();
}

