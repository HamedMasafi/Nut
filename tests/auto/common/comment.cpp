#include "comment.h"
#include "post.h"
#include "user.h"

Comment::Comment(QObject *parent) : Table(parent)
{
    init();
}

int Comment::id() const
{
    return m_id;
}

QString Comment::message() const
{
    return m_message;
}

QDateTime Comment::saveDate() const
{
    return m_saveDate;
}

qreal Comment::point() const
{
    return m_point;
}

void Comment::setId(int id)
{
    if (m_id == id)
        return;

    m_id = id;
    emit idChanged(m_id);
}

void Comment::setMessage(QString message)
{
    if (m_message == message)
        return;

    m_message = message;
    emit messageChanged(m_message);
}

void Comment::setSaveDate(QDateTime saveDate)
{
    if (m_saveDate == saveDate)
        return;

    m_saveDate = saveDate;
    emit saveDateChanged(m_saveDate);
}

void Comment::setPoint(qreal point)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_point, point))
        return;

    m_point = point;
    emit pointChanged(m_point);
}

NUT_FOREIGN_KEY_IMPLEMENT(Comment, Post, int, post, post, setPost)
NUT_FOREIGN_KEY_IMPLEMENT(Comment, User, int, author, author, setAuthor)
