#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "point.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QThread>
#include <qevent.h>

namespace Gerber {
class Parser;
}

class DockWidget;

class MainWindow : public QMainWindow, private Ui::MainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

    void openFile(const QString& fileName);

    QAction* closeAllAct = nullptr;
    QAction* exportPdfAct = nullptr;
    static MainWindow* self;

    void resetActions()
    {
        for (QAction* action : toolpathActionList)
            action->setChecked(false);
    }

signals:
    void parseFile(const QString& filename);

private:
    enum { MaxRecentFiles = 20 };
    bool hasRecentFiles();
    inline QString fileKey() { return QStringLiteral("file"); }
    inline QString recentFilesKey() { return QStringLiteral("recentFileList"); }
    void updateRecentFileActions();
    void prependToRecentFiles(const QString& fileName);
    void openRecentFile();
    void setRecentFilesVisible(bool visible);
    void writeRecentFiles(const QStringList& files, QSettings& settings);

    QString lastPath;
    Gerber::Parser* gerberParser;
    QThread gerberThread;

    //////////////////////

    QString strippedName(const QString& fullFileName);
    QStringList readRecentFiles(QSettings& settings);

    void init();
    void about();
    void closeFiles();
    void createActions();
    void createDockWidget(QWidget* dwContent, int type);
    void createStatusBar();
    void createShtifts();
    void open();
    void saveSelectedToolpaths();
    void setCurrentFile(const QString& fileName);
    void showSettingsDialog();
    void readSettings();
    void writeSettings();
    void selectAll();
    void on_customContextMenuRequested(const QPoint& pos);

    QAction* recentFileActs[MaxRecentFiles + 1];
    QAction* recentFileSeparator;
    QAction* recentFileSubMenuAct;

    QString curFile;
    bool isUntitled;

    void fileProgress(const QString& fileName, int max, int value);
    void fileError(const QString& fileName, const QString& error);

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    DockWidget* dockWidget;
    QMenu* fileMenu;
    QMenu* helpMenu;
    QMenu* recentMenu;
    QMenu* serviceMenu;
    QToolBar* fileToolBar;
    QToolBar* toolpathToolBar;
    QToolBar* zoomToolBar;
    QVector<QAction*> toolpathActionList;

    // QWidget interface
protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

    // QMainWindow interface
public:
    virtual QMenu *createPopupMenu() override;
};

class DockWidget : public QDockWidget {
    Q_OBJECT
public:
    DockWidget(QWidget* parent = nullptr)
        : QDockWidget(parent)
    {
    }
    virtual ~DockWidget() {}

    // QWidget interface
protected:
    void closeEvent(QCloseEvent* event) override
    {
        if (widget())
            delete widget();
        MainWindow::self->resetActions();
        event->accept();
    }
};

#endif // MAINWINDOW_H
