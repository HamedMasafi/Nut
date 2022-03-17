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

#ifndef DATABASE_MODEL_H
#define DATABASE_MODEL_H

#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>

#include <QtNut/nut_global.h>

class QJsonObject;

QT_BEGIN_NAMESPACE

NUT_BEGIN_NAMESPACE

class TableModel;
struct RelationModel;
class NUT_EXPORT DatabaseModel : public QList<TableModel *>
{
    QString _databaseClassName;
    int _version;
    static QMap<QString, DatabaseModel *> _models;

public:
    DatabaseModel(const QString &name = QString());
    DatabaseModel(const DatabaseModel &other);
    DatabaseModel(const QJsonObject &json);
    ~DatabaseModel() = default;

    TableModel *tableByName(const QString &tableName) const;
    TableModel *tableByClassName(QString className) const;

    RelationModel *relationByClassNames(const QString &masterClassName,
                                        const QString &childClassName);
    RelationModel *relationByTableNames(const QString &masterClassName,
                                        const QString &childClassName);

    bool operator==(const DatabaseModel &other) const;
//    DatabaseModel operator +(const DatabaseModel &other);

    Q_DECL_DEPRECATED
    static DatabaseModel fromJson(QJsonObject &json);
    QJsonObject toJson() const;
    operator QJsonObject();

    int version() const;
    void setVersion(int version);

    bool remove(const QString &tableName);

    //TODO: may be private (called from DatabasePrivate::getCurrentSchema only)
    void fixRelations();

    static DatabaseModel *modelByName(const QString &name);
    static void deleteAllModels();
};

DatabaseModel operator +(const DatabaseModel &l, const DatabaseModel &r);
DatabaseModel operator |(const DatabaseModel &l, const DatabaseModel &r);

NUT_END_NAMESPACE

QT_END_NAMESPACE

#endif // DATABASE_MODEL_H
