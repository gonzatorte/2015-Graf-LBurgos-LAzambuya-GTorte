#include "Mesh.h"

Mesh::Mesh()
{
    //ctor
}

void Mesh::buildTriangles(Vector v1, Vector v2, Vector v3, Vector v4, int signoNormal) {
    float height;
    float width;
    Vector aux, normal, center;
    if (v1.z==v2.z && v2.z==v3.z && v3.z==v4.z) { //Cara frontal y de fondo
        height = abs((v1 - v2).y);
        width = abs((v1-v3).x);
        aux = Vector(-width/2, -height/2, 0);
        normal = Vector(0, 0, signoNormal);
    } else if (v1.x==v2.x && v2.x==v3.x && v3.x==v4.x) { //Caras laterales
        height = abs((v1 - v2).y);
        width = abs((v1-v3).z);
        aux = Vector(0, -1*signoNormal*height/2, signoNormal*width/2);
        normal = Vector(signoNormal, 0, 0);
    } else { //Base y techo
        height = abs((v1 - v2).z);
        width = abs((v1-v3).x);
        aux = Vector(-width/2, 0, height/2);
        normal = Vector(0, signoNormal, 0);
    }
    center = v1 + aux;
    for (int i=0; i<4; i++) {
        Triangle triangle;
        triangle.reflexion = reflexion;
        triangle.refraction = refraction;
        triangle.color = Vector(color.x, color.y, color.z);
        triangle.norm = normal;
        if (i==0) {
            triangle.v0 = v1;
            triangle.v1 = center;
            triangle.v2 = v2;

        } else if (i==1) {
            triangle.v0 = v2;
            triangle.v1 = center;
            triangle.v2 = v4;

        } else if (i==2) {
            triangle.v0 = v4;
            triangle.v1 = center;
            triangle.v2 = v3;

        } else {
            triangle.v0 = v3;
            triangle.v1 = center;
            triangle.v2 = v1;

        }
        triangles.push_back(triangle);

    }
}

void Mesh::read(tinyxml2::XMLElement* element) {
    Figure::read(element);
    tinyxml2::XMLElement* vertexElement;
    for(vertexElement=element->FirstChildElement(); vertexElement; vertexElement=vertexElement->NextSiblingElement()) {

        Vector vertex = Vector(atof(vertexElement->Attribute("x")), atof(vertexElement->Attribute("y")), atof(vertexElement->Attribute("z")));
        vertexs.push_back(vertex);
    }
    //Creo planos con los vertices
    Vector v1, v2 ,v3, v4;
    int index = 0;
    int normalSign = 1;
    // TODO: Porque son 10 planos?
    while (index<=10) {
        if (index <= 2) { //Cara lateral izquierda y de fondo.
            v1 = vertexs[index]; v2 = vertexs[index+1]; v3 = vertexs[index+2]; v4 = vertexs[index+3];
        } else if (index == 4) {  //Cara lateral derecha
            normalSign = -1;
            v1 = vertexs[5]; v2 = vertexs[4]; v3 = vertexs[7]; v4 = vertexs[6];
        } else if (index == 6) { // Cara frontal
            normalSign = -1;
            v1 = vertexs[0]; v2 = vertexs[1]; v3 = vertexs[6]; v4 = vertexs[7];
        } else if (index == 8) { // Base
            normalSign = 1;
            v1 = vertexs[0]; v2 = vertexs[2]; v3 = vertexs[6]; v4 = vertexs[4];
        } else if (index == 10) { // Techo
            normalSign = -1;
            v1 = vertexs[1]; v2 = vertexs[3]; v3 = vertexs[7]; v4 = vertexs[5];
        }
        buildTriangles(v1, v2, v3, v4, normalSign);
        index = index+2;
    }

}

Isect Mesh::intersect(Ray & ray) {

    double minDistance = 100000;
    Isect closest;
    closest.hited = false;
    for (int i=0; i < triangles.size(); i++) {
        Isect aux;
        aux = triangles[i].intersect(ray);
        if (aux.hited && minDistance > aux.distance) {
            closest = aux;
            minDistance = aux.distance;
        }
    }

    return closest;
}

Mesh::~Mesh()
{
    //dtor
}