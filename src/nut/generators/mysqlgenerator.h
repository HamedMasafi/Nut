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

#ifndef MYSQLGENERATOR_H
#define MYSQLGENERATOR_H

#include <QtCore/qglobal.h>

#include <QtNut/abstractsqlgenerator.h>

QT_BEGIN_NAMESPACE

class NUT_EXPORT MySqlGenerator : public AbstractSqlGenerator
{
public:
    explicit MySqlGenerator(Database *parent = nullptr);

    QString fieldType(FieldModel *field) override;
    QString escapeValue(const QVariant &v) const override;
    QVariant unescapeValue(const QMetaType::Type &type, const QVariant &dbValue) override;
//    QString phrase(const PhraseData *d) const;
    //    QString selectCommand(AgregateType t, QString agregateArg, QString tableName, QList<WherePhrase> &wheres, QList<WherePhrase> &orders, QList<RelationModel *> joins, int skip, int take);
    QString createConditionalPhrase(const PhraseData *d) const override;
    void appendSkipTake(QString &sql, int skip, int take) override;
    QString primaryKeyConstraint(const TableModel *table) const override;
    QString escaleFieldName(const QString &fieldName) const override;

private:
    bool readInsideParentese(QString &text, QString &out);
};

QT_END_NAMESPACE

#endif // MYSQLGENERATOR_H
