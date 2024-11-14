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
}

void WorkoutDialog::removeExercise()
{
    QList<QTableWidgetItem*> selectedItems = exerciseTable->selectedItems();
    if (!selectedItems.isEmpty()) {
        int row = selectedItems.first()->row();
        exerciseTable->removeRow(row);
    }
}

void WorkoutDialog::saveWorkout()
{
    if (nameEdit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), 
                           tr("Please enter a workout name."));
        return;
    }
    
    // TODO: Save exercise data here
    
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
    // Disable editing in read-only mode
    nameEdit->setReadOnly(isReadOnly);
    descriptionEdit->setReadOnly(isReadOnly);
    exerciseTable->setEditTriggers(isReadOnly ? QAbstractItemView::NoEditTriggers 
                                            : QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    
    // Show/hide buttons appropriately
    addExerciseButton->setVisible(!isReadOnly);
    removeExerciseButton->setVisible(!isReadOnly);
    saveButton->setVisible(!isReadOnly);
    editButton->setVisible(isReadOnly);
    
    // Update cancel button text
    cancelButton->setText(isReadOnly ? tr("Close") : tr("Cancel"));
}

void WorkoutDialog::editWorkout()
{
    setReadOnly(false);
    // When switching to edit mode, ensure save button is visible
    saveButton->setVisible(true);
}

