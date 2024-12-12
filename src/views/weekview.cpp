// weekview.cpp
#include "weekview.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include "../models/workout_status.h"

WeekView::WeekView(QWidget* parent)
    : QWidget(parent)
    , m_currentDate(QDate::currentDate())
    , m_selectedDate(QDate::currentDate())  // Initialize selected date
{
    m_gridLayout = new QGridLayout(this);
    m_gridLayout->setSpacing(1);
    m_gridLayout->setContentsMargins(1, 1, 1, 1);

    setupNavigation();
    createHeaderLabels();
    createWeekCells();
    
    // Connect signals for cells
    for (auto cell : m_cells) {
        if (cell) {  // Add null check
            connect(cell, &WeekViewCell::clicked,
                    this, &WeekView::handleCellClicked);
            connect(cell, &WeekViewCell::contextMenuRequested,
                    this, &WeekView::handleCellContextMenu);
        }
    }
}

void WeekView::createHeaderLabels()
{
    QStringList dayNames = {"Monday", "Tuesday", "Wednesday", 
                           "Thursday", "Friday", "Saturday", "Sunday"};
    
    for (int i = 0; i < dayNames.size(); ++i) {
        QLabel* label = new QLabel(dayNames[i], this);
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet("font-weight: bold; padding: 5px; color: white;");
        m_gridLayout->addWidget(label, 0, i);
    }
}

void WeekView::createWeekCells()
{
    // Clear existing cells
    qDeleteAll(m_cells);
    m_cells.clear();
    
    QDate weekStart = getWeekStart(m_currentDate);
    
    for (int i = 0; i < 7; ++i) {
        QDate cellDate = weekStart.addDays(i);
        WeekViewCell* cell = new WeekViewCell(cellDate, this);
        
        connect(cell, &WeekViewCell::clicked,
                this, &WeekView::handleCellClicked);
        connect(cell, &WeekViewCell::contextMenuRequested,
                this, &WeekView::handleCellContextMenu);
        
        m_gridLayout->addWidget(cell, 1, i);
        m_cells[cellDate] = cell;
    }
    
    loadWorkoutData();
}

void WeekView::loadWorkoutData()
{
    StorageManager& storage = StorageManager::instance();
    
    for (auto it = m_cells.begin(); it != m_cells.end(); ++it) {
        const QDate& date = it.key();
        WeekViewCell* cell = it.value();
        
        QString name, description;
        QVector<Exercise> exercises;
        WorkoutStatus status;
        
        if (storage.loadWorkout(date, name, description, exercises, status)) {
            cell->setWorkoutData(name, description, exercises, status);
        } else {
            cell->setWorkoutData("", "", 
                               QVector<Exercise>(), 
                               WorkoutStatus::NoWorkout);
        }
        cell->update();
    }
}

QDate WeekView::getWeekStart(const QDate& date) const
{
    int daysToMonday = date.dayOfWeek() - 1;
    return date.addDays(-daysToMonday);
}

void WeekView::setCurrentDate(const QDate& date)
{
    if (m_currentDate != date) {
        m_currentDate = date;
        updateView();
    }
}

void WeekView::updateView()
{
    createWeekCells();
}

void WeekView::handleCellClicked(const QDate& date)
{   
    if (!date.isValid()) {
        return;
    }

    if (auto oldCell = m_cells.value(m_selectedDate)) {
        oldCell->setSelected(false);
    }

    m_selectedDate = date;
    
    if (auto newCell = m_cells.value(m_selectedDate)) {
        newCell->setSelected(true);
    }

    emit dayClicked(date);
}

void WeekView::updateCell(const QDate& date)
{
    if (WeekViewCell* cell = m_cells.value(date)) {
        QString name, description;
        QVector<Exercise> exercises;
        WorkoutStatus status;
        
        StorageManager::instance().loadWorkout(date, name, description, exercises, status);
        cell->setWorkoutData(name, description, exercises, status);
        cell->update();
    }
}

bool WeekView::hasWorkout(const QDate& date) const
{
    if (auto it = m_cells.find(date); it != m_cells.end()) {
        return !it.value()->workoutName().isEmpty();
    }
    return false;
}

