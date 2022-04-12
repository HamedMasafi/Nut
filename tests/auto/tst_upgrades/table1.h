#ifndef TABLE1_H
#define TABLE1_H

#include "table.h"

class Table1 : public NUT_WRAP_NAMESPACE(Table)
{
    Q_OBJECT

    NUT_PRIMARY_AUTO_INCREMENT(id)
    NUT_DECLARE_FIELD(int, id, id, setId)

public:
    Q_INVOKABLE Table1(QObject *parent = Q_NULLPTR);

};

Q_DECLARE_METATYPE(Table1*)

#endif // TABLE1_H
