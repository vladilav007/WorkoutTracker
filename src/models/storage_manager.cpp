// src/models/storage_manager.cpp
#include "storage_manager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>

StorageManager& StorageManager::instance()
{
    static StorageManager instance;
    return instance;
}

bool StorageManager::saveWorkout(const QDate &date, 
                               const QString &name, 
                               const QString &description, 
                               const QVector<Exercise> &exercises,  // Changed from reference
                               CustomCalendarWidget::WorkoutStatus status)
{
    WorkoutData workout;
    workout.name = name;
    workout.description = description;
    workout.exercises = exercises;
    workout.status = status;
    
    workouts[date] = workout;
    return saveToFile(); // Return the result of saving
}

bool StorageManager::loadWorkout(const QDate& date, 
                               QString& name,
                               QString& description,
                               QVector<Exercise>& exercises,
                               CustomCalendarWidget::WorkoutStatus& status)
{
    if (!workouts.contains(date)) {
        return false;
    }
    
    const WorkoutData& workout = workouts[date];
    name = workout.name;
    description = workout.description;
    exercises = workout.exercises;
    status = workout.status;
    return true;
}

QString StorageManager::getWorkoutFilePath()
{
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(dataPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    return dir.filePath("workouts.json");
}

bool StorageManager::saveToFile(const QString& filename)
{
    QString filePath = filename.isEmpty() ? getWorkoutFilePath() : filename;
    qDebug() << "Saving workouts to:" << filePath;
    
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Could not open file for writing:" << filePath;
        return false;
    }
    
    QJsonObject root;
    QJsonArray workoutsArray;
    
    for (auto it = workouts.begin(); it != workouts.end(); ++it) {
        QJsonObject workoutObj = workoutToJson(it.value());
        workoutObj["date"] = it.key().toString(Qt::ISODate);
        workoutObj["status"] = static_cast<int>(it.value().status);
        workoutsArray.append(workoutObj);
    }
    
    root["workouts"] = workoutsArray;
    root["version"] = "1.0";
    
    QJsonDocument doc(root);
    file.write(doc.toJson(QJsonDocument::Indented));
    
    return true;
}

bool StorageManager::loadFromFile(const QString& filename)
{
    QString filePath = filename.isEmpty() ? getWorkoutFilePath() : filename;
    qDebug() << "Loading workouts from:" << filePath;
    
    QFile file(filePath);
    
    if (!file.exists()) {
        qInfo() << "No saved workouts found at:" << filePath;
        return true;
    }
    
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open file for reading:" << filePath;
        return false;
    }
    
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject()) {
        qWarning() << "Invalid JSON format in file:" << filePath;
        return false;
    }
    
    QJsonObject root = doc.object();
    QJsonArray workoutsArray = root["workouts"].toArray();
    
    workouts.clear();
    
    for (const QJsonValue& value : workoutsArray) {
        QJsonObject workoutObj = value.toObject();
        QDate date = QDate::fromString(workoutObj["date"].toString(), Qt::ISODate);
        WorkoutData workout = workoutFromJson(workoutObj);
        
        // Load status
        if (workoutObj.contains("status")) {
            workout.status = static_cast<CustomCalendarWidget::WorkoutStatus>(
                workoutObj["status"].toInt());
        }
        
        workouts[date] = workout;
        qDebug() << "Loaded workout for" << date << "with status" << workout.status;
    }
    
    return true;
}

StorageManager::WorkoutData StorageManager::workoutFromJson(const QJsonObject& json) const
{
    WorkoutData workout;
    workout.name = json["name"].toString();
    workout.description = json["description"].toString();
    workout.status = static_cast<CustomCalendarWidget::WorkoutStatus>(
        json["status"].toInt(static_cast<int>(CustomCalendarWidget::NoWorkout)));
    
    QJsonArray exercisesArray = json["exercises"].toArray();
    for (const QJsonValue& value : exercisesArray) {
        QJsonObject exerciseObj = value.toObject();
        Exercise exercise;
        exercise.name = exerciseObj["name"].toString();
        exercise.sets = exerciseObj["sets"].toInt();
        exercise.reps = exerciseObj["reps"].toInt();
        workout.exercises.append(exercise);
    }
    
    return workout;
}

QJsonObject StorageManager::workoutToJson(const WorkoutData& workout) const
{
    QJsonObject json;
    json["name"] = workout.name;
    json["description"] = workout.description;
    json["status"] = static_cast<int>(workout.status);
    
    QJsonArray exercisesArray;
    for (const Exercise& exercise : workout.exercises) {
        QJsonObject exerciseObj;
        exerciseObj["name"] = exercise.name;
        exerciseObj["sets"] = exercise.sets;
        exerciseObj["reps"] = exercise.reps;
        exercisesArray.append(exerciseObj);
    }
    
    json["exercises"] = exercisesArray;
    return json;
}

void StorageManager::clearAllData()
{
    workouts.clear();
    saveToFile();
}

QVector<QDate> StorageManager::getAllWorkoutDates() const
{
    return QVector<QDate>(workouts.keys().begin(), workouts.keys().end());
}

bool StorageManager::hasWorkout(const QDate& date) const
{
    return workouts.contains(date);
}