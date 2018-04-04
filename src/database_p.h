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

#ifndef DATABASE_P_H
#define DATABASE_P_H

#include "database.h"
#include "databasemodel.h"

#include <QDebug>

NUT_BEGIN_NAMESPACE

class ChangeLogTable;
class DatabasePrivate
{
    Database *q_ptr;
    Q_DECLARE_PUBLIC(Database)

public:
    explicit DatabasePrivate(Database *parent = nullptr);

    bool open(bool updateDatabase);

    bool updateDatabase();
    void createChangeLogs();
    bool putModelToDatabase();
    DatabaseModel getLastScheema();
    bool getCurrectScheema();

    bool checkClassInfo(const QMetaClassInfo &classInfo,
                                    QString &type, QString &name, QString &value);
    QSqlDatabase db;

    QString hostName;
    QString databaseName;
    int port;
    QString userName;
    QString password;
    QString connectionName;
    QString driver;

    SqlGeneratorBase *sqlGenertor;
    DatabaseModel currentModel;

    TableSet<ChangeLogTable> *changeLogs;

    QT_DEPRECATED
    QMap<QString, QString> tables;
    static QMap<QString, DatabaseModel> allTableMaps;
    static qulonglong lastId;

    QSet<TableSetBase *> tableSets;

    bool isDatabaseNew;
};

NUT_END_NAMESPACE

#endif // DATABASE_P_H
