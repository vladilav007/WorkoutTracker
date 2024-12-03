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
    update();
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

    QRect rect = this->rect().adjusted(1, 1, -1, -1);
    
    // Draw background with status color
    painter.fillRect(rect, getStatusColor());
    
    // Draw border
    bool isSelected = m_date == QDate::currentDate();
    painter.setPen(isSelected ? QPen(Qt::blue, 2) : QPen(Qt::lightGray, 1));
    painter.drawRect(rect);

    // Set text color based on status
    bool isWeekend = m_date.dayOfWeek() > 5;
    QColor textColor;
    
    if (m_status == WorkoutStatus::NoWorkout) {
        textColor = isWeekend ? QColor(244, 67, 54) : Qt::white;
    } else if (m_status == WorkoutStatus::Missed) {
        textColor = Qt::white;
    } else {
        textColor = Qt::black;
    }
    
    // Draw date
    painter.setPen(textColor);
    QFont dateFont = painter.font();
    dateFont.setBold(true);
    dateFont.setPointSize(12);
    painter.setFont(dateFont);
    painter.drawText(QRect(8, 8, 30, 25), Qt::AlignLeft | Qt::AlignVCenter,
                    QString::number(m_date.day()));

    // Draw workout info if exists
    if (!m_workoutName.isEmpty()) {
        QRect contentRect = rect.adjusted(8, 35, -8, -8);
        
        QFont nameFont = dateFont;
        nameFont.setPointSize(10);
        painter.setFont(nameFont);
        
        QFontMetrics fm(nameFont);
        QString elidedName = fm.elidedText(m_workoutName, Qt::ElideRight, contentRect.width());
        painter.drawText(contentRect, Qt::AlignTop | Qt::AlignLeft, elidedName);

        QFont normalFont = painter.font();
        normalFont.setBold(false);
        normalFont.setPointSize(9);
        painter.setFont(normalFont);
        
        QString exerciseInfo = QString("%1 exercises").arg(m_exercises.size());
        painter.drawText(contentRect.adjusted(0, fm.height() + 5, 0, 0),
                        Qt::AlignTop | Qt::AlignLeft,
                        exerciseInfo);
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