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

    bool ok = db.open();
    QTEST_ASSERT(ok);

    db.comments()->query()->remove();
    db.posts()->query()->remove();
    db.users()->query()->remove();
    db.scores()->query()->remove();
}

void BasicTest::dataScheema()
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
    user->setUsername("admin");
    user->setPassword("123456");
    db.users()->append(user);
    db.saveChanges();
}

void BasicTest::createPost()
{
    TIC();
    auto newPost = Nut::create<Post>();
    newPost->setTitle("post title");
    newPost->setSaveDate(QDateTime::currentDateTime());
    newPost->setPublic(false);

    db.posts()->append(newPost);

    for(int i = 0 ; i < 3; i++){
        auto comment = Nut::create<Comment>();
        comment->setMessage("comment #" + QString::number(i));
        comment->setSaveDate(QDateTime::currentDateTime());
        comment->setAuthorId(user->id());
        newPost->comments()->append(comment);
    }
    for (int i = 0; i < 10; ++i) {
        auto score = Nut::create<Score>();
        score->setScore(i % 5);
        newPost->scores()->append(score);
    }

    db.saveChanges();

    postId = newPost->id();

    QTEST_ASSERT(newPost->id() != 0);
    TOC();
    qDebug() << "New post inserted with id:" << newPost->id();
}

void BasicTest::createPost2()
{
    //create post on the fly
    QVariant postIdVar = db.posts()->query()->insert(
              (Post::titleField() = "This is a sample")
                & (Post::isPublicField() = true));

    QTEST_ASSERT(postIdVar.type() == QVariant::LongLong
                 || postIdVar.type() == QVariant::ULongLong
                 || postIdVar.type() == QVariant::Double);
    int postId = postIdVar.toInt();

    for(int i = 0 ; i < 3; i++){
        auto comment = Nut::create<Comment>();
        comment->setMessage("comment #" + QString::number(i + 2));
        comment->setSaveDate(QDateTime::currentDateTime());
        comment->setAuthor(user);
        //join child to master by id
        comment->setPostId(postId);
        db.comments()->append(comment);
    }
    db.saveChanges();

    QTEST_ASSERT(postId != 0);
}

void BasicTest::updatePostOnTheFly()
{
    auto c = db.posts()->query()
            ->where(Post::idField() == postId)
            ->update(Post::titleField() = "New title");

    QTEST_ASSERT(c == 1);
}

void BasicTest::selectPublicts()
{
    auto q = db.posts()->query()
            ->where(Post::isPublicField())
            ->count();

    auto q2 = db.posts()->query()
            ->where(!Post::isPublicField())
            ->count();

    QTEST_ASSERT(q == 1);
    QTEST_ASSERT(q2 == 1);
}

void BasicTest::selectPosts()
{
    auto q = db.posts()->query()
        ->join<Comment>()
        ->orderBy((!Post::saveDateField()) | Post::bodyField())
        ->setWhere(Post::idField() == postId);

    auto posts = q->toList();
    post = posts.at(0);
    post->setBody("");

    PRINT(posts.length());
    PRINT(posts.at(0)->comments()->length());
    QTEST_ASSERT(posts.length() == 1);
    qDebug() << posts.at(0)->comments()->length();
    QTEST_ASSERT(posts.at(0)->comments()->length() == 3);
    QTEST_ASSERT(posts.at(0)->title() == "post title");

    QTEST_ASSERT(posts.at(0)->comments()->at(0)->message() == "comment #0");
    QTEST_ASSERT(posts.at(0)->comments()->at(1)->message() == "comment #1");
    QTEST_ASSERT(posts.at(0)->comments()->at(2)->message() == "comment #2");
    db.cleanUp();
}

void BasicTest::selectScoreAverage()
{
    auto a = db.scores()->query()
            ->join<Post>()
            ->setWhere(Post::idField() == 1)
            ->average(Score::scoreField());
    qDebug() << a;
}

void BasicTest::selectFirst()
{
    auto posts = db.posts()->query()
            ->orderBy(Post::idField())
            ->first();

    QTEST_ASSERT(posts != Q_NULLPTR);
}

void BasicTest::selectPostsWithoutTitle()
{
    auto q = db.posts()->query();
    q->setWhere(Post::titleField().isNull());
    auto count = q->count();
    QTEST_ASSERT(count == 0);
}

void BasicTest::selectPostIds()
{
    auto q = db.posts()->query();
    auto ids = q->select(Post::idField());
qDebug() << ids.count();
    QTEST_ASSERT(ids.count() == 2);
}

void BasicTest::testDate()
{
    QDateTime d = QDateTime::currentDateTime();
    QTime t = QTime(d.time().hour(), d.time().minute(), d.time().second());
    d.setTime(t);

    auto newPost = Nut::create<Post>();
    newPost->setTitle("post title");
    newPost->setSaveDate(d);

    db.posts()->append(newPost);

    db.saveChanges(true);

    auto q = db.posts()->query()
            ->setWhere(Post::idField() == newPost->id())
            ->orderBy(Post::idField())
            ->first();

    qDebug() << q->saveDate() << d;
    QTEST_ASSERT(q->saveDate() == d);
}

void BasicTest::join()
{
    TIC();
    auto q = db.comments()->query()
            ->join<User>()
            ->join<Post>();

    auto comments = q->toList();
    TOC();
    QTEST_ASSERT(comments.length());
    QTEST_ASSERT(comments[0]->author());
    QTEST_ASSERT(comments[0]->author()->username() == "admin");
}


void BasicTest::selectWithInvalidRelation()
{
    auto q = db.posts()->query();
    q->join("Invalid_Class_Name");
    q->toList();
}

void BasicTest::modifyPost()
{
    auto q = db.posts()->query()
            ->setWhere(Post::idField() == postId)
            ->orderBy(Post::idField());

    Nut::Row<Post> post = q->first();

    QTEST_ASSERT(post != nullptr);

    post->setTitle("new name");
    db.saveChanges();

    q = db.posts()->query()
            ->setWhere(Post::idField() == postId)
            ->orderBy(Post::idField());

    post = q->first();
    PRINT(post->title());
    QTEST_ASSERT(post->title() == "new name");
}

void BasicTest::emptyDatabase()
{
//    auto commentsCount = db.comments()->query()->remove();
//    auto postsCount = db.posts()->query()->remove();
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
