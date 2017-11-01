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
#include "defines.h"
#include "tablemodel.h"
#include "postgresqlgenerator.h"
#include "mysqlgenerator.h"
#include "sqlitegenerator.h"
#include "sqlservergenerator.h"
#include "query.h"

#include <iostream>
#include <cstdarg>

#define __CHANGE_LOG_TABLE_NAME "__change_logs"

NUT_BEGIN_NAMESPACE

int DatabasePrivate::lastId = 0;
QMap<QString, DatabaseModel> DatabasePrivate::allTableMaps;

DatabasePrivate::DatabasePrivate(Database *parent) : q_ptr(parent)
{
}

bool DatabasePrivate::isLive(){
  Q_Q(Database);
  return db.exec("SELECT 1").isActive();
}

bool DatabasePrivate::open(bool update)
{
    Q_Q(Database);
//    if (update)
    bool isNew = getCurrectScheema();

    connectionName = q->metaObject()->className()
                     + QString::number(DatabasePrivate::lastId);

    if (driver == "QODBC" || driver == "QODBC3"){
      databaseName = QString("DRIVER={SQL Server};SERVER=%1;DATABASE=%2;").arg(hostName).arg(databaseName);
    }
    db = QSqlDatabase::addDatabase(driver, connectionName);
    db.setHostName(hostName);
    db.setDatabaseName(databaseName);
    db.setUserName(userName);
    db.setPassword(password);
    bool ok = db.open();

    if (!ok) {
        qWarning("Could not connect to database, error = %s",
                 db.lastError().text().toLocal8Bit().data());

        if (db.lastError().text().contains("database \"" + databaseName
                                           + "\" does not exist")
            || db.lastError().text().contains("Cannot open database")
            || db.lastError().text().contains("Unknown database '"
                                              + databaseName + "'")) {
            db.setDatabaseName(sqlGenertor->masterDatabaseName(databaseName));
            ok = db.open();
            qDebug("Creating database");
            if (ok) {
              if (driver == "QODBC" || driver == "QODBC3"){
                  QRegExp dname("DATABASE=(.*);");
                  dname.indexIn(databaseName);
                  db.exec("CREATE DATABASE " + dname.cap(1));
              }else{
                  db.exec("CREATE DATABASE " + databaseName);
              }              
                db.close();

                if (db.lastError().type() != QSqlError::NoError)
                    qWarning("Creating database error: %s",
                             db.lastError().text().toLatin1().data());

                return open(update);
            } else {
                qWarning("Unknown error detecting change logs, %s",
                         db.lastError().text().toLatin1().data());
            }
        }
        return false;
    }

    if(isNew)
        return updateDatabase();
    else
        return true;
}

bool DatabasePrivate::updateDatabase()
{
    Q_Q(Database);

    DatabaseModel last = getLastScheema();
    DatabaseModel current = currentModel;

    if (last == current) {
        qDebug("Databse is up-to-date");
        return true;
    }

    if (!last.count())
        qDebug("Databse is new");
    else
        qDebug("Databse is changed");

    QStringList sql = sqlGenertor->diff(last, current);
    db.transaction();
    foreach (QString s, sql) {
        db.exec(s);

        if (db.lastError().type() != QSqlError::NoError)
            qWarning("Error executing sql command, %s",
                     db.lastError().text().toLatin1().data());
    }
    storeScheemaInDB();
    bool ok = db.commit();

    if (db.lastError().type() == QSqlError::NoError) {

        q->databaseUpdated(last.versionMajor(), last.versionMinor(),
                           current.versionMajor(), current.versionMinor());
        QString versionText = QString::number(current.versionMajor()) + "_"
                              + QString::number(current.versionMinor());

        for (int i = 0; i < q->metaObject()->methodCount(); i++) {
            QMetaMethod m = q->metaObject()->method(i);
            if (m.name() == "update" + versionText) {
                m.invoke(q, Qt::DirectConnection,
                         Q_ARG(int, current.versionMajor()),
                         Q_ARG(int, current.versionMinor()));
                break;
            }
        }
    } else {
        qWarning("Unable update database, error = %s",
                 db.lastError().text().toLatin1().data());
    }

    return ok;
}

