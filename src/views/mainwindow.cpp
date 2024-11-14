#include "mainwindow.h"
#include "customcalendarwidget.h"
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
    
    statusLabel = new QLabel(this);
    statusLabel->setStyleSheet("QLabel { color: white; padding: 5px; }");
    mainLayout->addWidget(statusLabel);
    
    calendar = new CustomCalendarWidget(this);
    mainLayout->addWidget(calendar);
    
    // Подключаем сигнал только один раз
    connect(calendar, &CustomCalendarWidget::dayDoubleClicked, 
            this, &MainWindow::handleDayDoubleClicked);
    
    connect(calendar, &QCalendarWidget::clicked,
            this, &MainWindow::handleDayClicked);
            
    qDebug() << "Signals connected";
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
    editWorkoutAction = new QAction(tr("View/Edit Workout"), this);  // Изменили текст
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
    showWorkoutDialog(calendar->selectedDate(), false);
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

void MainWindow::switchToMonthView()
{
    // TODO: Implement switch to month view
}

void MainWindow::switchToWeekView()
{
    // TODO: Implement switch to week view
}


void MainWindow::handleDayDoubleClicked(const QDate &date)
{
    qDebug() << "Double click handler called for date:" << date;
    QString name, description;
    
    if (calendar->hasWorkout(date)) {
        if (calendar->getWorkoutData(date, name, description)) {
            WorkoutDialog* dialog = new WorkoutDialog(date, this);
            dialog->setWorkoutName(name);
            dialog->setWorkoutDescription(description);
            dialog->setReadOnly(true);
            dialog->exec();
            delete dialog;
        }
    }
}


void MainWindow::handleDayClicked(const QDate &date)
{
    CustomCalendarWidget::WorkoutStatus status = calendar->getDayStatus(date);
    QString statusStr;
    QString name, description;
    
    if (calendar->hasWorkout(date) && calendar->getWorkoutData(date, name, description)) {
        statusStr = QString("Workout: %1").arg(name);
    } else {
        switch (status) {
            case CustomCalendarWidget::Completed:
                statusStr = "Completed workout";
                statusLabel->setStyleSheet("QLabel { color: #4CAF50; padding: 5px; }");
                break;
            case CustomCalendarWidget::Missed:
                statusStr = "Missed workout";
                statusLabel->setStyleSheet("QLabel { color: #F44336; padding: 5px; }");
                break;
            case CustomCalendarWidget::RestDay:
                statusStr = "Rest day";
                statusLabel->setStyleSheet("QLabel { color: #9E9E9E; padding: 5px; }");
                break;
            default:
                statusStr = "No workout planned";
                statusLabel->setStyleSheet("QLabel { color: white; padding: 5px; }");
        }
    }
    
    statusLabel->setText(QString("Selected: %1 - %2")
                        .arg(date.toString("dd.MM.yyyy"))
                        .arg(statusStr));
}


void MainWindow::showWorkoutDialog(const QDate &date, bool readOnly)
{
    qDebug() << "Showing dialog for date:" << date << "readonly:" << readOnly;
    
    WorkoutDialog* dialog = new WorkoutDialog(date, this);
    
    if (readOnly && calendar->hasWorkout(date)) {
        QString name, description;
        if (calendar->getWorkoutData(date, name, description)) {
            dialog->setWorkoutName(name);
            dialog->setWorkoutDescription(description);
            dialog->setReadOnly(true);
            qDebug() << "Loading workout data:" << name << description;
        }
    }
    
    if (dialog->exec() == QDialog::Accepted && !readOnly) {
        QString name = dialog->getWorkoutName();
        QString description = dialog->getWorkoutDescription();
        qDebug() << "Saving workout data:" << name << description;
        
        calendar->setWorkoutData(date, name, description);
        calendar->setDayStatus(date, CustomCalendarWidget::NoWorkout);
        handleDayClicked(date);
    }
    
    delete dialog;
}
