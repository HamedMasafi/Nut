#ifndef TABLE1_H
#define TABLE1_H

#include "table.h"
#include <QJsonDocument>

class Table : public NUT_WRAP_NAMESPACE(Table)
{
    Q_OBJECT

    NUT_PRIMARY_AUTO_INCREMENT(id)
    NUT_DECLARE_FIELD(int, id, id, setId)

    NUT_DECLARE_FIELD(QJsonDocument, doc, doc, setDoc)

public:
    Q_INVOKABLE Table(QObject *parent = Q_NULLPTR);

};

Q_DECLARE_METATYPE(Table*)

#endif // TABLE1_H
