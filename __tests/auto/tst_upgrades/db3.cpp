#include "db3.h"

#include "table3.h"

DB3::DB3() : Nut::Database (),
    m_sampleTable(new Nut::TableSet<Table3>(this))
{

}
