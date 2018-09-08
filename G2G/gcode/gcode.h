#ifndef GCODE_H
#define GCODE_H

#include <QGraphicsItemGroup>
#include <QObject>
#include <graphicsitem.h>
#include <myclipper.h>
#include <tooldatabase/tool.h>

enum GCodeType {
    PROFILE,
    POCKET,
    DRILLING,
    MATERIAL_SETUP_FORM
};

class GCode : public ItemGroup {
public:
    GCode(const Paths& paths, const Tool& tool, double m_depth, GCodeType type);
    //    QRectF boundingRect() const override { return rect; }
    //    QPainterPath shape() const override { return m_shape; }
    //    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override
    //    {
    //        Q_UNUSED(widget);
    //        //    painter->setCompositionMode(QPainter::CompositionMode_Xor);
    //        painter->setBrush(Qt::NoBrush);
    //        QColor c(255, 255, 255, 100);
    //        if (option->state & QStyle::State_MouseOver) {
    //            c.setAlpha(150);
    //        }
    //        if (option->state & QStyle::State_Selected) {
    //            c.setAlpha(200);
    //        }
    //        painter->setPen(QPen(c, tool.diameter, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    //        painter->drawPath(m_drawShape);
    //        if (cutDepth > 0.0 && tool.angle > 0.0) {
    //            painter->setPen(QPen(c, d, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    //            painter->drawPath(m_drawShape);
    //        }
    //        painter->setPen(QPen(Qt::white, 0.0));
    //        painter->drawPath(m_drawShape);
    //    }
    //    int type() const { return QGraphicsItem::UserType + 3; }
    Paths getPaths() const;
    void save(const QString& name = QString());

    QString name() const;
    void setName(const QString& name);
    const GCodeType type;

private:
    QPainterPath m_shape;
    QPainterPath m_drawShape;
    Paths paths;
    QRectF rect;
    Tool tool;
    QString m_name;
    double m_depth;
    double d;
};
////////////////////////////////////////////////////
/// \brief The GItem class
///
class GItem : public GraphicsItem {
public:
    GItem(const Path& m_path);
    //~WorkItem() override {}

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    int type() const override;
    double w = 0.0;

private:
    QPainterPath m_shape;
    Path m_path;
    QRectF rect;
};

#endif // GCODE_H
