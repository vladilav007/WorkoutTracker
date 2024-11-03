#ifndef CUSTOMCALENDARWIDGET_H
#define CUSTOMCALENDARWIDGET_H

#include <QCalendarWidget>
#include <QColor>
#include <QMap>
#include <QDate>
#include <QMenu>
#include <QPropertyAnimation>

class CustomCalendarWidget : public QCalendarWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal selectionOpacity READ selectionOpacity WRITE setSelectionOpacity)

public:
    enum WorkoutStatus {
        NoWorkout,    // Default state for future workouts
        Completed,    // Successfully completed workout
        Missed,       // Missed workout
        RestDay       // Rest day
    };

    explicit CustomCalendarWidget(QWidget *parent = nullptr);

    // Public interface for status management
    void setDayStatus(const QDate &date, WorkoutStatus status);
    WorkoutStatus getDayStatus(const QDate &date) const;
    bool hasWorkout(const QDate &date) const;

    // Animation properties
    qreal selectionOpacity() const { return m_selectionOpacity; }
    void setSelectionOpacity(qreal opacity);

protected:
    void paintCell(QPainter *painter, const QRect &rect, QDate date) const override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    // Storage for day states
    QMap<QDate, WorkoutStatus> dayStatusMap;
    QMap<QDate, bool> workoutMap;
    
    // Animation related members
    QPropertyAnimation *selectionAnimation;
    qreal m_selectionOpacity;
    
    // Helper methods
    QColor getStatusColor(WorkoutStatus status) const;
    void createContextMenu(const QDate &date, const QPoint &pos);
    void startSelectionAnimation();
};

#endif // CUSTOMCALENDARWIDGET_H