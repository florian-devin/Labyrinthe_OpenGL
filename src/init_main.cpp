/*****************************************************************************\
 * TP CPE, 4ETI, TP synthese d'images
 * --------------
 *
 * Programme principal des appels OpenGL
 \*****************************************************************************/

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>

#define GLEW_STATIC 1
#include <GL/glew.h>

#if defined(__APPLE__)
#include <OpenGL/gl3.h>
#define __gl_h_
#include <GLUT/glut.h>
#define MACOSX_COMPATIBILITY GLUT_3_2_CORE_PROFILE
#else
#include <GL/glut.h>
#define MACOSX_COMPATIBILITY 0
#endif

#include "glhelper.hpp"
#include "mat4.hpp"
#include "vec3.hpp"
#include "vec2.hpp"
#include "triangle_index.hpp"
#include "vertex_opengl.hpp"
#include "image.hpp"
#include "mesh.hpp"

 //Labyrinthe
 /*
 --------> z
 |
 |
 |
 |
 v x

 */
int briques_tab[50][50] = { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0}, //debut labyrinthe
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,1,1,1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,0,0,0,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,1,1,1,1,1,1,0,0,1,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0}, //fin labyrinthe
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} };

/*****************************************************************************\
 * Variables globales
 *
 *
 \*****************************************************************************/

//identifiant du shader
GLuint shader_program_id;

//Matrice de transformation
struct transformation
{
  mat4 rotation;
  vec3 rotation_center;
  vec3 translation;

  transformation():rotation(),rotation_center(),translation(){}
};

//Informations correspondants à un model
struct model
{
  GLuint vao = 0;
  GLuint texture_id=0;
  int nbr_triangle;
  transformation transformation_model;
  vec3 angle = vec3(0.0f,0.0f,0.0f); // angle suivant x, y et z
  // rotation autour de y non implémentée
};

//Liste des modèls
model model_dinosaure;
//int x_dino = 25;
//int z_dino = 25;
model model_sol;
model model_cube;


//Transformation de la vue (camera)
transformation transformation_view;
float angle_view = 0.0f;

//Matrice de projection
mat4 projection;


int first_draw = 0;



//headers
void load_texture(const char* filename,GLuint *texture_id);

void init_model_1();
void init_model_2();
void init_model_3();

void dessin_labyrinthe();

void draw_model(model model_id);
bool valid_deplacement(float dx, float dy, float dz);

static void init()
{

  // Chargement du shader
  shader_program_id = glhelper::create_program_from_file(
      "shaders/shader.vert",
      "shaders/shader.frag"); CHECK_GL_ERROR();
  glUseProgram(shader_program_id);

  //matrice de projection
  projection = matrice_projection(60.0f*M_PI/180.0f,1.0f,0.01f,100.0f);
  GLint loc_projection = glGetUniformLocation(shader_program_id, "projection"); CHECK_GL_ERROR();
  if (loc_projection == -1) std::cerr << "Pas de variable uniforme : projection" << std::endl;
  glUniformMatrix4fv(loc_projection,1,false,pointeur(projection)); CHECK_GL_ERROR();

  //centre de rotation de la 'camera' (les objets sont centres en z=-2)
  transformation_view.rotation_center = vec3(0.0f,0.0f,-2.0f);

  //activation de la gestion de la profondeur
  glEnable(GL_DEPTH_TEST); CHECK_GL_ERROR();

  // Charge les modeles sur la carte graphique
  init_model_1();
  //model_dinosaure.transformation_model.translation.x = x_dino * 0.8f;
  //model_dinosaure.transformation_model.translation.z = z_dino * 0.8f;
  init_model_2();
  init_model_3();
}

