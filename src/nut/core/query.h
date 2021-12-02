/**************************************************************************
**
** This file is part of Nut project.
** https://github.com/HamedMasafi/Nut
**
** Nut is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Nut is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with Nut.  If not, see <http://www.gnu.org/licenses/>.
**
**************************************************************************/

#ifndef NUT_QUERY_H
#define NUT_QUERY_H

#include <QtCore/QVariant>
#include <QtCore/QDebug>
#include <QtCore/QScopedPointer>
#include <QtCore/QRegularExpression>
#include <QtCore/QMetaObject>
#include <QtSql/QSqlResult>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlQuery>

#ifndef NUT_RAW_POINTER
#include <QtCore/QSharedPointer>
#endif

#include <QtNut/table.h>
#include <QtNut/database.h>
#include <QtNut/databasemodel.h>
#include <QtNut/abstracttableset.h>
#include <QtNut/abstractsqlgenerator.h>
#include <QtNut/phrase.h>
#include <QtNut/tablemodel.h>
#include <QtNut/sqlmodel.h>

NUT_BEGIN_NAMESPACE

struct NUT_EXPORT QueryData {
    QString sql;
    QString className;
    QString tableName;
    QString select;
    Database *database;
    AbstractTableSet *tableSet;
    QStringList joins;
    QList<RelationModel*> relations;
    int skip;
    int take;
    PhraseList orderPhrase, fieldPhrase;
    ConditionalPhrase wherePhrase;

    QueryData *clone() {
        auto r = new QueryData;
        r->sql = sql;
        r->className = className;
        r->tableName = tableName;
        r->select = select;
        r->database = database;
        r->tableSet = tableSet;
        r->joins = joins;
        r->relations = relations;
        r->skip = skip;
        r->take = take;
        r->orderPhrase = orderPhrase;
        r->fieldPhrase = fieldPhrase;
        r->wherePhrase = wherePhrase;
        return r;
    }

    QueryData() : skip(0), take(0)
    { }

    QueryData(Database *db) : database(db), skip(0), take(0)
    { }
};

template <class T>
class Query
{
    QueryData *d;

public:
    explicit Query(Database *database, AbstractTableSet *tableSet);
    Query (const Query<T> &other);
    Query (Query<T> &&other);

    ~Query();

    Query<T>& operator=(const Query<T> &q);

    //ddl

    Query<T> &join(const QString &className);
    Query<T> &join(Table *c);

    template<class TABLE>
    Query<T> &join()
    {
        join(TABLE::staticMetaObject.className());
        return *this;
    }

    //    Query<T> &orderBy(QString fieldName, QString type);
    Query<T> &skip(int n);
    Query<T> &take(int n);
    Query<T> &fields(const PhraseList &ph);
    Query<T> &orderBy(const PhraseList &ph);
    Query<T> &where(const ConditionalPhrase &ph);
    Query<T> &setWhere(const ConditionalPhrase &ph);

    //data selecting
    Row<T> first();
    RowList<T> toList(int count = -1);
    template <typename F>
    QList<F> select(const FieldPhrase<F> f);

    template<typename O>
    QList<O> select(const std::function<O(const QSqlQuery &q)> allocator);

    int count();
    QVariant max(const FieldPhrase<int> &f);
    QVariant min(const FieldPhrase<int> &f);
    QVariant sum(const FieldPhrase<int> &f);
    QVariant average(const FieldPhrase<int> &f);


    //data mailpulation
    int update(const AssignmentPhraseList &ph);
    QVariant insert(const AssignmentPhraseList &p);
    int remove();

    QSqlQueryModel *toModel();
    void toModel(QSqlQueryModel *model);
    void toModel(SqlModel *model);

    //debug purpose
    QString sqlCommand() const;
};

template<typename T>
template<typename O>
Q_OUTOFLINE_TEMPLATE QList<O> Query<T>::select(const std::function<O (const QSqlQuery &)> allocator)
{
    //Q_D(AbstractQuery);
    QList<O> ret;

    d->joins.prepend(d->tableName);
    d->sql = d->database->sqlGenerator()->selectCommand(d->tableName,
                                                        AbstractSqlGenerator::SingleField,
                                                        QStringLiteral("*"),
                                                        d->wherePhrase,
                                                        d->relations,
                                                        d->skip,
                                                        d->take);

    QSqlQuery q = d->database->exec(d->sql);

    while (q.next()) {
        O obj = allocator(q);
        ret.append(obj);
    }

    return ret;
}

