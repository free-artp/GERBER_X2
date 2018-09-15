#ifndef DrillItem__H
#define DrillItem__H

#include "abstractnode.h"
#include <QObject>
#include <gcode/drl.h>

class DrillNode : public QObject, public AbstractNode {
    Q_OBJECT

public:
    DrillNode(Drill* file);
    ~DrillNode();
    // AbstractItem interface
public:
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    int columnCount() const override;
    int childCount() const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant data(const QModelIndex& index, int role) const override;

private:
    Qt::CheckState checkState = Qt::Checked;
    const int m_id;
};

#endif // DrillItem__H