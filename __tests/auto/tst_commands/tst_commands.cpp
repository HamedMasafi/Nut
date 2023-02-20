#include <QtTest>
#include <QJsonDocument>
#include <QSqlError>

#include "consts.h"

#include "tst_commands.h"
#include "query.h"
#include "tableset.h"
#include "tablemodel.h"
#include "databasemodel.h"

#include "post.h"
#include "comment.h"
#include "user.h"
#include "score.h"

CommandsTest::CommandsTest(QObject *parent) : QObject(parent)
{

}

void CommandsTest::initTestCase()
{
    REGISTER(Post);
    REGISTER(Comment);
    REGISTER(WeblogDatabase);

    db.setDriver(DRIVER);
    db.setHostName(HOST);
    db.setDatabaseName(DATABASE);
    db.setUserName(USERNAME);
    db.setPassword(PASSWORD);

    bool ok = db.open();

    QTEST_ASSERT(ok);
}

void CommandsTest::cmd1()
{
    Query<Post> *q = db.posts()->query()
            ->setWhere(Post::titleField() == "test" && Post::idField() < 4 + 5);

    q->toList();

    qDebug() << q->sqlCommand();
}

void CommandsTest::cmd2()
{
    Query<Post> *q = db.posts()->query()
            ->setWhere(!Post::idField().in({1, 2, 3, 4}));

    q->toList();
//    q->update(Post::idField()++);

    qDebug() << q->sqlCommand();
}

void CommandsTest::join()
{
    auto q = db.posts()->query()
            ->join<User>()
            ->toList();

}

QTEST_MAIN(CommandsTest)
