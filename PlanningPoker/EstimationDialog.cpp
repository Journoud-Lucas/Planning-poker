#include <QSignalMapper>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "EstimationDialog.h"

/// <summary>
/// Create dialog for selecting the estimate by the player
/// </summary>
/// <param name="taskDescription"> description of the task </param>
/// <param name="round"> actual round </param>
/// <param name="playerNumber"> actual player number </param>
/// <param name="nVecVoteNumber_in"> vector of possible vote numbers </param>
/// <param name="parent"> parent widget </param>
EstimationDialog::EstimationDialog(const QString& sTaskDescription_in, int nRound_in, const QString& sPlayerName_in, const QVector<int>& nVecVoteNumber_in, QWidget* pWidgetParent_in)
    : QDialog(pWidgetParent_in), m_nSelectedEstimate(0)
{
    setWindowTitle("Estimation");

    QLabel* pLabelTask = new QLabel("Task: " + sTaskDescription_in, this);
    QLabel* pLabelRound = new QLabel("Round: " + QString::number(nRound_in), this);
    QLabel* pLabelPlayer = new QLabel("Player: " + sPlayerName_in, this);

    QVBoxLayout* pVBoxLayoutMain = new QVBoxLayout(this);
    pVBoxLayoutMain->addWidget(pLabelTask);
    pVBoxLayoutMain->addWidget(pLabelRound);
    pVBoxLayoutMain->addWidget(pLabelPlayer);

    QHBoxLayout* pHBoxLayoutCards = new QHBoxLayout();

    QSignalMapper* pSignalMapper = new QSignalMapper(this);

    for (int nVoteNumberToAdd : nVecVoteNumber_in)
    {
        QPushButton* pButtonCard = new QPushButton(this);
        QString sCardFilePath;
        if (nVoteNumberToAdd == COFFEE_CARD_VALUE)
        {
            sCardFilePath = ":/img/cards/cafe.svg";
        }
        else
        {
            sCardFilePath = ":/img/cards/" + QString::number(nVoteNumberToAdd) + ".svg";
        }

        QPixmap pixmapCard(sCardFilePath);
        pButtonCard->setIcon(QIcon(pixmapCard));
        pButtonCard->setIconSize(QSize(100, 150));

        pHBoxLayoutCards->addWidget(pButtonCard);

        pSignalMapper->setMapping(pButtonCard, nVoteNumberToAdd);
        connect(pButtonCard, SIGNAL(clicked()), pSignalMapper, SLOT(map()));
    }

    connect(pSignalMapper, SIGNAL(mappedInt(int)), this, SLOT(SelectEstimate(int)));

    pVBoxLayoutMain->addLayout(pHBoxLayoutCards);
    setLayout(pVBoxLayoutMain);

    if (!nVecVoteNumber_in.isEmpty())
    {
        m_nSelectedEstimate = nVecVoteNumber_in[0];
    }
}

/// <summary>
/// Return the selected estimate by the player
/// </summary>
/// <returns> the selected estimate </returns>
int EstimationDialog::GetSelectedEstimate() const
{
    return m_nSelectedEstimate;
}

/// <summary>
/// Slot for selecting the estimate by the player
/// </summary>
/// <param name="nEstimate_in"> the selected estimate </param>
void EstimationDialog::SelectEstimate(int nEstimate_in)
{
    m_nSelectedEstimate = nEstimate_in;
    accept();
}