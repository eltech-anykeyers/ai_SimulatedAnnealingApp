#include <simulated_annealing_app.hpp>
#include <QApplication>

int main( int argc, char* argv[] )
{
    QApplication app( argc, argv );
    SimulatedAnnealingApp mainWindow;
    mainWindow.show();

    return app.exec();
}
