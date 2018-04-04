#ifndef POST_H
#define POST_H

#include <QtCore/qglobal.h>
#include <QtCore/QDateTime>
#include "table.h"
#include "database.h"
#include "databasemodel.h"

#ifdef NUT_NAMESPACE
using namespace NUT_NAMESPACE;
#endif

class Comment;
class Score;
class Post : public Table
{
    Q_OBJECT

    NUT_PRIMARY_AUTO_INCREMENT(id)
    NUT_DECLARE_FIELD(int, id, id, setId)

    NUT_NOT_NULL(title)
    NUT_LEN(title, 50)
    NUT_DECLARE_FIELD(QString, title, title, setTitle)

    NUT_DECLARE_FIELD(QDateTime, saveDate, saveDate, setSaveDate)

    NUT_DECLARE_FIELD(QString, body, body, setBody)
    NUT_DECLARE_FIELD(bool, isPublic, isPublic, setPublic)

    NUT_DECLARE_CHILD_TABLE(Comment, comments)
    NUT_DECLARE_CHILD_TABLE(Score, scores)

public:
    Q_INVOKABLE Post(QObject *parentTableSet = nullptr);

signals:

public slots:
};

Q_DECLARE_METATYPE(Post*)

#endif // POST_H
