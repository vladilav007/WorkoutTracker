#include "workoutdialog.h"
#include <QLabel>
#include <QMessageBox>
#include <QHeaderView>

WorkoutDialog::WorkoutDialog(const QDate &date, QWidget *parent)
    : QDialog(parent), workoutDate(date)
{
    setWindowTitle(tr("New Workout - %1").arg(date.toString("dd.MM.yyyy")));
    setupUI();
}

WorkoutDialog::WorkoutDialog(const QDate &date, const QString &name, 
                           const QString &description, QWidget *parent)
    : QDialog(parent), workoutDate(date)
{
    setWindowTitle(tr("Edit Workout - %1").arg(date.toString("dd.MM.yyyy")));
    setupUI();
    
    // Fill existing data
    nameEdit->setText(name);
    descriptionEdit->setText(description);
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
    auto buttonLayout = new QHBoxLayout;
    addExerciseButton = new QPushButton(tr("Add Exercise"), this);
    removeExerciseButton = new QPushButton(tr("Remove Exercise"), this);
    buttonLayout->addWidget(addExerciseButton);
    buttonLayout->addWidget(removeExerciseButton);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    
    // Dialog buttons
    auto dialogButtons = new QHBoxLayout;
    saveButton = new QPushButton(tr("Save"), this);
    cancelButton = new QPushButton(tr("Cancel"), this);
    dialogButtons->addStretch();
    dialogButtons->addWidget(saveButton);
    dialogButtons->addWidget(cancelButton);
    mainLayout->addLayout(dialogButtons);
    
    // Connect signals
    connect(addExerciseButton, &QPushButton::clicked, this, &WorkoutDialog::addExercise);
    connect(removeExerciseButton, &QPushButton::clicked, this, &WorkoutDialog::removeExercise);
    connect(saveButton, &QPushButton::clicked, this, &WorkoutDialog::saveWorkout);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    
    setMinimumWidth(500);
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
    // Validation
    if (nameEdit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), 
                           tr("Please enter a workout name."));
        return;
    }
    
    // TODO: Save workout data
    // Here we'll add actual saving when we implement the data model
    
    accept();
}