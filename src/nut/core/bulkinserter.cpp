#include "bulkinserter.h"
#include "bulkinserter_p.h"

#include "phrases/phraselist.h"
#include "database.h"
#include "abstractsqlgenerator.h"
#include "databasemodel.h"

#include <QtCore/QDebug>

NUT_BEGIN_NAMESPACE

BulkInserterPrivate::BulkInserterPrivate(Database *db)
    : database(db), fieldCount(0)
{

}

BulkInserter::BulkInserter(Database *db, QString &className)
    : d_ptr(new BulkInserterPrivate(db))
{
    Q_D(BulkInserter);

    for (auto &m: db->model())
        if (m->className() == className)
            d->className = m->name();
}

BulkInserter::BulkInserter(const BulkInserter &other)
{
    d_ptr = other.d_ptr;
}

BulkInserter::BulkInserter(BulkInserter &&other)
{
    d_ptr = other.d_ptr;
    other.d_ptr = nullptr;
}

void BulkInserter::setFields(const PhraseList &ph)
{
    Q_D(BulkInserter);
    d->fields = ph;
    d->fieldCount = static_cast<size_t>(ph.data.count());
}

void BulkInserter::insert(std::initializer_list<QVariant> vars)
{
    Q_D(BulkInserter);

    if (vars.size() != d->fieldCount) {
        qInfo("Number of rows mistake");
        return;
    }

    QVariantList list;
    std::initializer_list<QVariant>::iterator it;
    for (it = vars.begin(); it != vars.end(); ++it)
        list.append(*it);
    d->variants.append(list);
}

int BulkInserter::apply()
{
    Q_D(BulkInserter);
    auto sql = d->database->sqlGenerator()->insertBulk(d->className,
                                                       d->fields,
                                                       d->variants);
    QSqlQuery q = d->database->exec(sql);
    return q.numRowsAffected();
}

NUT_END_NAMESPACE
