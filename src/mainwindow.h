#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "calendarwidget.h"

#include <QWidget>
#include <QMainWindow>

class QDateEdit;
class QVBoxLayout;
class QHBoxLayout;
class QToolButton;
class QMenu;
class QAction;


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow();

protected:
    void closeEvent(QCloseEvent* e) override;
private slots:
    void importSettings();
    void exportSettings();

    void clearMonthEvents();
    void clearAllEvents();

    void popupAbout();
    void popupAboutQt();

private:
    void initDate();

    void createLayouts();
    void createActions();
    void createMenus();

    void saveSettings();
    void retrieveSettings();

    CalendarWidget* calendarWidget;
    QWidget* mainWidget;

    QDateEdit* dateEdit;

    QToolButton* leftToolButton;
    QToolButton* rightToolButton;

    QVBoxLayout* midLayout;
    QHBoxLayout* mainLayout;

    QMenu* fileMenu;
    QMenu* editMenu;
    QMenu* helpMenu;

    QAction* importAction;
    QAction* exportAction;
    QAction* exitAction;

    QAction* clearMonthEventsAction;
    QAction* clearAllEventsAction;

    QAction* aboutAction;
    QAction* aboutQtAction;
};

#endif // MAINWINDOW_H
