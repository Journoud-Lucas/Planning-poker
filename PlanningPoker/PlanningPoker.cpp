#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QInputDialog>
#include <QMessageBox>
#include <QFile>

#include "PlanningPoker.h"
#include "EstimationDialog.h"
#include "JsonViewerDialog.h"

/// <summary>
/// Create window for Planning Poker application
/// </summary>
/// <param name="pWidgetParent_in"> Parent widget </param>
PlanningPoker::PlanningPoker(QWidget* pWidgetParent_in) : QMainWindow(pWidgetParent_in), sJsonFileName(""), jsonArrayTasks(QJsonArray())
{
    QWidget* pWidgetCentral = new QWidget(this);
    QVBoxLayout* pVBoxLayoutMain = new QVBoxLayout(pWidgetCentral);

    QHBoxLayout* pHBoxLayoutMain = new QHBoxLayout();
    QPushButton* pPushButtonChooseJson = new QPushButton("Choose JSON", this);
    pPushButtonStartSession = new QPushButton("Start a session", this);
    QPushButton* pPushButtonCreateJson = new QPushButton("Create JSON", this);
    pLabelJsonName = new QLabel("No JSON files selected", this);
    pPushButtonVisualizeJson = new QPushButton("Visualize JSON", this);

    pHBoxLayoutMain->addWidget(pPushButtonChooseJson);
    pHBoxLayoutMain->addWidget(pPushButtonStartSession);
    pHBoxLayoutMain->addWidget(pPushButtonCreateJson);
    pVBoxLayoutMain->addLayout(pHBoxLayoutMain);
    pVBoxLayoutMain->addWidget(pLabelJsonName);
    pHBoxLayoutMain->addWidget(pPushButtonVisualizeJson);

    pPushButtonStartSession->setEnabled(false);
    pPushButtonVisualizeJson->setEnabled(false);
    setCentralWidget(pWidgetCentral);

    connect(pPushButtonChooseJson, &QPushButton::clicked, this, &PlanningPoker::ChooseJsonFile);
    connect(pPushButtonStartSession, &QPushButton::clicked, this, &PlanningPoker::StartPokerSession);
    connect(pPushButtonCreateJson, &QPushButton::clicked, this, &PlanningPoker::CreateJson);
    connect(pPushButtonVisualizeJson, &QPushButton::clicked, this, &PlanningPoker::VisualizeJson);
}

/// <summary>
/// Choose a JSON file to use for the Planning Poker session
/// </summary>
void PlanningPoker::ChooseJsonFile(void)
{
    QString sFileName = QFileDialog::getOpenFileName(this, "Select a JSON file", "", "JSON Files (*.json)");
    if (!sFileName.isEmpty())
    {
        sJsonFileName = sFileName;
        jsonArrayTasks = ReadJsonFile(sJsonFileName)["tasks"].toArray();
        pPushButtonStartSession->setEnabled(true);
        pPushButtonVisualizeJson->setEnabled(true);
    }
}

/// <summary>
/// Start the Planning Poker session
/// </summary>
void PlanningPoker::StartPokerSession(void)
{
    bool bOk = false;
    int nPlayerCount = QInputDialog::getInt(this, "Number of players", "Enter the number of players:", 2, 2, 10, 1, &bOk);

    if (!bOk || nPlayerCount <= 0) return;

    QStringList sListModes = { "Strict", "Average" };
    QString sChosenMode = QInputDialog::getItem(this, "Mode Selection", "Choose a mode:", sListModes, 0, false, &bOk);

    if (!bOk) return;

    QVector<int> nVecNumberForVote = { 0, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89 };
    QVector<int> nVecEstimates(nPlayerCount);

    for (int i = 0; i < jsonArrayTasks.size(); ++i)
    {
        QJsonObject jsonObjectTask = jsonArrayTasks[i].toObject();
        QString sTaskDescription = jsonObjectTask["description"].toString();
        int nRoundCounter = 0;

        bool bConsensusReached = false;
        while (!bConsensusReached)
        {
            nRoundCounter++;
            for (int j = 0; j < nPlayerCount; ++j)
            {
                EstimationDialog estimationDialogPlayer(sTaskDescription, nRoundCounter, j + 1, nVecNumberForVote, this);
                if (estimationDialogPlayer.exec() == QDialog::Accepted)
                {
                    nVecEstimates[j] = estimationDialogPlayer.GetSelectedEstimate();
                }
                else
                {
                    return;
                }
            }

            if (sChosenMode == "Strict")
            {
                bConsensusReached = std::all_of(nVecEstimates.begin(), nVecEstimates.end(),
                    [&nVecEstimates](int nEst) { return nEst == nVecEstimates[0]; });
                if (!bConsensusReached)
                {
                    QMessageBox::information(this, "New Round", "The estimates are not unanimous. Starting a new round.");
                }
            }
            else if (sChosenMode == "Average")
            {
                bConsensusReached = std::all_of(nVecEstimates.begin(), nVecEstimates.end(),
                    [&nVecEstimates, nRoundCounter ](int est) { return (est == nVecEstimates[0]) || (nRoundCounter >= 2); });
                if (!bConsensusReached)
                {
                    QMessageBox::information(this, "New Round", "The estimates are not unanimous. Starting a new (last) round");
                }
            }
        }

        if (sChosenMode == "Average")
        {
            int nSum = std::accumulate(nVecEstimates.begin(), nVecEstimates.end(), 0);
            float nAverage = nSum / nVecEstimates.size();
            jsonObjectTask["note"] = nAverage;
            QMessageBox::information(this, "Round Finish", QString("The task estimation is: ") + QString::number(nAverage));
        }
        else
        {
            jsonObjectTask["note"] = nVecEstimates[0];
            QMessageBox::information(this, "Round Finish", QString("The task estimation is: ") + QString::number(nVecEstimates[0]));
        }

        jsonArrayTasks[i] = jsonObjectTask;
    }

    QJsonObject jsonObjectUpdated;
    jsonObjectUpdated["tasks"] = jsonArrayTasks;
    WriteJsonFile(sJsonFileName, jsonObjectUpdated);
}

