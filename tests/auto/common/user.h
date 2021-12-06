#ifndef USER_H
#define USER_H

#include <QtNut/table.h>
#include <QtNut/tableset.h>

#include <QtCore/QUuid>
#include <QtCore/QString>

#ifdef NUT_NAMESPACE
using namespace NUT_NAMESPACE;
#endif

class Comment;
class Score;
class User : public NUT_WRAP_NAMESPACE(Table)
{
    Q_OBJECT

    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)

    NUT_PRIMARY_AUTO_INCREMENT(id)
    NUT_FIELD(int, id)

    NUT_NOT_NULL(username)
    NUT_LEN(username, 50)
    NUT_FIELD(QString, username)

    NUT_NOT_NULL(password)
    NUT_LEN(password, 50)
    NUT_FIELD(QString, password)

    NUT_DECLARE_CHILD_TABLE(Comment, comments)
    NUT_DECLARE_CHILD_TABLE(Score, scores)

    int m_id;
    QString m_username;
    QString m_password;

public:
    Q_INVOKABLE User(QObject *parentTableSet = nullptr);

    int id() const;
    QString username() const;
    QString password() const;

public Q_SLOTS:
    void setId(int id);
    void setUsername(QString username);
    void setPassword(QString password);

Q_SIGNALS:
    void idChanged(int id);
    void usernameChanged(QString username);
    void passwordChanged(QString password);
};

Q_DECLARE_METATYPE(User*)

#endif // USER_H
