#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCalendarWidget>
#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void createNewWorkout();
    void editWorkout();
    void switchToMonthView();
    void switchToWeekView();

private:
    void setupUI();
    void createActions();
    void createToolBar();

    // UI components
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QCalendarWidget *calendar;
    QToolBar *toolBar;

    // Actions
    QAction *newWorkoutAction;
    QAction *editWorkoutAction;
    QAction *monthViewAction;
    QAction *weekViewAction;
};

#endif // MAINWINDOW_H