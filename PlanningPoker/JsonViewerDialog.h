#pragma once

#include <QDialog>
#include <QTreeWidget>

class JsonViewerDialog : public QDialog {
    Q_OBJECT

public:
    explicit JsonViewerDialog(const QJsonObject& jsonObjectToView_in, QWidget* pWidgetParent_in = nullptr);

private:
    QTreeWidget* m_pTreeWidgetDisplay;
    QPushButton* m_pPushButtonCloseButton;

    void PopulateTasks(const QJsonArray& jsonArrayTaskToPopulate_in);
};