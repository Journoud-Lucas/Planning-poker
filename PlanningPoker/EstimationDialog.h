#pragma once

#include <QDialog>
#include <QPushButton>

const int COFFEE_CARD_VALUE = -1;

class EstimationDialog : public QDialog
{
    Q_OBJECT

public:
    EstimationDialog(const QString& sTaskDescription_in, int nRound_in, const QString& sPlayerName_in, const QVector<int>& nVecVoteNumber_in, QWidget* pWidgetParent_in = nullptr);
    int GetSelectedEstimate(void) const;

private slots:
    void SelectEstimate(int nEstimate_in);

private:
    int m_nSelectedEstimate;
};