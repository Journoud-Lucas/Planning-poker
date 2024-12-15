#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QJsonArray>
#include <QLabel>

class PlanningPoker : public QMainWindow {
    Q_OBJECT

public:
    explicit PlanningPoker(QWidget* parent = nullptr);

private slots:
    void ChooseJsonFile(void);
    void StartPokerSession(void);
    void CreateJson(void);
    void VisualizeJson(void);

private:
    QString sJsonFileName;
    QPushButton* pPushButtonStartSession;
    QLabel* pLabelJsonName;
    QJsonArray jsonArrayTasks;
    QPushButton* pPushButtonVisualizeJson;

    QJsonObject ReadJsonFile(const QString& sFileName_in);
    void WriteJsonFile(const QString& sFileName_in, const QJsonObject& pJsonObjectToWrite_in);
    void ValidJsonSelected(void);
};