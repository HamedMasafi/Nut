#ifndef TABLE3_H
#define TABLE3_H

#include "table.h"

class Table3 : public NUT_WRAP_NAMESPACE(Table)
{
    Q_OBJECT

    NUT_PRIMARY_AUTO_INCREMENT(id)
    NUT_DECLARE_FIELD(int, id, id, setId)

    NUT_DECLARE_FIELD(QString, grade, grade, setGrade)

public:
    Q_INVOKABLE Table3(QObject *parent = Q_NULLPTR);

};

Q_DECLARE_METATYPE(Table3*)

#endif // TABLE3_H