bool DatabasePrivate::getCurrectScheema()
{
    Q_Q(Database);

    if (allTableMaps.contains(q->metaObject()->className())) {
        currentModel = allTableMaps[q->metaObject()->className()];
        return false;
    }

    tables.clear();

    // TODO: change logs must not be in model
    int changeLogTypeId = qRegisterMetaType<ChangeLogTable *>();
    currentModel.append(
        new TableModel(changeLogTypeId, __CHANGE_LOG_TABLE_NAME));
    tables.insert(ChangeLogTable::staticMetaObject.className(),
                  __CHANGE_LOG_TABLE_NAME);

    changeLogs = new TableSet<ChangeLogTable>(q);

    for (int i = 0; i < q->metaObject()->classInfoCount(); i++) {
        QMetaClassInfo ci = q->metaObject()->classInfo(i);
        QString ciName
            = QString(ci.name()).replace(__nut_NAME_PERFIX, "").replace("\"",
                                                                        "");
        if (ciName.startsWith(__nut_TABLE))
            tables.insert(ciName.split(" ").at(1), ci.value());

        if (ciName == __nut_DB_VERSION) {
            QStringList version
                = QString(ci.value()).replace("\"", "").split('.');
            bool ok = false;
            if (version.length() == 1) {
                currentModel.setVersionMajor(version.at(0).toInt(&ok));
            } else if (version.length() == 2) {
                currentModel.setVersionMajor(version.at(0).toInt(&ok));
                currentModel.setVersionMinor(version.at(1).toInt(&ok));
            }

            if (!ok)
                qFatal("NUT_DB_VERSION macro accept version in format 'x' or "
                       "'x[.y]' only, and x,y must be integer values\n");
        }
    }

    for (int i = 1; i < q->metaObject()->propertyCount(); i++) {
        QMetaProperty tableProperty = q->metaObject()->property(i);
        int typeId = QMetaType::type(tableProperty.typeName());

        if (tables.values().contains(tableProperty.name())
            && (unsigned)typeId >= QVariant::UserType) {
            TableModel *sch = new TableModel(typeId, tableProperty.name());
            currentModel.append(sch);
        }
    }

    foreach (TableModel *sch, currentModel)
        foreach (RelationModel *fk, sch->foregionKeys())
            fk->table = currentModel.tableByClassName(fk->className);

    allTableMaps.insert(q->metaObject()->className(), currentModel);
    return true;
}

