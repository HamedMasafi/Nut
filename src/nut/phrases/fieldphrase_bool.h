#ifndef NUT_FIELDPHRASE_BOOL_H
#define NUT_FIELDPHRASE_BOOL_H

#include <QtNut/nut_global.h>
#include <QtNut/fieldphrase.h>
#include <QtNut/fieldphrase.h>

QT_BEGIN_NAMESPACE

NUT_BEGIN_NAMESPACE

template<>
class FieldPhrase<bool> : public AbstractFieldPhrase
{
public:
    FieldPhrase(const char *className, const char *s) :
          AbstractFieldPhrase(className, s)
    {}

    AssignmentPhrase operator =(const bool &other) {
        return AssignmentPhrase(this, other);
    }

    FieldPhrase<bool> operator !()
    {
        FieldPhrase<bool> f(data->className, data->fieldName);
        //        f.data = new PhraseData(data);
        f.data->isNot = !data->isNot;
        return f;
    }

    operator ConditionalPhrase()
    {
        return ConditionalPhrase(this, PhraseData::Equal, !data->isNot);
    }
};

NUT_END_NAMESPACE

QT_END_NAMESPACE

#endif // NUT_FIELDPHRASE_BOOL_H
