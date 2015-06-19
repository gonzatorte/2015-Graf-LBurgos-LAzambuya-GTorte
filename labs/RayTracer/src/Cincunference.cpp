#include "Circunference.h"
#include "Figure.h"
#include <math.h>
#include "Scene.h"


Circunference::Circunference(){
    //ctor
}

void Circunference::read(tinyxml2::XMLElement* element) {
    center = Vector(atof(element->Attribute("centerX")), atof(element->Attribute("centerY")), atof(element->Attribute("centerZ")));
    norm = Vector(atof(element->Attribute("normalX")), atof(element->Attribute("normalY")), atof(element->Attribute("normalZ")));
    radius = atof(element->Attribute("radius"));
    Figure::read(element);
}

vector<Isect> Circunference::intersect(Ray & ray) {
    vector <Isect> intersecciones;
    double cosciente = norm * (center - ray.origin);
    double denominador = norm * (ray.direction);
    if (denominador != 0) { // En caso que sea cero quiere decir que el plano es paralelo al rayo
        double t = cosciente/denominador;
        if (t>=0) {
            Vector isectPoint = ray.origin + ray.direction * t;
            bool isSurfacePoint = false;
//            if (norm.x != 0) { // Es un plano en X
//                isSurfacePoint = (center.y - height/2) <= isectPoint.y && isectPoint.y <= (center.y + height/2)
//                    && (center.z - width/2) <= isectPoint.z && isectPoint.z <= (center.z + width/2);
//            } else if (norm.y != 0) { //Es un plano en y
//                isSurfacePoint = (center.z - height/2) <= isectPoint.z && isectPoint.z <= (center.z + height/2)
//                    && (center.x - width/2) <= isectPoint.x && isectPoint.x <= (center.x + width/2);
//            } else { //Es un plano en z
//                isSurfacePoint = (center.y - height/2) <= isectPoint.y && isectPoint.y <= (center.y + height/2)
//                    && (center.x - width/2) <= isectPoint.x && isectPoint.x <= (center.x + width/2);
//            }
            if(isSurfacePoint) {
                Isect isect;
                isect.figure = this;
                isect.surfacePoint = isectPoint;
                isect.normal = norm;
                isect.enter = (ray.direction * isect.normal) < 0;
                isect.distance = t;
                intersecciones.push_back(isect);
            }
        }
    }
    return intersecciones;
}

Circunference::~Circunference(){
    //dtor
}