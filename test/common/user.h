#ifndef USER_H
#define USER_H

#include "table.h"
#include "tableset.h"

#include <QtCore/QUuid>
#include <QtCore/QString>

#ifdef NUT_NAMESPACE
using namespace NUT_NAMESPACE;
#endif

class Comment;
class Score;
class User : public Nut::Table
{
    Q_OBJECT

    NUT_PRIMARY_AUTO_INCREMENT(id)
    NUT_DECLARE_FIELD(int, id, id, setId)

    NUT_NOT_NULL(username)
    NUT_LEN(username, 50)
    NUT_DECLARE_FIELD(QString, username, username, setUsername)

    NUT_NOT_NULL(password)
    NUT_LEN(password, 50)
    NUT_DECLARE_FIELD(QString, password, password, setPassword)

    NUT_DECLARE_CHILD_TABLE(Comment, comments)
    NUT_DECLARE_CHILD_TABLE(Score, scores)

public:
    Q_INVOKABLE User(QObject *parentTableSet = nullptr);
};

Q_DECLARE_METATYPE(User*)

#endif // USER_H
