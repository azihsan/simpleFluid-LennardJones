#include "mdsimulator.h"
#include "helpwidget.h"
#include "ui_mdsimulator.h"

mdSimulator::mdSimulator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mdSimulator)
{

    ui->setupUi(this);


    // Connection of set default parameters from default button UI signal
    connect( ui->defaultButton, SIGNAL(clicked()), this, SLOT(setDefault()) );

    // Connection set user defined parameters from setParamButton UI signal
    connect( ui->setParamButton, SIGNAL(clicked()), this, SLOT(setParameters()) );

    // Connection for calling help widget from helpButton UI signal
    connect( ui->helpButton, SIGNAL(clicked()), this, SLOT(openHelpWidget()) );

    // Connection for parameter setting
    connect( ui->simulateButton, SIGNAL(clicked()), this, SLOT(startSimulate()));

    //connetction for stop button
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(closeSimulation()));



    // To disable simulate button in the beginning of the widget show due to no input available in the beginning
    ui->simulateButton->setDisabled(true);
    ui->closeButton->setDisabled(true);

}

void mdSimulator::simulate()
{
   /* ----------------------------------------------------------------------
     This is the main engine for simulation, atoms object which have been defined defined will be simulated with MD Functions
    ----------------------------------------------------------------------*/

         double totalPE, totalKE, totalEn ; // dummy variable for storing potential energy, kinetic energy, and total energy

         reset(atom, n_atom); // reset acceleration, potential, and kinetic energy

         leapfrog(atom, timeStep, density, n_atom, 0); // half step integration of acceleration

         wrapping_all(atom,density, n_atom); // wrapping atom in order to stay in the box

         pot_force(atom, density, n_atom); // calculation potential energy and force of atoms

         leapfrog(atom, timeStep, density, n_atom, 1); // the rest half step integraion of acceleration

         totalPE = totalPotentialEnergyCalc(atom, n_atom); // calculation for total potential energy

         totalKE = totalKineticEnergyCalc(atom, n_atom); // calculation for total kinetic energy

         totalEn = totalEnergyCalc(atom, n_atom); // calculation for total energy

         // Below we will see assign the result of the calculation to Graphical repsentation, there is a multiplying factor
         // called realRegion to translate from molecule representation to graphical representation.
         for(int i = 0 ; i<n_atom*n_atom; i++){
             // realRegion is variable to translate to QGraphicsView. a 420 factor is length of QGraphicsScene.
             qatom[i].setPosGraphic((float)atom[i].getXPosition()  *realRegion,(float)atom[i].getYPosition() *realRegion);
         }

         // Below is real time plotting
         static QTime time(QTime::currentTime());
         timeElapsed = time.elapsed()/100; // time elapsed since start of demo

         // plotting the data
         ui->totalEnergyPlot->graph(0)->addData(timeElapsed, totalEn);
         ui->KEPlot->graph(0)->addData(timeElapsed, totalKE);
         ui->PEPlot->graph(0)->addData(timeElapsed, totalPE);

        // make key axis range scroll with the data (at a constant range size of 8):
         ui->totalEnergyPlot->xAxis->setRange(timeElapsed, 8, Qt::AlignRight);
         ui->totalEnergyPlot->graph(0)->rescaleAxes(); //as a data will vary during simulation thus rescaling is important
         ui->totalEnergyPlot->replot(); //replot to fit the new data
         ui->KEPlot->xAxis->setRange(timeElapsed, 8, Qt::AlignRight);
         ui->KEPlot->graph(0)->rescaleAxes(); //as a data will vary during simulation thus rescaling is important
         ui->KEPlot->replot(); //replot to fit the new data
         ui->PEPlot->xAxis->setRange(timeElapsed, 8, Qt::AlignRight);
         ui->PEPlot->graph(0)->rescaleAxes(); //as a data will vary during simulation thus rescaling is important
         ui->PEPlot->replot(); //replot to fit the new data


        // this static member of QTimer called singleShot that only send one time signal
        // this signal will be sent to advance slot owned by scene.
        // later this slot will give a signal QGraphicsItem::advance(int phase) to move the item or in this simulation we call qatom
        QTimer::singleShot(0,scene,SLOT(advance()));

}

//slot definition for close the simulation
void mdSimulator::closeSimulation()
{
   delete atom;
   delete ui;
}

