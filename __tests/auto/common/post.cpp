#include "post.h"
#include "comment.h"
#include "score.h"
#include "tableset.h"

Post::Post(QObject *parent) : Table(parent),
    m_id(0), m_title(""),
    m_comments(new TableSet<Comment>(this)),
    m_scores(new TableSet<Score>(this))
{
    init();
}

int Post::id() const
{
    return m_id;
}

QString Post::title() const
{
    return m_title;
}

QDateTime Post::saveDate() const
{
    return m_saveDate;
}

QString Post::body() const
{
    return m_body;
}

bool Post::isPublic() const
{
    return m_isPublic;
}

void Post::setId(int id)
{
    if (m_id == id)
        return;

    m_id = id;
    Q_EMIT idChanged(m_id);
}

void Post::setTitle(QString title)
{
    if (m_title == title)
        return;

    m_title = title;
    Q_EMIT titleChanged(m_title);
}

void Post::setSaveDate(QDateTime saveDate)
{
    if (m_saveDate == saveDate)
        return;

    m_saveDate = saveDate;
    Q_EMIT saveDateChanged(m_saveDate);
}

void Post::setBody(QString body)
{
    if (m_body == body)
        return;

    m_body = body;
    Q_EMIT bodyChanged(m_body);
}

void Post::setPublic(bool isPublic)
{
    if (m_isPublic == isPublic)
        return;

    m_isPublic = isPublic;
    Q_EMIT isPublicChanged(m_isPublic);
}

NUT_IMPLEMENT_CHILD_TABLE(Post, Comment, comments)
NUT_IMPLEMENT_CHILD_TABLE(Post, Score, scores)