//Fonction d'affichage
static void display_callback()
{
  //effacement des couleurs du fond d'ecran
  glClearColor(0.5f, 0.6f, 0.9f, 1.0f); CHECK_GL_ERROR();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); CHECK_GL_ERROR();

  //définie le programme à utiliser
  glUseProgram(shader_program_id);

  // Affecte les parametres uniformes de la vue (identique pour tous les modeles de la scene)
  {

    //envoie de la rotation
    GLint loc_rotation_view = glGetUniformLocation(shader_program_id, "rotation_view"); CHECK_GL_ERROR();
    if (loc_rotation_view == -1) std::cerr << "Pas de variable uniforme : rotation_view" << std::endl;
    glUniformMatrix4fv(loc_rotation_view,1,false,pointeur(transformation_view.rotation)); CHECK_GL_ERROR();

    //envoie du centre de rotation
    vec3 cv = transformation_view.rotation_center;
    GLint loc_rotation_center_view = glGetUniformLocation(shader_program_id, "rotation_center_view"); CHECK_GL_ERROR();
    if (loc_rotation_center_view == -1) std::cerr << "Pas de variable uniforme : rotation_center_view" << std::endl;
    glUniform4f(loc_rotation_center_view , cv.x,cv.y,cv.z , 0.0f); CHECK_GL_ERROR();

    //envoie de la translation
    vec3 tv = transformation_view.translation;
    GLint loc_translation_view = glGetUniformLocation(shader_program_id, "translation_view"); CHECK_GL_ERROR();
    if (loc_translation_view == -1) std::cerr << "Pas de variable uniforme : translation_view" << std::endl;
    glUniform4f(loc_translation_view , tv.x,tv.y,tv.z , 0.0f); CHECK_GL_ERROR();
  }

  // Affiche les modeles 
  if (first_draw == 0) {
      model_dinosaure.transformation_model.translation = vec3(14.15f, 0.0f, -9.3f); //placement du dino au debut 
      model_dinosaure.angle = vec3(-0.029f, -1.6f, -1.0f);
      first_draw = 1;
  }

  draw_model(model_dinosaure);
  draw_model(model_sol);
  dessin_labyrinthe();




  //Changement de buffer d'affichage pour eviter un effet de scintillement
  glutSwapBuffers();
}

/*****************************************************************************\
 * keyboard_callback                                                         *
 \*****************************************************************************/
static void keyboard_callback(unsigned char key, int, int)
{
  float d_angle=0.1f;
  float dz=0.2f;

  //quitte le programme si on appuie sur les touches 'q', 'Q', ou 'echap'
  //enregistre l'image si on appuie sur la touche 'p'
  switch (key)
  {
    case 27:
      exit(0);
      break;

    case 'k':
      model_dinosaure.angle.y += d_angle;
      break;
    case 'm':
      model_dinosaure.angle.y -= d_angle;
      break;

    case 'z':
        if (valid_deplacement(dz * sin((model_dinosaure.angle.y * M_PI) / 3.1), .0f, dz * cos((model_dinosaure.angle.y * M_PI) / 3.1))) {
            model_dinosaure.transformation_model.translation.z += dz * cos((model_dinosaure.angle.y * M_PI) / 3.1);
            model_dinosaure.transformation_model.translation.x += dz * sin((model_dinosaure.angle.y * M_PI) / 3.1);
            transformation_view.translation.z -= dz;
        }
        
        break;

    case 's':
        if (valid_deplacement(-dz * sin((model_dinosaure.angle.y * M_PI) / 3.1), .0f, -dz * cos((model_dinosaure.angle.y * M_PI) / 3.1))) {
            model_dinosaure.transformation_model.translation.z -= dz * cos((model_dinosaure.angle.y * M_PI) / 3.1);
            model_dinosaure.transformation_model.translation.x -= dz * sin((model_dinosaure.angle.y * M_PI) / 3.1);
            transformation_view.translation.z += dz;
        }
        break;

    case 'd':
        if (valid_deplacement(-dz * cos((model_dinosaure.angle.y * M_PI) / 3.1), .0f, dz * sin((model_dinosaure.angle.y * M_PI) / 3.1))) {
            model_dinosaure.transformation_model.translation.x -= dz * cos((model_dinosaure.angle.y * M_PI) / 3.1);
            model_dinosaure.transformation_model.translation.z += dz * sin((model_dinosaure.angle.y * M_PI) / 3.1);
            transformation_view.translation.x += dz;
        }
        
        break;

    case 'q':
        if (valid_deplacement(dz * cos((model_dinosaure.angle.y * M_PI) / 3.1), .0f, -dz * sin((model_dinosaure.angle.y * M_PI) / 3.1))) {
            model_dinosaure.transformation_model.translation.x += dz * cos((model_dinosaure.angle.y * M_PI) / 3.1);
            model_dinosaure.transformation_model.translation.z -= dz * sin((model_dinosaure.angle.y * M_PI) / 3.1);
        }
        break;

 
    case 'f':
      angle_view -= d_angle;
      break;


    case 'e':
      transformation_view.translation.z += dz;
      break;

  }


  model_dinosaure.transformation_model.rotation = matrice_rotation(model_dinosaure.angle.y, 0.0f,1.0f,0.0f) * matrice_rotation(model_dinosaure.angle.x, 1.0f,0.0f,0.0f);
  transformation_view.rotation = matrice_rotation(angle_view , 0.0f,1.0f,0.0f);
  // Des exemples de camera vous sont données dans le programme 10

  {
        // Exemple camera troisieme personne :
        transformation_view.rotation_center = vec3();
        vec3 target = model_dinosaure.transformation_model.translation + vec3(0., -0.5, -2.);
        vec3 pos_camera = target - model_dinosaure.transformation_model.rotation * vec3(0., -1., 3.);
        transformation_view.rotation = matrice_lookat(pos_camera, target, vec3(0., 1., 0.));
        transformation_view.translation = extract_translation(transformation_view.rotation);
  }
  std::cout << "model_dinosaure.angle  =";
  std::cout << model_dinosaure.angle << std::endl;
  //std::cout << "val z =";
  //std::cout << model_dinosaure.transformation_model.translation.z << std::endl;
}

