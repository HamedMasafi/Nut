#include "sampletable.h"

SampleTable::SampleTable(QObject *parent) : Nut::Table(parent)
{
    init();
}

int SampleTable::id() const
{
    return m_id;
}

QString SampleTable::name() const
{
    return m_name;
}

QString SampleTable::lastName() const
{
    return m_lastName;
}

void SampleTable::setId(int id)
{
    if (m_id == id)
        return;

    m_id = id;
    emit idChanged(m_id);
}

void SampleTable::setName(QString name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(m_name);
}

void SampleTable::setLastName(QString lastName)
{
    if (m_lastName == lastName)
        return;

    m_lastName = lastName;
    emit lastNameChanged(m_lastName);
}
