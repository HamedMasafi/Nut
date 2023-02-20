#include <QDebug>

#include "user.h"
#include "post.h"
#include "comment.h"
#include "user.h"
#include "score.h"
#include "weblogdatabase.h"

WeblogDatabase::WeblogDatabase() : Database(),
    m_posts(new TableSet<Post>(this)),
    m_comments(new TableSet<Comment>(this)),
    m_users(new TableSet<User>(this)),
    m_scores(new TableSet<Score>(this))
{
}
