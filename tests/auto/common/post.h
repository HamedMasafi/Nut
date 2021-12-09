#ifndef POST_H
#define POST_H

#include <QtCore/qglobal.h>
#include <QtCore/QDateTime>
#include <QtNut/table.h>
#include <QtNut/database.h>
#include <QtNut/databasemodel.h>
#include <QSharedPointer>

#ifdef NUT_NAMESPACE
using namespace NUT_NAMESPACE;
#endif

class Comment;
class Score;
class Post : public Table
{
    Q_OBJECT

    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QDateTime saveDate READ saveDate WRITE setSaveDate NOTIFY saveDateChanged)
    Q_PROPERTY(QString body READ body WRITE setBody NOTIFY bodyChanged)
    Q_PROPERTY(bool isPublic READ isPublic WRITE setPublic NOTIFY isPublicChanged)

    NUT_PRIMARY_AUTO_INCREMENT(id)
    NUT_FIELD(int, id)

    NUT_NOT_NULL(title)
    NUT_LEN(title, 50)
    NUT_FIELD(QString, title)

    NUT_FIELD(QDateTime, saveDate)

    NUT_FIELD(QString, body)
    NUT_FIELD(bool, isPublic)

    NUT_DECLARE_CHILD_TABLE(Comment, comments)
    NUT_DECLARE_CHILD_TABLE(Score, scores)

    int m_id;
    QString m_title;
    QDateTime m_saveDate;
    QString m_body;
    bool m_isPublic;

public:
    Q_INVOKABLE Post(QObject *parentTableSet = nullptr);

    int id() const;
    QString title() const;
    QDateTime saveDate() const;
    QString body() const;
    bool isPublic() const;

Q_SIGNALS:
    void idChanged(int id);
    void titleChanged(QString title);
    void saveDateChanged(QDateTime saveDate);
    void bodyChanged(QString body);
    void isPublicChanged(bool isPublic);

public Q_SLOTS:
    void setId(int id);
    void setTitle(QString title);
    void setSaveDate(QDateTime saveDate);
    void setBody(QString body);
    void setPublic(bool isPublic);
};

Q_DECLARE_METATYPE(Post*)

#endif // POST_H
