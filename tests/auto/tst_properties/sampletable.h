#ifndef SAMPLETABLE_H
#define SAMPLETABLE_H

#include <QtNut/table.h>

class SampleTable : public Nut::Table
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

    int m_id;
    QString m_name;

    NUT_FIELD(int, id)
    NUT_FIELD(QString, name)

public:
    explicit SampleTable(QObject *parent = nullptr);

    int id() const;
    QString name() const;

public slots:
    void setId(int id);
    void setName(QString name);

signals:
    void idChanged(int id);
    void nameChanged(QString name);
};

#endif // SAMPLETABLE_H
