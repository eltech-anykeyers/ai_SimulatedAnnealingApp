#include <simulated_annealing_app.hpp>
#include <simulated_annealing_widget.hpp>

SimulatedAnnealingApp::SimulatedAnnealingApp( QWidget* parent )
    : QMainWindow( parent )
{
    auto widget = new SimulatedAnnealingWidget();

    this->setCentralWidget( widget );
    this->setWindowTitle( "Simulated Annealing" );
}
