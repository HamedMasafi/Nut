#ifndef DB1_H
#define DB1_H

#include "database.h"

class Table1;

class DB1 : public NUT_WRAP_NAMESPACE(Database)
{
    Q_OBJECT

    NUT_DB_VERSION(1)

    NUT_DECLARE_TABLE(Table1, sampleTable)

public:
    DB1();
};

Q_DECLARE_METATYPE(DB1*)

#endif // DB1_H
