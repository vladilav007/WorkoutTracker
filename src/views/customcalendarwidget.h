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
    // Workout status enumeration for color coding
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
    // Override paintCell to implement custom cell rendering
    void paintCell(QPainter *painter, const QRect &rect, QDate date) const override;

private:
    // Map to store status for each date
    QMap<QDate, WorkoutStatus> dayStatusMap;
    
    // Helper method to get color based on status
    QColor getStatusColor(WorkoutStatus status) const;
};

#endif // CUSTOMCALENDARWIDGET_H