// get method to get parameters from user interface, below there is a checking validity of input parameter
void mdSimulator::getParameters()
{
    //-------------------------------------------------------------------------
    // Getting the user Parameters from the lineEdits and checking it for validity and limit
    //-------------------------------------------------------------------------
    bool ok;
    int i = 0 ; // for flag error

    // get input from UI for atom(s)
    // only integer is allowed
    ui->nAtomInp->selectAll();
    QString atomInput=ui->nAtomInp->selectedText();
    n_atom = atomInput.toInt(&ok);
    // minimum value that can be inputted
    if (!ok || n_atom<=1){
        QMessageBox::information(
           this, "Invalid Input", "Please enter a valid value for n atom or it will be changed to default value");
        ui->nAtomInp->setText("10"); // default input for N (NXN atoms)
        n_atom = 10;
        i = 1 ;
    }
    // maximum value that can be inputted
    if (n_atom>20){
        QMessageBox::information(
           this, "Sorry", "The total NxN atoms is limited  to N = 30. It will be changed to default value");
        ui->nAtomInp->setText("10"); // default input for N (NXN atoms)
        n_atom = 10;
        i = 1 ;

    }

    // get input from UI for density
    // get double variable
    ui->densityInp->selectAll();
    QString densityInput = ui->densityInp->selectedText();
    density= densityInput.toDouble(&ok);
    // minimum value that can be inputted
    if (!ok || density <= 0){
        QMessageBox::information(
           this, "Invalid Input", "Please enter a valid value for density ");
        ui->densityInp->setText("0.8"); // default input for density of the system
        density = 0.8;
        i = 1 ;

    }
    // maximum value that can be inputted
    if (density > 2){
        QMessageBox::information(
           this, "Sorry", "The value of density is limited to 2. It will be changed to default value");
        ui->densityInp->setText("0.8"); // default input for density of the system
        density = 0.8;
        i = 1 ;

    }

    // get input from UI for temperature
    // get double variable
    ui->temperatureInp->selectAll();
    QString temperatureInput=ui->temperatureInp->selectedText();
    temperature = temperatureInput.toDouble(&ok);
    // minimum value that can be inputted
    if (!ok || temperature<=0){
        QMessageBox::information(
           this, "Invalid Input", "Please enter a valid value temperature or it will be changed to default value");
        ui->temperatureInp->setText("1"); // default input for temperature
        temperature = 1;
        i = 1 ;
    }

    // maximum value that can be inputted
    if (temperature>3){
        QMessageBox::information(
           this, "Sorry", "The maximum temperature is 3. It will be changed to default value");
        ui->temperatureInp->setText("1"); // default input for temperature
        temperature = 1;
        i = 1 ;

    }
    // get input from UI for time step
    // get double variable
    ui->timeStepInp->selectAll();
    QString timeStepInput=ui->timeStepInp->selectedText();
    timeStep= timeStepInput.toDouble(&ok);
     // minimum value that can be inputted
    if (!ok || timeStep<=0){
        QMessageBox::information(
           this, "Invalid Input", "Please enter a valid value time step or it will be changed to default value");
        ui->timeStepInp->setText("0.003"); // default input for time step
        timeStep = 0.003;
        i = 1 ;
    }

    // maximum value that can be inputted
    if (timeStep>0.1){
        QMessageBox::information(
         this, "Sorry", "The time step is limited maximally 0.1. It will be changed to default value.");
        ui->timeStepInp->setText("0.003"); // default input for time step
        timeStep = 0.003;
        i = 1 ;
    }

    //check for error flag
    if (i == 0){
    init_scene();
    ui->setParamButton->setDisabled(true);
    ui->defaultButton->setDisabled(true);
    ui->simulateButton->setEnabled(true);
    }

}

void mdSimulator::init_scene(){

    //alocate atoms deriving from molecule class
        atom = new molecule[n_atom*n_atom];

    //set scene by scene of atomic movement
        scene = new QGraphicsScene(this);
        ui->simulationView->setScene(scene);

    //allocate Graphical Atom(qatom) deriving from molecule class
        qatom = new myAtom[n_atom*n_atom];

        //set anti-aliasing for movement of atom in order to get the smoothness of movement
        ui->simulationView->setRenderHint(QPainter::Antialiasing);


        //defining scene area
        scene->setSceneRect(420,10,590,470);
        QPen mypen = QPen(Qt::red);

        init_pos(atom, density, n_atom); //initiation of position each atom

        realRegion = 460 / region() ; // translate the region of defined system to Graphics View

         // construct the graphics atom items as well as passing position value

        for( int i = 0; i<n_atom * n_atom ;i++){

            //set position of graphical atom(qatom) then add into the scene
            qatom[i].setPosGraphic((float)atom[i].getXPosition() *realRegion,  (float)atom[i].getYPosition()*realRegion);
            scene->addItem(&qatom[i]);
         }

         for (int i = 0; i<n_atom*n_atom; i++){
            // assigning velocity to each atom based on pseudorandom
             pseudorand(&atom[i], temperature, n_atom);

         }
}

