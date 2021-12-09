#ifndef SCORE_H
#define SCORE_H

#include <QUuid>
#include <QtNut/table.h>
#include <QSharedPointer>
#include <QMetaType>

class User;
class Post;
class Score : public NUT_WRAP_NAMESPACE(Table)
{
    Q_OBJECT

    NUT_PRIMARY_AUTO_INCREMENT(id)
    NUT_DECLARE_FIELD(int, id, id, setId)

    NUT_DECLARE_FIELD(int, score, score, setScore)

    NUT_FOREIGN_KEY_DECLARE(Post, int, post, post, setPost)
    NUT_FOREIGN_KEY_DECLARE(User, QUuid, author, author, setAuthor)

public:
    Q_INVOKABLE Score(QObject *parent = Q_NULLPTR);
};

#endif // SCORE_H
