#ifndef SAMPLETABLE_H
#define SAMPLETABLE_H

#include <QTime>
#include <QDateTime>
#include <QDate>

#include "table.h"

class SampleTable : public NUT_WRAP_NAMESPACE(Table)
{
    Q_OBJECT

    NUT_PRIMARY_AUTO_INCREMENT(id)
    NUT_DECLARE_FIELD(int, id, id, setId)

    NUT_DECLARE_FIELD(QDate, d, d, setD)
    NUT_DECLARE_FIELD(QTime, t, t, setT)
    NUT_DECLARE_FIELD(QDateTime, dt, dt, setDT)

public:
    Q_INVOKABLE SampleTable(QObject *parent = Q_NULLPTR);
};

Q_DECLARE_METATYPE(SampleTable*)

#endif // SAMPLETABLE_H
