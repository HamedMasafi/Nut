#ifndef SAMPLEDATABASE_H
#define SAMPLEDATABASE_H

#include <QtNut/Database>

class SampleTable;
class SampleDataBase : public Nut::Database
{
    Q_OBJECT
    NUT_DB_VERSION(1)
    NUT_DECLARE_TABLE(SampleTable, items)

public:
    SampleDataBase();
};

#endif // SAMPLEDATABASE_H
