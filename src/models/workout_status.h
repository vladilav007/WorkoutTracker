// src/models/workout_status.h
#ifndef WORKOUT_STATUS_H
#define WORKOUT_STATUS_H

#include <QDebug>

enum class WorkoutStatus {
    NoWorkout,
    Completed,
    Missed,
    RestDay
};

// Add debug output operator
inline QDebug operator<<(QDebug debug, const WorkoutStatus& status)
{
    switch (status) {
        case WorkoutStatus::NoWorkout:
            return debug << "NoWorkout";
        case WorkoutStatus::Completed:
            return debug << "Completed";
        case WorkoutStatus::Missed:
            return debug << "Missed";
        case WorkoutStatus::RestDay:
            return debug << "RestDay";
        default:
            return debug << "Unknown";
    }
}

#endif // WORKOUT_STATUS_H