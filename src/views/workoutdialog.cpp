#include "workoutdialog.h"
#include <QLabel>
#include <QMessageBox>
#include <QHeaderView>

WorkoutDialog::WorkoutDialog(const QDate &date, QWidget *parent)
    : QDialog(parent)
    , workoutDate(date)
    , isReadOnly(false)
{
    setWindowTitle(tr("Workout - %1").arg(date.toString("dd.MM.yyyy")));
    setupUI();
}

void WorkoutDialog::setupUI()
{
    // Main layout
    auto mainLayout = new QVBoxLayout(this);
    
    // Workout name
    auto nameLabel = new QLabel(tr("Workout Name:"), this);
    nameEdit = new QLineEdit(this);
    mainLayout->addWidget(nameLabel);
    mainLayout->addWidget(nameEdit);
    
    // Description
    auto descLabel = new QLabel(tr("Description:"), this);
    descriptionEdit = new QTextEdit(this);
    descriptionEdit->setMaximumHeight(100);
    mainLayout->addWidget(descLabel);
    mainLayout->addWidget(descriptionEdit);
    
    // Exercise table
    setupExerciseTable();
    mainLayout->addWidget(exerciseTable);
    
    // Table control buttons
    auto exerciseButtonLayout = new QHBoxLayout;  // Changed name to be more specific
    addExerciseButton = new QPushButton(tr("Add Exercise"), this);
    removeExerciseButton = new QPushButton(tr("Remove Exercise"), this);
    exerciseButtonLayout->addWidget(addExerciseButton);
    exerciseButtonLayout->addWidget(removeExerciseButton);
    exerciseButtonLayout->addStretch();
    mainLayout->addLayout(exerciseButtonLayout);
    
    // Add edit button and final buttons
    editButton = new QPushButton(tr("Edit"), this);
    saveButton = new QPushButton(tr("Save"), this);
    cancelButton = new QPushButton(tr("Cancel"), this);

    auto dialogButtonLayout = new QHBoxLayout;  // Changed name to be more specific
    dialogButtonLayout->addWidget(editButton);
    dialogButtonLayout->addStretch();
    dialogButtonLayout->addWidget(saveButton);
    dialogButtonLayout->addWidget(cancelButton);
    mainLayout->addLayout(dialogButtonLayout);
    
    // Connect signals
    connect(addExerciseButton, &QPushButton::clicked, this, &WorkoutDialog::addExercise);
    connect(removeExerciseButton, &QPushButton::clicked, this, &WorkoutDialog::removeExercise);
    connect(saveButton, &QPushButton::clicked, this, &WorkoutDialog::saveWorkout);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(editButton, &QPushButton::clicked, this, &WorkoutDialog::editWorkout);
    
    setMinimumWidth(500);
    updateControlsState();
}

void WorkoutDialog::setupExerciseTable()
{
    exerciseTable = new QTableWidget(0, 3, this);
    
    // Set headers
    QStringList headers;
    headers << tr("Exercise Name") << tr("Sets") << tr("Reps");
    exerciseTable->setHorizontalHeaderLabels(headers);
    
    // Adjust table properties
    exerciseTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    exerciseTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    exerciseTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    exerciseTable->setColumnWidth(1, 70);
    exerciseTable->setColumnWidth(2, 70);
}

void WorkoutDialog::addExercise()
{
    int row = exerciseTable->rowCount();
    exerciseTable->insertRow(row);
    
    QTableWidgetItem *nameItem = new QTableWidgetItem("");
    QTableWidgetItem *setsItem = new QTableWidgetItem("0");
    QTableWidgetItem *repsItem = new QTableWidgetItem("0");
    
    exerciseTable->setItem(row, 0, nameItem);
    exerciseTable->setItem(row, 1, setsItem);
    exerciseTable->setItem(row, 2, repsItem);
}

void WorkoutDialog::removeExercise()
{
    QList<QTableWidgetItem*> selectedItems = exerciseTable->selectedItems();
    if (!selectedItems.isEmpty()) {
        int row = selectedItems.first()->row();
        exerciseTable->removeRow(row);
    }
}

QVector<Exercise> WorkoutDialog::getExercises() const
{
    return getCurrentExercises();
}

void WorkoutDialog::setExercises(const QVector<Exercise> &exercises)
{
    exerciseTable->setRowCount(0);
    for (const Exercise &exercise : exercises) {
        addExerciseRow(exercise.name, exercise.sets, exercise.reps);
    }
}

QVector<Exercise> WorkoutDialog::getCurrentExercises() const
{
    QVector<Exercise> exercises;
    for (int row = 0; row < exerciseTable->rowCount(); ++row) {
        Exercise exercise;
        exercise.name = exerciseTable->item(row, 0)->text();
        exercise.sets = exerciseTable->item(row, 1)->text().toInt();
        exercise.reps = exerciseTable->item(row, 2)->text().toInt();
        exercises.append(exercise);
    }
    return exercises;
}

void WorkoutDialog::saveWorkout()
{
    if (nameEdit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), 
                           tr("Please enter a workout name."));
        return;
    }
    
    WorkoutStatus currentStatus = WorkoutStatus::NoWorkout;
    QString oldName, oldDescription;
    QVector<Exercise> oldExercises;
    
    if (StorageManager::instance().loadWorkout(workoutDate, 
            oldName, oldDescription, oldExercises, currentStatus)) {
        StorageManager::instance().saveWorkout(workoutDate, 
            nameEdit->text(), 
            descriptionEdit->toPlainText(), 
            getCurrentExercises(),
            currentStatus);
    } else {
        StorageManager::instance().saveWorkout(workoutDate, 
            nameEdit->text(), 
            descriptionEdit->toPlainText(), 
            getCurrentExercises(),
            WorkoutStatus::NoWorkout);
    }
    
    accept();
}

void WorkoutDialog::setReadOnly(bool readOnly)
{
    isReadOnly = readOnly;
    updateControlsState();
}

void WorkoutDialog::addExerciseRow(const QString &name, int sets, int reps)
{
    int row = exerciseTable->rowCount();
    exerciseTable->insertRow(row);
    
    QTableWidgetItem *nameItem = new QTableWidgetItem(name);
    QTableWidgetItem *setsItem = new QTableWidgetItem(QString::number(sets));
    QTableWidgetItem *repsItem = new QTableWidgetItem(QString::number(reps));
    
    exerciseTable->setItem(row, 0, nameItem);
    exerciseTable->setItem(row, 1, setsItem);
    exerciseTable->setItem(row, 2, repsItem);
}

void WorkoutDialog::updateControlsState()
{
    nameEdit->setReadOnly(isReadOnly);
    descriptionEdit->setReadOnly(isReadOnly);
    exerciseTable->setEditTriggers(isReadOnly ? QAbstractItemView::NoEditTriggers 
                                            : QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    
    addExerciseButton->setVisible(!isReadOnly);
    removeExerciseButton->setVisible(!isReadOnly);
    saveButton->setVisible(!isReadOnly);
    editButton->setVisible(isReadOnly);
    
    cancelButton->setText(isReadOnly ? tr("Close") : tr("Cancel"));
}

void WorkoutDialog::editWorkout()
{
    setReadOnly(false);
    saveButton->setVisible(true);
}