//template <typename T>
//inline Query<T> &createQuery(TableSet<T> *tableSet)
//{
//    return tableSet->query();
//}

template <class T>
Q_OUTOFLINE_TEMPLATE Query<T>::Query(Database *database, AbstractTableSet *tableSet)
    : d(new QueryData(database))
{
    //Q_D(AbstractQuery);

    d->database = database;
    d->tableSet = tableSet;
    d->className = QString::fromUtf8(T::staticMetaObject.className());
    d->tableName =
            d->database->model()
            .tableByClassName(d->className)
            ->name();
}

template<class T>
Q_OUTOFLINE_TEMPLATE Query<T>::Query(const Query<T> &other)  {
    d = other.d->clone();
}

template<class T>
Q_OUTOFLINE_TEMPLATE Query<T>::Query(Query<T> &&other) {
    d = std::move(other.d);
    other.d = nullptr;
}

template <class T>
Q_OUTOFLINE_TEMPLATE Query<T>::~Query()
{
    //Q_D(AbstractQuery);
    delete d;
}

template<class T>
Q_OUTOFLINE_TEMPLATE Query<T> &Query<T>::operator=(const Query<T> &q)
{
    if (this != &q)
    {
        QueryData *p = q.d ? q.d->clone() : nullptr;
        delete d;
        d = p;
    }
    return *this;
}

