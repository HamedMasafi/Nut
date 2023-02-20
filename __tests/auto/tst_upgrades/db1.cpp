#include "db1.h"

#include "table1.h"

DB1::DB1() : Nut::Database (),
    m_sampleTable(new Nut::TableSet<Table1>(this))
{

}
