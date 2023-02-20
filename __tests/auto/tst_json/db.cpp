#include "db.h"

#include "sampletable.h"

DB::DB() : Nut::Database (),
    m_sampleTable(new Nut::TableSet<Table>(this))
{

}
