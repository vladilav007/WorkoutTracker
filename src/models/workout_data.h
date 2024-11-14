// workout_data.h
#ifndef WORKOUT_DATA_H
#define WORKOUT_DATA_H

#include <QString>
#include <QVector>
#include <QDate>
#include <QJsonObject>
#include <QJsonArray>

struct Exercise {
    QString name;
    int sets;
    int reps;

    // Serialization methods
    QJsonObject toJson() const;
    static Exercise fromJson(const QJsonObject &json);
};

struct WorkoutData {
    QString name;
    QString description;
    QVector<Exercise> exercises;
    QDate date;

    // Serialization methods
    QJsonObject toJson() const;
    static WorkoutData fromJson(const QJsonObject &json);
};

class WorkoutStorage {
public:
    static WorkoutStorage& instance();
    
    // Storage management
    void addWorkout(const WorkoutData &workout);
    bool hasWorkout(const QDate &date) const;
    WorkoutData getWorkout(const QDate &date) const;
    void removeWorkout(const QDate &date);
    
    // File operations
    bool saveToFile(const QString &filename = "workouts.json");
    bool loadFromFile(const QString &filename = "workouts.json");

private:
    WorkoutStorage() = default;
    QMap<QDate, WorkoutData> workouts;
};

#endif // WORKOUT_DATA_H