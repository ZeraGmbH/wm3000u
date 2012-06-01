#ifndef VERSIONSVIEWBASE_H
#define VERSIONSVIEWBASE_H

#include <qvariant.h>
#include <qdialog.h>
#include "wmglobal.h"

namespace Ui {
    class VersionViewBase;
}

class VersionsViewBase : public QDialog
{
    Q_OBJECT

public:
    explicit VersionsViewBase( QWidget* parent = 0);
    ~VersionsViewBase();

public slots:
    virtual void ReceiveVersionData( tVersSerial * p );
    virtual void ShowVersionSlot();

private:
    Ui::VersionViewBase *ui;
    tVersSerial* pVersion;

};

#endif // VERSIONSVIEWBASE_H
