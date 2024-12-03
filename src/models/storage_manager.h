// storage_manager.h
#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <QString>
#include <QDate>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include <QDebug>
#include "types.h"
#include "workout_status.h"

class StorageManager {
public:
    static StorageManager& instance();
    
    bool saveWorkout(const QDate& date, 
                    const QString& name, 
                    const QString& description, 
                    const QVector<Exercise>& exercises,
                    WorkoutStatus status);
    
    bool loadWorkout(const QDate& date, 
                    QString& name, 
                    QString& description,
                    QVector<Exercise>& exercises,
                    WorkoutStatus& status);
    
    bool saveToFile(const QString& filename = QString());
    bool loadFromFile(const QString& filename = QString());
    
    void clearAllData();
    QVector<QDate> getAllWorkoutDates() const;
    bool hasWorkout(const QDate& date) const;

private:
    StorageManager() = default;
    StorageManager(const StorageManager&) = delete;
    StorageManager& operator=(const StorageManager&) = delete;

    struct WorkoutData {
        QString name;
        QString description;
        QVector<Exercise> exercises;
        WorkoutStatus status;
    };

    QMap<QDate, WorkoutData> workouts;
    
    QString getWorkoutFilePath();
    QJsonObject workoutToJson(const WorkoutData& workout) const;
    WorkoutData workoutFromJson(const QJsonObject& json) const;
};

#endif // STORAGE_MANAGER_H