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

#ifndef POSTGRESQLGENERATOR_H
#define POSTGRESQLGENERATOR_H

#include <QtCore/qglobal.h>

#include <QtNut/abstractsqlgenerator.h>

QT_BEGIN_NAMESPACE

NUT_BEGIN_NAMESPACE

class NUT_EXPORT PostgreSqlGenerator : public AbstractSqlGenerator
{
private:
    bool readInsideParentese(QString &text, QString &out);
    bool isPostGisType(const QMetaType::Type &t) const;

public:
    explicit PostgreSqlGenerator(Database *parent = nullptr);

    QString fieldType(FieldModel *field) override;

    QString diffField(FieldModel *oldField, FieldModel *newField) override;

    // SqlGeneratorBase interface
public:
    QString escapeValue(const QVariant &v) const override;
    QVariant unescapeValue(const QMetaType::Type &type, const QVariant &dbValue) override;

    // SqlGeneratorBase interface
protected:
    QString createConditionalPhrase(const PhraseData *d) const override;
};

NUT_END_NAMESPACE

QT_END_NAMESPACE

#endif // POSTGRESQLGENERATOR_H
