# Nut


[![CI build](https://github.com/HamedMasafi/Nut/workflows/CI%20build/badge.svg)](https://github.com/HamedMasafi/Nut/actions)
[![GitLicense](https://gitlicense.com/badge/hamedmasafi/nut)](https://gitlicense.com/license/hamedmasafi/nut)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/f3802610beb946068f6cd2c2b6608a8b)](https://www.codacy.com/app/HamedMasafi/Nut?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=HamedMasafi/Nut&amp;utm_campaign=Badge_Grade)

## Advanced, Powerful and easy to use ORM for Qt5

## Features
- Easy to use
- Support PosgtreSQL, MySQL, SQLite and Microsoft Sql Server
- Automatically create and update database
- Support for IDE autocomplete. No hard-coding is needed
- Detecting table joins
- Support common C++ and Qt-specific types ([Full list](doc/datatypes.md))
- Bulk insertation

## Qick start
### Create table

#### sampletable.h
```cpp
#ifndef SAMPLETABLE_H
#define SAMPLETABLE_H

#include <QtNut/table.h>

class SampleTable : public Nut::Table
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

    int m_id;
    QString m_name;

    // BEGIN OF NUT MACROS
    NUT_PRIMARY_KEY(id)
    NUT_FIELD(int, id)
    NUT_FIELD(QString, name)
    //END OF NUT MACROS

public:
    explicit SampleTable(QObject *parent = nullptr);

    int id() const;
    QString name() const;

public Q_SLOTS:
    void setId(int id);
    void setName(QString name);

Q_SIGNALS:
    void idChanged(int id);
    void nameChanged(QString name);
};

#endif // SAMPLETABLE_H
```

#### sampletable.cpp:
```cpp
#include "sampletable.h"

SampleTable::SampleTable(QObject *parent) : Nut::Table(parent)
{
    init();
}

int SampleTable::id() const
{
    return m_id;
}

QString SampleTable::name() const
{
    return m_name;
}

QString SampleTable::lastName() const
{
    return m_lastName;
}

void SampleTable::setId(int id)
{
    if (m_id == id)
        return;

    m_id = id;
    Q_EMIT idChanged(m_id);
}

void SampleTable::setName(QString name)
{
    if (m_name == name)
        return;

    m_name = name;
    Q_EMIT nameChanged(m_name);
}
```

### Create database
#### sampledatabase.h
```cpp
#ifndef SAMPLEDATABASE_H
#define SAMPLEDATABASE_H

#include <QtNut/Database>

class SampleTable;
class SampleDataBase : public NUT_WRAP_NAMESPACE(Database)
{
    Q_OBJECT
    NUT_DB_VERSION(1)
    NUT_DECLARE_TABLE(SampleTable, items)

public:
    SampleDataBase();
};

#endif // SAMPLEDATABASE_H
```

sampledatabase.cpp
```cpp
#include "sampledatabase.h"
#include "sampletable.h"

SampleDataBase::SampleDataBase() : Nut::Database()
  , m_items(new Nut::TableSet<SampleTable>(this))
{

}

```

#### Sample codes:
```cpp
qRegisterMetaType<SampleTable*>();
qRegisterMetaType<SampleDataBase*>();

db.setDriver("QSQLITE");
db.setDatabaseName("data.sb");

if (db.open()) {
    qFatal() << "Unable to open the database";
}


// Read add rows from database
auto list = db.items()->query().toList();
// list is QList<QSharedPointer<SampleTable>>
    
//Select all the people named David.
auto onlyDavids = db.items()->query().where(SampleTable::nameField() == "David").toList();
onlyDavids.at(0)->setName("John"); // change him name to John
db.saveChanges(); // save changed to the database

// Remove all Johns from the database
db.items()->query().where(SampleTable::nameField() == "John").remove();

// Select rows with folowwing ids: 1, 4, 5, 6
db.items()->query().where(SampleTable::idField().in({1, 4, 5, 6}));

// Select from id 10 to 20
db.items()->query().where(SampleTable::idField().between(10, 20));

// Some other samples
db.items()->query().where(SampleTable::idField() <= 7);
db.items()->query().where(SampleTable::idField() > 0 || SampleTable::idField() == -3);
db.items()->query().where(SampleTable::idField() > 10 && (SampleTable::nameField() == "John" || SampleTable::nameField() == "Jim"));

// Select biggest id
auto biggestId = db.items()->query().max(SampleTable::idField());
```

## Getting started
- [Sample codes](doc/start.md)
- [Shared pointer and regular mode](doc/sharedpointer.md)
- [Create database class](doc/database.md)
- [Create table class](doc/table.md)
- [Using queries](doc/query.md)
- [Supported data types](doc/datatypes.md)

## Help needed!
We need more documentation or wiki. If you can help to improve docs please fork now!

### Donate
Bitcoin address: 1Dn1WHKkaxanXe4cTGDk4cFRRABxLUpEVj


![Wallet address](btc-qr.png)

For more information read [Wiki](wiki).