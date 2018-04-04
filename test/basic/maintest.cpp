#include <QtTest>
#include <QJsonDocument>
#include <QSqlError>
#include <QElapsedTimer>

#include "consts.h"

#include "maintest.h"
#include "query.h"
#include "tableset.h"
#include "tablemodel.h"
#include "databasemodel.h"

#include "user.h"
#include "post.h"
#include "comment.h"
#include "score.h"

#define PRINT(x) qDebug() << #x "=" << x;
#define TIC()  QElapsedTimer timer; timer.start()
#define TOC()  qDebug() << QString("Elapsed time: %1ms for %2") \
    .arg(timer.elapsed() / 1000.) \
    .arg(__func__)

MainTest::MainTest(QObject *parent) : QObject(parent)
{
}

void MainTest::initTestCase()
{
    qDebug() << "User type id:" << qRegisterMetaType<User*>();
    qDebug() << "Post type id:" << qRegisterMetaType<Post*>();
    qDebug() << "Score type id:" << qRegisterMetaType<Score*>();
    qDebug() << "Comment type id:" << qRegisterMetaType<Comment*>();
    qDebug() << "DB type id:" << qRegisterMetaType<WeblogDatabase*>();

    db.setDriver(DRIVER);
    db.setHostName(HOST);
    db.setDatabaseName("nut_tst_basic");
    db.setUserName(USERNAME);
    db.setPassword(PASSWORD);

    bool ok = db.open();

    db.commentTable()->query()->remove();
    db.postTable()->query()->remove();

    QTEST_ASSERT(ok);
}

void MainTest::dataScheema()
{
//    auto json = db.model().toJson();
//    auto model = DatabaseModel::fromJson(json);

    //    qDebug() << model.toJson();
    //    qDebug() << db.model().toJson();
    //    QTEST_ASSERT(model == db.model());
}

void MainTest::createUser()
{
    user = new User;
    user->setUsername("admin");
    user->setPassword("123456");
    db.userTable()->append(user);
    db.saveChanges();
}

void MainTest::createPost()
{
    TIC();
    Post *newPost = new Post;
    newPost->setTitle("post title");
    newPost->setSaveDate(QDateTime::currentDateTime());

    db.postTable()->append(newPost);

    for(int i = 0 ; i < 3; i++){
        Comment *comment = new Comment;
        comment->setMessage("comment #" + QString::number(i));
        comment->setSaveDate(QDateTime::currentDateTime());
        comment->setAuthorId(user->id());
        newPost->comments()->append(comment);
    }
    for (int i = 0; i < 10; ++i) {
        Score *score = new Score;
        score->setScore(i % 5);
        newPost->scores()->append(score);
    }

    db.saveChanges();

    postId = newPost->id();

    QTEST_ASSERT(newPost->id() != 0);
    TOC();
    qDebug() << "New post inserted with id:" << newPost->id();
}

void MainTest::createPost2()
{
    Post *newPost = new Post;
    newPost->setTitle("post title");
    newPost->setSaveDate(QDateTime::currentDateTime());

    db.postTable()->append(newPost);
    db.saveChanges();

    for(int i = 0 ; i < 3; i++){
        Comment *comment = new Comment;
        comment->setMessage("comment #" + QString::number(i + 2));
        comment->setSaveDate(QDateTime::currentDateTime());
        comment->setAuthor(user);
        comment->setPostId(newPost->id());
        db.commentTable()->append(comment);
    }
    db.saveChanges();

    QTEST_ASSERT(newPost->id() != 0);
    qDebug() << "New post2 inserted with id:" << newPost->id();
}

void MainTest::updatePostOnTheFly()
{
    auto c = db.postTable()->query()
            ->where(Post::idField() == postId)
            ->update(Post::titleField() = "New title");

    QTEST_ASSERT(c == 1);
}

