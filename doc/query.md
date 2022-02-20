# Creating query
```cpp
auto q = db.posts().query();
```

You can also create query in one command:
```cpp
auto result = db.posts().query()
    ->where(Post::idField() == 1)
    ->toList();
```
Now, _result_ contains **QList\<QSharedPointer\<Post\>\>** and can be used in code. query has other commands like: sum, avg, max, min and etc

## Getting first record in query
```cpp
auto post = db.posts().query()
    ->setWhere(Post::idField() == 1)
    ->first();

if(post)
    qDebug() << "Post found in database";
else 
    qDebug() << "No post found!";

```
## Sorting result
```cpp
auto posts = db.posts().query()
    ->where(Post::idField() == 1)
    ->orderBy(Post::idField())
    ->toList();
```
Also, you can sort descending by adding **!** to field name
```cpp
auto posts = db.posts().query()
    ->where(Post::idField() == 1)
    ->orderBy(!Post::idField())
    ->toList();
```

## Selecting single field
```cpp
auto ids = db.posts().query()
    ->select(Post::idField());
//ids is type of QList<int>
```
## Getting sum, count, min, max
```cpp
auto q = db.posts().query();
auto sum = q.sum(Post::idField());
auto max = q.max(Post::idField());
auto min = q.min(Post::idField());
auto count = q.count(Post::idField());
```

## Checking field exists in list of values
```cpp
auto post = db.posts().query()
    ->where(Post::idField().in(QList<int>() << 1 << 2 << 3 << 4) || Post::isAccepted())
    ->first();
```
Or
```cpp
auto post = db.posts().query()
    ->where(Post::idField().in({1, 2, 3, 4}) || Post::isAccepted())
    ->first();
```
