#include "mygraphicsview.h"
#include "edid.h"
#include "mygraphicsview.h"
#include "myscene.h"
#include "qdruler.h"
#include <QGLWidget>
#include <QSettings>
#include <QTimer>
#include <QTransform>
#include <QtWidgets>
#include <mainwindow.h>

MyGraphicsView* MyGraphicsView::self = nullptr;

MyGraphicsView::MyGraphicsView(QWidget* parent)
    : QGraphicsView(parent)
{
    setCacheMode(CacheBackground);
    setOptimizationFlags(DontSavePainterState | DontClipPainter | DontAdjustForAntialiasing);
    setViewportUpdateMode(SmartViewportUpdate);
    setDragMode(RubberBandDrag);
    setInteractive(true);
    ////////////////////////////////////

    // add two rulers on top and left.
    setViewportMargins(RulerBreadth, 0, 0, RulerBreadth);

    // add grid layout
    QGridLayout* gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(0);
    gridLayout->setMargin(0);

    // create rulers
    hRuler = new QDRuler(QDRuler::Horizontal, this);
    vRuler = new QDRuler(QDRuler::Vertical, this);
    hRuler->SetMouseTrack(true);
    vRuler->SetMouseTrack(true);

    // add items to grid layout
    QLabel* corner = new QLabel("<html><head/><body><p><span style=\" color:#ffffff;\">mm</span></p></body></html>", this);
    corner->setAlignment(Qt::AlignCenter);
    corner->setFixedSize(RulerBreadth, RulerBreadth);

    gridLayout->addWidget(corner, 1, 0);
    gridLayout->addWidget(hRuler, 1, 1);
    gridLayout->addWidget(vRuler, 0, 0);
    gridLayout->addWidget(viewport(), 0, 1);

    //    gridLayout->addWidget(vRuler, 0, 0);
    //    gridLayout->addWidget(viewport(), 0, 1);
    //    gridLayout->addWidget(corner, 1, 0);
    //    gridLayout->addWidget(hRuler, 1, 1);
    //    gridLayout->addWidget(horizontalScrollBar(), 2, 0, 2, 0);
    //    gridLayout->addWidget(verticalScrollBar(), 0, 2, 0, 2);
    // finally set layout
    //setLayout(gridLayout);
    qDebug() << children(); //layout();

    connect(horizontalScrollBar(), &QScrollBar::valueChanged, this, &MyGraphicsView::UpdateRuler);
    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &MyGraphicsView::UpdateRuler);

    scale(1.0, -1.0); //flip vertical
    zoom100();

    QSettings settings;
    settings.beginGroup("Viewer");
    setViewport(settings.value("OpenGl").toBool() ? new QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::AlphaChannel | QGL::Rgba)) : new QWidget);
    setRenderHint(QPainter::Antialiasing, settings.value("Antialiasing", false).toBool());
    viewport()->setObjectName("viewport");
    settings.endGroup();
    setScene(new MyScene(this));
    setStyleSheet("QGraphicsView { background: black }");
    self = this;
}

MyGraphicsView::~MyGraphicsView()
{
    self = nullptr;
}

void MyGraphicsView::setScene(QGraphicsScene* Scene)
{
    QGraphicsView::setScene(Scene);
    UpdateRuler();
}

void MyGraphicsView::zoomFit()
{
    scene()->setSceneRect(scene()->itemsBoundingRect());
    fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
    //scale(0.95, 0.95);
    UpdateRuler();
}

void MyGraphicsView::zoomToSelected()
{
    QRectF rect;
    for (const QGraphicsItem* item : scene()->selectedItems()) {
        if (item->type() == DrillItemType)
            continue;
        if (rect.isEmpty())
            rect = item->boundingRect();
        else
            rect = rect.united(item->boundingRect());
    }
    if (rect.isEmpty())
        return;
    fitInView(rect, Qt::KeepAspectRatio);
    zoomOut();
}

void MyGraphicsView::zoom100()
{
    double x = 1.0, y = 1.0;
    if (0) {
        x = qAbs(1.0 / transform().m11() / (25.4 / physicalDpiX()));
        y = qAbs(1.0 / transform().m22() / (25.4 / physicalDpiY()));
    } else {
        QSizeF size(GetEdid()); // size in mm
        QRect scrGeometry = QGuiApplication::primaryScreen()->geometry(); // size in pix
        x = qAbs(1.0 / transform().m11() / (size.height() / scrGeometry.height()));
        y = qAbs(1.0 / transform().m22() / (size.width() / scrGeometry.width()));
    }
    scale(x, y);
    UpdateRuler();
}
#ifdef ANIM
enum {
    DURATION = 300,
    INTERVAL = 30
};
#endif

void MyGraphicsView::zoomIn()
{
    if (transform().m11() > 10000.0)
        return;
#ifdef ANIM
    numScheduledScalings += 1;
    QTimeLine* anim = new QTimeLine(DURATION, this);
    anim->setUpdateInterval(INTERVAL);
    connect(anim, &QTimeLine::valueChanged, this, &MyGraphicsView::ScalingTime);
    connect(anim, &QTimeLine::finished, this, &MyGraphicsView::AnimFinished);
    anim->start();
#else
    scale(zoomFactor, zoomFactor);
    UpdateRuler();
#endif
}

void MyGraphicsView::zoomOut()
{
    if (transform().m11() < 1.0)
        return;
#ifdef ANIM
    numScheduledScalings -= 1;
    QTimeLine* anim = new QTimeLine(DURATION, this);
    anim->setUpdateInterval(INTERVAL);
    connect(anim, &QTimeLine::valueChanged, this, &MyGraphicsView::ScalingTime);
    connect(anim, &QTimeLine::finished, this, &MyGraphicsView::AnimFinished);
    anim->start();
#else
    scale(1.0 / zoomFactor, 1.0 / zoomFactor);
    UpdateRuler();
#endif
}