/*****************************************************************************\
 * special_callback                                                          *
 \*****************************************************************************/
static void special_callback(int key, int,int)
{
  float d_angle = 0.1f;
  float dL=0.03f;
  switch (key)
  {
    case GLUT_KEY_UP:
      model_dinosaure.transformation_model.translation.y += dL; //rotation avec la touche du haut
      break;
    case GLUT_KEY_DOWN:
      model_dinosaure.transformation_model.translation.y -= dL; //rotation avec la touche du bas
      break;
    case GLUT_KEY_LEFT:
        model_dinosaure.angle.y -= d_angle;
      break;
    case GLUT_KEY_RIGHT:
      model_dinosaure.angle.y += d_angle;
      break;
  }
}


/*****************************************************************************\
 * timer_callback                                                            *
 \*****************************************************************************/
static void timer_callback(int)
{
  //demande de rappel de cette fonction dans 25ms
  glutTimerFunc(25, timer_callback, 0);

  //reactualisation de l'affichage
  glutPostRedisplay();
}

int main(int argc, char** argv)
{
  //**********************************************//
  //Lancement des fonctions principales de GLUT
  //**********************************************//

  //initialisation
  glutInit(&argc, argv);

  //Mode d'affichage (couleur, gestion de profondeur, ...)
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | MACOSX_COMPATIBILITY);

  //Taille de la fenetre a l'ouverture
  glutInitWindowSize(1920, 1080);

  //Titre de la fenetre
  glutCreateWindow("Labyrinthe");

  //Fonction de la boucle d'affichage
  glutDisplayFunc(display_callback);

  //Fonction de gestion du clavier
  glutKeyboardFunc(keyboard_callback);

  //Fonction des touches speciales du clavier (fleches directionnelles)
  glutSpecialFunc(special_callback);

  //Fonction d'appel d'affichage en chaine
  glutTimerFunc(25, timer_callback, 0);

  //Option de compatibilité
  glewExperimental = true;

  //Initialisation des fonctions OpenGL
  glewInit();

  //Affiche la version openGL utilisée
  std::cout << "OpenGL: " << (GLchar *)(glGetString(GL_VERSION)) << std::endl;

  //Notre fonction d'initialisation des donnees et chargement des shaders
  init();

  //Lancement de la boucle (infinie) d'affichage de la fenetre
  glutMainLoop();

  //Plus rien n'est execute apres cela
  return 0;
}

