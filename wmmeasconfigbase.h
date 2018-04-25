#ifndef WMMEASCONFIGBASE_H
#define WMMEASCONFIGBASE_H

#include <qvariant.h>
#include <qdialog.h>
#include "formatinfo.h"
#include "eparameter.h"

namespace Ui {
    class WMMeasConfigBase;
}

class WMMeasConfigBase : public QDialog
{
    Q_OBJECT

public:
    explicit WMMeasConfigBase(QWidget* parent = 0);
    ~WMMeasConfigBase();

public slots:
    virtual void ReceiveFormatInfoSlot(bool dc, int m, int m2, int n, cFormatInfo * fi );
    virtual void TotalRMScheckedSlot();
    virtual void H1RMScheckedSlot();

signals:
    void SendFormatInfoSignal(int, int, int, cFormatInfo*);

private:
    Ui::WMMeasConfigBase *ui;
    eUnit* m_LPUnit[3];
    eUnit* m_ErrUnit[2];
    eUnit* m_AngleUnit[6];
    cFormatInfo m_Format[4];
    bool dcMode;
    int m_nDisplayMode;
    int m_nLPDisplayMode;

    void init();
    virtual void Actualize();

private slots:
    virtual void LPComboActivatedSlot( int index );
    virtual void ErrComboActivatedSlot( int index );
    virtual void WComboActivatedSlot( int index );
    virtual void IECCheckedSlot();
    virtual void ANSICheckedSlot();
    virtual void SendData2AppSlot();

};

#endif // WMMEASCONFIGBASE_H
