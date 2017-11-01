# Nut

## Advanced, Powerful and easy to use ORM for Qt5


## Features:

 - Easy to use
 - Support PosgtreSQL, MySQL, SQLite and Microsoft Sql Server
 - Automatically create and update database
 - IDE auto complete support, No hard-code nedded
 - Table join detect

## Sample Codes
### Read data from database:

```cpp
auto q = db.posts()->query();
q->setWhere(Post::idField() == postId);
auto posts = q->toList();
// now posts is a QList<Post*> contain all posts in
//  database that has id equal to postId variable
auto post = q->first();
// post is first row in database that its id is equal to postId
```

### Adding to database:
```cpp
Post *newPost = new Post;
newPost->setTitle("post title");

db.posts()->append(newPost);

for(int i = 0 ; i < 3; i++){
    Comment *comment = new Comment;
    comment->setMessage("comment #" + QString::number(i));

    newPost->comments()->append(comment);
}
db.saveChanges();
```

### Modify database data:
```cpp
auto q = db.posts()->query();
q->setWhere(Post::idField() == postId);
Post *post = q->first();

if(post) {
    post->setTitle("new name");
    db.saveChanges();
} else {
    qWarning("No post found!");
}
```

For more information read [Wiki](wiki).
