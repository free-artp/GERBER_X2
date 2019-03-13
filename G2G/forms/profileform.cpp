#include "profileform.h"
#include "materialsetupform.h"
#include "normalizeraw.h"
#include "ui_profileform.h"

#include "filetree/filemodel.h"
#include "gcode/gcode.h"
#include <QDockWidget>

#include "tooldatabase/tooldatabase.h"
#include <QMessageBox>
#include <myclipper.h>
#include <myscene.h>

ProfileForm::ProfileForm(QWidget* parent)
    : QWidget(parent)
    , ToolPathUtil("ProfileForm")
    , ui(new Ui::ProfileForm)
{
    ui->setupUi(this);
    ui->lblToolName->setText(tool.name);
    ui->dsbxDepth->setValue(MaterialSetup::thickness);

    auto rb_clicked = [&] {
        QStringList list = {
            ":/toolpath/outside_climb.png",
            ":/toolpath/inside_climb.png",
            ":/toolpath/on_climb.png",
            ":/toolpath/outside_conventional.png",
            ":/toolpath/inside_conventional.png",
            ":/toolpath/on_conventional.png"
        };

        if (ui->rbOutside->isChecked())
            side = Outer;
        else if (ui->rbInside->isChecked())
            side = Inner;
        else if (ui->rbOn->isChecked())
            side = On;

        updateName();

        if (ui->rbClimb->isChecked())
            direction = Climb;
        else if (ui->rbConventional->isChecked())
            direction = Conventional;

        ui->lblPixmap->setPixmap(QPixmap(list[side + direction * 3]));
    };

    connect(ui->rbClimb, &QRadioButton::clicked, rb_clicked);
    connect(ui->rbConventional, &QRadioButton::clicked, rb_clicked);
    connect(ui->rbInside, &QRadioButton::clicked, rb_clicked);
    connect(ui->rbOn, &QRadioButton::clicked, rb_clicked);
    connect(ui->rbOutside, &QRadioButton::clicked, rb_clicked);

    rb_clicked();
}

ProfileForm::~ProfileForm()
{
    qDebug("~PocketForm()");
    delete ui;
}

void ProfileForm::on_pbSelect_clicked()
{
    ToolDatabase tdb(this, { Tool::EndMill, Tool::Engraving });
    if (tdb.exec()) {
        tool = tdb.tool();
        ui->lblToolName->setText(tool.name);
        updateName();
    }
}

void ProfileForm::on_pbEdit_clicked()
{
}

void ProfileForm::on_pbCreate_clicked()
{
    create();
}

void ProfileForm::on_pbClose_clicked()
{
    if (parent())
        static_cast<QWidget*>(parent())->close();
}

void ProfileForm::create()
{
    MyScene* scene = MyScene::self;

    if (!tool.isValid()) {
        QMessageBox::warning(this, "No valid tool...!!!", tool.errorStr());
        return;
    }

    Paths wPaths;
    Paths wrPaths;
    Side boardSide = NullSide;

    G::File const* file = nullptr;

    for (QGraphicsItem* item : scene->selectedItems()) {
        if (item->type() == GerberItemType) {
            GerberItem* gi = static_cast<GerberItem*>(item);
            //            if (boardSide == NullSide) {
            if (!file)
                file = gi->file();
            if (file != gi->file()) {
                QMessageBox::warning(this, "", "Working items from different files!");
                return;
            }
            boardSide = gi->file()->side();
            //            }
            //            if (boardSide != gi->file()->side()) {
            //                QMessageBox::warning(this, "", "Working items from different sides!");
            //                return;
            //            }
        }
        if (item->type() == GerberItemType || item->type() == DrillItemType)
            wPaths.append(static_cast<GraphicsItem*>(item)->paths());
        if (item->type() == RawItemType)
            wrPaths.append(static_cast<GraphicsItem*>(item)->paths());
    }

    if (!wrPaths.isEmpty()) {
        if (side == On)
            wPaths = wrPaths;
        else
            wPaths = NormalizeRaw(wrPaths).paths();
    }

    if (boardSide == NullSide)
        boardSide = Top;

    if (wPaths.isEmpty()) {
        QMessageBox::warning(this, "!!!", tr("No selected..."));
        return;
    }

    GCodeFile* gcode = ToolPathCreator(wPaths).createProfile(tool, ui->rbConventional->isChecked(), ui->dsbxDepth->value(), side);

    if (gcode == nullptr) {
        QMessageBox::information(this, "!!!", tr("The tool does not fit in the Working items!"));
        return;
    }

    gcode->setFileName(ui->leName->text());
    gcode->setSide(boardSide);
    FileModel::self->addGcode(gcode);
}

void ProfileForm::updateName()
{
    static const QStringList name = { "Profile Outside", "Profile Inside", "Profile On" };
    ui->leName->setText(name[side] + " (" + tool.name + ")");
}
