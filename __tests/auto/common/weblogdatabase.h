#ifndef TDATABASE_H
#define TDATABASE_H

#include <QtNut/database.h>

#ifdef NUT_NAMESPACE
using namespace NUT_NAMESPACE;
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
Q_MOC_INCLUDE("user.h")
Q_MOC_INCLUDE("post.h")
Q_MOC_INCLUDE("score.h")
Q_MOC_INCLUDE("comment.h")
#endif

class Post;
class Comment;
class User;
class Score;
class WeblogDatabase : public Database
{
    Q_OBJECT

    NUT_DB_VERSION(1)

    NUT_DECLARE_TABLE(Post, posts)
    NUT_DECLARE_TABLE(Comment, comments)
    NUT_DECLARE_TABLE(User, users)
    NUT_DECLARE_TABLE(Score, scores)

public:
    WeblogDatabase();
};


#endif // TDATABASE_H
