#include <QtTest>
#include <QJsonDocument>
#include <QSqlError>

#include "consts.h"

#include "tst_benchmark.h"
#include "query.h"
#include "tableset.h"
#include "tablemodel.h"
#include "databasemodel.h"

#include "user.h"
#include "post.h"
#include "comment.h"
#include "score.h"

BenchmarkTest::BenchmarkTest(QObject *parent) : QObject(parent)
{

}

void BenchmarkTest::initTestCase()
{
    REGISTER(User);
    REGISTER(Post);
    REGISTER(Score);
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

void BenchmarkTest::insert1kPost()
{
    QElapsedTimer t;
    t.start();

    for (int i = 0; i < 100; ++i) {
        auto newPost = Nut::create<Post>();
        newPost->setTitle(QStringLiteral("post title"));
        newPost->setSaveDate(QDateTime::currentDateTime());

        db.posts()->append(newPost);
    }
    db.saveChanges();
    qDebug("1k post inserted in %lld ms", t.elapsed());

}

QTEST_MAIN(BenchmarkTest)
