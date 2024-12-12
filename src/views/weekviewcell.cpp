// weekviewcell.cpp
#include "weekviewcell.h"
#include <QPainter>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QFontMetrics>

WeekViewCell::WeekViewCell(const QDate& date, QWidget* parent)
    : QWidget(parent)
    , m_date(date)
    , m_status(WorkoutStatus::NoWorkout)
{
    setMinimumSize(150, 120);
    setMaximumSize(300, 200);
    setMouseTracking(true);
    
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::transparent);
    setAutoFillBackground(true);
    setPalette(pal);
}

void WeekViewCell::setWorkoutData(const QString& name, 
                               const QString& description,
                               const QVector<Exercise>& exercises,
                               WorkoutStatus status)
{
    m_workoutName = name;
    m_workoutDescription = description;
    m_exercises = exercises;
    m_status = status;
    update();  // Force immediate update
    
    // If status changed, force parent update too
    if (parentWidget()) {
        parentWidget()->update();
    }
}

void WeekViewCell::clear()
{
    m_workoutName.clear();
    m_workoutDescription.clear();
    m_exercises.clear();
    m_status = WorkoutStatus::NoWorkout;
    update();
}

void WeekViewCell::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect rect = this->rect().adjusted(2, 2, -2, -2);
    
    // Draw background with status color
    painter.fillRect(rect, getStatusColor());
    
    // Draw selection border
    if (m_isSelected) {
        painter.setPen(QPen(Qt::blue, 2));
        painter.drawRect(rect);
    } else {
        painter.setPen(QPen(Qt::lightGray, 1));
        painter.drawRect(rect);
    }
    
    // Draw date
    bool isWeekend = m_date.dayOfWeek() > 5;
    QColor textColor = isWeekend ? QColor(244, 67, 54) : Qt::white;
    
    painter.setPen(textColor);
    QFont dateFont = painter.font();
    dateFont.setBold(true);
    dateFont.setPointSize(14);
    painter.setFont(dateFont);
    painter.drawText(QRect(10, 10, rect.width() - 20, 30), 
                    Qt::AlignLeft | Qt::AlignVCenter,
                    QString::number(m_date.day()));

    // Draw workout info if exists
    if (!m_workoutName.isEmpty()) {
        QFont nameFont = painter.font();
        nameFont.setPointSize(12);
        nameFont.setBold(true);
        painter.setFont(nameFont);
        
        QRect nameRect = rect.adjusted(10, 40, -10, -rect.height()/2);
        painter.drawText(nameRect, Qt::AlignLeft | Qt::TextWordWrap, m_workoutName);
        
        QFont exerciseFont = painter.font();
        exerciseFont.setPointSize(10);
        exerciseFont.setBold(false);
        painter.setFont(exerciseFont);
        
        QRect exerciseRect = rect.adjusted(10, rect.height()/2, -10, -10);
        painter.drawText(exerciseRect, 
                        Qt::AlignLeft | Qt::AlignTop,
                        tr("%1 exercises").arg(m_exercises.size()));
    }
}

QColor WeekViewCell::getStatusColor() const
{
    switch (m_status) {
        case WorkoutStatus::Completed:
            return QColor(76, 175, 80);    // Green
        case WorkoutStatus::Missed:
            return QColor(244, 67, 54);    // Red
        case WorkoutStatus::RestDay:
            return QColor(158, 158, 158);  // Grey
        default:
            return QColor(45, 45, 45);     // Dark background
    }
}

void WeekViewCell::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked(m_date);
    }
    else if (event->button() == Qt::RightButton) {
        emit contextMenuRequested(m_date, event->globalPosition().toPoint());
    }
}

void WeekViewCell::contextMenuEvent(QContextMenuEvent* event)
{
    emit contextMenuRequested(m_date, event->globalPos());
    event->accept();
}

void WeekViewCell::setSelected(bool selected)
{
    m_isSelected = selected;
    update();
}