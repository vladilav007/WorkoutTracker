#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>
#include "customcalendarwidget.h"  // Добавляем включение заголовочного файла

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
    CustomCalendarWidget *calendar;  // Меняем тип с QCalendarWidget на CustomCalendarWidget
    QToolBar *toolBar;

    // Actions
    QAction *newWorkoutAction;
    QAction *editWorkoutAction;
    QAction *monthViewAction;
    QAction *weekViewAction;
};

#endif // MAINWINDOW_H