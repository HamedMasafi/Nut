#include "comment.h"
#include "score.h"

#include "user.h"

User::User(QObject *tableSet) : Table(tableSet),
    m_comments(new TableSet<Comment>(this)),
    m_scores(new TableSet<Score>(this))
{
}

NUT_IMPLEMENT_CHILD_TABLE(User, Comment, comments)
