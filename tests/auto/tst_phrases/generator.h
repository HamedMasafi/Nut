#ifndef GENERATOR_H
#define GENERATOR_H

#include <QtNut/sqlitegenerator.h>

class Generator : public Nut::SqliteGenerator
{
public:
    Generator();

    QString where(const Nut::ConditionalPhrase &where);
    QString order(const Nut::PhraseList &order);
    QString select(const Nut::PhraseList &select);
};

#endif // GENERATOR_H
