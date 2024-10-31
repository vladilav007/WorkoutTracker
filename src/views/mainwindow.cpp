#include "mainwindow.h"
#include <QStyle>
#include <QApplication>

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
}

void MainWindow::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    mainLayout = new QVBoxLayout(centralWidget);
    
    calendar = new QCalendarWidget(this);
    mainLayout->addWidget(calendar);
    
    // Set calendar properties
    calendar->setGridVisible(true);
    calendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    calendar->setNavigationBarVisible(true);
}

void MainWindow::createActions()
{
    newWorkoutAction = new QAction(tr("New Workout"), this);
    newWorkoutAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon));
    connect(newWorkoutAction, &QAction::triggered, this, &MainWindow::createNewWorkout);

    editWorkoutAction = new QAction(tr("Edit Workout"), this);
    editWorkoutAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    connect(editWorkoutAction, &QAction::triggered, this, &MainWindow::editWorkout);

    monthViewAction = new QAction(tr("Month View"), this);
    monthViewAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogApplyButton));
    connect(monthViewAction, &QAction::triggered, this, &MainWindow::switchToMonthView);

    weekViewAction = new QAction(tr("Week View"), this);
    weekViewAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogHelpButton));
    connect(weekViewAction, &QAction::triggered, this, &MainWindow::switchToWeekView);
}

void MainWindow::createToolBar()
{
    toolBar = addToolBar(tr("Main"));
    toolBar->addAction(newWorkoutAction);
    toolBar->addAction(editWorkoutAction);
    toolBar->addSeparator();
    toolBar->addAction(monthViewAction);
    toolBar->addAction(weekViewAction);
}

// Slots
void MainWindow::createNewWorkout()
{
    // TODO: Implement workout creation
}

void MainWindow::editWorkout()
{
    // TODO: Implement workout editing
}

void MainWindow::switchToMonthView()
{
    // TODO: Implement switch to month view
}

void MainWindow::switchToWeekView()
{
    // TODO: Implement switch to week view
}