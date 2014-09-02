#ifndef RELEASEINFO_H
#define RELEASEINFO_H


#include <QObject>
#include <QProcess>

class cReleaseInfo: public QObject
{
    Q_OBJECT

public:
    cReleaseInfo(QObject* parent=0);
    virtual ~cReleaseInfo();

public slots:
    virtual void show();

private:
    QProcess *m_pProcess;
};

#endif // RELEASEINFO_H
