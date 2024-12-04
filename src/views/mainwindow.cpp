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
    
    // Initial data load and status update
    loadWorkoutData();
    handleDayClicked(QDate::currentDate());
    
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
            
    connect(calendar, &CustomCalendarWidget::statusChanged,
            this, &MainWindow::handleCalendarStatusChanged);
            
    connect(weekView, &WeekView::dayClicked,
            this, &MainWindow::handleDayClicked);
            
    connect(weekView, &WeekView::statusChanged,
            [this](const QDate& date, WorkoutStatus status) {
                if (calendar) {
                    calendar->setDayStatus(date, status);
                }
            });
    
    updateViewVisibility();
    
    calendar->loadSavedData();
    calendar->update();
}

void MainWindow::setupWeekView()
{
    weekView = new WeekView(this);
    mainLayout->addWidget(weekView);
    weekView->hide(); // Initially hidden
    
    // Connect signals
    connect(weekView, &WeekView::dayClicked,
            this, &MainWindow::handleDayClicked);
    connect(weekView, &WeekView::statusChanged,
            [this](const QDate& date, WorkoutStatus status) {
                // Update calendar if it's not visible
                if (isMonthViewActive) {
                    calendar->setDayStatus(date, status);
                }
            });
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
    
    // Load data for both views
    for (const QDate& date : dates) {
        QString name, description;
        QVector<Exercise> exercises;
        WorkoutStatus status;
        
        if (StorageManager::instance().loadWorkout(date, name, description, exercises, status)) {
            // Update calendar
            calendar->setWorkoutData(date, name, description, exercises);
            calendar->setDayStatus(date, status);
            
            // Update week view if it exists
            if (weekView) {
                weekView->updateCell(date);
            }
        }
    }
    
    // Force update
    calendar->update();
    if (weekView) {
        weekView->setCurrentDate(weekView->currentDate());
    }
}

void MainWindow::switchToMonthView()
{
    if (!isMonthViewActive) {
        QDate currentDate = QDate::currentDate();
        if (weekView && weekView->selectedDate().isValid()) {
            currentDate = weekView->selectedDate();
        }
        
        isMonthViewActive = true;
        updateViewVisibility();
        
        if (calendar) {
            calendar->setSelectedDate(currentDate);
            calendar->update();
            handleDayClicked(currentDate);
        }
    }
}

void MainWindow::switchToWeekView()
{
    if (isMonthViewActive) {
        QDate currentDate = QDate::currentDate();
        if (calendar) {
            currentDate = calendar->selectedDate();
        }
        
        isMonthViewActive = false;
        updateViewVisibility();
        
        if (weekView) {
            weekView->setCurrentDate(currentDate);
            weekView->setSelectedDate(currentDate);
            weekView->update();
            handleDayClicked(currentDate);
        }
    }
}


void MainWindow::updateViewVisibility()
{
    QDate currentDate = QDate::currentDate();
    if (isMonthViewActive && weekView) {
        currentDate = weekView->selectedDate();
    } else if (!isMonthViewActive && calendar) {
        currentDate = calendar->selectedDate();
    }
    
    calendar->setVisible(isMonthViewActive);
    weekView->setVisible(!isMonthViewActive);
    
    if (isMonthViewActive) {
        calendar->loadSavedData();
        calendar->setSelectedDate(currentDate);
    } else {
        weekView->loadWorkoutData();
        weekView->setCurrentDate(currentDate);
        weekView->setSelectedDate(currentDate);
    }
    
    handleDayClicked(currentDate);
}

void MainWindow::handleDayClicked(const QDate &date)
{
    QString name, description;
    QVector<Exercise> exercises;
    WorkoutStatus status;
    
    QString statusText = QString("Selected: %1").arg(date.toString("dd.MM.yyyy"));
    
    if (StorageManager::instance().loadWorkout(date, name, description, exercises, status)) {
        statusText += QString(" - Workout: %1 (%2 exercises)").arg(name).arg(exercises.size());
        switch (status) {
            case WorkoutStatus::Completed:
                statusText += " - Completed";
                statusLabel->setStyleSheet("QLabel { color: #4CAF50; padding: 5px; }");
                break;
            case WorkoutStatus::Missed:
                statusText += " - Missed";
                statusLabel->setStyleSheet("QLabel { color: #F44336; padding: 5px; }");
                break;
            case WorkoutStatus::RestDay:
                statusText += " - Rest day";
                statusLabel->setStyleSheet("QLabel { color: #9E9E9E; padding: 5px; }");
                break;
            default:
                statusLabel->setStyleSheet("QLabel { color: white; padding: 5px; }");
        }
    }
    
    statusLabel->setText(statusText);
}

void MainWindow::handleCalendarStatusChanged(const QDate& date, WorkoutStatus status)
{
    // Update WeekView if it exists
    if (weekView) {
        weekView->updateCell(date);
    }
    
    // Get current workout data
    QString name, description;
    QVector<Exercise> exercises;
    WorkoutStatus currentStatus;
    
    bool hasWorkout = StorageManager::instance().loadWorkout(date, name, description, exercises, currentStatus);
    
    // Save with new status
    if (hasWorkout) {
        StorageManager::instance().saveWorkout(date, name, description, exercises, status);
    } else {
        StorageManager::instance().saveWorkout(date, "", "", QVector<Exercise>(), status);
    }
}

void MainWindow::showWorkoutDialog(const QDate &date, bool readOnly)
{
    WorkoutDialog* dialog = new WorkoutDialog(date, this);
    
    QString name, description;
    QVector<Exercise> exercises;
    WorkoutStatus status;
    
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
        
        status = hasExistingWorkout ? status : WorkoutStatus::NoWorkout;
        
        StorageManager::instance().saveWorkout(date, name, description, exercises, status);
        StorageManager::instance().saveToFile();  // Add explicit save
        
        calendar->setWorkoutData(date, name, description, exercises);
        calendar->setDayStatus(date, status);
        
        if (!isMonthViewActive) {
            weekView->updateCell(date);
        }
        
        handleDayClicked(date);
    }
    
    delete dialog;
}
