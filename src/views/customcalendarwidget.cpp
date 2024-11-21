// customcalendarwidget.cpp
#include "customcalendarwidget.h"
#include <QPainter>
#include <QTextCharFormat>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QMouseEvent>
#include <QDebug>

CustomCalendarWidget::CustomCalendarWidget(QWidget *parent)
    : QCalendarWidget(parent)
    , m_selectionOpacity(0.0)
{
    setGridVisible(true);
    setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    setNavigationBarVisible(true);
    
    // Setup selection animation
    selectionAnimation = new QPropertyAnimation(this, "selectionOpacity", this);
    selectionAnimation->setDuration(300);
    selectionAnimation->setStartValue(0.0);
    selectionAnimation->setEndValue(1.0);
    
    // Setup dark theme
    setStyleSheet(
        "QCalendarWidget QWidget { background-color: #2b2b2b; }"
        "QCalendarWidget QAbstractItemView:enabled { color: white; }"
        "QCalendarWidget QAbstractItemView:disabled { color: grey; }"
        "QCalendarWidget QMenu { color: white; }"
        "QCalendarWidget QSpinBox { color: white; }"
        "QCalendarWidget QToolButton { color: white; }"
        "QCalendarWidget QToolButton:hover { background-color: #404040; }"
    );
}

void CustomCalendarWidget::setDayStatus(const QDate &date, WorkoutStatus status)
{
    dayStatusMap[date] = status;
    workoutMap[date] = true;
    updateCell(date);
}

CustomCalendarWidget::WorkoutStatus CustomCalendarWidget::getDayStatus(const QDate &date) const
{
    return dayStatusMap.value(date, NoWorkout);
}

bool CustomCalendarWidget::hasWorkout(const QDate &date) const
{
    return workoutMap.value(date, false);
}

QColor CustomCalendarWidget::getStatusColor(WorkoutStatus status) const
{
    switch (status) {
        case Completed:
            return QColor(76, 175, 80);    // Material Design Green
        case Missed:
            return QColor(244, 67, 54);    // Material Design Red
        case RestDay:
            return QColor(158, 158, 158);  // Material Design Grey
        default:
            return QColor(45, 45, 45);     // Dark background for default state
    }
}

void CustomCalendarWidget::setSelectionOpacity(qreal opacity)
{
    m_selectionOpacity = opacity;
    update();
}

void CustomCalendarWidget::mousePressEvent(QMouseEvent *event)
{
    QCalendarWidget::mousePressEvent(event);
    
    // Обрабатываем правую кнопку мыши для контекстного меню
    if (event->button() == Qt::RightButton) {
        QDate date = selectedDate();
        QPoint globalPos = event->globalPosition().toPoint();
        createContextMenu(date, globalPos);
    }
}

void CustomCalendarWidget::startSelectionAnimation()
{
    selectionAnimation->stop();
    selectionAnimation->start();
}

void CustomCalendarWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QDate date = selectedDate();
    createContextMenu(date, event->globalPos());
}

void CustomCalendarWidget::createContextMenu(const QDate &date, const QPoint &pos)
{
    QMenu menu(this);
    
    QAction *completedAction = menu.addAction(tr("Mark as Completed"));
    QAction *missedAction = menu.addAction(tr("Mark as Missed"));
    QAction *plannedAction = menu.addAction(tr("Mark as Planned"));
    QAction *restAction = menu.addAction(tr("Mark as Rest Day"));
    
    connect(completedAction, &QAction::triggered, this, [this, date]() {
        setDayStatus(date, Completed);
    });
    
    connect(missedAction, &QAction::triggered, this, [this, date]() {
        setDayStatus(date, Missed);
    });
    
    connect(plannedAction, &QAction::triggered, this, [this, date]() {
        setDayStatus(date, NoWorkout);
    });
    
    connect(restAction, &QAction::triggered, this, [this, date]() {
        setDayStatus(date, RestDay);
    });
    
    menu.exec(pos);
}

void CustomCalendarWidget::paintCell(QPainter *painter, const QRect &rect, QDate date) const
{
    painter->save();

    WorkoutStatus status = getDayStatus(date);
    QColor bgColor = getStatusColor(status);
    
    if (date == selectedDate()) {
        bgColor = bgColor.lighter(120);
        painter->setPen(QPen(Qt::white, 2));
    } else {
        painter->setPen(Qt::gray);
    }
    
    painter->fillRect(rect, bgColor);
    painter->drawRect(rect);
    
    bool isWeekend = date.dayOfWeek() > 5;
    QColor textColor = (status == NoWorkout) 
        ? (isWeekend ? QColor(244, 67, 54) : QColor(255, 255, 255))
        : (status == Missed ? QColor(255, 255, 255) : QColor(0, 0, 0));
    
    painter->setPen(textColor);
    
    QFont font = painter->font();
    font.setPointSize(10);
    painter->setFont(font);
    painter->drawText(rect, Qt::AlignCenter, QString::number(date.day()));
    
    if (hasWorkout(date)) {
        int indicatorSize = 4;
        int margin = 2;
        QRect indicatorRect(
            rect.right() - indicatorSize - margin,
            rect.top() + margin,
            indicatorSize,
            indicatorSize
        );
        painter->fillRect(indicatorRect, textColor);
    }
    
    painter->restore();
}

void CustomCalendarWidget::setWorkoutData(const QDate &date, 
                                        const QString &name,
                                        const QString &description,
                                        const QVector<Exercise> &exercises)
{
    WorkoutInfo info;
    info.name = name;
    info.description = description;
    info.exercises = exercises;
    workoutData[date] = info;
    workoutMap[date] = true;
    updateCell(date);
}

bool CustomCalendarWidget::getWorkoutData(const QDate &date, 
                                        QString &name, 
                                        QString &description,
                                        QVector<Exercise> &exercises) const
{
    if (workoutData.contains(date)) {
        const WorkoutInfo &info = workoutData[date];
        name = info.name;
        description = info.description;
        exercises = info.exercises;
        return true;
    }
    return false;
}

void CustomCalendarWidget::loadSavedData()
{
    StorageManager& storage = StorageManager::instance();
    QVector<QDate> dates = storage.getAllWorkoutDates();
    
    for (const QDate& date : dates) {
        QString name, description;
        QVector<Exercise> exercises;
        CustomCalendarWidget::WorkoutStatus status;
        
        if (storage.loadWorkout(date, name, description, exercises, status)) {
            // Set workout indicator
            workoutMap[date] = true;
            
            // Set status
            dayStatusMap[date] = status;
            
            // Set workout data
            WorkoutInfo info;
            info.name = name;
            info.description = description;
            info.exercises = exercises;
            workoutData[date] = info;
            
            updateCell(date);
        }
    }
    
    // Force complete repaint
    update();
}
