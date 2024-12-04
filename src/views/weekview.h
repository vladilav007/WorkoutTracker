#ifndef WEEKVIEW_H
#define WEEKVIEW_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QMap>
#include <QDate>
#include <QMenu>
#include <QPushButton>
#include <QHBoxLayout>
#include "../models/types.h"
#include "../models/workout_status.h"
#include "../models/storage_manager.h"
#include "weekviewcell.h"

class WeekView : public QWidget {
    Q_OBJECT
public:
    explicit WeekView(QWidget* parent = nullptr);
    
    void setCurrentDate(const QDate& date);
    void loadWorkoutData();
    QDate currentDate() const { return m_currentDate; }
    void updateCell(const QDate& date);
    bool hasWorkout(const QDate& date) const;
    WeekViewCell* getCell(const QDate& date);
    void setSelectedDate(const QDate& date);
    QDate selectedDate() const;

signals:
    void dayClicked(const QDate& date);
    void statusChanged(const QDate& date, WorkoutStatus status);
    void workoutModified(const QDate& date);

public slots:
    void nextWeek();
    void prevWeek();

private slots:
    void updateView();
    void handleCellClicked(const QDate& date);
    void handleCellContextMenu(const QDate& date, const QPoint& globalPos);
    void updateCellStatus(const QDate& date, WorkoutStatus status);

private:
    QDate m_currentDate;
    QGridLayout* m_gridLayout;
    QMap<QDate, WeekViewCell*> m_cells;
    QPushButton* prevWeekButton;
    QPushButton* nextWeekButton;
    QLabel* weekLabel;
    QDate m_selectedDate;
    
    void createHeaderLabels();
    void createWeekCells();
    
    QDate getWeekStart(const QDate& date) const;
    void setupNavigation();
    void copyWorkout(const QDate& date);
    void pasteWorkout(const QDate& date);
    void updateWeekLabel();

    struct CopiedWorkoutData {
        QString name;
        QString description;
        QVector<Exercise> exercises;
        bool isNull() const { return name.isEmpty(); }
    };
    CopiedWorkoutData copiedWorkout;
};

#endif // WEEKVIEW_H