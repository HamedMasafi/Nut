#ifndef MAINTEST_H
#define MAINTEST_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>

#include "weblogdatabase.h"
class Post;
class CommandsTest : public QObject
{
    Q_OBJECT
    WeblogDatabase db;
    int postId;
    Post *post;
    Query<Post> *q;
public:
    explicit CommandsTest(QObject *parent = nullptr);

Q_SIGNALS:

private Q_SLOTS:
    void initTestCase();

    void cmd1();
    void cmd2();
    void join();
};

#endif // MAINTEST_H
