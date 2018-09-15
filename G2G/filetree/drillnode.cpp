#include "drillnode.h"
#include "fileholder.h"
#include "gerbernode.h"

#include <QFileInfo>
#include <mainwindow.h>

DrillNode::DrillNode(Drill* file)
    : m_id(FileHolder::addFile(file))
{
    AbstractNode::files.append(file->fileName());
    FileHolder::file<Drill>(m_id)->itemGroup()->addToTheScene();
    //FileHolder::file<Drill>(m_id)->itemGroup()->setZValue(-m_id);
    MyGraphicsView::self->ZoomFit();
    MyGraphicsView::self->Zoom100();
    MainWindow::self->closeAllAct->setEnabled(true);
}

DrillNode::~DrillNode()
{

    FileHolder::deleteFile(m_id);
    MainWindow::self->closeAllAct->setEnabled(FileHolder::isEmpty());
    if (MyScene::self) {
        MyScene::self->setSceneRect(MyScene::self->itemsBoundingRect());
        MyScene::self->update();
    }
}

bool DrillNode::setData(const QModelIndex& index, const QVariant& value, int role)
{
    switch (index.column()) {
    case 0:
        switch (role) {
        case Qt::CheckStateRole:
            checkState = value.value<Qt::CheckState>();
            FileHolder::file<Drill>(m_id)->itemGroup()->setVisible(checkState == Qt::Checked);
            return true;
        default:
            return false;
        }

    default:
        return false;
    }
    return false;
}

int DrillNode::columnCount() const { return 3; }

int DrillNode::childCount() const { return 0; }

Qt::ItemFlags DrillNode::flags(const QModelIndex& index) const
{
    switch (index.column()) {
    case 0:
        return Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemNeverHasChildren;
    default:
        return Qt::ItemIsEnabled | Qt::ItemNeverHasChildren;
    }
}

QVariant DrillNode::data(const QModelIndex& index, int role) const
{
    switch (index.column()) {
    case 0:
        switch (role) {
        case Qt::DisplayRole:
        case Qt::ToolTipRole:
            return FileHolder::file<Drill>(m_id)->fileName();
        case Qt::EditRole:
            return FileHolder::file<Drill>(m_id)->fileName();
        case Qt::CheckStateRole:
            return checkState;
        case Qt::DecorationRole:
            return QIcon::fromTheme("roll");
        case Qt::UserRole:
            return QVariant::fromValue(static_cast<void*>(FileHolder::file<Drill>(m_id)));
        default:
            return QVariant();
        }
    default:
        break;
    }

    return QVariant();
}