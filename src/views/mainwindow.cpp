#include "mainwindow.h"
#include "customcalendarwidget.h"
#include <QStyle>
#include <QApplication>
#include <QDate>
#include <QVBoxLayout>
#include <Qt>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    createActions();
    createToolBar();
    
    setWindowTitle(tr("Workout Tracker"));
    resize(800, 600);
}

MainWindow::~MainWindow()
{
    // No need for manual cleanup as Qt's parent-child system handles deletion
}

void MainWindow::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    mainLayout = new QVBoxLayout(centralWidget);
    
    calendar = new CustomCalendarWidget(this);
    mainLayout->addWidget(calendar);
    
    // Add test data for demonstration
    QDate today = QDate::currentDate();
    
    // Past workouts
    calendar->setDayStatus(today.addDays(-1), CustomCalendarWidget::Completed);  // Yesterday - completed
    calendar->setDayStatus(today.addDays(-2), CustomCalendarWidget::Missed);     // Day before - missed
    calendar->setDayStatus(today.addDays(-3), CustomCalendarWidget::RestDay);    // Three days ago - rest
    
    // Set Sundays as rest days
    QDate firstDayOfMonth = today.addDays(-today.day() + 1);
    for (int i = 0; i < 31; ++i) {
        QDate currentDate = firstDayOfMonth.addDays(i);
        if (currentDate.dayOfWeek() == Qt::Sunday) {
            calendar->setDayStatus(currentDate, CustomCalendarWidget::RestDay);
        }
    }
}

void MainWindow::createActions()
{
    // Create New Workout action
    newWorkoutAction = new QAction(tr("New Workout"), this);
    newWorkoutAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon));
    connect(newWorkoutAction, &QAction::triggered, this, &MainWindow::createNewWorkout);

    // Create Edit Workout action
    editWorkoutAction = new QAction(tr("Edit Workout"), this);
    editWorkoutAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    connect(editWorkoutAction, &QAction::triggered, this, &MainWindow::editWorkout);

    // Create Month View action
    monthViewAction = new QAction(tr("Month View"), this);
    monthViewAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogApplyButton));
    connect(monthViewAction, &QAction::triggered, this, &MainWindow::switchToMonthView);

    // Create Week View action
    weekViewAction = new QAction(tr("Week View"), this);
    weekViewAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogHelpButton));
    connect(weekViewAction, &QAction::triggered, this, &MainWindow::switchToWeekView);
}

void MainWindow::createToolBar()
{
    toolBar = addToolBar(tr("Main"));
    
    // Add actions to toolbar
    toolBar->addAction(newWorkoutAction);
    toolBar->addAction(editWorkoutAction);
    toolBar->addSeparator();
    toolBar->addAction(monthViewAction);
    toolBar->addAction(weekViewAction);
}

void MainWindow::createNewWorkout()
{
    // TODO: Implement workout creation
    // Will be implemented when we create the WorkoutDialog class
}

void MainWindow::editWorkout()
{
    // TODO: Implement workout editing
    // Will be implemented when we create the WorkoutDialog class
}

void MainWindow::switchToMonthView()
{
    // TODO: Implement switch to month view
    // Will be implemented when we create the MonthView class
}

void MainWindow::switchToWeekView()
{
    // TODO: Implement switch to week view
    // Will be implemented when we create the WeekView class
}