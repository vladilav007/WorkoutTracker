#ifndef CUSTOMCALENDARWIDGET_H
#define CUSTOMCALENDARWIDGET_H

#include <QCalendarWidget>
#include <QColor>
#include <QMap>
#include <QDate>

class CustomCalendarWidget : public QCalendarWidget
{
    Q_OBJECT

public:
    enum WorkoutStatus {
        NoWorkout,      // Белый/стандартный - будущая запланированная тренировка
        Completed,      // Зеленый - тренировка выполнена
        Missed,         // Красный - тренировка пропущена
        RestDay         // Серый - день отдыха
    };

    explicit CustomCalendarWidget(QWidget *parent = nullptr);

    void setDayStatus(const QDate &date, WorkoutStatus status);
    WorkoutStatus getDayStatus(const QDate &date) const;

protected:
    void paintCell(QPainter *painter, const QRect &rect, const QDate &date) const override;

private:
    QMap<QDate, WorkoutStatus> dayStatusMap;
    QColor getStatusColor(WorkoutStatus status) const;
};

#endif // CUSTOMCALENDARWIDGET_H