void draw_model(model m)
{

  //envoie des parametres uniformes
  {
    GLint loc_rotation_model = glGetUniformLocation(shader_program_id, "rotation_model"); CHECK_GL_ERROR();
    if (loc_rotation_model == -1) std::cerr << "Pas de variable uniforme : rotation_model" << std::endl;
    glUniformMatrix4fv(loc_rotation_model,1,false,pointeur(m.transformation_model.rotation));    CHECK_GL_ERROR();

    vec3 c = m.transformation_model.rotation_center;
    GLint loc_rotation_center_model = glGetUniformLocation(shader_program_id, "rotation_center_model"); CHECK_GL_ERROR();
    if (loc_rotation_center_model == -1) std::cerr << "Pas de variable uniforme : rotation_center_model" << std::endl;
    glUniform4f(loc_rotation_center_model , c.x,c.y,c.z , 0.0f);                                 CHECK_GL_ERROR();

    vec3 t = m.transformation_model.translation;
    GLint loc_translation_model = glGetUniformLocation(shader_program_id, "translation_model"); CHECK_GL_ERROR();
    if (loc_translation_model == -1) std::cerr << "Pas de variable uniforme : translation_model" << std::endl;
    glUniform4f(loc_translation_model , t.x,t.y,t.z , 0.0f);                                     CHECK_GL_ERROR();
  }

  glBindVertexArray(m.vao);CHECK_GL_ERROR();

  //affichage
  {
    glBindTexture(GL_TEXTURE_2D, m.texture_id);                             CHECK_GL_ERROR();
    glDrawElements(GL_TRIANGLES, 3*m.nbr_triangle, GL_UNSIGNED_INT, 0);     CHECK_GL_ERROR();
  }
}

void init_model_1()
{
  // Chargement d'un maillage a partir d'un fichier
  mesh m = load_obj_file("data/stegosaurus.obj");

  // Affecte une transformation sur les sommets du maillage
  float s = 0.2f;
  mat4 transform = mat4(   s, 0.0f, 0.0f, 0.0f,
      0.0f,    s, 0.0f,-0.9f,
      0.0f, 0.0f,   s ,-2.0f,
      0.0f, 0.0f, 0.0f, 1.0f);
  apply_deformation(&m,transform);

  // Centre la rotation du modele 1 autour de son centre de gravite approximatif
  model_dinosaure.transformation_model.rotation_center = vec3(0.0f,-0.5f,-2.0f);

  // Calcul automatique des normales du maillage
  update_normals(&m);
  // Les sommets sont affectes a une couleur blanche
  fill_color(&m,vec3(1.0f,1.0f,1.0f));

  //attribution d'une liste d'état (1 indique la création d'une seule liste)
  glGenVertexArrays(1, &model_dinosaure.vao);
  glBindVertexArray(model_dinosaure.vao);

  GLuint vbo;
  //attribution d'un buffer de donnees (1 indique la création d'un buffer)
  glGenBuffers(1,&vbo); CHECK_GL_ERROR();
  //affectation du buffer courant
  glBindBuffer(GL_ARRAY_BUFFER,vbo); CHECK_GL_ERROR();
  //copie des donnees des sommets sur la carte graphique
  glBufferData(GL_ARRAY_BUFFER,m.vertex.size()*sizeof(vertex_opengl),&m.vertex[0],GL_STATIC_DRAW); CHECK_GL_ERROR();

  // Active l'utilisation des données de positions (le 0 correspond à la location dans le vertex shader)
  glEnableVertexAttribArray(0); CHECK_GL_ERROR();
  // Indique comment le buffer courant (dernier vbo "bindé") est utilisé pour les positions des sommets
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_opengl), 0); CHECK_GL_ERROR();

  // Active l'utilisation des données de normales (le 1 correspond à la location dans le vertex shader)
  glEnableVertexAttribArray(1); CHECK_GL_ERROR();
  // Indique comment le buffer courant (dernier vbo "bindé") est utilisé pour les normales des sommets
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(vertex_opengl), (void*)sizeof(vec3)); CHECK_GL_ERROR();

  // Active l'utilisation des données de couleurs (le 2 correspond à la location dans le vertex shader)
  glEnableVertexAttribArray(2); CHECK_GL_ERROR();
  // Indique comment le buffer courant (dernier vbo "bindé") est utilisé pour les couleurs des sommets
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_opengl), (void*)(2*sizeof(vec3))); CHECK_GL_ERROR();

  // Active l'utilisation des données de textures (le 3 correspond à la location dans le vertex shader)
  glEnableVertexAttribArray(3); CHECK_GL_ERROR();
  // Indique comment le buffer courant (dernier vbo "bindé") est utilisé pour les textures des sommets
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_opengl), (void*)(3*sizeof(vec3))); CHECK_GL_ERROR();

  GLuint vboi;
  //attribution d'un autre buffer de donnees
  glGenBuffers(1,&vboi); CHECK_GL_ERROR();
  //affectation du buffer courant (buffer d'indice)
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi); CHECK_GL_ERROR();
  //copie des indices sur la carte graphique
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,m.connectivity.size()*sizeof(triangle_index),&m.connectivity[0],GL_STATIC_DRAW); CHECK_GL_ERROR();

  // Nombre de triangles de l'objet 1
  model_dinosaure.nbr_triangle = m.connectivity.size();

  // Chargement de la texture
  load_texture("data/stegosaurus.tga",&model_dinosaure.texture_id);
}

