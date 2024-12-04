// storage_manager.cpp
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
    
    QByteArray saveData = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(saveData));
    
    if (!doc.isObject()) {
        qWarning() << "Invalid JSON format in file:" << filePath;
        return false;
    }
    
    QJsonObject root = doc.object();
    QJsonArray workoutsArray = root["workouts"].toArray();
    
    workouts.clear();
    int loadedWorkouts = 0;
    
    for (const QJsonValue& value : workoutsArray) {
        if (!value.isObject()) continue;
        
        QJsonObject workoutObj = value.toObject();
        QString dateStr = workoutObj["date"].toString();
        if (dateStr.isEmpty()) continue;
        
        QDate date = QDate::fromString(dateStr, Qt::ISODate);
        if (!date.isValid()) {
            qWarning() << "Invalid date in workout data:" << dateStr;
            continue;
        }
        
        WorkoutData workout = workoutFromJson(workoutObj);
        workouts[date] = workout;
        loadedWorkouts++;
    }
    
    qDebug() << "Successfully loaded" << loadedWorkouts << "workouts";
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
    if (isSaving || !needsSaving) return true;
    
    isSaving = true;
    QString filePath = filename.isEmpty() ? getWorkoutFilePath() : filename;
    qDebug() << "Saving workouts to:" << filePath;
    
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Could not open file for writing:" << filePath;
        isSaving = false;
        return false;
    }
    
    QJsonObject root;
    QJsonArray workoutsArray;
    
    for (auto it = workouts.begin(); it != workouts.end(); ++it) {
        QJsonObject workoutObj = workoutToJson(it.value());
        workoutObj["date"] = it.key().toString(Qt::ISODate);
        workoutsArray.append(workoutObj);
    }
    
    root["workouts"] = workoutsArray;
    root["version"] = "1.0";
    
    QJsonDocument doc(root);
    file.write(doc.toJson(QJsonDocument::Indented));
    
    needsSaving = false;
    isSaving = false;
    return true;
}

QVector<QDate> StorageManager::getAllWorkoutDates() const
{
    return QVector<QDate>(workouts.keys().begin(), workouts.keys().end());
}

bool StorageManager::hasWorkout(const QDate& date) const
{
    return workouts.contains(date);
}

void StorageManager::clearAllData()
{
    workouts.clear();
    saveToFile();
}

bool StorageManager::saveWorkout(const QDate& date,
                               const QString& name,
                               const QString& description,
                               const QVector<Exercise>& exercises,
                               WorkoutStatus status)
{
    static bool isSaving = false;
    if (isSaving) return false;
    
    isSaving = true;
    
    try {
        WorkoutData workout;
        workout.name = name;
        workout.description = description;
        workout.exercises = exercises;
        workout.status = status;
        
        workouts[date] = workout;
        saveToFile();  // Save immediately
        
        isSaving = false;
        return true;
    } catch (...) {
        isSaving = false;
        qWarning() << "Exception during save workout";
        return false;
    }
}

bool StorageManager::loadWorkout(const QDate& date,
                               QString& name,
                               QString& description,
                               QVector<Exercise>& exercises,
                               WorkoutStatus& status)
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

StorageManager::WorkoutData StorageManager::workoutFromJson(const QJsonObject& json) const
{
    WorkoutData workout;
    workout.name = json["name"].toString();
    workout.description = json["description"].toString();
    workout.status = static_cast<WorkoutStatus>(json["status"].toInt(0));
    
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
