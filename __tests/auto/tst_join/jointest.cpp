#include <QtTest>
#include <QJsonDocument>
#include <QSqlError>

#include "consts.h"

#include "jointest.h"
#include "query.h"
#include "tableset.h"
#include "tablemodel.h"
#include "databasemodel.h"

#include "user.h"
#include "post.h"
#include "comment.h"
#include "score.h"

#define PRINT(x) qDebug() << #x "=" << x;
JoinTest::JoinTest(QObject *parent) : QObject(parent)
{
}

void JoinTest::initTestCase()
{
    REGISTER(User);
    REGISTER(Post);
    REGISTER(Comment);
    REGISTER(Score);
    REGISTER(WeblogDatabase);

    db.setDriver(DRIVER);
    db.setHostName(HOST);
    db.setDatabaseName("nut_tst_join.db");
    db.setUserName(USERNAME);
    db.setPassword(PASSWORD);

    bool ok = db.open();

//    db.comments()->query()->remove();
//    db.posts()->query()->remove();

    QTEST_ASSERT(ok);
}

void JoinTest::join()
{
    auto q = db.comments()->query()
            ->join<User>()
            ->join<Post>();

//    Comment *comment = q->first();
    auto comments = q->toList();
//    Comment *comment = q->toList().first();
//    qDebug() << q->sqlCommand();
    PRINT(comments.length());

//    QTEST_ASSERT(comments.length());
//    QTEST_ASSERT(comments[0]->author());
//    QTEST_ASSERT(comments[0]->author()->username() == "admin");
}

void JoinTest::join2()
{
    auto q = db.users()->query()
            ->join<Comment>()
            ->join<Score>();

//    Comment *comment = q->first();
    auto comments = q->toList();
//    Comment *comment = q->toList().first();
//    qDebug() << q->sqlCommand();
//    QTEST_ASSERT(comments.length());
//    QTEST_ASSERT(comments[0]->author());
//    QTEST_ASSERT(comments[0]->author()->username() == "admin");
}

QTEST_MAIN(JoinTest)
