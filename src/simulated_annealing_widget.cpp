#include <simulated_annealing_widget.hpp>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QHeaderView>

#include <QRandomGenerator>
#include <QtMath>
#include <QtDebug>

SimulatedAnnealingWidget::SimulatedAnnealingWidget( QWidget* parent )
    : QWidget( parent )
    , nVerticles( 5 )
{

    auto plusButton = new QPushButton( "+" );
    connect( plusButton, &QPushButton::clicked,
             this, &SimulatedAnnealingWidget::addVerticle );

    auto minusButton = new QPushButton( "-" );
    connect( minusButton, &QPushButton::clicked,
             this, &SimulatedAnnealingWidget::removeVerticle );

    auto findPathButton = new QPushButton( "Find path" );
    connect( findPathButton, &QPushButton::clicked,
             this, &SimulatedAnnealingWidget::findPath );

    auto buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget( plusButton );
    buttonsLayout->addWidget( minusButton );
    buttonsLayout->addWidget( findPathButton );

    inputTable = new QTableWidget( nVerticles, nVerticles );
    inputTable->horizontalHeader()->setDefaultSectionSize( 30 );
    inputTable->verticalHeader()->setDefaultSectionSize( 30 );
    inputTable->horizontalHeader()->setSectionResizeMode( QHeaderView::Fixed );
    inputTable->verticalHeader()->setSectionResizeMode( QHeaderView::Fixed );
    updateTable();
    connect( inputTable, &QTableWidget::cellChanged,
             [ this ]( qint32, qint32 ){ this->updateTable(); } );

    auto inputLayout = new QVBoxLayout();
    inputLayout->addWidget( inputTable );
    inputLayout->addLayout( buttonsLayout );

    outputWidget = new QTextEdit();
    outputWidget->setReadOnly( true );

    auto mainLayout = new QHBoxLayout();
    mainLayout->addLayout( inputLayout );
    mainLayout->addWidget( outputWidget );

    this->setLayout( mainLayout );
}

void SimulatedAnnealingWidget::findPath()
{    
    QVector< qint32 > currPath( nVerticles );
    std::iota( currPath.begin(), currPath.end(), 0 );

    static const qreal MAX_TEMP = 10.0;
    static const qreal MIN_TEMP = 0.00001;

    qreal currTemp = MAX_TEMP;
    qreal currEnergy = calculateEnergy( currPath );
    qint32 iterNo = 0;

    while( currTemp > MIN_TEMP )
    {
        auto nextPath = generateStateCandidate( currPath );
        auto nextEnergy = calculateEnergy( nextPath );

        if( nextEnergy < 0.0 ) throw std::logic_error( "bad path" );

        if( nextEnergy < currEnergy || needToDoTransition( nextEnergy - currEnergy, currTemp ) )
        {
            currEnergy = nextEnergy;
            currPath = nextPath;
        }
        currTemp = decreaseTemperature( MAX_TEMP, iterNo++ );
    }

    QString resultPath;
    for( const auto& vertex : currPath )
    {
        resultPath.append( QString( "%1 " ).arg( vertex ) );
    }
    outputWidget->clear();
    outputWidget->append( resultPath );
}


void SimulatedAnnealingWidget::addVerticle()
{
    nVerticles = std::min( nVerticles + 1, 10 );
    updateTable();
}

void SimulatedAnnealingWidget::removeVerticle()
{
    nVerticles = std::max( nVerticles - 1, 0 );
    updateTable();
}

void SimulatedAnnealingWidget::updateTable()
{
    inputTable->setColumnCount( nVerticles );
    inputTable->setRowCount( nVerticles );

    for( qint32 i = 0; i < nVerticles; ++i )
    {
        for( qint32 j = 0; j < nVerticles; ++j )
        {
            auto item = inputTable->item( i, j );
            if( !item )
            {
                inputTable->setItem( i, j, new QTableWidgetItem() );
                item = inputTable->item( i, j );
            }

            if( i == j )
            {
                item->setText( "0.0" );
            }
            else
            {
                if( item->text().isEmpty() )
                {
                    item->setText( "1.0" );
                }
            }
        }
    }
}

qreal SimulatedAnnealingWidget::calculateEnergy(
        const QVector< qint32 >& path ) const
{
    if( path.size() <= 1 ) return 0.0;

    bool ok;
    qreal result = 0.0;

    qint32 from, to = path[ 0 ];
    if( to >= nVerticles ) return -1.0;
    for( qint32 i = 1; i < path.size(); ++i )
    {
        from = to; to = path[ i ];
        if( to >= nVerticles ) return -1.0;

        auto cellText = inputTable->item( from, to )->text();
        auto cellValue = cellText.toDouble( &ok );
        if( !ok ) return -1.0;

        result += cellValue;
    }

    from = to; to = path[ 0 ];
    auto cellText = inputTable->item( from, to )->text();
    auto cellValue = cellText.toDouble( &ok );
    if( !ok ) return -1.0;

    result += cellValue;

    return result;
}

QVector< qint32 > SimulatedAnnealingWidget::generateStateCandidate(
        const QVector< qint32 >& prev ) const
{
    auto rndIndex1 = std::abs( qint32( QRandomGenerator::global()->generate() ) ) % prev.size();
    auto rndIndex2 = std::abs( qint32( QRandomGenerator::global()->generate() ) ) % prev.size();

    auto result = prev;
    std::swap( result[ rndIndex1 ], result[ rndIndex2 ] );
    return result;
}

qreal SimulatedAnnealingWidget::decreaseTemperature(
        const qreal initialTemperature, qint32 iterNo ) const
{
    return initialTemperature * 0.1 / iterNo;
}

bool SimulatedAnnealingWidget::needToDoTransition(
        const qreal dE, const qreal temp ) const
{
    static auto probability = []( const qreal dE, const qreal temp ){
        return qExp( -dE / temp );
    };

    auto val = QRandomGenerator::global()->generateDouble();

    return val < probability( dE, temp );
}
