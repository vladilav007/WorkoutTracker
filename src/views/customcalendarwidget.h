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

    qreal selectionOpacity() const { return m_selectionOpacity; }
    void setSelectionOpacity(qreal opacity);

    void setWorkoutData(const QDate &date, const QString &name, 
                    const QString &description,
                    const QVector<Exercise> &exercises);

    bool getWorkoutData(const QDate &date, QString &name, 
                   QString &description,
                   QVector<Exercise> &exercises) const;

signals:

protected:
    void paintCell(QPainter *painter, const QRect &rect, QDate date) const override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QMap<QDate, WorkoutStatus> dayStatusMap;
    QMap<QDate, bool> workoutMap;
    QPropertyAnimation *selectionAnimation;
    qreal m_selectionOpacity;

    struct WorkoutInfo {
        QString name;
        QString description;
        QVector<Exercise> exercises;
    };
    QMap<QDate, WorkoutInfo> workoutData;
    
    QColor getStatusColor(WorkoutStatus status) const;
    void createContextMenu(const QDate &date, const QPoint &pos);
    void startSelectionAnimation();
};

#endif // CUSTOMCALENDARWIDGET_H