template <class T>
Q_OUTOFLINE_TEMPLATE RowList<T> Query<T>::toList(int count)
{
    //Q_D(AbstractQuery);
    RowList<T> returnList;
    d->select = QStringLiteral("*");

    d->sql = d->database->sqlGenerator()->selectCommand(
                d->tableName, d->fieldPhrase, d->wherePhrase, d->orderPhrase,
                d->relations, d->skip, count);

    QSqlQuery q = d->database->exec(d->sql);
    if (q.lastError().isValid()) {
        qDebug() << q.lastError().text();
        return returnList;
    }

    QSet<TableModel*> relatedTables;
    relatedTables << d->database->model().tableByName(d->tableName);
    Q_FOREACH (RelationModel *rel, d->relations)
        relatedTables << rel->slaveTable << rel->masterTable;

    struct LevelData{
        QList<int> masters;
        QList<int> slaves;
        QList<QString> masterFields;
        QString keyFiledname;
        QVariant lastKeyValue;
        TableModel *table;
        Row<Table> lastRow;
    };
    QVector<LevelData> levels;
    QSet<QString> importedTables;
    auto add_table = [&](int i, TableModel* table) {
        if (importedTables.contains(table->name()))
            return;
        importedTables.insert(table->name());

        LevelData data;
        data.table = table;
        data.keyFiledname = data.table->name()
                            + QStringLiteral(".")
                            + data.table->primaryKey();
        data.lastKeyValue = QVariant();

        QHash<QString, QString> masters;
        Q_FOREACH (RelationModel *rel, d->relations)
            if (rel->slaveTable->name() == table->name())
                masters.insert(rel->masterTable->name(), rel->localProperty);

        for (int j = 0; j < levels.count(); ++j) {
            LevelData &dt = levels[j];

            QHashIterator<QString, QString> it(masters);
            while (it.hasNext()) {
                it.next();

                if (dt.table->name() == it.key()) {
                    data.masters.append(j);
                    data.masterFields.append(it.value());
                    dt.slaves.append(i);
                }
            }
        }

        levels.append(data);
    };
    for (int i = 0; i < d->relations.count(); ++i) {
        RelationModel *rel = d->relations[i];
        add_table(i, rel->masterTable);
        add_table(i, rel->slaveTable);
    }

    if (!importedTables.count()) {
        LevelData data;
        data.table = d->database->model().tableByName(d->tableName);
        data.keyFiledname = d->tableName + QStringLiteral(".") + data.table->primaryKey();
        data.lastKeyValue = QVariant();

        levels.append(data);
    }

    QVector<bool> checked;
    checked.reserve(levels.count());
    for (int i = 0; i < levels.count(); ++i)
        checked.append(false);

    while (q.next()) {
        checked.fill(false);

        int p = levels.count();
        int n = -1;

        while (p) {
            //            Q_ASSERT(p != lastP);
            //            if (p == lastP)
            //                qFatal("NULL Loop detected");

            ++n;
            n = n % levels.count();
            if (checked[n])
                continue;
            LevelData &data = levels[n];

            // check if key value is changed
            if (data.lastKeyValue == q.value(data.keyFiledname)) {
                --p;
//                qDebug() << "key os not changed for" << data.keyFiledname;
                continue;
            }

            // check if master if current table has processed
            Q_FOREACH (int m, data.masters)
                if (!checked[m]) {
//                    qDebug() << "row is checked";
                    continue;
                }

            checked[n] = true;
            --p;
            data.lastKeyValue = q.value(data.keyFiledname);

            //create table row
            Row<Table> row;
            if (data.table->className() == d->className) {
                Row<T> tmpRow = Nut::create<T>();
                row = tmpRow.template objectCast<Table>();

                row->init();
#ifdef NUT_RAW_POINTER
                returnList.append(dynamic_cast<T*>(table));
#else
                returnList.append(tmpRow);
#endif
                d->tableSet->add(row);

            } else {
                Table *table;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                auto childMetaObject = QMetaType(data.table->typeId()).metaObject();
#else
                const QMetaObject *childMetaObject
                        = QMetaType::metaObjectForType(data.table->typeId());
#endif
                table = qobject_cast<Table *>(childMetaObject->newInstance());
//                table = dynamic_cast<Table *>(QMetaType::create(data.table->typeId()));
                if (!table)
                    qFatal("Could not create instance of %s",
                           qPrintable(data.table->name()));
                row = createFrom(table);
            }

            QList<FieldModel*> childFields = data.table->fields();
            Q_FOREACH (FieldModel *field, childFields)
                row->setProperty(field->name.toLatin1().data(),
                                   d->database->sqlGenerator()->unescapeValue(
                                   field->type,
                                   q.value(data.table->name() + QStringLiteral(".") + field->name)));

            for (int i = 0; i < data.masters.count(); ++i) {
                int master = data.masters[i];
                auto tableset = levels[master].lastRow.data()->childTableSet(
                            data.table->className());
                if (tableset)
                    tableset->add(row);

                //set key
                {
                    QString setterName = data.masterFields[master];
                    setterName[0] = setterName[0].toUpper();
                    setterName.prepend(QStringLiteral("set"));

                    bool ok = row->metaObject()->invokeMethod(row.data(),
                                                              setterName.toStdString().c_str(),
                                                              Qt::DirectConnection,
                                                              Q_ARG(Nut::Row<Nut::Table>,
                                                                    levels[master].lastRow));

                    if (Q_UNLIKELY(!ok))
                        qWarning("Unable to invoke method %s on object", qPrintable(setterName));
                }
            }

            row->setStatus(Table::FetchedFromDB);
//            row->setParent(this);
            row->clear();

            //set last created row
            data.lastRow = row;
        } //while
    } // while

    return returnList;

}

template <typename T>
template <typename F>
Q_OUTOFLINE_TEMPLATE QList<F> Query<T>::select(const FieldPhrase<F> f)
{
    //Q_D(AbstractQuery);
    QList<F> ret;

    d->joins.prepend(d->tableName);
    d->sql = d->database->sqlGenerator()->selectCommand(
                d->tableName,
                AbstractSqlGenerator::SingleField, f.data->toString(),
                d->wherePhrase,
                d->relations,
                d->skip, d->take);

    QSqlQuery q = d->database->exec(d->sql);

    while (q.next()) {
        QVariant v = q.value(0);
        ret.append(v.value<F>());
    }

    return ret;
}

template <class T>
Q_OUTOFLINE_TEMPLATE Row<T> Query<T>::first()
{
    skip(0);
    RowList<T> list = toList(1);

    if (list.count())
        return list.first();
    else
        return nullptr;
}

template <class T>
Q_OUTOFLINE_TEMPLATE int Query<T>::count()
{
    //Q_D(AbstractQuery);

    d->joins.prepend(d->tableName);
    d->select = QStringLiteral("COUNT(*)");
    d->sql = d->database->sqlGenerator()->selectCommand(
                d->tableName,
                AbstractSqlGenerator::Count,
                QStringLiteral("*"),
                d->wherePhrase,
                d->relations);
    QSqlQuery q = d->database->exec(d->sql);

    if (q.next())
        return q.value(0).toInt();
    return 0;
}

