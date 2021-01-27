#ifndef SAMPLETABLE_H
#define SAMPLETABLE_H

#include <QtNut/table.h>

class SampleTable : public Nut::Table
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString lastName READ lastName WRITE setLastName NOTIFY lastNameChanged)

    int m_id;
    QString m_name;
    QString m_lastName;

    NUT_PRIMARY_KEY(id)
    NUT_FIELD(int, id)
    NUT_FIELD(QString, name)
    NUT_FIELD(QString, lastName)

public:
    explicit SampleTable(QObject *parent = nullptr);

    int id() const;
    QString name() const;
    QString lastName() const;

public slots:
    void setId(int id);
    void setName(QString name);
    void setLastName(QString lastName);

signals:
    void idChanged(int id);
    void nameChanged(QString name);
    void lastNameChanged(QString lastName);
};

#endif // SAMPLETABLE_H
