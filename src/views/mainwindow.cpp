#include "mainwindow.h"
#include "customcalendarwidget.h"
#include "../models/storage_manager.h"
#include <QStyle>
#include <QApplication>
#include <QDate>
#include <QVBoxLayout>
#include <Qt>
#include <QWidget>
#include <QPainter> 
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , isMonthViewActive(true)
{
    setupUI();
    createActions();
    createToolBar();
    
    qDebug() << "Loading saved workouts...";
    
    // Load data from storage
    if (!StorageManager::instance().loadFromFile()) {
        qWarning() << "Failed to load workout data!";
    }
    
    // Load data into views
    loadWorkoutData();
    
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
    
    statusLabel = new QLabel(this);
    statusLabel->setStyleSheet("QLabel { color: white; padding: 5px; }");
    mainLayout->addWidget(statusLabel);
    
    calendar = new CustomCalendarWidget(this);
    mainLayout->addWidget(calendar);
    
    setupWeekView();
    
    connect(calendar, &QCalendarWidget::clicked,
            this, &MainWindow::handleDayClicked);
            
    updateViewVisibility();
            
    qDebug() << "UI setup completed, signals connected";
}

void MainWindow::setupWeekView()
{
    weekView = new WeekView(this);
    mainLayout->addWidget(weekView);
    weekView->hide(); // Initially hidden
    
    // Connect only single click signal
    connect(weekView, &WeekView::dayClicked,
            this, &MainWindow::handleDayClicked);
}

void MainWindow::createActions()
{
    // Create New Workout action
    newWorkoutAction = new QAction(tr("New Workout"), this);
    QIcon plusIcon = QIcon::fromTheme("list-add");
    if (plusIcon.isNull()) {
        QPixmap pixmap(24, 24);
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.setPen(QPen(QColor("#4CAF50"), 2));
        painter.drawLine(12, 6, 12, 18);
        painter.drawLine(6, 12, 18, 12);
        plusIcon = QIcon(pixmap);
    }
    newWorkoutAction->setIcon(plusIcon);
    connect(newWorkoutAction, &QAction::triggered, this, &MainWindow::createNewWorkout);

    // Create View/Edit Workout action
    editWorkoutAction = new QAction(tr("View/Edit Workout"), this);
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
    QDate selectedDate = isMonthViewActive ? calendar->selectedDate() 
                                         : weekView->currentDate();
    showWorkoutDialog(selectedDate, false);
}

void MainWindow::editWorkout()
{
    QDate selectedDate = calendar->selectedDate();
    if (calendar->hasWorkout(selectedDate)) {
        qDebug() << "Opening workout for viewing/editing:" << selectedDate;
        showWorkoutDialog(selectedDate, true);
    } else {
        QMessageBox::information(this, tr("No Workout"),
            tr("No workout found for selected date. Use '+' button to create a new one."));
    }
}

void MainWindow::loadWorkoutData()
{
    QVector<QDate> dates = StorageManager::instance().getAllWorkoutDates();
    qDebug() << "Found" << dates.size() << "saved workouts";
    
    // Load data for calendar view
    calendar->loadSavedData();
    
    // Load data for week view if it exists
    if (weekView) {
        weekView->setCurrentDate(weekView->currentDate());
    }
}

void MainWindow::reloadWorkoutData()
{
    StorageManager::instance().loadFromFile();
    loadWorkoutData();
}

void MainWindow::switchToMonthView()
{
    isMonthViewActive = true;
    updateViewVisibility();
    loadWorkoutData(); // Reload data when switching views
}

void MainWindow::switchToWeekView()
{
    isMonthViewActive = false;
    updateViewVisibility();
    loadWorkoutData();  // Reload data when switching views
}

void MainWindow::updateViewVisibility()
{
    calendar->setVisible(isMonthViewActive);
    weekView->setVisible(!isMonthViewActive);
    
    if (isMonthViewActive) {
        calendar->loadSavedData();  // Reload calendar data
    } else if (weekView) {
        weekView->setCurrentDate(calendar->selectedDate());
    }
    
    setWindowTitle(tr("Workout Tracker - %1 View")
                  .arg(isMonthViewActive ? "Month" : "Week"));
}

void MainWindow::handleDayClicked(const QDate &date)
{
    QString name, description;
    QVector<Exercise> exercises;
    CustomCalendarWidget::WorkoutStatus status;
    bool hasWorkout = StorageManager::instance().loadWorkout(date, name, description, exercises, status);
    
    if (hasWorkout) {
        // If workout exists, show in read-only mode
        showWorkoutDialog(date, true);
        
        statusLabel->setText(QString("Selected: %1 - Workout: %2 (%3 exercises)")
                           .arg(date.toString("dd.MM.yyyy"))
                           .arg(name)
                           .arg(exercises.size()));
    } else {
        // If no workout exists, open dialog to create new one
        showWorkoutDialog(date, false);
        
        status = isMonthViewActive ? calendar->getDayStatus(date) : 
                                   CustomCalendarWidget::NoWorkout;
                                   
        QString statusStr = "No workout planned";
        statusLabel->setStyleSheet("QLabel { color: white; padding: 5px; }");
        
        statusLabel->setText(QString("Selected: %1 - %2")
                           .arg(date.toString("dd.MM.yyyy"))
                           .arg(statusStr));
    }
}

void MainWindow::showWorkoutDialog(const QDate &date, bool readOnly)
{
    qDebug() << "Showing dialog for date:" << date << "readonly:" << readOnly;
    
    WorkoutDialog* dialog = new WorkoutDialog(date, this);
    
    // Load existing data if available
    QString name, description;
    QVector<Exercise> exercises;
    CustomCalendarWidget::WorkoutStatus status;
    
    bool hasExistingWorkout = StorageManager::instance().loadWorkout(
        date, name, description, exercises, status);
        
    if (hasExistingWorkout) {
        dialog->setWorkoutName(name);
        dialog->setWorkoutDescription(description);
        dialog->setExercises(exercises);
        dialog->setReadOnly(readOnly);
    }
    
    if (dialog->exec() == QDialog::Accepted && !readOnly) {
        name = dialog->getWorkoutName();
        description = dialog->getWorkoutDescription();
        exercises = dialog->getExercises();
        
        // Preserve existing status or use default
        status = hasExistingWorkout ? status : CustomCalendarWidget::NoWorkout;
        
        // Save to storage
        StorageManager::instance().saveWorkout(date, name, description, exercises, status);
        
        // Update both views
        calendar->loadSavedData();  // Reload calendar data
        
        if (!isMonthViewActive && weekView) {
            weekView->setCurrentDate(weekView->currentDate());  // Reload week view
        }
        
        handleDayClicked(date);
    }
    
    delete dialog;
}
