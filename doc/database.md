Database class must inherit from Nut::Database class.
Database class can have NUT_DB_VERSION for declaring version number, version will be stored in database if upgrade needed.
```cpp
NUT_DB_VERSION(major, minor)
```

for every table in database NUT_DECLARE_TABLE macro should be use, usage:
```cpp
NUT_DECLARE_TABLE(class_name, table_name)
```

Sample database class:
```cpp
#include <Database>

class Post;
class Comment;
class WeblogDatabase : public Nut::Database
{
    Q_OBJECT

    NUT_DB_VERSION(1)

    NUT_DECLARE_TABLE(Post, post)
    NUT_DECLARE_TABLE(Comment, comment)

public:
    WeblogDatabase();
};
```

Child tables should initalize in constructor, Example:
```cpp
WeblogDatabase::WeblogDatabase() : Nut::Database()
    , m_posts(new TableSet<Post>(this))
    , m_comments(new TableSet<Comment>(this))
{
}
```