void init_model_2()
{
  //Creation manuelle du model 2

  //coordonnees geometriques des sommets
  vec3 p0=vec3(-25.0f,-0.9f,-25.0f);
  vec3 p1=vec3( 25.0f,-0.9f,-25.0f);
  vec3 p2=vec3( 25.0f,-0.9f, 25.0f);
  vec3 p3=vec3(-25.0f,-0.9f, 25.0f);

  //normales pour chaque sommet
  vec3 n0=vec3(0.0f,1.0f,0.0f);
  vec3 n1=n0;
  vec3 n2=n0;
  vec3 n3=n0;

  //couleur pour chaque sommet
  vec3 c0=vec3(1.0f,1.0f,1.0f);
  vec3 c1=c0;
  vec3 c2=c0;
  vec3 c3=c0;

  //texture du sommet
  vec2 t0=vec2(0.0f,0.0f);
  vec2 t1=vec2(1.0f,0.0f);
  vec2 t2=vec2(1.0f,1.0f);
  vec2 t3=vec2(0.0f,1.0f);

  vertex_opengl v0=vertex_opengl(p0,n0,c0,t0);
  vertex_opengl v1=vertex_opengl(p1,n1,c1,t1);
  vertex_opengl v2=vertex_opengl(p2,n2,c2,t2);
  vertex_opengl v3=vertex_opengl(p3,n3,c3,t3);

  //tableau entrelacant coordonnees-normales
  vertex_opengl geometrie[]={v0,v1,v2,v3};

  //indice des triangles
  triangle_index tri0=triangle_index(0,1,2);
  triangle_index tri1=triangle_index(0,2,3);
  triangle_index index[]={tri0,tri1};
  model_sol.nbr_triangle = 2;

  //attribution d'une liste d'état (1 indique la création d'une seule liste)
  glGenVertexArrays(1, &model_sol.vao);
  glBindVertexArray(model_sol.vao);

  GLuint vbo;
  //attribution d'un buffer de donnees (1 indique la création d'un buffer)
  glGenBuffers(1,&vbo);                                             CHECK_GL_ERROR();
  //affectation du buffer courant
  glBindBuffer(GL_ARRAY_BUFFER,vbo);                                CHECK_GL_ERROR();
  //copie des donnees des sommets sur la carte graphique
  glBufferData(GL_ARRAY_BUFFER,sizeof(geometrie),geometrie,GL_STATIC_DRAW);  CHECK_GL_ERROR();

  // Active l'utilisation des données de positions (le 0 correspond à la location dans le vertex shader)
  glEnableVertexAttribArray(0); CHECK_GL_ERROR();
  // Indique comment le buffer courant (dernier vbo "bindé") est utilisé pour les positions des sommets
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_opengl), 0); CHECK_GL_ERROR();

  // Active l'utilisation des données de normales (le 1 correspond à la location dans le vertex shader)
  glEnableVertexAttribArray(1); CHECK_GL_ERROR();
  // Indique comment le buffer courant (dernier vbo "bindé") est utilisé pour les normales des sommets
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(vertex_opengl), (void*)sizeof(vec3)); CHECK_GL_ERROR();

  // Active l'utilisation des données de couleurs (le 2 correspond à la location dans le vertex shader)
  glEnableVertexAttribArray(2); CHECK_GL_ERROR();
  // Indique comment le buffer courant (dernier vbo "bindé") est utilisé pour les couleurs des sommets
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_opengl), (void*)(2*sizeof(vec3))); CHECK_GL_ERROR();

  // Active l'utilisation des données de textures (le 3 correspond à la location dans le vertex shader)
  glEnableVertexAttribArray(3); CHECK_GL_ERROR();
  // Indique comment le buffer courant (dernier vbo "bindé") est utilisé pour les textures des sommets
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_opengl), (void*)(3*sizeof(vec3))); CHECK_GL_ERROR();

  GLuint vboi;
  //attribution d'un autre buffer de donnees
  glGenBuffers(1,&vboi);                                            CHECK_GL_ERROR();
  //affectation du buffer courant (buffer d'indice)
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi);                       CHECK_GL_ERROR();
  //copie des indices sur la carte graphique
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(index),index,GL_STATIC_DRAW);  CHECK_GL_ERROR();

  // Chargement de la texture
  load_texture("data/grass.tga",&model_sol.texture_id);

}
 
