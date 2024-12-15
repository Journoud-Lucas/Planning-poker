#pragma once

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>

class EstimationDialog : public QDialog
{
    Q_OBJECT

public:
    EstimationDialog(const QString& sTaskDescription_in, int nRound_in, int nPlayerNumber_in, const QVector<int>& nVecVoteNumber_in, QWidget* pWidgetParent_in = nullptr);
    int GetSelectedEstimate(void) const;

private:
    int nSelectedEstimate;
    QLabel* pLabelTask;
    QLabel* pLabelRound;
    QLabel* pLabelPlayer;
    QComboBox* pComboBoxEstimate;
};