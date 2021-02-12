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

#include <QtCore/QMetaProperty>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlResult>

#include "database.h"
#include "table.h"
#include "tableset.h"
#include "database_p.h"
#include "config/nut_global.h"
#include "tablemodel.h"
#include "postgresqlgenerator.h"
#include "mysqlgenerator.h"
#include "sqlitegenerator.h"
#include "sqlservergenerator.h"
#include "query.h"
#include "changelogtable.h"

#include <iostream>
#include <cstdarg>

#ifndef __CHANGE_LOG_TABLE_NAME
#   define __CHANGE_LOG_TABLE_NAME "__change_logs"
#endif

NUT_BEGIN_NAMESPACE

qulonglong DatabasePrivate::lastId = 0;
QMap<QString, DatabaseModel> DatabasePrivate::allTableMaps;

DatabasePrivate::DatabasePrivate(Database *parent) : q_ptr(parent),
    port(0), sqlGenerator(nullptr), changeLogs(nullptr),
    isDatabaseNew(false)
{
}

bool DatabasePrivate::open(bool update)
{
    if (db.isOpen())
        return true;
    Q_Q(Database);
//    if (update)
    connectionName = QString::fromUtf8(q->metaObject()->className())
                     + QString::number(DatabasePrivate::lastId);

    db = QSqlDatabase::addDatabase(driver, connectionName);
    db.setHostName(hostName);
    if (port)
        db.setPort(port);
    db.setDatabaseName(databaseName);
    db.setUserName(userName);
    db.setPassword(password);

    if (driver.startsWith(QStringLiteral("qsqlite"), Qt::CaseInsensitive)
            && !QFile::exists(databaseName)) {
        //Force to execute update database
        isDatabaseNew = true;
        update = true;
    }
    bool ok = db.open();

    if (!ok) {
        qWarning("Could not connect to database, error = %s",
                 db.lastError().text().toLocal8Bit().data());

        if (db.lastError().text().contains(QStringLiteral("database \"")
                                           + databaseName
                                           + QStringLiteral("\" does not exist"))
            || db.lastError().text().contains(QStringLiteral("Cannot open database"))
            || db.lastError().text().contains(QStringLiteral("Unknown database '")
                                              + databaseName
                                              + QStringLiteral("'"))) {

            db.close();
            db.setDatabaseName(sqlGenerator->masterDatabaseName(databaseName));
            ok = db.open();
            qDebug("Creating database");
            if (ok) {
                db.exec(QStringLiteral("CREATE DATABASE ") + databaseName);
                db.close();

                if (db.lastError().type() != QSqlError::NoError) {
                    qWarning("Creating database error: %s",
                             db.lastError().text().toLatin1().data());
                    return false;
                }

                isDatabaseNew = true;
                return open(update);
            }
            qWarning("Unknown error detecting change logs, %s",
                     db.lastError().text().toLatin1().data());

        }
        return false;
    }

//    if(update)
        return updateDatabase();
//    else
//        return true;
}

bool DatabasePrivate::updateDatabase()
{
    Q_Q(Database);

    if (!getCurrectSchema())
        return true;

    DatabaseModel last = isDatabaseNew ? DatabaseModel() : getLastSchema();
    DatabaseModel current = currentModel;

    if (last == current) {
        qDebug("Database is up-to-date");
        //TODO: crash without this and I don't know why!
        changeLogs->clearChilds();
        return true;
    }

    for (TableModel *&tm: current) {
        for (FieldModel *&fm: tm->fields()) {
            if (sqlGenerator->fieldType(fm).isEmpty()) {
                qWarning("The type (%s) is not supported for field %s::%s",
                         QMetaType::typeName(fm->type),
                         qPrintable(tm->className()),
                         qPrintable(fm->name));
                return false;
            }
        }
    }
    if (!last.count())
        qDebug("Database is new");
    else
        qDebug("Database is changed");

    QStringList sql = sqlGenerator->diff(last, current);

    db.transaction();
    for (QString &s: sql) {
        db.exec(s);

        if (db.lastError().type() != QSqlError::NoError) {
            qWarning("Error executing sql command `%s`, %s",
                     qPrintable(s),
                     db.lastError().text().toLatin1().data());
            return false;
        }
    }
    putModelToDatabase();
    bool ok = db.commit();

    if (db.lastError().type() == QSqlError::NoError) {

        q->databaseUpdated(last.version(), current.version());
        if (!last.count())
            q->databaseCreated();

    } else {
        qWarning("Unable update database, error = %s",
                 db.lastError().text().toLatin1().data());
    }

    return ok;
}

