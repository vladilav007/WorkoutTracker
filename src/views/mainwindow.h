// src/views/mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QLabel>
#include "../models/types.h"
#include "customcalendarwidget.h"
#include "workoutdialog.h"
#include "weekview.h"

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
    void showWorkoutDialog(const QDate &date, bool readOnly);
    void setupWeekView();
    void updateViewVisibility();
    void loadWorkoutData();    // New method
    void reloadWorkoutData();  // New method

    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    CustomCalendarWidget *calendar;
    WeekView *weekView;
    QToolBar *toolBar;
    QLabel *statusLabel;

    QAction *newWorkoutAction;
    QAction *editWorkoutAction;
    QAction *monthViewAction;
    QAction *weekViewAction;
    
    bool isMonthViewActive;
};

#endif // MAINWINDOW_H