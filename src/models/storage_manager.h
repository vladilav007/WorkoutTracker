#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <QString>
#include <QDate>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include "types.h"
#include "../views/customcalendarwidget.h"

class StorageManager {
public:
    static StorageManager& instance();
    
    // Сохранение и загрузка
    bool saveWorkout(const QDate& date, const QString& name, 
                    const QString& description, 
                    const QVector<Exercise>& exercises,
                    CustomCalendarWidget::WorkoutStatus status);
    
    bool loadWorkout(const QDate& date, QString& name, 
                    QString& description,
                    QVector<Exercise>& exercises,
                    CustomCalendarWidget::WorkoutStatus& status);
    
    // Работа с файлами
    bool saveToFile(const QString& filename = QString());
    bool loadFromFile(const QString& filename = QString());
    
    // Управление данными
    void clearAllData();
    QVector<QDate> getAllWorkoutDates() const;
    bool hasWorkout(const QDate& date) const;

private:
    StorageManager() = default;  // Конструктор по умолчанию
    StorageManager(const StorageManager&) = delete;  // Запрещаем копирование
    StorageManager& operator=(const StorageManager&) = delete;

    struct WorkoutData {
        QString name;
        QString description;
        QVector<Exercise> exercises;
        CustomCalendarWidget::WorkoutStatus status;
    };

    QMap<QDate, WorkoutData> workouts;
    
    // Вспомогательные методы
    QString getWorkoutFilePath();
    QJsonObject workoutToJson(const WorkoutData& workout) const;
    WorkoutData workoutFromJson(const QJsonObject& json) const;
};

#endif // STORAGE_MANAGER_H