bool DatabasePrivate::getCurrectSchema()
{
    Q_Q(Database);

    //is not first instanicate of this class
    if (allTableMaps.contains(QString::fromUtf8(q->metaObject()->className()))) {
        currentModel = allTableMaps[QString::fromUtf8(q->metaObject()->className())];
        return false;
    }

    QMap<QString, QString> tables;
    tables.clear();

    // TODO: change logs must not be in model
    int changeLogTypeId = qRegisterMetaType<ChangeLogTable*>();

    currentModel.append(
        new TableModel(changeLogTypeId, QStringLiteral(__CHANGE_LOG_TABLE_NAME)));
    tables.insert(QString::fromUtf8(ChangeLogTable::staticMetaObject.className()),
                  QStringLiteral(__CHANGE_LOG_TABLE_NAME));

    changeLogs = new TableSet<ChangeLogTable>(q);

    for (int i = 0; i < q->metaObject()->classInfoCount(); i++) {
        QString type;
        QString name;
        QString value;

        if (!nutClassInfoString(q->metaObject()->classInfo(i),
                                type, name, value)) {

            errorMessage = QStringLiteral("No valid table in ")
                               + QString::fromUtf8(q->metaObject()->classInfo(i).value());
            continue;
        }
        if (type == QStringLiteral(__nut_TABLE)) {
            //name: table class name
            //value: table variable name (table name in db)
            tables.insert(name, value);

            int typeId = QMetaType::type(name.toLocal8Bit() + "*");

            if (!typeId)
                qFatal("The class %s is not registered with qt meta object", qPrintable(name));

            TableModel *sch = new TableModel(typeId, value);
            currentModel.append(sch);
        }

        if (type == QStringLiteral(__nut_DB_VERSION)) {
            bool ok;
            int version = value.toInt(&ok);
            if (!ok)
                qFatal("NUT_DB_VERSION macro accept version in format 'x'");
            currentModel.setVersion(version);
        }
    }

    for (int i = 1; i < q->metaObject()->propertyCount(); i++) {
        QMetaProperty tableProperty = q->metaObject()->property(i);
        int typeId = QMetaType::type(tableProperty.typeName());

        if (tables.values().contains(QString::fromUtf8(tableProperty.name()))
            && (unsigned)typeId >= QVariant::UserType) {
            TableModel *sch = new TableModel(typeId, QString::fromUtf8(tableProperty.name()));
            currentModel.append(sch);
        }
    }

    for (TableModel *&table: currentModel) {
        for (FieldModel *&f: table->fields()) {
            if (f->isPrimaryKey && ! sqlGenerator->supportPrimaryKey(f->type))
                qFatal("The field of type %s does not support as primary key",
                       qPrintable(f->typeName));

            if (f->isAutoIncrement && ! sqlGenerator->supportAutoIncrement(f->type))
                qFatal("The field of type %s does not support as auto increment",
                       qPrintable(f->typeName));
        }

        for (RelationModel *&fk: table->foreignKeys())
            fk->masterTable = currentModel.tableByClassName(fk->masterClassName);
    }

    allTableMaps.insert(QString::fromUtf8(q->metaObject()->className()), currentModel);
    return true;
}

