#ifndef POINT_H
#define POINT_H

#include "myscene.h"

#include <QBrush>
#include <QGraphicsItem>
#include <QPen>

class Point : public QGraphicsObject {
    Q_OBJECT

public:
    Point(int type);
    ~Point();

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    QPainterPath shape() const override;
    int type() const override;

    void resetPos();
    void setPos(const QPointF& pos);
    void setPosX(double x);
    void setPosY(double y);

    enum {
        Zero,
        Home
    };

private:
    QRectF m_rect;
    QPainterPath m_path;
    QPainterPath m_shape;
    int m_type;
    void updateMaterialSetupForm();

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
};

class Shtift : public QObject, public QGraphicsItem {

public:
    Shtift();
    ~Shtift();
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    QPainterPath shape() const override;
    int type() const override;
    static QVector<Shtift*> shtifts();
    void resetPos();

private:
    QRectF m_rect;
    QPainterPath m_path;
    QPainterPath m_shape;
    static QVector<Shtift*> m_shtifts;
    QPointF m_lastPos;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
};

#endif // POINT_H
