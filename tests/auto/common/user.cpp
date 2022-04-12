#include "comment.h"
#include "score.h"

#include "user.h"

User::User(QObject *tableSet) : Nut::Table(tableSet),
      m_comments(new Nut::TableSet<Comment>(this)),
    m_scores(new Nut::TableSet<Score>(this))
{
    init();
}

int User::id() const
{
    return m_id;
}

QString User::username() const
{
    return m_username;
}

QString User::password() const
{
    return m_password;
}

void User::setId(int id)
{
    if (m_id == id)
        return;

    m_id = id;
    Q_EMIT idChanged(m_id);
}

void User::setUsername(QString username)
{
    if (m_username == username)
        return;

    m_username = username;
    Q_EMIT usernameChanged(m_username);
}

void User::setPassword(QString password)
{
    if (m_password == password)
        return;

    m_password = password;
    Q_EMIT passwordChanged(m_password);
}

NUT_IMPLEMENT_CHILD_TABLE(User, Comment, comments)
