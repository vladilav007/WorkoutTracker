// weekviewcell.h
#ifndef WEEKVIEWCELL_H
#define WEEKVIEWCELL_H

#include <QWidget>
#include <QDate>
#include "customcalendarwidget.h"
#include "../models/types.h"

class WeekViewCell : public QWidget {
    Q_OBJECT
public:
    explicit WeekViewCell(const QDate& date, QWidget* parent = nullptr);
    
    void setWorkoutData(const QString& name, 
                       const QString& description,
                       const QVector<Exercise>& exercises,
                       CustomCalendarWidget::WorkoutStatus status);
    void clear();
    
    // Getters
    QString workoutName() const { return m_workoutName; }
    QString workoutDescription() const { return m_workoutDescription; }
    QVector<Exercise> workoutExercises() const { return m_exercises; }
    CustomCalendarWidget::WorkoutStatus workoutStatus() const { return m_status; }

signals:
    void clicked(const QDate& date);
    void contextMenuRequested(const QDate& date, const QPoint& globalPos);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;

private:
    QDate m_date;
    QString m_workoutName;
    QString m_workoutDescription;
    QVector<Exercise> m_exercises;
    CustomCalendarWidget::WorkoutStatus m_status;
    QColor getStatusColor() const;
};

#endif // WEEKVIEWCELL_H