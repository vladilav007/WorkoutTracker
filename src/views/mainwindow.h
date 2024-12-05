// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QLabel>
#include <QTimer>
#include "../models/types.h"
#include "../models/workout_status.h"
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
    void handleCalendarStatusChanged(const QDate& date, WorkoutStatus status);

signals:
    void workoutDataLoaded();

private:
    void setupUI();
    void createActions();
    void createToolBar();
    void showWorkoutDialog(const QDate &date, bool readOnly);
    void setupWeekView();
    void updateViewVisibility();
    void loadWorkoutData();
    // Просто удалим reloadWorkoutData(), так как его функционал 
    // уже покрывается методом loadWorkoutData()

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
    bool isUpdating = false;
};

#endif // MAINWINDOW_H