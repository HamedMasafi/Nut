#ifndef SQLSERIALIZER_H
#define SQLSERIALIZER_H

#include <QVariant>



class SqlSerializer
{
public:
    SqlSerializer();

    bool readString(QString &text, QString &out) const;

    QVariant fromString(const QString &value, const QMetaType::Type &type) const;
    QString toString(const QVariant &value) const;
    QList<int> toListInt(const QString &s) const;
    QList<int> toListInt(const QString &s, const QString &sep) const;
    QList<qreal> toListReal(const QString &s) const;
    QList<qreal> toListReal(const QString &s, const QString &sep) const;
    QList<float> toListFloat(const QString &s) const;

    QString fromList(const QList<int> &list) const;
    QString fromList(const QList<qreal> &list) const;
    QString fromList(const QList<float> &list) const;
    QString fromVariantList(const QVariantList &list) const;

    QVariant deserialize(const QString &value, const QMetaType::Type &type) const;
    QString serialize(const QVariant &value) const;
private:
    virtual QString escapeString(const QString &str) const;
    virtual QString unescapeString(const QString &str) const;
};

#endif // SQLSERIALIZER_H
