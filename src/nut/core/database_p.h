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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Nut API.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef DATABASE_P_H
#define DATABASE_P_H

#include <QtCore/QDebug>
#include <QtCore/QSharedData>

#include <QtNut/database.h>
#include <QtNut/databasemodel.h>

QT_BEGIN_NAMESPACE

NUT_BEGIN_NAMESPACE

class ChangeLogTable;
class NUT_EXPORT DatabasePrivate
{
    Database *q_ptr;
    Q_DECLARE_PUBLIC(Database)

public:
    explicit DatabasePrivate(Database *parent);

    bool open(bool update);

    bool updateDatabase();
    void createChangeLogs();
    bool putModelToDatabase();
    DatabaseModel getLastSchema();
    bool getCurrectSchema();

    QSqlDatabase db;

    QString hostName;
    QString databaseName;
    int port;
    QString userName;
    QString password;
    QString connectionName;
    QString driver;

    AbstractSqlGenerator *sqlGenerator;
    DatabaseModel currentModel;

    TableSet<ChangeLogTable> *changeLogs;

    static QMap<QString, DatabaseModel> allTableMaps;
    static qulonglong lastId;

    QSet<AbstractTableSet *> tableSets;

    bool isDatabaseNew;

    QString errorMessage;
};

NUT_END_NAMESPACE

QT_END_NAMESPACE

#endif // DATABASE_P_H
