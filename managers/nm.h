#ifndef NM_H
#define NM_H

#include <QtPlugin>

class nm {
public:
    virtual void changeWindow(int id) = 0;
    virtual bool openSettings(int id) = 0;
    virtual bool openEvents(int id) = 0;
    virtual bool openGeneralReports(int id) = 0;
    virtual bool openReports(int index, int id = 0, QDate = QDate(), QDate = QDate()) = 0;
    // virtual bool openEvent(QDate date) = 0;


};

#define nm_iid "adi.nm"

Q_DECLARE_INTERFACE(nm, nm_iid)

#endif // NM_H
