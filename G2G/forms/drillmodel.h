#ifndef DRILLMODEL_H
#define DRILLMODEL_H

#include <QAbstractTableModel>
#include <QIcon>

enum {
    tAperture,
    tTool,
    tItem
};

class DrillModel : public QAbstractTableModel {
    typedef struct Row {
        Row(const QString& name, const QIcon& icon, int id)
            : name{ name, "" }
            , icon{ icon, QIcon() }
            , id{ id, -1 }
        {
        }
        QString name[2];
        QIcon icon[2];
        int id[2];
    } Row;
    QList<Row> m_data;
    int m_type;

public:
    DrillModel(int type, QObject* parent = nullptr);
    void appendRow(const QString& name, const QIcon& icon, int id);
    void setToolId(int row, int id);
    int toolId(int row);
    void setApertureId(int row, int id);
    int apertureId(int row);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
};
#endif // DRILLMODEL_H