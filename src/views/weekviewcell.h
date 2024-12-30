// weekviewcell.h
#ifndef WEEKVIEWCELL_H
#define WEEKVIEWCELL_H

#include <QWidget>
#include <QDate>
#include <QPainterPath>
#include "../models/types.h"
#include "../models/workout_status.h"

class WeekViewCell : public QWidget {
    Q_OBJECT
public:
    explicit WeekViewCell(const QDate& date, QWidget* parent = nullptr);
    
    void setWorkoutData(const QString& name, 
                       const QString& description,
                       const QVector<Exercise>& exercises,
                       WorkoutStatus status);
    void clear();

    void setSelected(bool selected);
    bool isSelected() const { return m_isSelected; }
    
    // Getters
    QString workoutName() const { return m_workoutName; }
    QString workoutDescription() const { return m_workoutDescription; }
    QVector<Exercise> workoutExercises() const { return m_exercises; }
    WorkoutStatus workoutStatus() const { return m_status; }

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
    WorkoutStatus m_status;
    QColor getStatusColor() const;
    bool m_isSelected = false;
};

#endif // WEEKVIEWCELL_H