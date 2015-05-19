#ifndef MODELFIGURE_H
#define MODELFIGURE_H

#include "Figure.h"
#include "ModelType.h"

class ModelFigure: public Figure{
    public:
        ModelFigure(){};
        void drawFigure();
        void moveFigure();
        Vector * orientation;
        ModelType * model;
        int x,y,z;
};

#endif // MODELFIGURE_H