/// <summary>
/// Create a new JSON file for the Planning Poker session
/// </summary>
void PlanningPoker::CreateJson(void)
{
    QString sFilename;

    while (true)
    {
        bool bOk;
        sFilename = QInputDialog::getText(this, "File Name Input",
            "Enter the name for the new JSON file:",
            QLineEdit::Normal, "", &bOk);

        if (!bOk)
        {
            return;
        }

        if (sFilename.isEmpty())
        {
            QMessageBox::warning(this, "Invalid Input", "Filename cannot be empty. Please try again.");
            continue;
        }

        if (!sFilename.endsWith(".json", Qt::CaseInsensitive))
        {
            sFilename += ".json";
        }

        QFileInfo fileInfoCheck(sFilename);
        if (fileInfoCheck.exists() && fileInfoCheck.isFile())
        {
            QMessageBox::warning(this, "File Exists",
                "A file with this name already exists. Please choose a different name.");
        }
        else
        {
            break;
        }
    }

    QJsonObject jsonObjectData;
    QJsonArray jsonArrayTask;

    bool bContinueAddingTasks = true;
    int nTaskCounter = 1;

    while (bContinueAddingTasks)
    {
        bool ok;
        QString description = QInputDialog::getText(this, "Task Input",
            "Enter a description for task " + QString::number(nTaskCounter) + " (Leave empty to finish):",
            QLineEdit::Normal, "", &ok);

        if (ok && !description.isEmpty())
        {
            QJsonObject newTask;
            newTask["description"] = description;
            newTask["note"] = QJsonValue();
            jsonArrayTask.append(newTask);
            nTaskCounter++;
        }
        else
        {
            bContinueAddingTasks = false;
        }
    }

    jsonObjectData["tasks"] = jsonArrayTask;
    WriteJsonFile(sFilename, jsonObjectData);

    QMessageBox::information(this, "New JSON", "JSON backlog created successfully.");
}

/// <summary>
/// Read a JSON file and return the JSON object
/// </summary>
/// <param name="sFileName_in"> File name of the JSON file to read </param>
/// <returns> JSON object read from the file </returns>
QJsonObject PlanningPoker::ReadJsonFile(const QString& sFileName_in)
{
    QFile fileJson(sFileName_in);
    if (!fileJson.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "Error", "Unable to open JSON file.");
        return QJsonObject();
    }

    QByteArray byteArrayJsonData = fileJson.readAll();
    QJsonDocument jsonDocumentReaded = QJsonDocument::fromJson(byteArrayJsonData);
    ValidJsonSelected();
    return jsonDocumentReaded.object();
}

/// <summary>
/// Update the label to show the selected JSON file
/// </summary>
void PlanningPoker::ValidJsonSelected(void)
{
    pLabelJsonName->setText(sJsonFileName);
}

/// <summary>
/// Write a JSON object to a file
/// </summary>
/// <param name="sFileName_in"> File name to write the JSON object to </param>
/// <param name="pJsonObjectToWrite_in"> JSON object to write to the file </param>
void PlanningPoker::WriteJsonFile(const QString& sFileName_in, const QJsonObject& pJsonObjectToWrite_in)
{
    QFile fileJson(sFileName_in);
    if (!fileJson.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, "Error", "Unable to write to JSON file.");
        return;
    }

    QJsonDocument jsonDocumentToWrite(pJsonObjectToWrite_in);
    fileJson.write(jsonDocumentToWrite.toJson());
}

/// <summary>
/// Visualize the JSON file in a dialog
/// </summary>
void PlanningPoker::VisualizeJson(void)
{
    if (sJsonFileName.isEmpty())
    {
        QMessageBox::warning(this, "No File", "No JSON file selected.");
        return;
    }

    QFile fileJson(sJsonFileName);
    if (!fileJson.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "Error", "Unable to open JSON file.");
        return;
    }

    QByteArray byteArrayjsonData = fileJson.readAll();
    QJsonDocument jsonDocumentToRead = QJsonDocument::fromJson(byteArrayjsonData);
    if (jsonDocumentToRead.isNull() || !jsonDocumentToRead.isObject())
    {
        QMessageBox::warning(this, "Error", "Unable to parse the JSON file.");
        return;
    }

    QJsonObject jsonObject = jsonDocumentToRead.object();
	JsonViewerDialog jsonViewerDialogDisplay(jsonObject, this);
    jsonViewerDialogDisplay.exec();
}