#ifndef SIMULATED_ANNEALING_APP_HPP
#define SIMULATED_ANNEALING_APP_HPP

#include <QMainWindow>

class SimulatedAnnealingApp : public QMainWindow
{
    Q_OBJECT

public:
    SimulatedAnnealingApp( QWidget* parent = Q_NULLPTR );
    virtual ~SimulatedAnnealingApp() override = default;
};

#endif /// SIMULATED_ANNEALING_APP_HPP
