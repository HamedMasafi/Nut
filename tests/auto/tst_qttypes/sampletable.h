#ifndef SAMPLETABLE_H
#define SAMPLETABLE_H

#include <QPoint>
#include <QPointF>
#include <QPolygon>
#include <QPolygonF>
#include <QColor>

#include "table.h"

class SampleTable : public NUT_WRAP_NAMESPACE(Table)
{
    Q_OBJECT

    NUT_PRIMARY_AUTO_INCREMENT(id)
    NUT_DECLARE_FIELD(int, id, id, setId)

    NUT_DECLARE_FIELD(QPoint, f_point, f_point, setPoint)
    NUT_DECLARE_FIELD(QPointF, f_pointf, f_pointf, setPointf)
    NUT_DECLARE_FIELD(QPolygon, f_polygon, f_polygon, setPolygon)
    NUT_DECLARE_FIELD(QPolygonF, f_polygonf, f_polygonf, setPolygonf)
    NUT_DECLARE_FIELD(QColor, f_color, f_color, setColor)

public:
    SampleTable(QObject *parent = nullptr);
};

#endif // SAMPLETABLE_H
