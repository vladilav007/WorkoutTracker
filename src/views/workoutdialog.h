#ifndef WORKOUTDIALOG_H
#define WORKOUTDIALOG_H

#include <QDialog>
#include <QDate>
#include <QLineEdit>
#include <QTextEdit>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>

class WorkoutDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor for creating new workout
    explicit WorkoutDialog(const QDate &date, QWidget *parent = nullptr);
    
    // Constructor for editing existing workout
    WorkoutDialog(const QDate &date, const QString &name, 
                 const QString &description, QWidget *parent = nullptr);

private slots:
    void addExercise();
    void removeExercise();
    void saveWorkout();

private:
    void setupUI();
    void setupExerciseTable();
    
    // UI elements
    QLineEdit *nameEdit;
    QTextEdit *descriptionEdit;
    QTableWidget *exerciseTable;
    QPushButton *addExerciseButton;
    QPushButton *removeExerciseButton;
    QPushButton *saveButton;
    QPushButton *cancelButton;
    
    QDate workoutDate;
};

#endif // WORKOUTDIALOG_H