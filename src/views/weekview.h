// weekview.h
#ifndef WEEKVIEW_H
#define WEEKVIEW_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QMap>
#include <QDate>
#include <QMenu>
#include "weekviewcell.h"
#include "../models/types.h"
#include "../models/storage_manager.h"

class WeekView : public QWidget {
    Q_OBJECT
public:
    explicit WeekView(QWidget* parent = nullptr);
    
    void setCurrentDate(const QDate& date);
    QDate currentDate() const { return m_currentDate; }
    void updateCell(const QDate& date);
    bool hasWorkout(const QDate& date) const;

signals:
    void dayClicked(const QDate& date);

private slots:
    void updateView();
    void handleCellClicked(const QDate& date);
    void handleCellContextMenu(const QDate& date, const QPoint& globalPos);
    void updateCellStatus(const QDate& date, CustomCalendarWidget::WorkoutStatus status);  // Added this

private:
    QDate m_currentDate;
    QGridLayout* m_gridLayout;
    QMap<QDate, WeekViewCell*> m_cells;
    
    void createHeaderLabels();
    void createWeekCells();
    void loadWorkoutData();
    QDate getWeekStart(const QDate& date) const;
};

#endif // WEEKVIEW_H