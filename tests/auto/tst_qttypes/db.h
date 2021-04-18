#ifndef DB_H
#define DB_H

#include "database.h"

class SampleTable;
class DB : public Nut::Database
{
    Q_OBJECT

    NUT_DB_VERSION(1)

    NUT_DECLARE_TABLE(SampleTable, sampleTables)

public:
    DB();
};

#endif // DB_H
