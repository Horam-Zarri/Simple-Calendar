#include "mainwindow.h"

#include <QBoxLayout>
#include <QSettings>
#include <QToolButton>
#include <QDateEdit>
#include <QMenu>
#include <QMenuBar>
#include <QFile>
#include <QFileDialog>
#include <QForeach>
#include <QMessageBox>

static const QDate minimumDate(1970,1,1);
static const QDate maximumDate(3000,1,1);

MainWindow::MainWindow()

{

    calendarWidget = new CalendarWidget();

    retrieveSettings();
    initDate();

    createLayouts();
    setCentralWidget(mainWidget);
    createActions();
    createMenus();

    calendarWidget->updateCalendarGrid();
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    saveSettings();
}

void MainWindow::importSettings()
{
    QString path = QFileDialog::getOpenFileName(this, "Select File To Import: ", ".", "Text Files (*.txt)");

    if (path.isEmpty())
        return;


    QFile file(path);

    if (file.open(QIODevice::ReadOnly)) {

        calendarWidget->clearTokens();

        QTextStream stream(&file);

        while (!stream.atEnd()) {

            QString str;
            stream.readLineInto(&str);

            QList<QString> list = str.split('/');

            QDate date(list.at(2).toInt(), list.at(0).toInt(), list.at(1).toInt());

            DateToken token(date, list.at(3), QColor::fromString(list.at(4)), QColor::fromString(list.at(5)));
            calendarWidget->insertDateToken(token);
        }

        calendarWidget->updateCalendarGrid();
    }
}

void MainWindow::exportSettings()
{
    QDir dir = QFileDialog::getExistingDirectory(this, "Select Directory To Export: ", ".");
    if (dir.isEmpty())
        return;

    QFile file(dir.absolutePath().append("/caldat.txt"));

    if (file.exists()) {
        QMessageBox::warning(this, "File exists in directory", "A file named caldat.txt exists in this directory"
                    "which conflicts with this app.");
        return;

    }

    if (file.open(QIODevice::WriteOnly)) {

        QTextStream stream(&file);

        foreach(const DateToken& token, calendarWidget->retrieveTokens()) {

            stream << token.accessToken() << '/' << token.textColor().name(QColor::HexArgb)
                   << '/' << token.outlineColor().name(QColor::HexArgb) << '\n';
        }

    }
}

void MainWindow::clearMonthEvents()
{
    QList<DateToken> list = calendarWidget->fetchMonthTokens();

    for (auto p = list.begin(); p != list.end(); p++)
        calendarWidget->eraseDateToken(*p);

    calendarWidget->updateCalendarGrid();
}

void MainWindow::clearAllEvents()
{
    QMessageBox::StandardButton button = QMessageBox::question(this, "Confirmation", "Are you sure you want to delete ALL EVENTS?", QMessageBox::Yes | QMessageBox::No);

    if (button == QMessageBox::No)
        return;

    calendarWidget->clearTokens();
    calendarWidget->updateCalendarGrid();
}

void MainWindow::popupAbout()
{
    QMessageBox::about(this, "About", "Version 1.0\nCreated by Horam Zarri\nGithub: Horam-Zarri");
}

void MainWindow::popupAboutQt()
{
    QMessageBox::aboutQt(this, "");
}


void MainWindow::initDate()
{
    dateEdit = new QDateEdit(calendarWidget->getDate());
    dateEdit->setCalendarPopup(true);
    dateEdit->setMinimumDate(minimumDate);
    dateEdit->setMaximumDate(maximumDate);


    QObject::connect(dateEdit, SIGNAL(dateChanged(QDate)), calendarWidget, SLOT(setDate(QDate)));
    QObject::connect(calendarWidget, SIGNAL(dateChanged(QDate)), dateEdit, SLOT(setDate(QDate)));
}

