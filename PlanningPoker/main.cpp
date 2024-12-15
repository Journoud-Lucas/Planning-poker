#include <QtWidgets/QApplication>

#include "PlanningPoker.h"

/// <summary>
/// main function for the PlanningPoker application
/// </summary>
/// <param name="argc"> number of arguments </param>
/// <param name="argv"> arguments </param>
/// <returns> exit code </returns>
int main(int argc, char *argv[])
{
    QApplication applicationMain(argc, argv);
    PlanningPoker planningPokerWindow;
    planningPokerWindow.show();
    return applicationMain.exec();
}