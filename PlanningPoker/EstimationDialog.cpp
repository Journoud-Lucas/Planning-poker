#include "EstimationDialog.h"

/// <summary>
/// Create dialog for selecting the estimate by the player
/// </summary>
/// <param name="taskDescription"> description of the task </param>
/// <param name="round"> actual round </param>
/// <param name="playerNumber"> actual player number </param>
/// <param name="nVecVoteNumber_in"> vector of possible vote numbers </param>
/// <param name="parent"> parent widget </param>
EstimationDialog::EstimationDialog(const QString& taskDescription, int round, int playerNumber, const QVector<int>& nVecVoteNumber_in, QWidget* parent)
    : QDialog(parent), nSelectedEstimate(0)
{
    setWindowTitle("Estimation");

    pLabelTask = new QLabel("Task: " + taskDescription, this);
    pLabelRound = new QLabel("Round: " + QString::number(round), this);
    pLabelPlayer = new QLabel("Player: " + QString::number(playerNumber), this);

    pComboBoxEstimate = new QComboBox(this);
    for (int nVoteNumberToAdd : nVecVoteNumber_in)
    {
        pComboBoxEstimate->addItem(QString::number(nVoteNumberToAdd));
    }
    connect(pComboBoxEstimate, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [this, nVecVoteNumber_in](int index) { nSelectedEstimate = nVecVoteNumber_in[index]; });

    QPushButton* pPushButtonOk = new QPushButton("OK", this);
    connect(pPushButtonOk, &QPushButton::clicked, this, &QDialog::accept);

    QVBoxLayout* pVBoxLayoutMain = new QVBoxLayout(this);
    pVBoxLayoutMain->addWidget(pLabelTask);
    pVBoxLayoutMain->addWidget(pLabelRound);
    pVBoxLayoutMain->addWidget(pLabelPlayer);
    pVBoxLayoutMain->addWidget(pComboBoxEstimate);
    pVBoxLayoutMain->addWidget(pPushButtonOk);

    setLayout(pVBoxLayoutMain);

    nSelectedEstimate = nVecVoteNumber_in[0];
}

/// <summary>
/// Return the selected estimate by the player
/// </summary>
/// <returns> the selected estimate </returns>
int EstimationDialog::GetSelectedEstimate() const
{ 
    return nSelectedEstimate;
}