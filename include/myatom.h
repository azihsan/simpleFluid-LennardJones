/* myAtom class is a class for gathering information from molecule class, we can say this is a graphical representation
 * of molecule in QtWidget. myAtom inherite the public member of QGraphicsItem thus myAtom could be add to QGraphicsView
 * to do the simulation.
 */
#ifndef MYATOM_H
#define MYATOM_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>


class myAtom : public QGraphicsItem
{
public:
    void setPosGraphic(float a, float b); // this method is to set position of atom in molecule class to myAtom

    void setPosXGraphic(float a); // method to set in x direction

    void setPosYGraphic(float a) ; // method to set in y direction

    float getPosXGraphic(); // method to get value in x direction

    float getPosYGraphic(); // method to get value in y direction


    QRectF boundingRect() const; //this is virtual method from QGraphicsItem class for defining a bounding of our item

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); //this method is also virtual method
    //from QGraphicsItem for defining our atom shape, color, brush, and so on.



protected:
    void advance(int phase); // is protected member of QGraphicsItem which is used for moving an atom in QGraphicsScene

private:
    // private member to store the molecule position to graphics representation
    float positionXGraphic;

    float positionYGraphic;

};

#endif // MYATOM_H
