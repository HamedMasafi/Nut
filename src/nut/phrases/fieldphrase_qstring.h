#ifndef NUT_FIELDPHRASE_QSTRING_H
#define NUT_FIELDPHRASE_QSTRING_H

#include <QtNut/nut_global.h>
#include <QtNut/abstractfieldphrase.h>
#include <QtNut/fieldphrase.h>

QT_BEGIN_NAMESPACE

NUT_BEGIN_NAMESPACE

template<>
class FieldPhrase<QString> : public AbstractFieldPhrase
{
public:
    FieldPhrase(const char *className, const char *s)
        : AbstractFieldPhrase(className, s)
    {}

    ConditionalPhrase like(const QString &term)
    {
        return ConditionalPhrase(this, PhraseData::Like, term);
    }

    ConditionalPhrase contains(const QString &term)
    {
        return ConditionalPhrase(this,
                                 PhraseData::Like,
                                 QVariant(QStringLiteral("%") + term
                                          + QStringLiteral("%")));
    }

    AssignmentPhrase operator=(const QVariant &v)
    {
        return AssignmentPhrase(this, v);
    }
};

NUT_END_NAMESPACE

QT_END_NAMESPACE

#endif // NUT_FIELDPHRASE_QSTRING_H
