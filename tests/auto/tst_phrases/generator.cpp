#include "generator.h"

Generator::Generator() : Nut::SqliteGenerator()
{

}

QString Generator::where(const Nut::ConditionalPhrase &where)
{
    return createConditionalPhrase(where.data);
}
