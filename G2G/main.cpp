#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QGLWidget>
#include <QLocale>
#include <QTranslator>

#include "application.h"
#include "mainwindow.h"

#ifndef linux
#include <qt_windows.h>
#endif

int main(int argc, char* argv[])
{

    //#ifndef linux
    //    HANDLE hCorvetEvent = CreateEventA(nullptr, FALSE, FALSE, ("Getber2Gcode"));
    //    if (GetLastError() == ERROR_ALREADY_EXISTS) {
    //        if (argc == 2) {
    //            QSettings* settings;
    //            settings = new QSettings("XrSoft", "G2G");
    //            settings->setValue("AddFile", QString::fromLocal8Bit(argv[1]).replace(QString("//"), QString("/")));
    //            CloseHandle(hCorvetEvent);
    //        }
    //        return 0;
    //    }
    //#endif

    //Q_INIT_RESOURCE(resources);
    QApplication app(argc, argv);

    app.setApplicationName("G2G");
    app.setOrganizationName("XrSoft");
    app.setApplicationVersion("0.7.3");

    QIcon::setThemeSearchPaths({ "../icons/breeze/", "icons/breeze/" });
    QIcon::setThemeName("Breeze");

    QGLFormat glf = QGLFormat::defaultFormat();
    glf.setSampleBuffers(true);
    glf.setSamples(16);
    QGLFormat::setDefaultFormat(glf);

    QString loc(QLocale().name().left(2));
    qDebug() << "locale:" << loc;
    QString trFolder(qApp->applicationDirPath() + "/translations/");
    QString trFileName(trFolder + qApp->applicationDisplayName() + "_" + loc + ".qm");
    if (QFile::exists(trFileName)) {
        QTranslator* translator = new QTranslator();
        if (translator->load(trFileName))
            app.installTranslator(translator);
        else
            delete translator;
    }
    QString baseTrFileName(trFolder + "qtbase_" + loc + ".qm");
    qDebug() << baseTrFileName;
    if (QFile::exists(trFileName)) {
        QTranslator* baseTranslator = new QTranslator();
        if (baseTranslator->load(baseTrFileName))
            app.installTranslator(baseTranslator);
        else
            delete baseTranslator;
    }

    MainWindow* mainWin = new MainWindow;
    mainWin->setIconSize({ 24, 24 });
    mainWin->show();

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file(s) to open.");
    parser.process(app);
    for (const QString& file : parser.positionalArguments()) {
        mainWin->openFile(file);
    }
    return app.exec();
}
