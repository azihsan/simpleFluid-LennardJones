/* This is the mdSimulator class derived from QWidget, inside this class there are methods and member that later will be used in
 * the simulation.
 * For simulation the hierarchy of the class is
 * QWidget -> QGraphicsView -> QGraphicsScene ->QGraphicItem
 */

#ifndef MDSIMULATOR_H
#define MDSIMULATOR_H

#include <QWidget>
#include <QtGui>
#include <QtCore>
#include <QTimer>
#include <QMessageBox> // for message to the user in case the mistype to input parameters
#include <QGraphicsScene>

#include "molecule.hpp"
#include "mdfunctions.hpp"
#include "myatom.h"
#include "helpwidget.h"
#include "qcustomplot.h"

namespace Ui {
class mdSimulator;
}

class mdSimulator : public QWidget
{
    Q_OBJECT

public:
    explicit mdSimulator(QWidget *parent = 0);

    // method to get parameters
    void getParameters();

    // method to get the default parameter
    void getDefault();

    // method to calculate region based on the amount of atoms and density
    double region();

    //method for scene initiation to add molecule
    void init_scene();

    //slot for real time plot
    void setupRealTimePlot();

public slots:

    // slot for starting simulation

    void startSimulate();

    //slot for continous simulation

    void simulate();

    //slot for close the simulation widget

    void closeSimulation();

    //slot to set all parameters to default value

    void setDefault();

    // Slot to set parameters

    void setParameters();

    //slot for calling help widget

     void openHelpWidget();

protected :


private:
    Ui::mdSimulator *ui;
    int n_atom; // private variable for storing n atom(s) from the input of user interface
    double temperature; // private variable for storing temperature from the input of user interface
    double density; // private variable for storing density from the input of user interface
    double timeStep; // private variable for storing time step of simulation from the input of user interface
    double timeElapsed; // private variable for time elapsed in real time plot
    float realRegion; // this is variable to store graphics region

    molecule *atom; //declaring a pointer to molecule class
    myAtom *qatom; //declaring a pointer to myAtom class
    QTimer *timer; //declaring a pointer to QTimer
    QGraphicsScene *scene; //declaring scene for simulation scene
    helpWidget *help; // help widget



};

#endif // MDSIMULATOR_H
