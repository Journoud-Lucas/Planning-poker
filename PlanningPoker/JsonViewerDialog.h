#pragma once

#include <QDialog>
#include <QTreeWidgetItem>

class JsonViewerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit JsonViewerDialog(const QJsonObject& jsonObjectToView_in, QWidget* pWidgetParent_in = nullptr);

private:
    QTreeWidget* pTreeWidgetDisplay;
    QPushButton* pPushButtonCloseButton;

    void PopulateTasks(const QJsonArray& jsonArrayTaskToPopulate_in);
};