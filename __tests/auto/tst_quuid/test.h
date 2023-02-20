#ifndef TEST_H
#define TEST_H

#include <QUuid>

#include "table.h"

class Test : public NUT_WRAP_NAMESPACE(Table)
{
    Q_OBJECT

    NUT_PRIMARY_KEY(id)
    NUT_DECLARE_FIELD(QUuid, id, id, setId)
    NUT_DECLARE_FIELD(QUuid, uuid, uuid, setUuid)

public:
    Q_INVOKABLE Test(QObject *parentTableSet = nullptr);
};

Q_DECLARE_METATYPE(Test*)

#endif // TEST_H
