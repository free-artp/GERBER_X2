#include "toolitem.h"
#include "tooldatabase.h"
#include "toolmodel.h"

#include <QJsonArray>
#include <QPixmap>

bool ToolItem::m_deleteEnable = false;

ToolItem::ToolItem(const ToolItem& item)
{
    if (item.m_isTool) {
        m_toolId = item.m_toolId;
        m_isTool = true;
        item.m_isTool = false;
    } else {
        m_name = item.m_name;
        m_note = item.m_note;
    }
    for (ToolItem* i : item.childItems) {
        addChild(new ToolItem(*i));
    }
}

ToolItem::ToolItem(int toolId)
    : m_toolId(toolId)
    , m_isTool(true)
{
}

ToolItem::ToolItem()
{
}

ToolItem::~ToolItem()
{
    if (m_isTool && m_deleteEnable)
        ToolHolder::tools.remove(m_toolId);
    qDeleteAll(childItems);
}

//void ToolItem::read(const QJsonObject& json)
//{
//    QJsonArray toolArray = json["tools"].toArray();
//    for (int treeIndex = 0; treeIndex < toolArray.size(); ++treeIndex) {
//        Tool tool;
//        QJsonObject toolObject = toolArray[treeIndex].toObject();
//        tool.read(toolObject);
//        tool.id = toolObject["id"].toInt();
//        ToolHolder::tools[tool.id] = tool;
//    }
//}

//void ToolItem::write(QJsonObject& json)
//{
//    QJsonArray toolArray;
//    QMap<int, Tool>::iterator i = ToolHolder::tools.begin();
//    while (i != ToolHolder::tools.constEnd()) {
//        QJsonObject toolObject;
//        i.value().write(toolObject);
//        toolObject["id"] = i.key();
//        toolArray.append(toolObject);
//        ++i;
//    }
//    json["tools"] = toolArray;
//}

int ToolItem::row() const
{
    if (parentItem != nullptr)
        return parentItem->childItems.indexOf(const_cast<ToolItem*>(this));
    return 0;
}

int ToolItem::childCount() const
{
    return childItems.size();
}

ToolItem* ToolItem::child(int row) const
{
    return childItems.at(row);
}

ToolItem* ToolItem::takeChild(int row)
{
    return childItems.takeAt(row);
}

void ToolItem::setChild(int row, ToolItem* item)
{
    if (item)
        item->parentItem = this;

    if (row < childItems.size()) {
        delete childItems[row];
        childItems[row] = item;
    }
}

bool ToolItem::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (index.isValid()) {
        switch (role) {
        case Qt::EditRole:
            switch (index.column()) {
            case 0:
                setName(value.toString());
                return true;
            case 1:
                setNote(value.toString());
                return true;
            default:
                return false;
            }
        default:
            return false;
        }
    }
    return false;
}

QVariant ToolItem::data(const QModelIndex& index, int role) const
{
    QPixmap p(16, 16);
    p.fill(Qt::red);

    if (index.isValid()) {
        switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            switch (index.column()) {
            case 0:
                return name();
            case 1:
                return note();
            default:
                return QVariant();
            }
        case Qt::DecorationRole:
            if (index.column() == 0)
                if (m_isTool)
                    return ToolHolder::tools[m_toolId].icon();
                else
                    return Icon(ToolFolderIcon);
        case Qt::UserRole:
            return ToolHolder::tools[m_toolId].type;
        case Qt::UserRole + 1:
            return m_toolId;
        default:
            return QVariant();
        }
    }
    return QVariant();
}

Qt::ItemFlags ToolItem::flags() const
{
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsSelectable;
    if (!m_isTool)
        flags |= Qt::ItemIsDropEnabled;
    return flags;
}

int ToolItem::toolId() const
{
    return m_toolId;
}

Tool& ToolItem::tool()
{
    static Tool tmp;

    if (m_isTool)
        return ToolHolder::tools[m_toolId];
    return tmp;
}

bool ToolItem::isTool() const
{
    return m_isTool;
}

void ToolItem::setIsTool()
{
    m_isTool = true;
    if (ToolHolder::tools.size())
        m_toolId = ToolHolder::tools.lastKey() + 1;
    ToolHolder::tools[m_toolId].diameter = 0.0;
}

QString ToolItem::note() const
{
    return m_isTool ? (ToolHolder::tools[m_toolId].note.isEmpty() ? "Tool Id " + QString::number(m_toolId) : ToolHolder::tools[m_toolId].note) : m_note;
}

void ToolItem::setNote(const QString& value)
{
    (m_isTool ? ToolHolder::tools[m_toolId].note : m_note) = value;
}

void ToolItem::setDeleteEnable(bool deleteEnable)
{
    m_deleteEnable = deleteEnable;
}

QString ToolItem::name() const
{
    return m_isTool ? ToolHolder::tools[m_toolId].name : m_name;
}

void ToolItem::setName(const QString& value)
{
    (m_isTool ? ToolHolder::tools[m_toolId].name : m_name) = value;
}

void ToolItem::addChild(ToolItem* item)
{
    if (item)
        item->parentItem = this;
    childItems.append(item);
}

void ToolItem::insertChild(int row, ToolItem* item)
{
    if (item)
        item->parentItem = this;
    if (row < childItems.size())
        childItems.insert(row, item);
    else if (row == childItems.size())
        childItems.append(item);
}

void ToolItem::removeChild(int row)
{
    m_deleteEnable = true;
    delete childItems.at(row);
    childItems.removeAt(row);
    m_deleteEnable = false;
}

ToolItem* ToolItem::parent() { return parentItem; }