void init_model_3()
    {
        // Chargement d'un maillage a partir d'un fichier
        mesh m = load_obj_file("data/Cube.obj");

        // Affecte une transformation sur les sommets du maillage
        float s = 0.4f;
        mat4 transform = mat4(s  , 0.0f, 0.0f, 0.0f,
                             0.0f, s   , 0.0f, -0.5f,
                             0.0f, 0.0f, s   , 0.0f,
                             0.0f, 0.0f, 0.0f, 1.0f);

        apply_deformation(&m, transform);

        // Centre la rotation du modele 1 autour de son centre de gravite approximatif
        model_cube.transformation_model.rotation_center = vec3(0.0f, -0.5f, 0.0f);
        
        // Calcul automatique des normales du maillage
        update_normals(&m);
        // Les sommets sont affectes a une couleur blanche
        fill_color(&m, vec3(1.0f, 1.0f, 1.0f));

        //attribution d'une liste d'état (1 indique la création d'une seule liste)
        glGenVertexArrays(1, &model_cube.vao);
        glBindVertexArray(model_cube.vao);

        GLuint vbo;
        //attribution d'un buffer de donnees (1 indique la création d'un buffer)
        glGenBuffers(1, &vbo); CHECK_GL_ERROR();
        //affectation du buffer courant
        glBindBuffer(GL_ARRAY_BUFFER, vbo); CHECK_GL_ERROR();
        //copie des donnees des sommets sur la carte graphique
        glBufferData(GL_ARRAY_BUFFER, m.vertex.size() * sizeof(vertex_opengl), &m.vertex[0], GL_STATIC_DRAW); CHECK_GL_ERROR();

        // Active l'utilisation des données de positions (le 0 correspond à la location dans le vertex shader)
        glEnableVertexAttribArray(0); CHECK_GL_ERROR();
        // Indique comment le buffer courant (dernier vbo "bindé") est utilisé pour les positions des sommets
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_opengl), 0); CHECK_GL_ERROR();

        // Active l'utilisation des données de normales (le 1 correspond à la location dans le vertex shader)
        glEnableVertexAttribArray(1); CHECK_GL_ERROR();
        // Indique comment le buffer courant (dernier vbo "bindé") est utilisé pour les normales des sommets
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(vertex_opengl), (void*)sizeof(vec3)); CHECK_GL_ERROR();

        // Active l'utilisation des données de couleurs (le 2 correspond à la location dans le vertex shader)
        glEnableVertexAttribArray(2); CHECK_GL_ERROR();
        // Indique comment le buffer courant (dernier vbo "bindé") est utilisé pour les couleurs des sommets
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_opengl), (void*)(2 * sizeof(vec3))); CHECK_GL_ERROR();

        // Active l'utilisation des données de textures (le 3 correspond à la location dans le vertex shader)
        glEnableVertexAttribArray(3); CHECK_GL_ERROR();
        // Indique comment le buffer courant (dernier vbo "bindé") est utilisé pour les textures des sommets
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_opengl), (void*)(3 * sizeof(vec3))); CHECK_GL_ERROR();

        GLuint vboi;
        //attribution d'un autre buffer de donnees
        glGenBuffers(1, &vboi); CHECK_GL_ERROR();
        //affectation du buffer courant (buffer d'indice)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboi); CHECK_GL_ERROR();
        //copie des indices sur la carte graphique
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m.connectivity.size() * sizeof(triangle_index), &m.connectivity[0], GL_STATIC_DRAW); CHECK_GL_ERROR();

        // Nombre de triangles de l'objet 1
        model_cube.nbr_triangle = m.connectivity.size();
        std::cout << model_cube.nbr_triangle << std::endl;
        // Chargement de la texture
        load_texture("data/mur_en_pierre2.tga", &model_cube.texture_id);
    }

