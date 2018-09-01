#ifndef TOOLEDITFORM_H
#define TOOLEDITFORM_H

#include "tool.h"

#include <QWidget>

namespace Ui {
class ToolEditForm;
}

class ToolItem;

class ToolEditForm : public QWidget {
    Q_OBJECT

public:
    explicit ToolEditForm(QWidget* parent = nullptr);
    ~ToolEditForm();

    void setItem(ToolItem* item);
    void setTool(const Tool& tool);

signals:
    void itemChanged(ToolItem* item);

private slots:

    void on_cbxFeedSpeeds_currentIndexChanged(int index);
    void on_cbxToolType_currentIndexChanged(int index);

    void on_leName_textChanged(const QString& arg1);
    void on_teNote_textChanged();

    void calculate(int index, double value);
    void valueChanged(double value);

    void on_pbApply_clicked();

private:
    Ui::ToolEditForm* ui;

    ToolItem* m_item = nullptr;
    Tool m_tool;
    double m_feed = 1.0;

    void updateName();
    void setRed();
    void setVisibleWidgets(bool visible);
};

#endif // TOOLEDITFORM_H