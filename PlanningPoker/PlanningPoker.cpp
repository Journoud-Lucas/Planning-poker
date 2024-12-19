#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QInputDialog>
#include <QMessageBox>

#include "PlanningPoker.h"
#include "EstimationDialog.h"
#include "JsonViewerDialog.h"

/// <summary>
/// Create window for Planning Poker application
/// </summary>
/// <param name="pWidgetParent_in"> parent widget </param>
PlanningPoker::PlanningPoker(QWidget* pWidgetParent_in)
    : QMainWindow(pWidgetParent_in), m_sJsonFileName(""), m_jsonArrayTasks(QJsonArray())
{
    QWidget* pWidgetCentral = new QWidget(this);
    QVBoxLayout* pVBoxLayoutMain = new QVBoxLayout(pWidgetCentral);

    QHBoxLayout* pHBoxLayoutCenter = new QHBoxLayout();
    QPushButton* pPushButtonChooseJson = new QPushButton("Choose JSON File", this);
    m_pPushButtonStartSession = new QPushButton("Start a session", this);
    QPushButton* pPushButtonCreateJson = new QPushButton("Create JSON", this);
    m_pPushButtonVisualizeJson = new QPushButton("Visualize JSON", this);

    QFont fontButton;
    fontButton.setPointSize(14);
    fontButton.setBold(true);

    QList<QPushButton*> listPPushButtonbutton = { pPushButtonChooseJson, m_pPushButtonStartSession, pPushButtonCreateJson, m_pPushButtonVisualizeJson };
    for (QPushButton* pPushButton : listPPushButtonbutton)
    {
        pPushButton->setFont(fontButton);
        pPushButton->setStyleSheet(
            "QPushButton { background-color: #1976D2; color: white; border-radius: 5px; padding: 10px; }"
            "QPushButton:disabled { background-color: #FF4F4F; color: white; border-radius: 5px; }"
        );
        pHBoxLayoutCenter->addWidget(pPushButton, 0, Qt::AlignCenter);
    }

    pVBoxLayoutMain->addLayout(pHBoxLayoutCenter, Qt::AlignCenter);

    m_pLabelJsonName = new QLabel("No JSON files selected", this);
    QFont fontLabel;
    fontLabel.setPointSize(10);
    m_pLabelJsonName->setFont(fontLabel);
    m_pLabelJsonName->setStyleSheet("color: #666;");
    pVBoxLayoutMain->addWidget(m_pLabelJsonName, 0, Qt::AlignBottom | Qt::AlignCenter);

    m_pPushButtonStartSession->setEnabled(false);
    m_pPushButtonVisualizeJson->setEnabled(false);

    setFixedSize(800, 600);
    setStyleSheet("background-color: #F0F0F0;");
    setCentralWidget(pWidgetCentral);

    connect(pPushButtonChooseJson, &QPushButton::clicked, this, &PlanningPoker::ChooseJsonFile);
    connect(m_pPushButtonStartSession, &QPushButton::clicked, this, &PlanningPoker::StartPokerSession);
    connect(pPushButtonCreateJson, &QPushButton::clicked, this, &PlanningPoker::CreateJson);
    connect(m_pPushButtonVisualizeJson, &QPushButton::clicked, this, &PlanningPoker::VisualizeJson);
}

/// <summary>
/// Choose a JSON file to use for the Planning Poker session
/// </summary>
void PlanningPoker::ChooseJsonFile(void)
{
    QString sFileName = QFileDialog::getOpenFileName(this, "Select a JSON file", "", "JSON Files (*.json)");
    if (!sFileName.isEmpty())
    {
        m_sJsonFileName = sFileName;
        QJsonObject jsonObjectFileContent = ReadJsonFile(m_sJsonFileName);
        m_jsonArrayTasks = jsonObjectFileContent["tasks"].toArray();
        m_sVecPlayerNames = jsonObjectFileContent["playerNames"].toVariant().toStringList().toVector();
        ValidJsonSelected();
        UpdateResumeButtonState(jsonObjectFileContent);
    }
}

/// <summary>
/// Update the state of the resume button based on the JSON file content
/// </summary>
/// <param name="jsonObjectFileContent_in"> JSON object content </param>
void PlanningPoker::UpdateResumeButtonState(const QJsonObject& jsonObjectFileContent_in)
{
    m_pPushButtonStartSession->setEnabled(true);
    m_pPushButtonVisualizeJson->setEnabled(true);

    if (jsonObjectFileContent_in.contains("finished") && jsonObjectFileContent_in["finished"].toBool())
    {
        m_pPushButtonStartSession->setEnabled(false);
        QMessageBox::information(this, "Session Complete", "The session for this file is complete.");
        m_pPushButtonStartSession->setText("Start a session");
    }
    else if (jsonObjectFileContent_in.contains("mode"))
    {
        m_pPushButtonStartSession->setText("Resume Session");
    }
    else
    {
        m_pPushButtonStartSession->setText("Start a session");
    }
}

