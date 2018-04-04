#ifndef SCORE_H
#define SCORE_H

#include "table.h"

class User;
class Post;
class Score : public Nut::Table
{
    Q_OBJECT

    NUT_PRIMARY_AUTO_INCREMENT(id)
    NUT_DECLARE_FIELD(int, id, id, setId)

    NUT_DECLARE_FIELD(int, score, score, setScore)

    NUT_FOREGION_KEY(Post, int, post, post, setPost)
    NUT_FOREGION_KEY(User, int, user, user, setUser)

public:
    Q_INVOKABLE Score(QObject *parent = nullptr);
};

#endif // SCORE_H