//public slot for doing simulation, it will receive the signal from simulate button in UI
void mdSimulator::startSimulate()
{
    setupRealTimePlot(); //function for setting up the plot

    //disable simulate, default, set parameters button while simulation is running
    ui->simulateButton->setDisabled(true);
    ui->defaultButton->setDisabled(true);
    ui->setParamButton->setDisabled(true);

    // enable the close button
    ui->closeButton->setEnabled(true);

    // this is the connect with timer
    // for every 1000ms timer will send signal to widget slot called simulate() to do simulation.
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(simulate())) ;
    timer->start(100);
}

//public slot for default setting
void mdSimulator::getDefault()
{
    setDefault(); // call set default function
}

void mdSimulator::setDefault() //all the input will be set to default by this method
{


    ui->nAtomInp->setText("10"); // default input for N (NXN atoms)
    n_atom = 10;

    ui->densityInp->setText("0.8"); // default input for density of the system
    density = 0.8;

    ui->temperatureInp->setText("1.4"); // default input for temperature
    temperature = 1.4;

    ui->timeStepInp->setText("0.003"); // default input for time step
    timeStep = 0.003;

    ui->simulateButton->setEnabled(true);
    ui->defaultButton->setDisabled(true);
    ui->setParamButton->setDisabled(true);

    init_scene();
}

double mdSimulator::region()
{
    return (1./sqrt(density)) * n_atom ;
}

void mdSimulator::setParameters() // set method for user-defined parameters
{
    getParameters(); // call
}

void mdSimulator::setupRealTimePlot()
{

    ui->totalEnergyPlot->addGraph(); //adding graph to totalEnergyPlot
    ui->totalEnergyPlot->graph(0)->setPen(QPen(QColor(40, 110, 255))); //generate a blue line for plotting
    ui->totalEnergyPlot->graph(0)->setName("Total Energy");
    ui->KEPlot->addGraph(); //adding graph to totalEnergyPlot
    ui->KEPlot->graph(0)->setPen(QPen(QColor(204, 0, 0))); //generate a red line for plotting
    ui->KEPlot->graph(0)->setName("Kinetic Energy");
    ui->PEPlot->addGraph(); //adding graph to totalEnergyPlot
    ui->PEPlot->graph(0)->setPen(QPen(QColor(0, 204, 0))); //generate a green line for plotting
    ui->PEPlot->graph(0)->setName("Potential Energy");

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%s");
    ui->totalEnergyPlot->xAxis->setTicker(timeTicker);
    ui->totalEnergyPlot->axisRect()->setupFullAxesBox();
    ui->totalEnergyPlot->xAxis->setLabel("time (MD unit)");
    ui->totalEnergyPlot->yAxis->setLabel("Energy(MD Unit)");

    ui->PEPlot->xAxis->setTicker(timeTicker);
    ui->PEPlot->axisRect()->setupFullAxesBox();
    ui->PEPlot->xAxis->setLabel("time (MD unit)");
    ui->PEPlot->yAxis->setLabel("Energy(MD Unit)");

    ui->KEPlot->xAxis->setTicker(timeTicker);
    ui->KEPlot->axisRect()->setupFullAxesBox();
    ui->KEPlot->xAxis->setLabel("time (MD unit)");
    ui->KEPlot->yAxis->setLabel("Energy(MD Unit)");


    ui->totalEnergyPlot->plotLayout()->insertRow(0);
    ui->totalEnergyPlot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->totalEnergyPlot, "Energy Plot", QFont("sans", 12, QFont::Bold)));

    ui->PEPlot->plotLayout()->insertRow(0);
    ui->PEPlot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->PEPlot, "Potential Energy Plot", QFont("sans", 12, QFont::Bold)));

    ui->KEPlot->plotLayout()->insertRow(0);
    ui->KEPlot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->KEPlot, "Kinetic Energy Plot", QFont("sans", 12, QFont::Bold)));

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->totalEnergyPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->totalEnergyPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->totalEnergyPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->totalEnergyPlot->yAxis2, SLOT(setRange(QCPRange)));
    ui->totalEnergyPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    connect(ui->KEPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->KEPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->KEPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->KEPlot->yAxis2, SLOT(setRange(QCPRange)));
    ui->KEPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    connect(ui->PEPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->PEPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->PEPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->PEPlot->yAxis2, SLOT(setRange(QCPRange)));
    ui->PEPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

}

void mdSimulator::openHelpWidget()
{
    // open help widget
    help = new helpWidget;

    // set its position on the screen
    help->move(450,200);

    // this line is necessary to really receive the destroyed signal when the widget is closed!
    help->setAttribute(Qt::WA_DeleteOnClose, true);

    // show help widget
    help->show();


}

