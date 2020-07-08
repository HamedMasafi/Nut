The class must inherits from Table class

## Add primary key field
Primary key can be auto increment

```cpp
NUT_PRIMARY_AUTO_INCREMENT(id)
NUT_DECLARE_FIELD(int, id, id, setId)
```

for declaring primary key use _NUT_PRIMARY_KEY_ macro, if primary key is auto increment use _NUT_PRIMARY_AUTO_INCREMENT_

| Macro                         | Description                                     |
| ----------------------------- |:------------------------------------------------|
| NUT_PRIMARY_KEY(x)            | The field *x* is primary key                    |
| NUT_AUTO_INCREMENT(x)         | The field *x* is auto increment                 |
| NUT_PRIMARY_AUTO_INCREMENT(x) | The field *x* is primary key and auto increment |

## Declare field
```cpp
NUT_DECLARE_FIELD(type, property_name, read_method_name, write_method_name)
```
## Additional meta data
| Macro                         | Description                                      |
| ----------------------------- |:-------------------------------------------------|
| NUT_NOT_NULL(x)               | The field *x* is not allowed to store NULL value |
| NUT_LEN(x, len)               | Max length of *x* is *len* in string types and in numeric typed field *x* will be store in *len* bytes   |
| NUT_DEFAULT_VALUE(x, def)     | Default value of *x* is *def*                    |
| NUT_UNIQUE(x)                 | Field *x* is unique                               |
| NUT_DISPLAY_NAME(field, name) | Sets display name for field (used in model creation |

## Sample table
```cpp
class Post : public Table
{
    Q_OBJECT

    NUT_PRIMARY_AUTO_INCREMENT(id)
    NUT_DECLARE_FIELD(int, id, id, setId)

    NUT_NOT_NULL(title)
    NUT_LEN(title, 50)
    NUT_DECLARE_FIELD(QString, title, title, setTitle)

    NUT_DECLARE_FIELD(QDateTime, saveDate, saveDate, setSaveDate)

    NUT_LEN(body, 200)
    NUT_DECLARE_FIELD(QString, body, body, setBody)
public:
    explicit Post(QObject *tableSet = 0);

};

Q_DECLARE_METATYPE(Post*)

```

## Declare child table
If current table has one-to-many relation ship it must be declared. For example post table has a slave table named comment, every post has many comment:
```cpp
NUT_DECLARE_CHILD_TABLE(Comment, comments)
```

First argument id table name and second is field name, m_comments must be initalized in constructor:
```cpp
Post::Post(QObject *parent) : Table(parent),
    m_comments(new TableSet<Comment>(this)), m_id(0), m_title("")
{

}
```

For more example take a look at _tests/common_ folder
