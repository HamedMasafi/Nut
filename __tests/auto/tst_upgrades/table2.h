#ifndef TABLE2_H
#define TABLE2_H

#include "table.h"

class Table2 : public NUT_WRAP_NAMESPACE(Table)
{
    Q_OBJECT

    NUT_PRIMARY_AUTO_INCREMENT(id)
    NUT_DECLARE_FIELD(int, id, id, setId)

    NUT_DECLARE_FIELD(QString, str, str, setStr)
    NUT_DECLARE_FIELD(int, grade, grade, setGrade)

public:
    Q_INVOKABLE Table2(QObject *parent = Q_NULLPTR);

};

Q_DECLARE_METATYPE(Table2*)

#endif // TABLE2_H
