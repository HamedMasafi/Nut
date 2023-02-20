#include "testdatabase.h"
#include "test.h"

TestDatabase::TestDatabase(QObject *parent)
    : Database(parent), m_tests(new Nut::TableSet<Test>(this))
{

}