void dessin_labyrinthe() {
    int nbr_ligne = 50;
    int nbr_colone  = 50;

    float x = -25.0f;
    float z = -23.0f;
    int i, j;
    for (j = 0; j < nbr_colone; j++) {
        z = z + 0.8f; //profondeur
        x = -20.0f;
        for (i = 0; i < nbr_ligne; i++) {
            x = x + 0.8f; //cote
            if (briques_tab[i][j] == 1) {
                model_cube.transformation_model.translation = vec3(x, 0.0f, z);
                draw_model(model_cube);
            }
        }
    }
}

bool valid_deplacement(float dx, float dy, float dz) {
    //std::cout <<"\n" << std::endl;
    //std::cout << "Entre dans la fonction valid" << std::endl;
    bool ret = true;
    int b = 20;
    int x = round((model_dinosaure.transformation_model.translation.x + dx + 19.1f ) / 0.795f );
    int z = round((model_dinosaure.transformation_model.translation.z + dz + 20.0f ) / 0.7919 ) ;
    //std::cout << "val x =";
    //std::cout << x << std::endl;
    //std::cout << "val z =";
    //std::cout << z << std::endl;
    if (x >= 0 && x <= 49 && z >= 0 && z <= 49) {
        if (briques_tab[x][z] == 1) {
            ret = false;
        }
    }
    else {
        ret = true;
        std::cout << "Deplacement hord de la zone" << std::endl;
    }
    
    return ret;
}

void load_texture(const char* filename,GLuint *texture_id)
{
  // Chargement d'une texture (seul les textures tga sont supportes)
  Image  *image = image_load_tga(filename);
  if (image) //verification que l'image est bien chargee
  {

    //Creation d'un identifiant pour la texture
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); CHECK_GL_ERROR();
    glGenTextures(1, texture_id); CHECK_GL_ERROR();

    //Selection de la texture courante a partir de son identifiant
    glBindTexture(GL_TEXTURE_2D, *texture_id); CHECK_GL_ERROR();

    //Parametres de la texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); CHECK_GL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); CHECK_GL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); CHECK_GL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); CHECK_GL_ERROR();

    //Envoie de l'image en memoire video
    if(image->type==IMAGE_TYPE_RGB){ //image RGB
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data); CHECK_GL_ERROR();}
    else if(image->type==IMAGE_TYPE_RGBA){ //image RGBA
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data); CHECK_GL_ERROR();}
    else{
      std::cout<<"Image type not handled"<<std::endl;}

    delete image;
  }
  else
  {
    std::cerr<<"Erreur chargement de l'image, etes-vous dans le bon repertoire?"<<std::endl;
    abort();
  }

  GLint loc_texture = glGetUniformLocation(shader_program_id, "texture"); CHECK_GL_ERROR();
  if (loc_texture == -1) std::cerr << "Pas de variable uniforme : texture" << std::endl;
  glUniform1i (loc_texture,0); CHECK_GL_ERROR();
}
