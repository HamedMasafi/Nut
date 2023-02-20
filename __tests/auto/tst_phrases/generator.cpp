#include "generator.h"

Generator::Generator() : Nut::SqliteGenerator()
{

}

QString Generator::where(const Nut::ConditionalPhrase &where)
{
    return createConditionalPhrase(where.data);
}

QString Generator::order(const Nut::PhraseList &order)
{
    return createOrderPhrase(order);
}

QString Generator::select(const Nut::PhraseList &select)
{
    return createFieldPhrase(select);
}
