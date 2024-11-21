// customcalendarwidget.h
#ifndef CUSTOMCALENDARWIDGET_H
#define CUSTOMCALENDARWIDGET_H

#include <QCalendarWidget>
#include <QColor>
#include <QMap>
#include <QDate>
#include <QMenu>
#include <QPropertyAnimation>
#include "../models/types.h"
#include "../models/storage_manager.h"

class CustomCalendarWidget : public QCalendarWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal selectionOpacity READ selectionOpacity WRITE setSelectionOpacity)

public:
    enum WorkoutStatus {
        NoWorkout,
        Completed,
        Missed,
        RestDay
    };

    explicit CustomCalendarWidget(QWidget *parent = nullptr);

    void setDayStatus(const QDate &date, WorkoutStatus status);
    WorkoutStatus getDayStatus(const QDate &date) const;
    bool hasWorkout(const QDate &date) const;
    void setWorkoutData(const QDate &date, const QString &name, const QString &description);
    void setWorkoutData(const QDate &date, const QString &name, 
                       const QString &description,
                       const QVector<Exercise> &exercises);
    bool getWorkoutData(const QDate &date, QString &name, 
                       QString &description,
                       QVector<Exercise> &exercises) const;
    
    void loadSavedData();
    
    // Add animation related methods
    qreal selectionOpacity() const { return m_selectionOpacity; }
    void setSelectionOpacity(qreal opacity);
    void startSelectionAnimation();

protected:
    void paintCell(QPainter *painter, const QRect &rect, QDate date) const override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QMap<QDate, WorkoutStatus> dayStatusMap;
    QMap<QDate, bool> workoutMap;
    qreal m_selectionOpacity;
    QPropertyAnimation* selectionAnimation;

    struct WorkoutInfo {
        QString name;
        QString description;
        QVector<Exercise> exercises;
    };
    QMap<QDate, WorkoutInfo> workoutData;
    
    QColor getStatusColor(WorkoutStatus status) const;
    void createContextMenu(const QDate &date, const QPoint &pos);
};

#endif // CUSTOMCALENDARWIDGET_H