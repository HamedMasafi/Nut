#include "db2.h"

#include "table2.h"

DB2::DB2() : Nut::Database (),
    m_sampleTable(new Nut::TableSet<Table2>(this))
{

}
