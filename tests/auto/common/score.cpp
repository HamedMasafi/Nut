#include "score.h"
#include "user.h"
#include "post.h"

Score::Score(QObject *parent) : NUT_WRAP_NAMESPACE(Table)(parent)
{

}

NUT_FOREIGN_KEY_IMPLEMENT(Score, Post, int, post, post, setPost)
NUT_FOREIGN_KEY_IMPLEMENT(Score, User, QUuid, author, author, setAuthor)
