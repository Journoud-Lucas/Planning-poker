#include <QVBoxLayout>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonObject>
#include <QPushButton>

#include "JsonViewerDialog.h"

/// <summary>
/// Create dialog to display the JSON object
/// </summary>
/// <param name="jsonObject"> jsonObject to display </param>
/// <param name="parent"> parent widget </param>
JsonViewerDialog::JsonViewerDialog(const QJsonObject& jsonObjectToView_in, QWidget* pWidgetParent_in)
    : QDialog(pWidgetParent_in)
{
    setWindowTitle("JSON Viewer");

    QFont fontToUse;
    fontToUse.setPointSize(12);

    m_pTreeWidgetDisplay = new QTreeWidget(this);
    m_pTreeWidgetDisplay->setHeaderLabels(QStringList() << "Description" << "Note");
    m_pTreeWidgetDisplay->setFont(fontToUse);
    m_pTreeWidgetDisplay->setStyleSheet("background-color: #FFFFFF; color: #333;");

    QString sMode = jsonObjectToView_in.contains("mode") ? jsonObjectToView_in["mode"].toString() : "Unknown";
    bool bFinished = jsonObjectToView_in.contains("finished") ? jsonObjectToView_in["finished"].toBool() : false;

    if (jsonObjectToView_in.contains("playerNames") && jsonObjectToView_in["playerNames"].isArray())
    {
        QJsonArray jsonArrayPlayer = jsonObjectToView_in["playerNames"].toArray();
        QStringList sListPlayerNames;
        for (const QJsonValue& jsonValuePlayer : jsonArrayPlayer)
        {
            sListPlayerNames.append(jsonValuePlayer.toString());
        }
        QMessageBox::information(this, "Session Info", QString("Mode: ") + sMode + "\nFinished: " + (bFinished ? "Yes" : "No") + "\nPlayers: " + sListPlayerNames.join(", "));
    }

    if (jsonObjectToView_in.contains("tasks") && jsonObjectToView_in["tasks"].isArray())
    {
        QJsonArray jsonArrayTasks = jsonObjectToView_in["tasks"].toArray();
        PopulateTasks(jsonArrayTasks);
    }

    m_pPushButtonCloseButton = new QPushButton("Close", this);
    m_pPushButtonCloseButton->setFont(fontToUse);
    m_pPushButtonCloseButton->setStyleSheet("QPushButton { background-color: #1976D2; color: white; border-radius: 5px; }");

    connect(m_pPushButtonCloseButton, &QPushButton::clicked, this, &QDialog::accept);

    QVBoxLayout* pVBoxlayout = new QVBoxLayout(this);
    pVBoxlayout->addWidget(m_pTreeWidgetDisplay);
    pVBoxlayout->addWidget(m_pPushButtonCloseButton);

    setLayout(pVBoxlayout);
    resize(700, 500);
}

/// <summary>
/// Populate the tasks in the tree widget
/// </summary>
/// <param name="tasksArray"> tasks array to populate </param>
void JsonViewerDialog::PopulateTasks(const QJsonArray& jsonArrayTaskToPopulate_in) {
    for (int i = 0; i < jsonArrayTaskToPopulate_in.size(); ++i) {
        const QJsonObject& jsonObjectOpen = jsonArrayTaskToPopulate_in[i].toObject();

        QTreeWidgetItem* pTreeWidgetItemDisplay = new QTreeWidgetItem(m_pTreeWidgetDisplay);

        QString sDescription = jsonObjectOpen.value("description").toString();
        QString sNote = (jsonObjectOpen.value("note").isNull()) ? "No Note" : QString::number(jsonObjectOpen.value("note").toDouble());

        pTreeWidgetItemDisplay->setText(0, sDescription.isEmpty() ? "No Description" : sDescription);
        pTreeWidgetItemDisplay->setText(1, sNote);
    }
}