void MyGraphicsView::wheelEvent(QWheelEvent* event)
{
    if (1)
        switch (event->modifiers()) {
        case Qt::ControlModifier:
            if (abs(event->delta()) == 120) {
                if (event->delta() > 0)
                    zoomIn();
                else
                    zoomOut();
            }
            break;
        case Qt::ShiftModifier:
            if (!event->angleDelta().x())
                QAbstractScrollArea::horizontalScrollBar()->setValue(QAbstractScrollArea::horizontalScrollBar()->value() - (event->delta()));
            break;
        case Qt::NoModifier:
            if (!event->angleDelta().x())
                QAbstractScrollArea::verticalScrollBar()->setValue(QAbstractScrollArea::verticalScrollBar()->value() - (event->delta()));
            //            else
            //                QAbstractScrollArea::horizontalScrollBar()->setValue(QAbstractScrollArea::horizontalScrollBar()->value() - (event->delta()));
            break;
        default:
            QGraphicsView::wheelEvent(event);
            return;
        }
    else
        switch (event->modifiers()) {
        case Qt::ControlModifier:
            if (event->angleDelta().x() != 0)
                QAbstractScrollArea::horizontalScrollBar()->setValue(QAbstractScrollArea::horizontalScrollBar()->value() - (event->delta()));
            else
                QAbstractScrollArea::verticalScrollBar()->setValue(QAbstractScrollArea::verticalScrollBar()->value() - (event->delta()));
            break;
        case Qt::ShiftModifier:
            QAbstractScrollArea::horizontalScrollBar()->setValue(QAbstractScrollArea::horizontalScrollBar()->value() - (event->delta()));
            break;
        case Qt::NoModifier:
            if (abs(event->delta()) == 120) {
                if (event->delta() > 0)
                    zoomIn();
                else
                    zoomOut();
            }
            break;
        default:
            QGraphicsView::wheelEvent(event);
            return;
        }
    event->accept();
}

void MyGraphicsView::UpdateRuler()
{
    layout()->setContentsMargins(0, 0, 0, horizontalScrollBar()->isVisible() ? horizontalScrollBar()->height() : 0);
    updateSceneRect(QRectF()); //actualize mapFromScene
    QPoint p = mapFromScene(QPointF());
    vRuler->SetOrigin(p.y());
    hRuler->SetOrigin(p.x());
    vRuler->SetRulerZoom(qAbs(transform().m22() * 0.1));
    hRuler->SetRulerZoom(qAbs(transform().m11() * 0.1));
}

void MyGraphicsView::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
        return;
    }
    event->ignore();
}

void MyGraphicsView::dropEvent(QDropEvent* event)
{
    for (QUrl& var : event->mimeData()->urls())
        emit fileDroped(var.path().remove(0, 1));
    event->acceptProposedAction();
}

void MyGraphicsView::dragMoveEvent(QDragMoveEvent* event)
{
    event->acceptProposedAction();
}

void MyGraphicsView::resizeEvent(QResizeEvent* event)
{
    QGraphicsView::resizeEvent(event);
    UpdateRuler();
}

void MyGraphicsView::mousePressEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::MiddleButton) {
        setInteractive(false);
        // по нажатию средней кнопки мыши создаем событие ее отпускания выставляем моду перетаскивания и создаем событие зажатой левой кнопки мыши
        QMouseEvent releaseEvent(QEvent::MouseButtonRelease, event->localPos(), event->screenPos(), event->windowPos(), Qt::LeftButton, 0, event->modifiers());
        QGraphicsView::mouseReleaseEvent(&releaseEvent);
        setDragMode(ScrollHandDrag);
        QMouseEvent fakeEvent(event->type(), event->localPos(), event->screenPos(), event->windowPos(), Qt::LeftButton, event->buttons() | Qt::LeftButton, event->modifiers());
        QGraphicsView::mousePressEvent(&fakeEvent);
    } else if (event->button() == Qt::RightButton) {
        // это что бы при вызове контекстного меню ничего постороннего не было
        setDragMode(NoDrag);
        setInteractive(false);
        QGraphicsView::mousePressEvent(event);
    } else {
        // это для выделения рамкой  - работа по-умолчанию левой кнопки мыши
        QGraphicsView::mousePressEvent(event);
    }
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MiddleButton) {
        // отпускаем левую кнопку мыши которую виртуально зажали в mousePressEvent
        QMouseEvent fakeEvent(event->type(), event->localPos(), event->screenPos(), event->windowPos(), Qt::LeftButton, event->buttons() & ~Qt::LeftButton, event->modifiers());
        QGraphicsView::mouseReleaseEvent(&fakeEvent);
        setDragMode(RubberBandDrag);
        setInteractive(true);
    } else if (event->button() == Qt::RightButton) {
        // это что бы при вызове контекстного меню ничего постороннего не было
        QGraphicsView::mousePressEvent(event);
        setDragMode(RubberBandDrag);
        setInteractive(true);
    } else {
        QGraphicsView::mouseReleaseEvent(event);
    }
}

void MyGraphicsView::mouseDoubleClickEvent(QMouseEvent* event)
{
    QGraphicsView::mouseDoubleClickEvent(event);
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    vRuler->SetCursorPos(event->pos());
    hRuler->SetCursorPos(event->pos());
    mouseMove(mapToScene(event->pos()));
    QGraphicsView::mouseMoveEvent(event);
}
