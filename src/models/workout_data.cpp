// workout_data.cpp
#include "workout_data.h"
#include <QFile>
#include <QJsonDocument>

QJsonObject Exercise::toJson() const {
    QJsonObject json;
    json["name"] = name;
    json["sets"] = sets;
    json["reps"] = reps;
    return json;
}

Exercise Exercise::fromJson(const QJsonObject &json) {
    Exercise exercise;
    exercise.name = json["name"].toString();
    exercise.sets = json["sets"].toInt();
    exercise.reps = json["reps"].toInt();
    return exercise;
}

QJsonObject WorkoutData::toJson() const {
    QJsonObject json;
    json["name"] = name;
    json["description"] = description;
    json["date"] = date.toString(Qt::ISODate);
    
    QJsonArray exercisesArray;
    for (const Exercise &exercise : exercises) {
        exercisesArray.append(exercise.toJson());
    }
    json["exercises"] = exercisesArray;
    
    return json;
}

WorkoutData WorkoutData::fromJson(const QJsonObject &json) {
    WorkoutData workout;
    workout.name = json["name"].toString();
    workout.description = json["description"].toString();
    workout.date = QDate::fromString(json["date"].toString(), Qt::ISODate);
    
    QJsonArray exercisesArray = json["exercises"].toArray();
    for (const QJsonValue &value : exercisesArray) {
        workout.exercises.append(Exercise::fromJson(value.toObject()));
    }
    
    return workout;
}

WorkoutStorage& WorkoutStorage::instance() {
    static WorkoutStorage instance;
    return instance;
}

void WorkoutStorage::addWorkout(const WorkoutData &workout) {
    workouts[workout.date] = workout;
}

bool WorkoutStorage::hasWorkout(const QDate &date) const {
    return workouts.contains(date);
}

WorkoutData WorkoutStorage::getWorkout(const QDate &date) const {
    return workouts.value(date);
}

void WorkoutStorage::removeWorkout(const QDate &date) {
    workouts.remove(date);
}

bool WorkoutStorage::saveToFile(const QString &filename) {
    QJsonArray workoutsArray;
    for (const WorkoutData &workout : workouts) {
        workoutsArray.append(workout.toJson());
    }
    
    QJsonDocument doc(workoutsArray);
    QFile file(filename);
    
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    
    file.write(doc.toJson());
    return true;
}

bool WorkoutStorage::loadFromFile(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isArray()) {
        return false;
    }
    
    workouts.clear();
    QJsonArray workoutsArray = doc.array();
    
    for (const QJsonValue &value : workoutsArray) {
        WorkoutData workout = WorkoutData::fromJson(value.toObject());
        workouts[workout.date] = workout;
    }
    
    return true;
}