void WeekView::updateCellStatus(const QDate& date, WorkoutStatus status)
{
    if (auto cell = m_cells.value(date)) {
        QString name = cell->workoutName();
        QString description = cell->workoutDescription();
        QVector<Exercise> exercises = cell->workoutExercises();
        
        // Сначала обновляем в хранилище
        StorageManager::instance().saveWorkout(date, name, description, exercises, status);
        StorageManager::instance().saveToFile();  // Явно сохраняем
        
        // Затем обновляем ячейку
        cell->setWorkoutData(name, description, exercises, status);
        cell->update();
        
        // Испускаем сигнал для синхронизации
        emit statusChanged(date, status);
    }
}

void WeekView::handleCellContextMenu(const QDate& date, const QPoint& globalPos)
{
    QMenu menu(this);
    
    QAction* completedAction = menu.addAction(tr("Mark as Completed"));
    QAction* missedAction = menu.addAction(tr("Mark as Missed"));
    QAction* plannedAction = menu.addAction(tr("Mark as Planned"));
    QAction* restAction = menu.addAction(tr("Mark as Rest Day"));
    
    connect(completedAction, &QAction::triggered, this, [this, date]() {
        updateCellStatus(date, WorkoutStatus::Completed);
    });
    
    connect(missedAction, &QAction::triggered, this, [this, date]() {
        updateCellStatus(date, WorkoutStatus::Missed);
    });
    
    connect(plannedAction, &QAction::triggered, this, [this, date]() {
        updateCellStatus(date, WorkoutStatus::NoWorkout);
    });
    
    connect(restAction, &QAction::triggered, this, [this, date]() {
        updateCellStatus(date, WorkoutStatus::RestDay);
    });
    
    menu.exec(globalPos);
}

void WeekView::setupNavigation()
{
    auto navigationLayout = new QHBoxLayout;
    
    prevWeekButton = new QPushButton("<", this);
    nextWeekButton = new QPushButton(">", this);
    weekLabel = new QLabel(this);
    weekLabel->setAlignment(Qt::AlignCenter);
    
    navigationLayout->addWidget(prevWeekButton);
    navigationLayout->addWidget(weekLabel);
    navigationLayout->addWidget(nextWeekButton);
    
    connect(prevWeekButton, &QPushButton::clicked, this, &WeekView::prevWeek);
    connect(nextWeekButton, &QPushButton::clicked, this, &WeekView::nextWeek);
    
    // Добавляем навигацию в основной layout перед сеткой
    m_gridLayout->addLayout(navigationLayout, 0, 0, 1, 7);
    
    // Сдвигаем заголовки дней недели
    createHeaderLabels(); // Перемещаем заголовки на строку ниже
    
    updateWeekLabel();
}

void WeekView::prevWeek()
{
    setCurrentDate(m_currentDate.addDays(-7));
}

void WeekView::nextWeek()
{
    setCurrentDate(m_currentDate.addDays(7));
}

void WeekView::updateWeekLabel()
{
    QDate weekStart = getWeekStart(m_currentDate);
    QDate weekEnd = weekStart.addDays(6);
    weekLabel->setText(tr("Week: %1 - %2")
                      .arg(weekStart.toString("dd.MM"))
                      .arg(weekEnd.toString("dd.MM.yyyy")));
}

void WeekView::copyWorkout(const QDate& date)
{
    QString name, description;
    QVector<Exercise> exercises;
    WorkoutStatus status;
    
    if (StorageManager::instance().loadWorkout(date, name, description, exercises, status)) {
        copiedWorkout.name = name;
        copiedWorkout.description = description;
        copiedWorkout.exercises = exercises;
    }
}

void WeekView::pasteWorkout(const QDate& date)
{
    if (!copiedWorkout.isNull()) {
        StorageManager::instance().saveWorkout(
            date, 
            copiedWorkout.name,
            copiedWorkout.description,
            copiedWorkout.exercises,
            WorkoutStatus::NoWorkout
        );
        
        updateCell(date);
        emit workoutModified(date);
    }
}

void WeekView::setSelectedDate(const QDate& date)
{
    if (auto oldCell = m_cells.value(m_selectedDate)) {
        oldCell->setSelected(false);
    }
    
    m_selectedDate = date;
    
    if (auto newCell = m_cells.value(m_selectedDate)) {
        newCell->setSelected(true);
    }
    
    update();
}

QDate WeekView::selectedDate() const
{
    return m_selectedDate;
}

WeekViewCell* WeekView::getCell(const QDate& date)
{
    return m_cells.value(date);
}
