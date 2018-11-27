#ifndef SIMULATED_ANNEALING_WIDGET_HPP
#define SIMULATED_ANNEALING_WIDGET_HPP

#include <QWidget>
#include <QTableWidget>
#include <QTextEdit>

#include <QVector>

class SimulatedAnnealingWidget : public QWidget
{
    Q_OBJECT

public:
    SimulatedAnnealingWidget( QWidget* parent = Q_NULLPTR );
    virtual ~SimulatedAnnealingWidget() override = default;

public slots:
    void findPath();
    void addVerticle();
    void removeVerticle();
    void updateTable();

protected:
    qreal calculateEnergy( const QVector< qint32 >& path ) const;
    QVector< qint32 > generateStateCandidate( const QVector< qint32 >& prev ) const;
    qreal decreaseTemperature( const qreal initialTemperature, qint32 iterNo ) const;
    bool needToDoTransition( const qreal dE, const qreal temp ) const;

private:
    qint32 nVerticles;

    QTableWidget* inputTable;
    QTextEdit* outputWidget;
};

#endif /// SIMULATED_ANNEALING_WIDGET_HPP
