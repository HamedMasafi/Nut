#ifndef GENERATOR_H
#define GENERATOR_H

#include <QtNut/sqlitegenerator.h>

class Generator : public Nut::SqliteGenerator
{
public:
    Generator();

    QString where(const Nut::ConditionalPhrase &where);
};

#endif // GENERATOR_H