DatabaseModel DatabasePrivate::getLastSchema()
{
    Row<ChangeLogTable> u = changeLogs->query()
            .orderBy(!ChangeLogTable::idField())
            .first();

    if (u) {
        QJsonParseError e;
        QJsonObject json = QJsonDocument::fromJson(u->data()
                                                       .replace(QStringLiteral("\\\""),
                                                                QStringLiteral("\""))
                                                       .toUtf8(),
                                                   &e)
                               .object();

        DatabaseModel ret = json;
        return ret;
        /*
        foreach (QString key, json.keys()) {
            TableModel *sch = new TableModel(json.value(key).toObject(), key);
            ret.append(sch);
        }*/
    }
    return DatabaseModel();

    //    QSqlQuery query = q->exec("select * from __change_logs order by id
    //    desc limit 1");
    //    DatabaseModel ret;
    //    if(query.next()){
    //        QJsonObject json =
    //        QJsonDocument::fromJson(query.value("data").toByteArray()).object();

    //        foreach (QString key, json.keys()) {
    //            TableModel *sch = new TableModel(json.value(key).toObject(),
    //            key);
    //            ret.append(sch);
    //        }
    //    }
    //    return ret;
}

bool DatabasePrivate::putModelToDatabase()
{
    Q_Q(Database);
    DatabaseModel current = currentModel;
    /*current.remove(__CHANGE_LOG_TABLE_NAME)*/;

    auto changeLog = create<ChangeLogTable>();
    changeLog->setData(QString::fromUtf8(QJsonDocument(current.toJson()).toJson(QJsonDocument::Compact)));
    changeLog->setVersion(current.version());
    changeLogs->append(changeLog);
    q->saveChanges(true);
    changeLog->deleteLater();

    return true;

    //    QSqlQuery query(db);
    //    query.prepare("insert into __change_logs (data) values (:data)");
    //    query.bindValue(":data",
    //    QString(QJsonDocument(currentModel.toJson()).toJson()));
    //    bool ret = query.exec();
    //    if(query.lastError().type() != QSqlError::NoError)
    //        qWarning(QString("storeSchemaInDB" +
    //        query.lastError().text()).toLatin1().data());
    //    return ret;
}

void DatabasePrivate::createChangeLogs()
{
    //    currentModel.model("change_log")
    QStringList diff = sqlGenerator->diff(nullptr,
                                          currentModel.tableByName(
                                              QStringLiteral("__change_log")));

    for (QString &s: diff)
        db.exec(s);
}

/*!
 * \class Database
 * \brief Database class
 */

Database::Database(QObject *parent)
    : QObject(parent), d_ptr(new DatabasePrivate(this))
{
    DatabasePrivate::lastId++;
}

Database::Database(const Database &other)
    : QObject(other.parent()), d_ptr(new DatabasePrivate(this))
{
    DatabasePrivate::lastId++;

    setDriver(other.driver());
    setHostName(other.hostName());
    setPort(other.port());
    setDatabaseName(other.databaseName());
    setUserName(other.userName());
    setPassword(other.password());
}

Database::Database(const QSqlDatabase &other)
{
    //TODO: make a polish here
    DatabasePrivate::lastId++;

    setDriver(other.driverName());
    setHostName(other.hostName());
    setPort(other.port());
    setDatabaseName(other.databaseName());
    setUserName(other.userName());
    setPassword(other.password());
    qRegisterMetaType<ChangeLogTable*>();
}

Database::Database(Database &&other)
{
    d_ptr = other.d_ptr;
    other.d_ptr = nullptr;
}

Database::~Database()
{
    Q_D(Database);
    if (d->db.isOpen())
        d->db.close();

    delete d_ptr;
}

QString Database::databaseName() const
{
    Q_D(const Database);
    return d->databaseName;
}

QString Database::hostName() const
{
    Q_D(const Database);
    return d->hostName;
}

int Database::port() const
{
    Q_D(const Database);
    return d->port;
}

QString Database::userName() const
{
    Q_D(const Database);
    return d->userName;
}

QString Database::password() const
{
    Q_D(const Database);
    return d->password;
}

/*!
 * \brief Database::connectionName
 * \return Connection name of current Database \l QSqlDatabase::connectionName
 */
QString Database::connectionName() const
{
    Q_D(const Database);
    return d->connectionName;
}

QString Database::driver() const
{
    Q_D(const Database);
    return d->driver;
}

