#ifndef NUT_NAMESPACE_H
#define NUT_NAMESPACE_H

//avoid ide warnings
#include <QtNut/nut_global.h>

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtCore/QMetaClassInfo>

QT_BEGIN_NAMESPACE

NUT_BEGIN_NAMESPACE

inline bool nutClassInfo(const QMetaClassInfo &classInfo,
                         QString &type, QString &name, QVariant &value)
{
    if (!QString::fromUtf8(classInfo.name()).startsWith(QStringLiteral(__nut_NAME_PERFIX)))
        return false;

    QStringList parts = QString::fromUtf8(classInfo.value()).split(QStringLiteral("\n"));
    if (parts.count() != 3)
        return false;

    type = parts[0];
    name = parts[1];
    value = QVariant::fromValue(parts[2]);
    return true;
}

inline bool nutClassInfoString(const QMetaClassInfo &classInfo,
                               QString &type, QString &name, QString &value)
{
    if (!QString::fromUtf8(classInfo.name()).startsWith(QStringLiteral(__nut_NAME_PERFIX)))
        return false;

    QStringList parts = QString::fromUtf8(classInfo.value()).split(QStringLiteral("\n"));
    if (parts.count() != 3)
        return false;

    type = parts[0];
    name = parts[1];
    value = parts[2];
    return true;
}

inline bool nutClassInfoBool(const QMetaClassInfo &classInfo,
                             QString &type, QString &name, bool &value)
{
    if (!QString::fromUtf8(classInfo.name()).startsWith(QStringLiteral(__nut_NAME_PERFIX)))
        return false;

    QStringList parts = QString::fromUtf8(classInfo.value()).split(QStringLiteral("\n"));
    if (parts.count() != 3)
        return false;

    QString buffer = parts[2].toLower();
    if (buffer != QStringLiteral("true") && buffer != QStringLiteral("false"))
        return false;

    type = parts[0];
    name = parts[1];
    value = (buffer == QStringLiteral("true"));
    return true;
}

inline bool nutClassInfoInt(const QMetaClassInfo &classInfo,
                            QString &type, QString &name, bool &value)
{
    if (!QString::fromUtf8(classInfo.name()).startsWith(QStringLiteral(__nut_NAME_PERFIX)))
        return false;

    QStringList parts = QString::fromUtf8(classInfo.value()).split(QStringLiteral("\n"));
    if (parts.count() != 3)
        return false;
    bool ok;
    type = parts[0];
    name = parts[1];
    value = parts[2].toInt(&ok);
    return ok;
}

#ifdef NUT_RAW_POINTER
    template<typename T>
    using RowList = QList<T *>;

    template<class T>
    using WeakRowList = QList<T *>;

    template<typename T>
    using RowSet = QSet<T *>;

    template<typename T>
    using Row = T *;

    template<class T>
    inline Row<T> create()
    {
        return new T;
    }

    template<class T>
    inline T *get(const Row<T> row)
    {
        return row;
    }

    template<class T>
    inline T *get(const QSharedPointer<T> row)
    {
        return row.data();
    }
#else
    template<class T>
    using RowList = QList<QSharedPointer<T>>;

    template<class T>
    using WeakRowList = QList<QWeakPointer<T>>;

    template<class T>
    using RowSet = QSet<QSharedPointer<T>>;

    template<typename T>
    using Row = QSharedPointer<T>;

    template<typename T>
    using WeakRow = QWeakPointer<T>;

    template<class T>
    inline Row<T> create()
    {
        return QSharedPointer<T>(new T);
    }

    template<class T>
    inline Row<T> create(QObject *parent)
    {
        return QSharedPointer<T>(new T(parent));
    }

    template<class T>
    inline Row<T> createFrom(T *row)
    {
        return QSharedPointer<T>(row);
    }
    template<class T>
    inline Row<T> createFrom(const QSharedPointer<T> row)
    {
        return row;
    }
#endif

NUT_END_NAMESPACE

QT_END_NAMESPACE

#endif // NUT_NAMESPACE_H
