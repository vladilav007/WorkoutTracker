#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QLabel>
#include "customcalendarwidget.h"
#include "workoutdialog.h" 

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
    void handleDayClicked(const QDate &date);

private:
    void setupUI();
    void createActions();
    void createToolBar();

    // UI components
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    CustomCalendarWidget *calendar;
    QToolBar *toolBar;
    QLabel *statusLabel;

    // Actions
    QAction *newWorkoutAction;
    QAction *editWorkoutAction;
    QAction *monthViewAction;
    QAction *weekViewAction;
};

#endif // MAINWINDOW_H