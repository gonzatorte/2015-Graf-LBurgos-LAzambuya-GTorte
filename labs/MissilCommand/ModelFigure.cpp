#include "ModelFigure.h"
//#include <SDL/SDL_opengl.h>
#include <GL/glu.h>

void ModelFigure::drawFigure(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, model->id_texture);

    glBegin(GL_TRIANGLES);
    for (int l_index=0;l_index<model->polygons_qty;l_index++)
    {
        //----------------- FIRST VERTEX -----------------
        glTexCoord2f( model->mapcoord[ model->polygon[l_index].a ].u,
                      model->mapcoord[ model->polygon[l_index].a ].v);
        glVertex3f( model->vertex[ model->polygon[l_index].a ].x,
                    model->vertex[ model->polygon[l_index].a ].y,
                    model->vertex[ model->polygon[l_index].a ].z);

        //----------------- SECOND VERTEX -----------------
        glTexCoord2f( model->mapcoord[ model->polygon[l_index].b ].u,
                      model->mapcoord[ model->polygon[l_index].b ].v);
        glVertex3f( model->vertex[ model->polygon[l_index].b ].x,
                    model->vertex[ model->polygon[l_index].b ].y,
                    model->vertex[ model->polygon[l_index].b ].z);

        //----------------- THIRD VERTEX -----------------
        glTexCoord2f( model->mapcoord[ model->polygon[l_index].c ].u,
                      model->mapcoord[ model->polygon[l_index].c ].v);
        glVertex3f( model->vertex[ model->polygon[l_index].c ].x,
                    model->vertex[ model->polygon[l_index].c ].y,
                    model->vertex[ model->polygon[l_index].c ].z);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glScalef(orientation->x, orientation->y, orientation->z);
}

void ModelFigure::moveFigure(){
    eulerIntegrate();
    glTranslatef(x,y,z);
}