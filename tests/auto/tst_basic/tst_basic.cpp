#include <QtTest>
#include <QJsonDocument>
#include <QSqlError>
#include <QElapsedTimer>

#include "consts.h"

#include "tst_basic.h"
#include "query.h"
#include "tableset.h"
#include "tablemodel.h"
#include "databasemodel.h"

#include "user.h"
#include "post.h"
#include "comment.h"
#include "score.h"

BasicTest::BasicTest(QObject *parent) : QObject(parent)
{
}

void BasicTest::initTestCase()
{
    //register all entities with Qt-MetaType mechanism
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

    qDebug().noquote() << "Connecting to" << DATABASE;

    bool ok = db.open();
    QVERIFY(ok);

    db.comments()->query().remove();
    db.posts()->query().remove();
    db.users()->query().remove();
    db.scores()->query().remove();
}

void BasicTest::dataSchema()
{
//    auto json = db.model().toJson();
//    auto model = DatabaseModel::fromJson(json);

    //    qDebug() << model.toJson();
    //    qDebug() << db.model().toJson();
    //    QTEST_ASSERT(model == db.model());
}

void BasicTest::createUser()
{
    user = Nut::create<User>();
    user->setUsername(QStringLiteral("admin"));
    user->setPassword(QStringLiteral("123456"));
    db.users()->append(user);
    QTEST_ASSERT(db.saveChanges() != 0);
    QTEST_ASSERT(user->id() != 0);
}

void BasicTest::createPost()
{
    TIC();
    auto newPost = Nut::create<Post>();
    newPost->setTitle(QStringLiteral("post title"));
    newPost->setSaveDate(QDateTime::currentDateTime());
    newPost->setPublic(false);

    db.posts()->append(newPost);

    for(int i = 0 ; i < 3; i++){
        auto comment = Nut::create<Comment>();
        comment->setMessage(QStringLiteral("comment #") + QString::number(i));
        comment->setSaveDate(QDateTime::currentDateTime());
        comment->setAuthorId(user->id());
        newPost->comments()->append(comment);
    }
    for (int i = 0; i < 10; ++i) {
        auto score = Nut::create<Score>();
        score->setScore(i % 5);
        score->setCondition(1); // test keyword on mysql
        newPost->scores()->append(score);
    }

    QTEST_ASSERT(db.saveChanges() != 0);

    postId = newPost->id();

    QTEST_ASSERT(newPost->id() != 0);

    TOC();
}

void BasicTest::createPost2()
{
    //create post on the fly
    QVariant postIdVar = db.posts()->query().insert(
              (Post::titleField() = QStringLiteral("This is a sample"))
                & (Post::isPublicField() = true));

    QVERIFY(postIdVar.type() == QVariant::LongLong
                 || postIdVar.type() == QVariant::ULongLong
                 || postIdVar.type() == QVariant::Double);
    int postId = postIdVar.toInt();

    for(int i = 0 ; i < 3; i++){
        auto comment = Nut::create<Comment>();
        comment->setMessage(QStringLiteral("comment #") + QString::number(i + 2));
        comment->setSaveDate(QDateTime::currentDateTime());
        comment->setAuthor(user);
        //join child to master by id
        comment->setPostId(postId);
        db.comments()->append(comment);
    }
    QTEST_ASSERT(db.saveChanges() != 0);

    QVERIFY(postId != 0);
}

void BasicTest::updatePostOnTheFly()
{
    auto c = db.posts()->query()
            .where(Post::idField() == postId)
            .update(Post::titleField() = QStringLiteral("New title"));

    QCOMPARE(c, 1);

    auto titles = db.posts()
                      ->query()
                      .where(Post::idField() == postId)
                      .select(Post::titleField());

    QCOMPARE(titles.count(), 1);
    QCOMPARE(titles.at(0), QStringLiteral("New title"));
}

void BasicTest::selectPublicts()
{
    auto publinPostsCount = db.posts()->query()
            .where(Post::isPublicField() == true)
            .count();

    auto nonPublicPostsCount = db.posts()->query()
            .where(Post::isPublicField() == false)
            .count();

    QCOMPARE(publinPostsCount, 1);
    QCOMPARE(nonPublicPostsCount, 1);
}

