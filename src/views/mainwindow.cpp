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
}

void MainWindow::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    mainLayout = new QVBoxLayout(centralWidget);
    
    // Create status label
    statusLabel = new QLabel(this);
    statusLabel->setStyleSheet("QLabel { color: white; padding: 5px; }");
    mainLayout->addWidget(statusLabel);
    
    calendar = new CustomCalendarWidget(this);
    mainLayout->addWidget(calendar);
    
    // Connect the click signal
    connect(calendar, &QCalendarWidget::clicked,
            this, &MainWindow::handleDayClicked);
    
    // Add test data for demonstration
    QDate today = QDate::currentDate();
    
    // Past workouts
    calendar->setDayStatus(today.addDays(-1), CustomCalendarWidget::Completed);
    calendar->setDayStatus(today.addDays(-2), CustomCalendarWidget::Missed);
    calendar->setDayStatus(today.addDays(-3), CustomCalendarWidget::RestDay);
    
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
    toolBar->addAction(newWorkoutAction);
    toolBar->addAction(editWorkoutAction);
    toolBar->addSeparator();
    toolBar->addAction(monthViewAction);
    toolBar->addAction(weekViewAction);
}

void MainWindow::createNewWorkout()
{
    WorkoutDialog dialog(calendar->selectedDate(), this);
    if (dialog.exec() == QDialog::Accepted) {
        // Just mark that this day has a workout but keep NoWorkout status
        calendar->setDayStatus(calendar->selectedDate(), CustomCalendarWidget::NoWorkout);
        handleDayClicked(calendar->selectedDate());
    }
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

void MainWindow::handleDayClicked(const QDate &date)
{
    CustomCalendarWidget::WorkoutStatus status = calendar->getDayStatus(date);
    QString statusStr;
    
    switch (status) {
        case CustomCalendarWidget::Completed:
            statusStr = "Completed workout";
            statusLabel->setStyleSheet("QLabel { color: #4CAF50; padding: 5px; }");  // Green
            break;
        case CustomCalendarWidget::Missed:
            statusStr = "Missed workout";
            statusLabel->setStyleSheet("QLabel { color: #F44336; padding: 5px; }");  // Red
            break;
        case CustomCalendarWidget::RestDay:
            statusStr = "Rest day";
            statusLabel->setStyleSheet("QLabel { color: #9E9E9E; padding: 5px; }");  // Grey
            break;
        default:
            statusStr = "No workout planned";
            statusLabel->setStyleSheet("QLabel { color: white; padding: 5px; }");
    }
    
    statusLabel->setText(QString("Selected: %1 - %2")
                        .arg(date.toString("dd.MM.yyyy"))
                        .arg(statusStr));
}