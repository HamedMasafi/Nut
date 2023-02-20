#include "db.h"

#include "sampletable.h"

DB::DB(): Nut::Database (),
      m_sampleTables(new Nut::TableSet<SampleTable>(this))
{

}