void MainWindow::createLayouts()
{
    midLayout = new QVBoxLayout();
    midLayout->addWidget(dateEdit);
    midLayout->addWidget(calendarWidget);

    leftToolButton = new QToolButton();
    leftToolButton->setIcon(QIcon(":/icons/leftArrow.png"));

    connect(leftToolButton, SIGNAL(clicked(bool)), calendarWidget, SLOT(decrementMonth()));

    rightToolButton = new QToolButton();
    rightToolButton->setIcon(QIcon(":/icons/rightArrow.png"));

    connect(rightToolButton, SIGNAL(clicked(bool)), calendarWidget, SLOT(incrementMonth()));

    mainLayout = new QHBoxLayout();
    mainLayout->addWidget(leftToolButton);
    mainLayout->addLayout(midLayout);
    mainLayout->addWidget(rightToolButton);

    mainWidget = new QWidget();
    mainWidget->setLayout(mainLayout);

}

void MainWindow::createActions()
{
    importAction = new QAction("Import");
    importAction->setShortcut(QKeySequence::Open);
    importAction->setShortcutContext(Qt::ShortcutContext::ApplicationShortcut);
    importAction->setIcon(QIcon(":/icons/import.png"));
    connect(importAction, SIGNAL(triggered(bool)), this, SLOT(importSettings()));

    exportAction = new QAction("Export");
    exportAction->setShortcut(QKeySequence::Save);
    exportAction->setShortcutContext(Qt::ShortcutContext::ApplicationShortcut);
    exportAction->setIcon(QIcon(":/icons/export.png"));
    connect(exportAction, SIGNAL(triggered(bool)), this, SLOT(exportSettings()));

    exitAction = new QAction("&Exit");
    exitAction->setShortcutContext(Qt::ShortcutContext::ApplicationShortcut);
    exitAction->setShortcut(QKeySequence::Close);
    exitAction->setIcon(QIcon(":/icons/exit.png"));
    connect(exitAction, SIGNAL(triggered(bool)), this, SLOT(close()));

    clearMonthEventsAction = new QAction("Clear Month Events");
    clearMonthEventsAction->setIcon(QIcon(":/icons/clear.png"));
    connect(clearMonthEventsAction, SIGNAL(triggered(bool)), this, SLOT(clearMonthEvents()));

    clearAllEventsAction = new QAction("Clear All Events");
    clearAllEventsAction->setIcon(QIcon(":/icons/clear.png"));
    connect(clearAllEventsAction, SIGNAL(triggered(bool)), this, SLOT(clearAllEvents()));

    aboutAction = new QAction("About");
    connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(popupAbout()));

    aboutQtAction = new QAction("About Qt");
    connect(aboutQtAction, SIGNAL(triggered(bool)), this, SLOT(popupAboutQt()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(importAction);
    fileMenu->addAction(exportAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu("&Menu");
    editMenu->addAction(clearMonthEventsAction);
    editMenu->addAction(clearAllEventsAction);

    helpMenu = menuBar()->addMenu("Help");
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
}



void MainWindow::saveSettings()
{

    QSettings settings("HZ-Software", "Calendar");


    settings.setValue("eventcount", calendarWidget->retrieveTokens().size());
    settings.beginGroup("events");
    settings.remove("");


    auto& list = calendarWidget->retrieveTokens();


    int eventNumber = 0;

    for (auto p = list.cbegin(); p != list.cend(); p++) {

        QString val = DateToken::itos(eventNumber);

        settings.setValue(val.append("date"), p->date());
        settings.setValue(val.append("str"), p->accessToken().split('/').at(3));
        settings.setValue(val.append("textcolor"), p->textColor());
        settings.setValue(val.append("outcolor"), p->outlineColor());

        eventNumber++;
    }

    settings.endGroup();


}

void MainWindow::retrieveSettings()
{
    QSettings settings("HZ-Software", "Calendar");
    int count = settings.value("eventcount", -1).toInt();

    if (count == -1)
        return;


    settings.beginGroup("events");

    for (int i = 0; i < count; i++) {

        QString val = DateToken::itos(i);

        QDate tokenDate = settings.value(val.append("date")).toDate();
        QString tokenStr = settings.value(val.append("str")).toString();
        QColor tokenTColor = settings.value(val.append("textcolor")).value<QColor>();
        QColor tokenOColor = settings.value(val.append("outcolor")).value<QColor>();

        DateToken tok(tokenDate, tokenStr, tokenTColor, tokenOColor);
        calendarWidget->insertDateToken(tok);

    }
}
