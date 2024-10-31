#include "customcalendarwidget.h"
#include <QPainter>
#include <QTextCharFormat>

CustomCalendarWidget::CustomCalendarWidget(QWidget *parent)
    : QCalendarWidget(parent)
{
    // Настройка внешнего вида календаря
    setGridVisible(true);
    setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    setNavigationBarVisible(true);
}

void CustomCalendarWidget::setDayStatus(const QDate &date, WorkoutStatus status)
{
    dayStatusMap[date] = status;
    updateCell(date); // Обновляем отображение ячейки
}

CustomCalendarWidget::WorkoutStatus CustomCalendarWidget::getDayStatus(const QDate &date) const
{
    return dayStatusMap.value(date, NoWorkout);
}

QColor CustomCalendarWidget::getStatusColor(WorkoutStatus status) const
{
    switch (status) {
        case Completed:
            return QColor(144, 238, 144); // Светло-зеленый
        case Missed:
            return QColor(255, 182, 193); // Светло-красный
        case RestDay:
            return QColor(220, 220, 220); // Серый
        default:
            return QColor(255, 255, 255); // Белый
    }
}

void CustomCalendarWidget::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
{
    // Заполняем фон ячейки в зависимости от статуса
    WorkoutStatus status = getDayStatus(date);
    painter->fillRect(rect, getStatusColor(status));

    // Отрисовываем рамку ячейки
    painter->setPen(Qt::gray);
    painter->drawRect(rect);

    // Отрисовываем число
    painter->setPen(Qt::black);
    painter->drawText(rect, Qt::AlignCenter, QString::number(date.day()));
}