/*!
 * \brief Database::model
 * \return The model of this database
 */
DatabaseModel Database::model() const
{
    Q_D(const Database);
    return d->currentModel;
}

QString Database::tableName(QString className)
{
    TableModel *m = model().tableByClassName(className);
    if (m)
        return m->name();
    else
        return QString();
}

void Database::setDatabaseName(QString databaseName)
{
    Q_D(Database);
    d->databaseName = databaseName;
}

void Database::setHostName(QString hostName)
{
    Q_D(Database);
    d->hostName = hostName;
}

void Database::setPort(int port)
{
    Q_D(Database);
    d->port = port;
}

void Database::setUserName(QString username)
{
    Q_D(Database);
    d->userName = username;
}

void Database::setPassword(QString password)
{
    Q_D(Database);
    d->password = password;
}

void Database::setConnectionName(QString connectionName)
{
    Q_D(Database);
    d->connectionName = connectionName;
}

void Database::setDriver(QString driver)
{
    Q_D(Database);
    d->driver = driver.toUpper();
}

AbstractSqlGenerator *Database::sqlGenerator() const
{
    Q_D(const Database);
    return d->sqlGenerator;
}

QSqlDatabase Database::database()
{
    Q_D(Database);
    return d->db;
}

void Database::databaseCreated()
{

}

void Database::databaseUpdated(int oldVersion, int newVersion)
{
    Q_UNUSED(oldVersion)
    Q_UNUSED(newVersion)
}


/**
 * @brief Database::open
 * Opens the database connection using the current connection values.
 * Returns true on success; otherwise returns false.
 * @return bool
 */
bool Database::open()
{
    return open(true);
}

bool Database::open(bool updateDatabase)
{
    Q_D(Database);

    if (d->driver == QStringLiteral("QPSQL") || d->driver == QStringLiteral("QPSQL7"))
        d->sqlGenerator = new PostgreSqlGenerator(this);
    else if (d->driver == QStringLiteral("QMYSQL") || d->driver == QStringLiteral("QMYSQL3"))
        d->sqlGenerator = new MySqlGenerator(this);
    else if (d->driver == QStringLiteral("QSQLITE") || d->driver == QStringLiteral("QSQLITE3"))
        d->sqlGenerator = new SqliteGenerator(this);
    else if (d->driver == QStringLiteral("QODBC") || d->driver == QStringLiteral("QODBC3")) {
        QString driverName = QString();
        QStringList parts = d->databaseName.toLower().split(';');
        for (QString &p: parts)
            if (p.trimmed().startsWith(QStringLiteral("driver=")))
                driverName = p.split('=').at(1).toLower().trimmed();

//        if (driverName == "{sql server}")
            d->sqlGenerator = new SqlServerGenerator(this);
        // TODO: add ODBC driver for mysql, postgres, ...
    }

    if (!d->sqlGenerator) {
        qFatal("Sql generator for driver %s not found",
                 driver().toLatin1().constData());
    }

    return d->open(updateDatabase);
}

void Database::close()
{
    Q_D(Database);
    d->db.close();
}

QSqlQuery Database::exec(const QString &sql)
{
    Q_D(Database);

    QSqlQuery q = d->db.exec(sql);
    if (d->db.lastError().type() != QSqlError::NoError)
        qWarning("Error executing sql command: %s; Command=%s",
                 d->db.lastError().text().toLatin1().data(),
                 sql.toUtf8().constData());
    return q;
}

void Database::add(AbstractTableSet *t)
{
    Q_D(Database);
    d->tableSets.insert(t);
}

int Database::saveChanges(bool cleanUp)
{
    Q_D(Database);

    if (!d->db.isOpen()) {
        qWarning("Database is not open");
        return 0;
    }

    int rowsAffected = 0;
    for (AbstractTableSet *ts: d->tableSets)
        rowsAffected += ts->save(this, cleanUp);

    return rowsAffected;
}

void Database::cleanUp()
{
    Q_D(Database);
    for (AbstractTableSet *ts: d->tableSets)
        ts->clearChilds();
}

NUT_END_NAMESPACE
