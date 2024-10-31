#include "customcalendarwidget.h"
#include <QPainter>
#include <QTextCharFormat>

CustomCalendarWidget::CustomCalendarWidget(QWidget *parent)
    : QCalendarWidget(parent)
{
    // Setup calendar appearance
    setGridVisible(true);
    setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    setNavigationBarVisible(true);
    
    // Set dark theme for calendar
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
    updateCell(date);
}

CustomCalendarWidget::WorkoutStatus CustomCalendarWidget::getDayStatus(const QDate &date) const
{
    return dayStatusMap.value(date, NoWorkout);
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
            return QColor(66, 66, 66);     // Dark background for future workouts
    }
}

void CustomCalendarWidget::paintCell(QPainter *painter, const QRect &rect, QDate date) const
{
    // Save current painter settings
    painter->save();

    // Fill cell background based on status
    WorkoutStatus status = getDayStatus(date);
    QColor bgColor = getStatusColor(status);
    painter->fillRect(rect, bgColor);
    
    // Configure text color
    bool isWeekend = date.dayOfWeek() > 5;
    QColor textColor;
    
    if (status == NoWorkout) {
        // Red for weekends, white for workdays if no status
        textColor = isWeekend ? QColor(244, 67, 54) : QColor(255, 255, 255);
    } else {
        // Use contrasting colors for days with status
        textColor = (status == Missed) ? QColor(255, 255, 255) : QColor(0, 0, 0);
    }
    
    painter->setPen(textColor);
    
    // Draw day number
    QFont font = painter->font();
    font.setPointSize(10);
    painter->setFont(font);
    painter->drawText(rect, Qt::AlignCenter, QString::number(date.day()));
    
    // Draw status indicator (if status is not NoWorkout)
    if (status != NoWorkout) {
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
    
    // Restore painter settings
    painter->restore();
}