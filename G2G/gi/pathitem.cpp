#include "pathitem.h"

#include <QPainter>

PathItem::PathItem(const Path& path)
{
    m_paths = { path };
    if (m_paths[0][0] != m_paths[0].last())
        m_paths[0].append(m_paths[0][0]);
    m_shape.addPolygon(PathToQPolygon(m_paths[0]));
    double k = m_pen.widthF() / 2;
    rect = m_shape.boundingRect() + QMarginsF(k, k, k, k);
    //    setAcceptTouchEvents(false);
    //    setAcceptedMouseButtons(false);
    //    setAcceptHoverEvents(false);
    //    setEnabled(false);
    //    setFlags(GraphicsItemFlags(0));
    setCacheMode(DeviceCoordinateCache);
}

QRectF PathItem::boundingRect() const
{
    return rect;
}

QPainterPath PathItem::shape() const { return m_shape; }

void PathItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/)
{
    if (0) {
        painter->setBrush(QBrush(Qt::NoBrush));
        painter->setPen(m_pen);
        painter->setPen(QPen(Qt::white, 0.0));
        painter->drawPath(m_shape);
        //  painter->drawPath(m_shape);
        painter->setBrush(QBrush(Qt::red));
        const double d = m_pen.widthF();
        QRectF r(m_paths[0].first().X * dScale - d / 2, m_paths[0].first().Y * dScale - d / 2, d, d);
        painter->drawEllipse(r);
    } else {
        painter->setBrush(QBrush(Qt::NoBrush));
        painter->setPen(m_pen);
        painter->drawPath(m_shape);
        //  if (w) {
        //  painter->setPen(QPen(Qt::red, 0.0));
        // //  QLineF line(
        // //  QPointF(m_path[0].X * dScale, m_path[0].Y * dScale),
        // //  QPointF(m_path[1].X * dScale, m_path[1].Y * dScale));
        // //  double length = /*w * (*/ 50 / MyGraphicsView::self->matrix().m11() /*)*/;
        // //  double angle = line.angle();
        // //  line.setLength(length);
        // //  painter->drawLine(line);
        // //  line.setAngle(angle + 10);
        // //  painter->drawLine(line);
        // //  line.setAngle(angle - 10);
        // //  painter->drawLine(line);
        // //  for (int i = 0; i < m_path.size() - 1; ++i) {
        // //  QLineF line(
        // //  QPointF(m_path[i].X * dScale, m_path[i].Y * dScale),
        // //  QPointF(m_path[i + 1].X * dScale, m_path[i + 1].Y * dScale));

        // //  double length = /*w * (*/ 50 / MyGraphicsView::self->matrix().m11() /*)*/;
        // //  double angle = line.angle();

        // //  line.setLength(length);
        // //  painter->drawLine(line);

        // //  line.setAngle(angle + 10);
        // //  painter->drawLine(line);

        // //  line.setAngle(angle - 10);
        // //  painter->drawLine(line);
        // // }
        // }
    }
}

int PathItem::type() const { return PATH_ITEM; }

Paths PathItem::paths() const
{
    return m_paths;
}