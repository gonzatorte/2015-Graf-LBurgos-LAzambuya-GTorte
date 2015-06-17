#include "Shade.h"

Shade::Shade(){
    //ctor
}

Shade::~Shade(){
    //dtor
}

bool shadow(Ray & ray, Figure * fig){
    Scene* s = Scene::getInstance();
    bool interfiere = true;
    Isect isect;
    vector<Isect> isects = fig->intersect(ray);
    if (!isects.empty()){
        float minDistance = isects[0].distance;
        for (list<Figure*>::iterator it_figure = s->figures.begin(); it_figure!=s->figures.end(); ++it_figure){
            vector<Isect> fig_isects = (*it_figure)->intersect(ray);
            if (!fig_isects.empty()){
                if (fig_isects[0].distance < minDistance){
                    interfiere = false;
                    break;
                }
            }
        }
    }
    return interfiere;
}

Vector specularDirection(Vector incidentRay, Vector normal){
    return (incidentRay + normal * (incidentRay * normal * -2)).UnitVector();
}

bool transmisionDirection(Figure * medio_in, Figure * medio_out, Vector incidentRay, Vector normal, Vector & res){
    float n1 = medio_in ? medio_in->ktran : 1;
    float n2 = medio_out ? medio_out->ktran : 1;
    float eta = n1/n2;
    float c1 = - (incidentRay * normal);
    float cs2 = 1 - eta*eta * (1 - c1*c1);
    if (cs2 < 0)
        return false;
    res = incidentRay * eta + normal * (eta*c1-sqrt(cs2));
    return true;
}

Vector Shade::shadeRay(Ray &ray, Isect & isect, int level, int weight){
    Scene* s = Scene::getInstance();
    Figure* figure = isect.figure;
    Vector normal = isect.normal;
    Vector point = isect.surfacePoint;
    Trace trace;
    Vector color;
    Vector colorReflexion;
    color.x = figure->color.x;
    color.y = figure->color.y;
    color.z = figure->color.z;

    if (ray.direction.dotProduct(normal)>0){
        normal = normal*-1;
    }

    list<Light*>::iterator it;
    for (it=s->lights.begin(); it!=s->lights.end(); ++it){
        Light * curr_light = (*it);

        color = color + (curr_light->ambient_intesity * figure->kamb);

        Ray rayL = Ray(curr_light->position, point - curr_light->position);
        float difuse_angle = -rayL.direction.UnitVector() * normal;
        if((difuse_angle > 0) && shadow(rayL, figure)){
            color = color + (curr_light->difuse_intesity * (figure->kdif * difuse_angle));
//            color = color +
//                    (curr_light->spec_intesity * figure->kspec *
//                             powf((ray.origin*(-1)) * specular_vector, figure->shininess));
        }
    }

    int maxLevel = 3;
    int minWeight = 0.1;
    if (level + 1 < maxLevel){
        // Reflexion
        if (weight * figure->kspec > minWeight && figure->reflexion){
            Ray rayStart;
            rayStart.direction = specularDirection(ray.direction, normal);
            rayStart.origin = rayStart.direction + point;
            colorReflexion = trace.traceRay(rayStart, level + 1, weight * figure->kspec);
            color = color + colorReflexion * figure->kspec;
        }

//        if (weight * isect.figure->ktran > minWeight && isect.figure->refraction){
//            Ray rayStart;
//            rayStart.origin = rayStart.direction + point;
//            no_total_ref = transmisionDirection(ray.tran, isect.figure->tran, ray, isect.normal, rayStart.direction);
//            if (no_total_ref){
//                colorRefraction = trace.traceRay(rayStart, level + 1, weight * isect.figure->ktran);
//                color = color + colorRefraction * isect.figure->ktran;
//            }
//        }

    }

    color.x = color.x < 256 ? color.x : 255;
    color.y = color.y < 256 ? color.y : 255;
    color.z = color.z < 256 ? color.z : 255;

    return color;
}
