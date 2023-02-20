#ifndef DB1_H
#define DB1_H

#include "database.h"

class Table;

class DB : public NUT_WRAP_NAMESPACE(Database)
{
    Q_OBJECT

    NUT_DB_VERSION(1)

    NUT_DECLARE_TABLE(Table, sampleTable)

public:
    DB();
};

Q_DECLARE_METATYPE(DB*)

#endif // DB1_H
