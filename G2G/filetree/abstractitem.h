#ifndef TREEITEM_H
#define TREEITEM_H

#include <QObject>
#include <QVariant>
#include <QDebug>
#include <QAbstractItemModel>
#include <QTimer>

class AbstractItem {
    AbstractItem(const AbstractItem&) Q_DECL_EQ_DELETE;
    AbstractItem& operator=(const AbstractItem&) Q_DECL_EQ_DELETE;
    static int c;

public:
    AbstractItem();
    virtual ~AbstractItem();
    AbstractItem* child(int row);
    AbstractItem* parent();
    int row() const;

    virtual bool setData(const QModelIndex& index, const QVariant& value, int role) = 0;
    virtual int columnCount(/*const QModelIndex& parent*/) const = 0;
    virtual int rowCount(/*const QModelIndex& parent*/) const = 0;
    virtual Qt::ItemFlags flags(const QModelIndex& index) const = 0;
    virtual QVariant data(const QModelIndex& index, int role) const = 0;

    void add(AbstractItem* item);
    void insert(int row, AbstractItem* item);
    void remove(int row);
    void set(int row, AbstractItem* item);

protected:
    AbstractItem* parentItem = nullptr;
    QList<AbstractItem*> childItems;
};

#include "file.h"
#include "folder.h"
#include "milling.h"

#endif // TREEITEM_H