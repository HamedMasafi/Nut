#ifndef DB3_H
#define DB3_H

#include "database.h"

class Table3;

class DB3 : public NUT_WRAP_NAMESPACE(Database)
{
    Q_OBJECT

    NUT_DB_VERSION(1)

    NUT_DECLARE_TABLE(Table3, sampleTable)

public:
    DB3();
};

Q_DECLARE_METATYPE(DB3*)

#endif // DB3_H
