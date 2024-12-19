#pragma once

#include <QJsonArray>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QVector>

class PlanningPoker : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlanningPoker(QWidget* parent = nullptr);

private slots:
    void ChooseJsonFile(void);
    void StartPokerSession(void);
    void CreateJson(void);
    void VisualizeJson(void);

private:
    QString m_sJsonFileName;
    QPushButton* m_pPushButtonStartSession;
    QLabel* m_pLabelJsonName;
    QJsonArray m_jsonArrayTasks;
    QPushButton* m_pPushButtonVisualizeJson;
    QVector<QString> m_sVecPlayerNames;

    QJsonObject ReadJsonFile(const QString& sFileName_in);
    void WriteJsonFile(const QString& sFileName_in, const QJsonObject& jsonObjectToWrite_in);
    void ValidJsonSelected(void);

    void UpdateResumeButtonState(const QJsonObject& jsonObjectFileContent_in);
};