void MainTest::selectPublicts()
{
    auto q = db.postTable()->query()
            ->where(Post::isPublicField())
            ->toList();

    auto q2 = db.postTable()->query()
            ->where(!Post::isPublicField())
            ->toList();
}

void MainTest::selectPosts()
{
    auto q = db.postTable()->query()
        ->join<Comment>()//Comment::authorIdField() == Post::idField())
        ->orderBy(!Post::saveDateField() | Post::bodyField())
        ->setWhere(Post::idField() == postId);

    auto posts = q->toList();
    post = posts.at(0);
    post->setBody("");

    PRINT(posts.length());
    PRINT(posts.at(0)->comments()->length());
    QTEST_ASSERT(posts.length() == 1);
    QTEST_ASSERT(posts.at(0)->comments()->length() == 3);
    QTEST_ASSERT(posts.at(0)->title() == "post title");

    QTEST_ASSERT(posts.at(0)->comments()->at(0)->message() == "comment #0");
    QTEST_ASSERT(posts.at(0)->comments()->at(1)->message() == "comment #1");
    QTEST_ASSERT(posts.at(0)->comments()->at(2)->message() == "comment #2");
    db.cleanUp();
}

void MainTest::selectScoreAverage()
{
    auto a = db.scoreTable()->query()
            ->join<Post>()
            ->setWhere(Post::idField() == 1)
            ->average(Score::scoreField());
    qDebug() << a;
}

void MainTest::selectFirst()
{
    auto posts = db.postTable()->query()
        ->first();

    QTEST_ASSERT(posts != nullptr);
}

void MainTest::selectPostsWithoutTitle()
{
    auto q = db.postTable()->query();
    q->setWhere(Post::titleField().isNull());
    auto count = q->count();
    QTEST_ASSERT(count == 0);
}

void MainTest::selectPostIds()
{
    auto q = db.postTable()->query();
    auto ids = q->select(Post::idField());
qDebug() << q->sqlCommand();
    QTEST_ASSERT(ids.count() == 2);
}

void MainTest::testDate()
{
    QDateTime d = QDateTime::currentDateTime();
    QTime t = QTime(d.time().hour(), d.time().minute(), d.time().second());
    d.setTime(t);

    Post *newPost = new Post;
    newPost->setTitle("post title");
    newPost->setSaveDate(d);

    db.postTable()->append(newPost);

    db.saveChanges(true);

    auto q = db.postTable()->query()
            ->setWhere(Post::idField() == newPost->id())
            ->first();

    QTEST_ASSERT(q->saveDate() == d);
}

void MainTest::join()
{
    TIC();
    auto q = db.commentTable()->query()
            ->join<User>()
            ->join<Post>();

    auto comments = q->toList();

    TOC();
    QTEST_ASSERT(comments.length());
    QTEST_ASSERT(comments[0]->author());
    QTEST_ASSERT(comments[0]->author()->username() == "admin");
}


void MainTest::selectWithInvalidRelation()
{
    auto q = db.postTable()->query();
    q->join("Invalid_Class_Name");
    q->toList();
}

void MainTest::modifyPost()
{
    auto q = db.postTable()->query();
    q->setWhere(Post::idField() == postId);

    Post *post = q->first();

    QTEST_ASSERT(post != 0);

    post->setTitle("new name");
    db.saveChanges();

    q = db.postTable()->query()
            ->setWhere(Post::idField() == postId);

    post = q->first();
    PRINT(post->title());
    QTEST_ASSERT(post->title() == "new name");
}

void MainTest::emptyDatabase()
{
    auto commentsCount = db.commentTable()->query()->remove();
    auto postsCount = db.postTable()->query()->remove();
    QTEST_ASSERT(postsCount == 3);
    QTEST_ASSERT(commentsCount == 6);
}

void MainTest::cleanupTestCase()
{
    post->deleteLater();
    user->deleteLater();

    qDeleteAll(TableModel::allModels());
    DatabaseModel::deleteAllModels();
}

QTEST_MAIN(MainTest)
