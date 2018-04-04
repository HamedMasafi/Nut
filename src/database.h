/**************************************************************************
**
** This file is part of Nut project.
** https://github.com/HamedMasafi/Nut
**
** Nut is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Nut is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with Nut.  If not, see <http://www.gnu.org/licenses/>.
**
**************************************************************************/

#ifndef NUTDATABASE_H
#define NUTDATABASE_H

#include <QtCore/qglobal.h>
#include <QtCore/QList>
#include <QtSql/QSqlDatabase>

#include "defines.h"
#include "tableset.h"

NUT_BEGIN_NAMESPACE

class DatabaseModel;
class DatabasePrivate;
class TableSetBase;
class SqlGeneratorBase;
class ChangeLogTable;
class NUT_EXPORT Database : public QObject
{
    Q_OBJECT

    DatabasePrivate *d_ptr;
    Q_DECLARE_PRIVATE(Database)

public:
    explicit Database(QObject *parent = nullptr);
    explicit Database(const Database &other);
    explicit Database(const QSqlDatabase &other);
    ~Database();

    bool open();
    bool open(bool updateDatabase);
    void close();

    QSqlQuery exec(QString sql);

    int saveChanges(bool cleanUp = false);
    void cleanUp();

    QString databaseName() const;
    QString hostName() const;
    int port() const;
    QString userName() const;
    QString password() const;
    QString connectionName() const;
    QString driver() const;

    DatabaseModel model() const;
    QString tableName(QString className);

    SqlGeneratorBase *sqlGenertor() const;
    QSqlDatabase database();

protected:
    //remove minor version
    virtual void databaseUpdated(QString oldVersion, QString newVersion);

public slots:
    void setDatabaseName(QString databaseName);
    void setHostName(QString hostName);
    void setPort(int port);
    void setUserName(QString userName);
    void setPassword(QString password);
    void setConnectionName(QString connectionName);
    void setDriver(QString driver);

private:
    void add(TableSetBase *);

    friend class TableSetBase;
};

NUT_END_NAMESPACE

#endif  // NUTDATABASE_H
