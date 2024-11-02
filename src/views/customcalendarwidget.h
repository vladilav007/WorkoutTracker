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
        NoWorkout,    // White/default - future scheduled workout
        Completed,    // Green - workout completed
        Missed,       // Red - workout missed
        RestDay       // Grey - rest day
    };

    explicit CustomCalendarWidget(QWidget *parent = nullptr);

    // Methods to manage day status
    void setDayStatus(const QDate &date, WorkoutStatus status);
    WorkoutStatus getDayStatus(const QDate &date) const;

protected:
    void paintCell(QPainter *painter, const QRect &rect, QDate date) const override;

private:
    // Map to store status for each date
    QMap<QDate, WorkoutStatus> dayStatusMap;
    
    // Helper method
    QColor getStatusColor(WorkoutStatus status) const;
};

#endif // CUSTOMCALENDARWIDGET_H