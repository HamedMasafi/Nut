#ifndef MAINTEST_H
#define MAINTEST_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>

class Post;
class User;

namespace Nut {
class PhraseList;
class AssignmentPhraseList;
class ConditionalPhrase;
}
class PhrasesTest : public QObject
{
    Q_OBJECT

public:
    explicit PhrasesTest(QObject *parent = nullptr);

Q_SIGNALS:

private Q_SLOTS:
    void initTestCase();

    void condition_numeric_sqlite();
    void condition_string_sqlite();
    void condition_bool_sqlite();
    void condition_datetime_sqlite();

    void order_sqlite();
    void select_sqlite();

    void extra();

    void no1();
    void mix();

private:
    void select(const Nut::PhraseList &ph);
    void where(const Nut::ConditionalPhrase &ph);
    void update(const Nut::AssignmentPhraseList &p);
    void insert(const Nut::AssignmentPhraseList &p);
    void order_by(const Nut::PhraseList &ph);
};

#endif // MAINTEST_H
