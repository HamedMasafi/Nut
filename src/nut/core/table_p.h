#ifndef TABLEPRIVATE_H
#define TABLEPRIVATE_H

#include <QtCore/QSet>
#include <QtCore/QSharedData>

#include <QtNut/nut_global.h>
#include <QtNut/table.h>

NUT_BEGIN_NAMESPACE

class TableModel;
class Table;
class AbstractTableSet;
class AbstractForeignContainer;
class TablePrivate : public QSharedData {
    Table *q_ptr;
    Q_DECLARE_PUBLIC(Table)

public:
    TablePrivate();


    TableModel *model;
    Table::Status status;
    QSet<QString> changedProperties;
    AbstractTableSet *parentTableSet;
    QSet<AbstractTableSet*> childTableSets;
    QMap<QString, AbstractForeignContainer*> foreignContainers;
    void refreshModel();
};

NUT_END_NAMESPACE

#endif // TABLEPRIVATE_H