template <class T>
Q_OUTOFLINE_TEMPLATE QVariant Query<T>::max(const FieldPhrase<int> &f)
{
    //Q_D(AbstractQuery);

    d->joins.prepend(d->tableName);
    d->sql = d->database->sqlGenerator()->selectCommand(
                d->tableName,
                AbstractSqlGenerator::Max, f.data->toString(),
                d->wherePhrase,
                d->relations);
    QSqlQuery q = d->database->exec(d->sql);

    if (q.next())
        return q.value(0).toInt();
    return 0;
}

template <class T>
Q_OUTOFLINE_TEMPLATE QVariant Query<T>::min(const FieldPhrase<int> &f)
{
    //Q_D(AbstractQuery);

    d->joins.prepend(d->tableName);
    d->sql = d->database->sqlGenerator()->selectCommand(
                d->tableName,
                AbstractSqlGenerator::Min, f.data->toString(),
                d->wherePhrase,
                d->relations);
    QSqlQuery q = d->database->exec(d->sql);

    if (q.next())
        return q.value(0).toInt();
    return 0;
}

template <class T>
Q_OUTOFLINE_TEMPLATE QVariant Query<T>::sum(const FieldPhrase<int> &f)
{
    //Q_D(AbstractQuery);

    d->joins.prepend(d->tableName);
    d->sql = d->database->sqlGenerator()->selectCommand(
                d->tableName,
                AbstractSqlGenerator::Sum, f.data->toString(),
                d->wherePhrase,
                d->relations);
    QSqlQuery q = d->database->exec(d->sql);

    if (q.next())
        return q.value(0).toInt();
    return 0;
}

template <class T>
Q_OUTOFLINE_TEMPLATE QVariant Query<T>::average(const FieldPhrase<int> &f)
{
    //Q_D(AbstractQuery);

    d->joins.prepend(d->tableName);
    d->sql = d->database->sqlGenerator()->selectCommand(
                d->tableName,
                AbstractSqlGenerator::Average, f.data->toString(),
                d->wherePhrase,
                d->relations);
    QSqlQuery q = d->database->exec(d->sql);

    if (q.next())
        return q.value(0).toInt();
    return 0;
}

template<class T>
Q_OUTOFLINE_TEMPLATE QVariant Query<T>::insert(const AssignmentPhraseList &p)
{
    //Q_D(AbstractQuery);
    d->sql = d->database->sqlGenerator()
            ->insertCommand(d->tableName, p);
    QSqlQuery q = d->database->exec(d->sql);

   return q.lastInsertId();
}

template <class T>
Q_OUTOFLINE_TEMPLATE Query<T> &Query<T>::join(const QString &className)
{
    //Q_D(AbstractQuery);

    RelationModel *rel = d->database->model()
            .relationByClassNames(d->className, className);
    if (!rel)
        rel = d->database->model()
                .relationByClassNames(className, d->className);

    if (!rel) {
        qDebug() << "No relation between" << d->className
                << "and" << className;
        return *this;
    }

    d->relations.append(rel);
    d->joins.append(className);
    return *this;
}

template<class T>
Q_OUTOFLINE_TEMPLATE Query<T> &Query<T>::join(Table *c)
{
    join(c->metaObject()->className());
    return *this;
}

template <class T>
Q_OUTOFLINE_TEMPLATE Query<T> &Query<T>::where(const ConditionalPhrase &ph)
{
    //Q_D(AbstractQuery);
    if (d->wherePhrase.data)
        d->wherePhrase = d->wherePhrase && ph;
    else
        d->wherePhrase = ph;
    return *this;
}

template <class T>
Q_OUTOFLINE_TEMPLATE Query<T> &Query<T>::setWhere(const ConditionalPhrase &ph)
{
    //Q_D(AbstractQuery);
    d->wherePhrase = ph;
    return *this;
}

template<class T>
Q_OUTOFLINE_TEMPLATE Query<T> &Query<T>::skip(int n)
{
    //Q_D(AbstractQuery);
    d->skip = n;
    return *this;
}

template<class T>
Q_OUTOFLINE_TEMPLATE Query<T> &Query<T>::take(int n)
{
    //Q_D(AbstractQuery);
    d->take = n;
    return *this;
}