/// <summary>
/// Start the Planning Poker session
/// </summary>
void PlanningPoker::StartPokerSession(void)
{
    bool bOk = false;
    QString sChosenMode;
    QJsonObject jsonObjectUpdated;

    QJsonObject jsonObjectSession = ReadJsonFile(m_sJsonFileName);

    if (m_pPushButtonStartSession->text() == "Resume Session")
    {
        if (jsonObjectSession.contains("mode"))
        {
            sChosenMode = jsonObjectSession["mode"].toString();
            jsonObjectUpdated["mode"] = sChosenMode;
            m_sVecPlayerNames = jsonObjectSession["playerNames"].toVariant().toStringList().toVector();
            jsonObjectUpdated["playerNames"] = QJsonArray::fromStringList(m_sVecPlayerNames.toList());
        }
        else
        {
            QMessageBox::warning(this, "Error", "Session mode not found. Cannot resume session.");
            return;
        }
    }
    else
    {
        m_sVecPlayerNames.clear();
        int nPlayerCounter = 1;
        while (true)
        {
            QString playerName = QInputDialog::getText(this, "Player Name Input", QString("Enter the name for player %1 (Leave empty to finish):").arg(nPlayerCounter));
            if (playerName.isEmpty())
            {
                break;
            }
            m_sVecPlayerNames.append(playerName);
            nPlayerCounter++;
        }

        if (m_sVecPlayerNames.isEmpty())
        {
            QMessageBox::warning(this, "Error", "No players added. The session cannot start.");
            return;
        }

        QStringList sListModes = { "Strict", "Average" };
        sChosenMode = QInputDialog::getItem(this, "Mode Selection", "Choose a mode:", sListModes, 0, false, &bOk);

        if (!bOk) return;

        jsonObjectUpdated["mode"] = sChosenMode;
        jsonObjectUpdated["playerNames"] = QJsonArray::fromStringList(m_sVecPlayerNames.toList());
    }

    m_jsonArrayTasks = jsonObjectSession["tasks"].toArray();
    jsonObjectUpdated["tasks"] = m_jsonArrayTasks;

    QVector<int> nVecNumberForVote = { 0, 1, 2, 3, 5, 8, 13, 20, 40, 100, COFFEE_CARD_VALUE };
    QVector<int> nVecEstimates(m_sVecPlayerNames.size());

    int nResumeTaskIndex = 0;
    for (int i = 0; i < m_jsonArrayTasks.size(); ++i)
    {
        QJsonObject jsonObjectTask = m_jsonArrayTasks[i].toObject();
        if (!jsonObjectTask.contains("note") || jsonObjectTask["note"].isNull())
        {
            nResumeTaskIndex = i;
            break;
        }
    }

    for (int i = nResumeTaskIndex; i < m_jsonArrayTasks.size(); ++i)
    {
        QJsonObject jsonObjectTask = m_jsonArrayTasks[i].toObject();
        QString sTaskDescription = jsonObjectTask["description"].toString();
        int nRoundCounter = 0;

        bool bConsensusReached = false;
        while (!bConsensusReached)
        {
            nRoundCounter++;
            for (int j = 0; j < m_sVecPlayerNames.size(); ++j)
            {
                EstimationDialog estimationDialogPlayer(sTaskDescription, nRoundCounter, m_sVecPlayerNames[j], nVecNumberForVote, this);
                if (estimationDialogPlayer.exec() == QDialog::Accepted)
                {
                    int nEstimate = estimationDialogPlayer.GetSelectedEstimate();
                    if (nEstimate == COFFEE_CARD_VALUE)
                    {
                        WriteJsonFile(m_sJsonFileName, jsonObjectUpdated);
                        QMessageBox::information(this, "Coffee Break", "A coffee card was selected. Session paused.");

                        jsonObjectSession = ReadJsonFile(m_sJsonFileName);
                        m_jsonArrayTasks = jsonObjectSession["tasks"].toArray();
                        UpdateResumeButtonState(jsonObjectSession);
                        return;
                    }
                    nVecEstimates[j] = nEstimate;
                }
                else
                {
                    WriteJsonFile(m_sJsonFileName, jsonObjectUpdated);
                    jsonObjectSession = ReadJsonFile(m_sJsonFileName);
                    m_jsonArrayTasks = jsonObjectSession["tasks"].toArray();
                    UpdateResumeButtonState(jsonObjectSession);
                    return;
                }
            }

            if (sChosenMode == "Strict")
            {
                bConsensusReached = std::all_of(nVecEstimates.begin(), nVecEstimates.end(),
                    [&](int est) { return est == nVecEstimates[0]; });
                if (!bConsensusReached)
                {
                    QMessageBox::information(this, "New Round", "The estimates are not unanimous. Starting a new round.");
                }
            }
            else if (sChosenMode == "Average")
            {
                bConsensusReached = std::all_of(nVecEstimates.begin(), nVecEstimates.end(),
                    [&](int est) { return (est == nVecEstimates[0]) || (nRoundCounter >= 2); });
                if (!bConsensusReached)
                {
                    QMessageBox::information(this, "New Round", "The estimates are not unanimous. Starting a new (last) round.");
                }
            }
        }

        if (sChosenMode == "Average")
        {
            int nSum = std::accumulate(nVecEstimates.begin(), nVecEstimates.end(), 0);
            float nAverage = static_cast<float>(nSum) / static_cast<float>(nVecEstimates.size());
            jsonObjectTask["note"] = nAverage;
            QMessageBox::information(this, "Round Finish", QString("The task estimation is: ") + QString::number(nAverage));
        }
        else
        {
            jsonObjectTask["note"] = nVecEstimates[0];
            QMessageBox::information(this, "Round Finish", QString("The task estimation is: ") + QString::number(nVecEstimates[0]));
        }

        m_jsonArrayTasks[i] = jsonObjectTask;
        jsonObjectUpdated["tasks"] = m_jsonArrayTasks;
        WriteJsonFile(m_sJsonFileName, jsonObjectUpdated);
    }

    jsonObjectUpdated["finished"] = true;
    WriteJsonFile(m_sJsonFileName, jsonObjectUpdated);

    QMessageBox::information(this, "Session Complete", "All tasks are now estimated and the session is completed.");
    jsonObjectSession = ReadJsonFile(m_sJsonFileName);
    UpdateResumeButtonState(jsonObjectSession);
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
        sFilename = QInputDialog::getText(this, "File Name Input", "Enter the name for the new JSON file:", QLineEdit::Normal, "", &bOk);

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
            QMessageBox::warning(this, "File Exists", "A file with this name already exists. Please choose a different name.");
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
        bool bOk;
        QString sDescription = QInputDialog::getText(this, "Task Input", "Enter a description for task " + QString::number(nTaskCounter) + " (Leave empty to finish):", QLineEdit::Normal, "", &bOk);

        if (bOk && !sDescription.isEmpty())
        {
            QJsonObject jsonObjectNewTask;
            jsonObjectNewTask["description"] = sDescription;
            jsonObjectNewTask["note"] = QJsonValue();
            jsonArrayTask.append(jsonObjectNewTask);
            nTaskCounter++;
        }
        else
        {
            bContinueAddingTasks = false;
        }
    }

    jsonObjectData["tasks"] = jsonArrayTask;
    jsonObjectData["finished"] = false;
    WriteJsonFile(sFilename, jsonObjectData);

    QMessageBox::information(this, "New JSON", "JSON backlog created successfully.");

    m_sJsonFileName = sFilename;
    QJsonObject jsonObjectFileContent = ReadJsonFile(m_sJsonFileName);
    m_jsonArrayTasks = jsonObjectFileContent["tasks"].toArray();
    ValidJsonSelected();
    UpdateResumeButtonState(jsonObjectFileContent);
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
    QJsonDocument jsonDocumentReaded_out = QJsonDocument::fromJson(byteArrayJsonData);
    ValidJsonSelected();
    return jsonDocumentReaded_out.object();
}

