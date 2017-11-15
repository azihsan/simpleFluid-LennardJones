#include "myatom.h"

//here advance is
void myAtom::advance(int phase)
{
    if(!phase) return;

    setPos(positionXGraphic,positionYGraphic);

}

// while assigning value from setPosGraphic argument, due to the different orientation owned by Qt
// thus we have to inverse the positionYGraphic

void myAtom::setPosGraphic(float posX, float posY)
{
    positionXGraphic = posX;
    positionYGraphic = -posY; //inversing positionYGraphic

    setPos(positionXGraphic,positionYGraphic); //set position in graphics representation
}

void myAtom::setPosXGraphic(float a)
{
    positionXGraphic = a ;
}

void myAtom::setPosYGraphic(float a)
{
    positionYGraphic = -a;
}

float myAtom::getPosYGraphic()
{
    return (positionYGraphic);
}

float myAtom::getPosXGraphic()
{
    return (positionXGraphic);
}


QRectF myAtom::boundingRect() const
{
   return QRect(715,245,25,25); // defining our item position and radius
}

void myAtom::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec = boundingRect(); //rec is float value boundingRect()
    QBrush Brush(Qt::blue);
    painter->setBrush(Brush); //setting brush to blue
    painter->setPen(Qt::NoPen); //to remove the outline
    painter->drawEllipse(rec); //set the shape of item

}
