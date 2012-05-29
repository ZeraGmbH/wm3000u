// headerdatei blinktextedit.h
// definiert  cBlinkTextEdit: QTextEdit


#ifndef BLINKTEXTEDIT_H
#define BLINKTEXTEDIT_H

#include <qobject.h>
#include <qstring.h>
#include <qtextedit.h>
#include <qevent.h>
#include <qsize.h>
#include <qtimer.h>

class cBlinkTextEdit: public QTextEdit
{
    Q_OBJECT

public:
   cBlinkTextEdit(QWidget* parent = 0, const char * name = 0);
    virtual ~cBlinkTextEdit();
    virtual QSize sizeHint() const;
    
public slots:
    virtual void append(const QString&);
    virtual void clear();
    virtual void show();
    void setBlink(bool);
    
private:
    QString saveText;
    QSize TextSize;
    bool isBlink, isOn;
    QTimer *BlinkTimer;
};

#endif