DatabaseModel DatabasePrivate::getLastScheema()
{
    Q_Q(Database);

    ChangeLogTable *u = changeLogs->query()
            ->orderBy(!ChangeLogTable::idField())
            ->first();

    DatabaseModel ret(q->metaObject()->className());

    if (u) {
        QJsonObject json
            = QJsonDocument::fromJson(
                  QByteArray(u->data().toLocal8Bit().data())).object();

        foreach (QString key, json.keys()) {
            TableModel *sch = new TableModel(json.value(key).toObject(), key);
            ret.append(sch);
        }

        u->deleteLater();
    }
    return ret;

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

bool DatabasePrivate::storeScheemaInDB()
{
    Q_Q(Database);
    DatabaseModel current = currentModel;
    /*current.remove(__CHANGE_LOG_TABLE_NAME)*/;

    ChangeLogTable *changeLog = new ChangeLogTable();
    changeLog->setData(QJsonDocument(current.toJson()).toJson());
    changeLog->setVersionMajor(current.versionMajor());
    changeLog->setVersionMinor(current.versionMinor());
    changeLogs->append(changeLog);
    q->saveChanges();
    changeLog->deleteLater();

    return true;

    //    QSqlQuery query(db);
    //    query.prepare("insert into __change_logs (data) values (:data)");
    //    query.bindValue(":data",
    //    QString(QJsonDocument(currentModel.toJson()).toJson()));
    //    bool ret = query.exec();
    //    if(query.lastError().type() != QSqlError::NoError)
    //        qWarning(QString("storeScheemaInDB" +
    //        query.lastError().text()).toLatin1().data());
    //    return ret;
}

void DatabasePrivate::createChangeLogs()
{
    //    currentModel.model("change_log")
    QString diff = sqlGenertor->diff(0, currentModel.tableByName("__change_log"));

    db.exec(diff);
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

Database::Database(const Database &other, QObject *parent)
    : QObject(parent), d_ptr(new DatabasePrivate(this))
{
    DatabasePrivate::lastId++;

    setDriver(other.driver());
    setHostName(other.hostName());
    setDatabaseName(other.databaseName());
    setUserName(other.userName());
    setPassword(other.password());
}

Database::~Database()
{
    Q_D(Database);
    if (d->db.isOpen())
        d->db.close();

    if (d_ptr)
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
        return QString::null;;
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

SqlGeneratorBase *Database::sqlGenertor() const
{
    Q_D(const Database);
    return d->sqlGenertor;
}

void Database::databaseUpdated(int oldMajor, int oldMinor, int newMajor,
                               int newMinor)
{
    Q_UNUSED(oldMajor);
    Q_UNUSED(oldMinor);
    Q_UNUSED(newMajor);
    Q_UNUSED(newMinor);
}


bool Database::open()
{
    return open(true);
}

bool Database::isLive(){
    Q_D(Database);
    return d->isLive();
}

bool Database::open(bool updateDatabase)
{
    Q_D(Database);

    if (d->driver == "QPSQL" || d->driver == "QPSQL7")
        d->sqlGenertor = new PostgreSqlGenerator(this);
    else if (d->driver == "QMYSQL" || d->driver == "QMYSQL3")
        d->sqlGenertor = new MySqlGenerator(this);
    else if (d->driver == "QSQLITE" || d->driver == "QSQLITE3")
        d->sqlGenertor = new SqliteGenerator(this);
    else if (d->driver == "QODBC" || d->driver == "QODBC3") {
        QString driverName = QString::null;
        QStringList parts = d->databaseName.toLower().split(';');
        foreach (QString p, parts)
            if (p.trimmed().startsWith("driver="))
                driverName = p.split('=').at(1).toLower().trimmed();

//        if (driverName == "{sql server}")
            d->sqlGenertor = new SqlServerGenerator(this);
        // TODO: add ODBC driver for mysql, postgres, ...
    }

    if (!d->sqlGenertor) {
        qFatal("Sql generator for driver %s not found",
                 driver().toLatin1().constData());
        return false;
    } else {
        return d->open(updateDatabase);
    }
}

void Database::close()
{
    Q_D(Database);
    d->db.close();
}

QSqlQuery Database::exec(QString sql)
{
    Q_D(Database);

    QSqlQuery q = d->db.exec(sql);
    if (d->db.lastError().type() != QSqlError::NoError)
        qWarning("Error executing sql command: %s; Command=%s",
                 d->db.lastError().text().toLatin1().data(),
                 sql.toUtf8().constData());
    return q;
}

void Database::add(TableSetBase *t)
{
    Q_D(Database);
    d->tableSets.insert(t);
}

int Database::saveChanges(bool cleanUp)
{
    Q_D(Database);
    int rowsAffected = 0;
    foreach (TableSetBase *ts, d->tableSets)
        rowsAffected += ts->save(this, cleanUp);
    return rowsAffected;
}

void Database::cleanUp()
{
    Q_D(Database);
    foreach (TableSetBase *ts, d->tableSets)
        ts->clearChilds();
}

NUT_END_NAMESPACE
