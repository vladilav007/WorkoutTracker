// weekview.cpp
#include "weekview.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

WeekView::WeekView(QWidget* parent)
    : QWidget(parent)
    , m_currentDate(QDate::currentDate())
{
    m_gridLayout = new QGridLayout(this);
    m_gridLayout->setSpacing(1);
    m_gridLayout->setContentsMargins(1, 1, 1, 1);

    createHeaderLabels();
    createWeekCells();
    
    // Connect signals for cells
    for (auto cell : m_cells) {
        connect(cell, &WeekViewCell::clicked,
                this, &WeekView::handleCellClicked);
        connect(cell, &WeekViewCell::contextMenuRequested,
                this, &WeekView::handleCellContextMenu);
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
        CustomCalendarWidget::WorkoutStatus status;
        
        if (storage.loadWorkout(date, name, description, exercises, status)) {
            cell->setWorkoutData(name, description, exercises, status);
        } else {
            cell->setWorkoutData("", "", QVector<Exercise>(), CustomCalendarWidget::NoWorkout);
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
    emit dayClicked(date);
}

void WeekView::updateCell(const QDate& date)
{
    if (auto it = m_cells.find(date); it != m_cells.end()) {
        QString name, description;
        QVector<Exercise> exercises;
        CustomCalendarWidget::WorkoutStatus status;
        
        if (StorageManager::instance().loadWorkout(date, name, 
                                                 description,
                                                 exercises,
                                                 status)) {
            it.value()->setWorkoutData(name, description, exercises, status);
        } else {
            it.value()->setWorkoutData("", "", QVector<Exercise>(), 
                                     CustomCalendarWidget::NoWorkout);
        }
        it.value()->update();
    }
}

bool WeekView::hasWorkout(const QDate& date) const
{
    if (auto it = m_cells.find(date); it != m_cells.end()) {
        return !it.value()->workoutName().isEmpty();
    }
    return false;
}

void WeekView::updateCellStatus(const QDate& date, CustomCalendarWidget::WorkoutStatus status)
{
    if (auto cell = m_cells.value(date)) {
        // Update storage first
        QString name = cell->workoutName();
        QString description = cell->workoutDescription();
        QVector<Exercise> exercises = cell->workoutExercises();
        
        StorageManager::instance().saveWorkout(date, name, description, exercises, status);
        
        // Then update the cell
        cell->setWorkoutData(name, description, exercises, status);
        cell->update();
    }
}

void WeekView::handleCellContextMenu(const QDate& date, const QPoint& globalPos)
{
    QMenu menu(this);
    
    QAction* completedAction = menu.addAction(tr("Mark as Completed"));
    QAction* missedAction = menu.addAction(tr("Mark as Missed"));
    QAction* plannedAction = menu.addAction(tr("Mark as Planned"));
    QAction* restAction = menu.addAction(tr("Mark as Rest Day"));
    
    connect(completedAction, &QAction::triggered, [this, date]() {
        updateCellStatus(date, CustomCalendarWidget::Completed);
    });
    
    connect(missedAction, &QAction::triggered, [this, date]() {
        updateCellStatus(date, CustomCalendarWidget::Missed);
    });
    
    connect(plannedAction, &QAction::triggered, [this, date]() {
        updateCellStatus(date, CustomCalendarWidget::NoWorkout);
    });
    
    connect(restAction, &QAction::triggered, [this, date]() {
        updateCellStatus(date, CustomCalendarWidget::RestDay);
    });
    
    menu.exec(globalPos);
}