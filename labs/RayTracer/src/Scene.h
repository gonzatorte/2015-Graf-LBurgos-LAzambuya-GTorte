#ifndef SCENE_H
#define SCENE_H

#include "tinyxml2.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Plane.h"
#include "Screen.h"
#include "Camera.h"
#include "Trace.h"
#include "Ray.h"
#include <list>

using namespace std;

class Scene
{
    public:
        Scene();
        static Scene* getInstance();
        void sceneRead();
        void createScreen();
        list<Figure*>* figures;
        virtual ~Scene();
        Screen* screen;
        Camera* camera;
        int width, height;
    protected:
    private:
};

#endif // SCENE_H