template<class T>
Q_OUTOFLINE_TEMPLATE Query<T> &Query<T>::fields(const PhraseList &ph)
{
    //Q_D(AbstractQuery);
    d->fieldPhrase = ph;
    return *this;
}

//template <class T>
//Q_OUTOFLINE_TEMPLATE Query<T> &Query<T>::orderBy(QString fieldName,
//                                                 QString type)
//{
//    //Q_D(AbstractQuery);
//    d->orderPhrases.append(fieldName, type);
//    return *this;
//}

template <class T>
Q_OUTOFLINE_TEMPLATE Query<T> &Query<T>::orderBy(const PhraseList &ph)
{
    //Q_D(AbstractQuery);
    d->orderPhrase = ph;
    return *this;
}

template <class T>
Q_OUTOFLINE_TEMPLATE int Query<T>::update(const AssignmentPhraseList &ph)
{
    //Q_D(AbstractQuery);

    d->sql = d->database->sqlGenerator()->updateCommand(
                d->tableName,
                ph,
                d->wherePhrase);

    QSqlQuery q = d->database->exec(d->sql);

    return q.numRowsAffected();
}

template <class T>
Q_OUTOFLINE_TEMPLATE int Query<T>::remove()
{
    //Q_D(AbstractQuery);

    d->sql = d->database->sqlGenerator()->deleteCommand(
                d->tableName, d->wherePhrase);
    QSqlQuery q = d->database->exec(d->sql);

    return q.numRowsAffected();
}

template <class T>
Q_OUTOFLINE_TEMPLATE QSqlQueryModel *Query<T>::toModel()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    toModel(model);
    return model;
}

template <class T>
Q_OUTOFLINE_TEMPLATE void Query<T>::toModel(QSqlQueryModel *model)
{
    //Q_D(AbstractQuery);

    d->sql = d->database->sqlGenerator()->selectCommand(
                d->tableName,
                d->fieldPhrase,
                d->wherePhrase, d->orderPhrase, d->relations,
                d->skip, d->take);

    DatabaseModel dbModel = d->database->model();
    model->setQuery(d->sql, d->database->database());

    int fieldIndex = 0;

    if (d->fieldPhrase.data.count()) {
        Q_FOREACH (const PhraseData *pd, d->fieldPhrase.data) {
            QString displayName = dbModel
                                      .tableByClassName(QString::fromUtf8(pd->className))
                                      ->field(QString::fromUtf8(pd->fieldName))
                                      ->displayName;

            model->setHeaderData(fieldIndex++,
                                 Qt::Horizontal,
                                 displayName);
        }
    } else {
        TableModel *tbl = d->database->model().tableByName(d->tableName);
        Q_FOREACH (FieldModel *f, tbl->fields()) {
            model->setHeaderData(fieldIndex++,
                                 Qt::Horizontal,
                                 f->displayName);
        }
    }
}

template<class T>
Q_OUTOFLINE_TEMPLATE void Query<T>::toModel(SqlModel *model)
{
    //Q_D(AbstractQuery);

    d->sql = d->database->sqlGenerator()->selectCommand(
                d->tableName,
                d->fieldPhrase,
                d->wherePhrase, d->orderPhrase, d->relations,
                d->skip, d->take);

    model->setTable(toList());
    /*
    DatabaseModel dbModel = d->database->model();
    model->setQuery(d->sql, d->database->database());

    int fieldIndex = 0;

    if (d->fieldPhrase.data.count()) {
        Q_FOREACH (const PhraseData *pd, d->fieldPhrase.data) {
            QString displayName = dbModel.tableByClassName(pd->className)
                    ->field(pd->fieldName)->displayName;

            model->setHeaderData(fieldIndex++,
                                 Qt::Horizontal,
                                 displayName);
        }
    } else {
        TableModel *tbl = d->database->model().tableByName(d->tableName);
        Q_FOREACH (FieldModel *f, tbl->fields()) {
            model->setHeaderData(fieldIndex++,
                                 Qt::Horizontal,
                                 f->displayName);
        }
    }*/
}

template <class T>
Q_OUTOFLINE_TEMPLATE QString Query<T>::sqlCommand() const
{
    //Q_D(const AbstractQuery);
    return d->sql;
}

//TODO: complete this class later
//class RawQuery : public Query<void>
//{
//public:
//    void setRawCommand(const QString &sql) {

//    }
//};

NUT_END_NAMESPACE

#endif // NUT_QUERY_H
