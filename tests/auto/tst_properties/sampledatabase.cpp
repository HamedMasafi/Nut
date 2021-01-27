#include "sampledatabase.h"
#include "sampletable.h"

SampleDataBase::SampleDataBase() : Nut::Database()
  , m_items(new Nut::TableSet<SampleTable>(this))
{

}
