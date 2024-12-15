#include <QPushButton>
#include <QVBoxLayout>
#include <QJsonObject>
#include <QJsonArray>

#include "JsonViewerDialog.h"

/// <summary>
/// Create dialog to display the JSON object
/// </summary>
/// <param name="jsonObject"> jsonObject to display </param>
/// <param name="parent"> parent widget </param>
JsonViewerDialog::JsonViewerDialog(const QJsonObject& jsonObjectToView_in, QWidget* pWidgetParent_in) : QDialog(pWidgetParent_in)
{
    setWindowTitle("JSON Viewer");

    pTreeWidgetDisplay = new QTreeWidget(this);
    pTreeWidgetDisplay->setHeaderLabels(QStringList() << "Description" << "Note");

    if (jsonObjectToView_in.contains("tasks") && jsonObjectToView_in["tasks"].isArray())
    {
        QJsonArray jsonArrayTasks = jsonObjectToView_in["tasks"].toArray();
        PopulateTasks(jsonArrayTasks);
    }

    pPushButtonCloseButton = new QPushButton("Close", this);
    connect(pPushButtonCloseButton, &QPushButton::clicked, this, &QDialog::accept);

    QVBoxLayout* pVBoxlayout = new QVBoxLayout(this);
    pVBoxlayout->addWidget(pTreeWidgetDisplay);
    pVBoxlayout->addWidget(pPushButtonCloseButton);

    setLayout(pVBoxlayout);
    resize(600, 400);
}

/// <summary>
/// Populate the tasks in the tree widget
/// </summary>
/// <param name="tasksArray"> tasks array to populate </param>
void JsonViewerDialog::PopulateTasks(const QJsonArray& jsonArrayTaskToPopulate_in)
{
    for (int i = 0; i < jsonArrayTaskToPopulate_in.size(); ++i)
    {
        const QJsonObject& jsonObjectOpen = jsonArrayTaskToPopulate_in[i].toObject();

        QTreeWidgetItem* pTreeWidgetItemDisplay = new QTreeWidgetItem(pTreeWidgetDisplay);

        QString sDescription = jsonObjectOpen.value("description").toString();
        QString sNote = QString::number(jsonObjectOpen.value("note").toInt());

        pTreeWidgetItemDisplay->setText(0, sDescription.isEmpty() ? "No Description" : sDescription);

        pTreeWidgetItemDisplay->setText(1, sNote.isEmpty() ? "No Note" : sNote);
    }
}