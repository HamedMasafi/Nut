#ifndef BULKINSERTER_H
#define BULKINSERTER_H

#include <initializer_list>
#include <QtCore/QDebug>
#include <QtNut/phraselist.h>
#include <QtNut/fieldphrase.h>

#include <QtNut/nut_global.h>

NUT_BEGIN_NAMESPACE

class PhraseList;
class Database;
class BulkInserterPrivate;
class NUT_EXPORT BulkInserter
{
    Q_DECLARE_PRIVATE(BulkInserter)

public:
    BulkInserter(Database *db, QString &className);
    BulkInserter(const BulkInserter &other);
    BulkInserter(BulkInserter &&other);

    void setFields(const PhraseList &ph);

    void insert(std::initializer_list<QVariant> vars);
    template<typename... Args>
    void insert(Args... args) {
        insert({args...});
    }
    int apply();

private:
    BulkInserterPrivate *d_ptr;
};

NUT_END_NAMESPACE

#endif // BULKINSERTER_H