void BasicTest::selectPosts()
{
    auto q = db.posts()->query()
        .join<Comment>()
        .orderBy((!Post::saveDateField()) | Post::bodyField())
        .where(Post::idField() == postId);

    auto posts = q.toList();
    post = posts.at(0);
    post->setBody(QLatin1String());

    PRINT(posts.length());
    PRINT(posts.at(0)->comments()->length());
    QCOMPARE(posts.length(), 1);
    QCOMPARE(posts.at(0)->comments()->length(), 3);
    QCOMPARE(posts.at(0)->title(), QStringLiteral("post title"));

    QCOMPARE(posts.at(0)->comments()->at(0)->message(), QStringLiteral("comment #0"));
    QCOMPARE(posts.at(0)->comments()->at(1)->message(), QStringLiteral("comment #1"));
    QCOMPARE(posts.at(0)->comments()->at(2)->message(), QStringLiteral("comment #2"));
    db.cleanUp();
}

void BasicTest::selectScoreAverage()
{
    bool ok;
    auto avg = db.scores()
                   ->query()
                   .join<Post>()
                   .where(Post::idField() == postId)
                   .average(Score::scoreField())
                   .toInt(&ok);

    QVERIFY(ok);
    QCOMPARE(avg, 2);
}

void BasicTest::selectScoreSum()
{
    auto sum = db.scores()->query().sum(Score::scoreField());
    QCOMPARE(sum, 20);
}

void BasicTest::selectScoreCount()
{
    auto count = db.scores()->query().count();
    QCOMPARE(count, 10);
}

void BasicTest::selectFirst()
{
    auto posts = db.posts()->query()
            .orderBy(Post::idField())
            .first();

    QVERIFY(posts != Q_NULLPTR);
}

void BasicTest::selectPostsWithoutTitle()
{
    auto q = db.posts()->query();
    q.where(Post::titleField().isNull());
    auto count = q.count();
    QCOMPARE(count, 0);
}

void BasicTest::selectPostIds()
{
    auto q = db.posts()->query();
    auto ids = q.select(Post::idField());

    QCOMPARE(ids.count(), 2);
}

void BasicTest::selectPostsWithComments()
{
    auto posts = db.posts()->query().join<Comment>().toList();

    QCOMPARE(posts.first()->comments()->length(), 3);
}

void BasicTest::selectCommantsWithPost()
{
    auto comments = db.comments()->query().join<Post>().toList();
    QCOMPARE(comments.length(), 6);
    QVERIFY(!comments.first()->post().isNull());
}

void BasicTest::testDate()
{
    QDateTime d = QDateTime::currentDateTime();
    QTime t = QTime(d.time().hour(), d.time().minute(), d.time().second());
    d.setTime(t);

    auto newPost = Nut::create<Post>();
    newPost->setTitle(QStringLiteral("post title"));
    newPost->setSaveDate(d);
    newPost->setPublic(true);

    db.posts()->append(newPost);

    db.saveChanges();

    auto q = db.posts()->query()
            .where(Post::idField() == newPost->id())
            .orderBy(Post::idField())
            .first();

    QCOMPARE(q->saveDate(), d);
}

void BasicTest::testLimitedQuery()
{
    auto comments = db.comments()->query()
                        .toList(2);
    QCOMPARE(comments.length(), 2);
}

void BasicTest::join()
{
//    TIC();
//    auto q = db.comments()->query()
//            ->join<User>()
//            ->join<Post>();

//    auto comments = q->toList();

//    TOC();
//    QTEST_ASSERT(comments.length());
//    QTEST_ASSERT(comments[0]->author());
//    QTEST_ASSERT(comments[0]->author()->username() == "admin");
}


void BasicTest::selectWithInvalidRelation()
{
    auto q = db.posts()->query();
    q.join(QStringLiteral("Invalid_Class_Name"));
    q.toList();
}

void BasicTest::modifyPost()
{
    auto q = db.posts()->query()
            .where(Post::idField() == postId)
            .orderBy(Post::idField());

    Nut::Row<Post> post = q.first();

    QTEST_ASSERT(post != nullptr);

    post->setTitle(QStringLiteral("new name"));
    db.saveChanges();

    q = db.posts()->query()
            .where(Post::idField() == postId)
            .orderBy(Post::idField());

    post = q.first();
    PRINT(post->title());
    QCOMPARE(post->title(), "new name");
}

void BasicTest::emptyDatabase()
{
//    auto commentsCount = db.comments()->query().remove();
//    auto postsCount = db.posts()->query().remove();
//    QTEST_ASSERT(postsCount == 3);
//    QTEST_ASSERT(commentsCount == 6);
}

void BasicTest::cleanupTestCase()
{
//    post->deleteLater();
//    user->deleteLater();

    //release models before exiting
//    qDeleteAll(TableModel::allModels());

//    if (QFile::remove("nut_tst_basic"))
//        qDebug() << "database removed";

    PRINT_FORM(db);
}

QTEST_MAIN(BasicTest)
