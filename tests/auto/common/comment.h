#ifndef COMMENT_H
#define COMMENT_H

#include <QtCore/qglobal.h>
#include <QtCore/QDateTime>
#include <QtNut/table.h>

#ifdef NUT_NAMESPACE
using namespace NUT_NAMESPACE;
#endif

class Post;
class User;
class Comment : public Table
{
    Q_OBJECT

    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY messageChanged)
    Q_PROPERTY(QDateTime saveDate READ saveDate WRITE setSaveDate NOTIFY saveDateChanged)
    Q_PROPERTY(qreal point READ point WRITE setPoint NOTIFY pointChanged)

    NUT_PRIMARY_AUTO_INCREMENT(id)
    NUT_FIELD(int, id)
    NUT_FIELD(QString, message)
    NUT_FIELD(QDateTime, saveDate)
    NUT_FIELD(qreal, point)

    NUT_FOREIGN_KEY_DECLARE(Post, int, post, post, setPost)
    NUT_FOREIGN_KEY_DECLARE(User, int, author, author, setAuthor)

    int m_id;
    QString m_message;
    QDateTime m_saveDate;
    qreal m_point;

public:
    Q_INVOKABLE explicit Comment(QObject *parentTableSet = nullptr);

    int id() const;
    QString message() const;
    QDateTime saveDate() const;
    qreal point() const;

public slots:
    void setId(int id);
    void setMessage(QString message);
    void setSaveDate(QDateTime saveDate);
    void setPoint(qreal point);

signals:
    void idChanged(int id);
    void messageChanged(QString message);
    void saveDateChanged(QDateTime saveDate);
    void pointChanged(qreal point);
};

Q_DECLARE_METATYPE(Comment*)

#endif // COMMENT_H
