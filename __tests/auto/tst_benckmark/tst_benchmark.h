#ifndef MAINTEST_H
#define MAINTEST_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>

#include "weblogdatabase.h"
class Post;
class BenchmarkTest : public QObject
{
    Q_OBJECT
    WeblogDatabase db;
    int postId;
    Post *post;
    Query<Post> *q;
public:
    explicit BenchmarkTest(QObject *parent = nullptr);

Q_SIGNALS:

private Q_SLOTS:
    void initTestCase();

    void insert1kPost();
};

#endif // MAINTEST_H