/// <summary>
/// Update the label to show the selected JSON file
/// </summary>
void PlanningPoker::ValidJsonSelected(void)
{
    m_pLabelJsonName->setText(m_sJsonFileName);
}

/// <summary>
/// Write a JSON object to a file
/// </summary>
/// <param name="sFileName_in"> File name to write the JSON object to </param>
/// <param name="pJsonObjectToWrite_in"> JSON object to write to the file </param>
void PlanningPoker::WriteJsonFile(const QString& sFileName_in, const QJsonObject& jsonObjectToWrite_in)
{
    QFile fileJson(sFileName_in);
    if (!fileJson.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, "Error", "Unable to write to JSON file.");
        return;
    }

    QJsonDocument jsonDocumentToWrite(jsonObjectToWrite_in);
    fileJson.write(jsonDocumentToWrite.toJson());
}

/// <summary>
/// Visualize the JSON file in a dialog
/// </summary>
void PlanningPoker::VisualizeJson(void)
{
    if (m_sJsonFileName.isEmpty())
    {
        QMessageBox::warning(this, "No File", "No JSON file selected.");
        return;
    }

    QFile fileJson(m_sJsonFileName);
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

    QJsonObject jsonObjectToView = jsonDocumentToRead.object();
    JsonViewerDialog jsonViewerDialogDisplay(jsonObjectToView, this);
    jsonViewerDialogDisplay.exec();
}