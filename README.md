# Nut

## Build result
| Brancc name   | Icon  |
| ------------- |:-------------:|
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/8bdf1279bc9c418ca2b95ec4b8972aa7)](https://app.codacy.com/app/HamedMasafi/Nut?utm_source=github.com&utm_medium=referral&utm_content=HamedMasafi/Nut&utm_campaign=Badge_Grade_Dashboard)
| master        | [![Build Status](https://travis-ci.org/HamedMasafi/Nut.svg?branch=master)](https://travis-ci.org/HamedMasafi/Nut) |
| dev           | [![Build Status](https://travis-ci.org/HamedMasafi/Nut.svg?branch=dev)](https://travis-ci.org/HamedMasafi/Nut) |


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
auto q = db.posts()->createQuery();
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
auto q = db.posts()->createQuery();
q->setWhere(Post::idField() == postId);
Post *post = q->first();

if(post) {
    post->setTitle("new name");
    db.saveChanges();
} else {
    qWarning("No post found!");
}
```

### Donate
Butcoin address: 1Dn1WHKkaxanXe4cTGDk4cFRRABxLUpEVj


![Wallet addresst](btc-qr.png)

For more information read [Wiki](wiki).
