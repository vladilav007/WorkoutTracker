#ifndef WORKOUTDIALOG_H
#define WORKOUTDIALOG_H

#include <QDialog>
#include <QDate>
#include <QLineEdit>
#include <QTextEdit>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "../models/types.h"

class WorkoutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WorkoutDialog(const QDate &date, QWidget *parent = nullptr);
    WorkoutDialog(const QDate &date, const QString &name, 
                 const QString &description, QWidget *parent = nullptr);

    void setReadOnly(bool readOnly);
    void addExerciseRow(const QString &name, int sets, int reps);

    QString getWorkoutName() const { return nameEdit->text(); }
    QString getWorkoutDescription() const { return descriptionEdit->toPlainText(); }
    void setWorkoutName(const QString &name) { nameEdit->setText(name); }
    void setWorkoutDescription(const QString &desc) { descriptionEdit->setText(desc); }
    QVector<Exercise> getExercises() const;
    void setExercises(const QVector<Exercise> &exercises);

private slots:
    void addExercise();
    void removeExercise();
    void saveWorkout();
    void editWorkout();

private:
    void setupUI();
    void setupExerciseTable();
    void updateControlsState();
    
    QLineEdit *nameEdit;
    QTextEdit *descriptionEdit;
    QTableWidget *exerciseTable;
    QPushButton *addExerciseButton;
    QPushButton *removeExerciseButton;
    QPushButton *saveButton;
    QPushButton *cancelButton;
    QPushButton *editButton;
    
    QDate workoutDate;
    bool isReadOnly;
    QVector<Exercise> getCurrentExercises() const;
};

#endif // WORKOUTDIALOG_H