#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>
#include<bits/stdc++.h>
#include<string>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"
#include "Headers/ThirdPersonCamera.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

// Include Terrain
#include "Headers/Terrain.h"

#include "Headers/AnimationUtils.h"

// Include Colision headers functions
#include "Headers/Colisiones.h"

// Font rendering include
#include "Headers/FontTypeRendering.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

float screenWidth;
float screenHeight;

float Cordx = 0.0;
float Cordy = 0.0;
float Cordz = 0.0;
float movx = 0.0;
float movz = 0.0;
GLFWwindow *window;

Shader shader;
//Shader con skybox
Shader shaderSkybox;
//Shader con multiples luces
Shader shaderMulLighting;
//Shader para el terreno
Shader shaderTerrain;

// Shader para dibujar un objeto con solo textura
Shader shaderTexture;

Shader shaderHongo;

std::shared_ptr<Camera> camera(new ThirdPersonCamera());
float distanceFromTarget = 12.0;

Box boxIntro;
Box boxPlusLife;
Box boxMinLife;
Box boxMinLife2;
Sphere skyboxSphere(20, 20);
Box boxCesped;
Box boxWalls;
Box boxHighway;
Box boxLandingPad;
Sphere esfera1(10, 10);
Box boxCollider;
Sphere sphereCollider(10, 10);
Cylinder rayModel(10, 10, 1.0, 1.0, 1.0);
// Models complex instances

// Mayow
Model mayowModelAnimate;

//Zombie
Model zombie;

Model hongo;
Model hongo_G;
Model hongo_M;
Model hongo_R;
Model hongo_V;


Model modelLamp1;//Arbol
Model modelLamp2;//arbol

Model modelArbol1; 
Model modelArbol2;
Model modelArbol3; 

// arboles con hojas rducidos 
Model modelarbolconhojas1;
Model modelarbolconhojas2;

Model modelLaberinto;
Model modelArbolGigant;

// Aldea/ casas 
Model modelcasa;
// prueba de posiciones 
Model Position; 
// rocas
Model modelRock1;
Model modelRock2;

// Terrain model instance
Terrain terrain(-1, -1, 300, 20, "../Textures/NuevasTexturas/Mapa3v2.png");
Terrain terrain2(-1, -1, 300, 8, "../Textures/NuevasTexturas/labe2.png");  

GLuint textureCespedID;
GLuint textureTerrainRID, textureTerrainGID, textureTerrainBID, textureTerrainBlendMapID;
GLuint skyboxTextureID;
GLuint textureInit1ID, textureInit2ID, textureInit3ID, textureInit4ID, textureActivaID, textureScreenID, textureSaveID, 
		textureBlood1ID, textureBlood2ID, textureBlood3ID, textureBlood4ID, textureEnd1ID, textureEnd2ID;

GLuint textureHongoID;

// Modelo para el render del texto
FontTypeRendering::FontTypeRendering *modelText;

bool iniciaPartida = false, presionarOpcion = false, terminarpartida= false;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { "../Textures/mp_bloodvalley/blood-valley_ft.tga",
		"../Textures/mp_bloodvalley/blood-valley_bk.tga",
		"../Textures/mp_bloodvalley/blood-valley_up.tga",
		"../Textures/mp_bloodvalley/blood-valley_dn.tga",
		"../Textures/mp_bloodvalley/blood-valley_rt.tga",
		"../Textures/mp_bloodvalley/blood-valley_lf.tga" };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

// Model matrix definitions

// personaje Principal 
glm::mat4 modelMatrixMayow = glm::mat4(1.0f);
// zombies NPC
glm::mat4 modelMatrixZombie = glm::mat4(1.0f);
// hongos
glm::mat4 modelMatrixHongo = glm::mat4(1.0f);
// arbol del laberinto
glm::mat4 modelMatrixArrbolGrande = glm::mat4(1.0f);
// laberinto
glm::mat4 modelMatrixLabe = glm::mat4(1.0f);
// casas // aldea 
glm::mat4 modelMatrixcasa = glm::mat4(1.0f);
// prueba
glm::mat4 modelMatrixPosition = glm::mat4(1.0f);
// rocas 
glm::mat4 modelMatrixRock1 = glm::mat4(1.0f);
glm::mat4 modelMatrixRock2 = glm::mat4(1.0f);
// arboles con hojas
glm::mat4 modelMatrixarbolconhojas1 = glm::mat4(1.0f);
glm::mat4 modelMatrixarbolconhojas2 = glm::mat4(1.0f);


int animationMayowIndex = 1;
int modelSelected = 0;
bool enableCountSelected = true;


double deltaTime;
double currTime, lastTime;


// Jump variables
bool isJump = false;
float GRAVITY = 3.5;
double tmv = 0;
double startTimeJump = 0;

//Mostrar Hongo
bool mostrarHongo = true;
bool speedfast = false;
bool fastActive = false;
const float speedWalk = 0.25;
const float speedRun = 0.35;
float speedP = speedWalk;
float speedP_rev = speedP - 0.1;
int cont = 0;
bool mkColision = true;	//realiza colision una sola vez

int minLife = 0;
bool plusLife = false;
int idxPlusLife = 0;

// Contadores para hongos
int cont_Hongo_A = 0;
int cont_Hongo_M = 0;
int cont_Hongo_R = 0;
int cont_Hongo_V = 0;


//Vida Personaje
int vida = 5;
bool deadP = false; //protagonista muerto
bool txBlood1 = false;
bool txBlood2 = false;

//+++++++++++++++ Posiciones Hongos ++++++++++++++++++++++

std::vector<glm::vec4> hongo_G_pos ={
	glm::vec4(11.0, 0, -5.0, true),
	glm::vec4(11.0, 0, -7.0, true),
	glm::vec4(11.0, 0, -9.0, true),
	glm::vec4(11.0, 0, -11.0, true)
	// glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true),
	// glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true),
	// glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true),
	// glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true),
	// glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true),
};

std::vector<glm::vec4> hongo_A_pos ={
	glm::vec4(9.0, 0, -5.0, true),
	glm::vec4(9.0, 0, -7.0, true),
	glm::vec4(9.0, 0, -9.0, true),
	glm::vec4(9.0, 0, -11.0, true)
	// glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true),
	// glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true),
	// glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true),
	// glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true),
	// glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true),
};

std::vector<glm::vec4> hongo_M_pos ={
	glm::vec4(7.0, 0, -5.0, true),
	glm::vec4(7.0, 0, -7.0, true),
	glm::vec4(7.0, 0, -9.0, true)

	//glm::vec4(34.5, 0, -42.8, true), glm::vec4(45.7, 0, -62.1, true), glm::vec4(76.9, 0, -117.3, true), glm::vec4(10.5, 0, -197.46, true), glm::vec4(-15.7, 0, -187.7, true),
	// glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true),
	// glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true),
	// glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true),
	// glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true),
};


std::vector<glm::vec4> hongo_R_pos ={
	glm::vec4(3.0, 0, -5.0, true),
	glm::vec4(3.0, 0, -7.0, true),
	// glm::vec4(10.0, 0, 20.0, true), glm::vec4(5.0, 0, -7.0, true), glm::vec4(0, 0, 0, true), glm::vec4(-24, 0, -4, true), glm::vec4(-19.2, 0, -29.6, true),
	// glm::vec4(-38.4, 0, -11.2, true), glm::vec4(-61.6, 0, 25.6, true), glm::vec4(-59.2, 0, 64, true), glm::vec4(-37.6, 0, 45.6, true), glm::vec4(16, 0, 40, true),
	// glm::vec4(-7.2, 0, -56.8, true), glm::vec4(4.8, 0, -102.4, true), //glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true),
	// glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true),
	// glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true),
	// glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true),
	// glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true),
	// glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true),
	glm::vec4(3.0, 0, -9.0, true),
	glm::vec4(3.0, 0, -11.0, true)
};

std::vector<glm::vec4> hongo_V_pos ={
	glm::vec4(1.0, 0, -5.0, true),
	glm::vec4(1.0, 0, -7.0, true)
	// glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true),
	// glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true),
	// glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true),
	// glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true),
	// glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true), glm::vec4(, , , true),
};
//+++++++++++++++ Terminan Posiciones Hongos ++++++++++++++++++++++


//++++++++++++++++++ Posiciones Arboles +++++++++++++++++++++++++++
// Lamps position
std::vector<glm::vec3> lamp1Position = {
	glm::vec3(-7.03, 0, -19.14),
	glm::vec3(24.41, 0, -34.57),
	glm::vec3(-10.15, 0, -54.1)
	
};
// proyecto 
// arboles sin hojas con elementos
std::vector<glm::vec3> arboles1position = {
	
	glm::vec3(58,0,-14),
	glm::vec3(60,0,-32),
	glm::vec3( 26, 0 ,0 ),
	glm::vec3( 66, 0 ,0 ),
	glm::vec3( 34, 0 ,-28),
	glm::vec3( 30, 0 , -86),
	glm::vec3( -26, 0 , -70),
	glm::vec3( -16, 0 , -100),
	glm::vec3( -12, 0 ,60 ),
	glm::vec3( -40, 0 ,50 ),
	glm::vec3( -86, 0 ,38 ),
	glm::vec3( -116, 0 ,-4 ),
	glm::vec3( -90, 0 ,-4 ),
	glm::vec3( -84, 0 ,32 )
	
};
std::vector<float>  arboles1Orientacion = {
	-17.0, -82.67, 23.70
};
std::vector<glm::vec3> arboles2position = {
	glm::vec3( 78, 0 ,-22 ),
	glm::vec3( 88, 0 ,-20 ),
	glm::vec3( 84, 0 ,-56 ),
	glm::vec3( 80, 0 ,-70 ),
	glm::vec3( 46, 0 ,-40 ),
	glm::vec3( -14, 0 ,-30 ),
	glm::vec3( 64, 0 ,-98 ),
	glm::vec3( 90, 0 ,-150 ),
	glm::vec3( 80, 0 ,-114 ),
	glm::vec3( 87, 0 ,-105 ),
	glm::vec3( 45, 0 , 2),
	glm::vec3( 52, 0 , -14),
	glm::vec3( 20, 0 ,-28 ),
	glm::vec3( 24, 0 ,-40 ),
	glm::vec3(-22 , 0 , 4),
	glm::vec3( -14, 0 ,0 ),
	glm::vec3( -6, 0 ,20 ),
	glm::vec3( -40, 0 ,78 ),
	glm::vec3( -94, 0 , 50),
	glm::vec3( -100, 0 , 26),
	glm::vec3(-80 , 0 ,-4 ),
	glm::vec3( -80, 0 ,38 ),
	glm::vec3( -84, 0 ,-56 ),
	glm::vec3(-140, 0 , -84)
	
};
std::vector<float>  arboles2Orientacion = {
	-17.0, -82.67, 23.70
};
std::vector<glm::vec3> arboles3position = {
	glm::vec3( -4, 0 ,-20 ),
	glm::vec3( 82, 0 ,-112 ),
	glm::vec3( 90, 0 ,-132 ),
	glm::vec3( 30, 0 ,-16 ),
	glm::vec3( 22, 0 ,-56 ),
	glm::vec3( 12, 0 , -80),
	glm::vec3( -18, 0 ,-70 ),
	glm::vec3( -42, 0 ,-94 ),
	glm::vec3(-28 , 0 ,-95 ),
	glm::vec3( -14, 0 ,10 ),
	glm::vec3(-80 , 0 ,48 ),
	glm::vec3( -110, 0 ,10 )
	
};
std::vector<float>  arboles3Orientacion = {
	-17.0, -82.67, 23.70
};

// arboles con hojas  reducidos
std::vector<glm::vec3> arbolesConHojas1position = {
	glm::vec3( 10, 0 ,25 ), // prueba
	glm::vec3( 22, 0 ,-54 ),
	glm::vec3( 78, 0 ,-134 ),
	glm::vec3( -2, 0 ,-58 ),
	glm::vec3( -84, 0 ,68 ),
	glm::vec3( -78, 0 ,16 ),
	glm::vec3(-92 , 0 ,30 )
		
};
std::vector<float>  arbolesConHojas1Orientacion = {
	-17.0, -82.67, 23.70
};
std::vector<glm::vec3> arbolesConHojas2position = {
	glm::vec3( 10, 0 ,20 ),// prueba 
	glm::vec3( 48, 0 ,-92 ),
	glm::vec3( 50, 0 ,-82 ),
	glm::vec3( 12, 0 ,-16 ),
	glm::vec3( -24, 0 ,-104 ),
	glm::vec3( -14, 0 ,-105 ),
	glm::vec3( -20, 0 ,-16 ),
	glm::vec3( -92, 0 ,24 )
	
};
std::vector<float>  arbolesConHojas2Orientacion = {
	-17.0, -82.67, 23.70
};
//+++++++++++++++++++++ Terminan posiciones arboles +++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++ Posiciones Rocas +++++++++++++++++++++++++++++++++++
// rocas 
std::vector<glm::vec3> Rock1Position = {
	glm::vec3( 10, 0 ,10 ),	// prueba 
	glm::vec3( 5, 0 ,-66 ),
	glm::vec3( 32, 0 ,-40 ),
	glm::vec3( 0, 0 ,-30 ),
	glm::vec3( 78, 0 ,-148 ),
	glm::vec3( 68, 0 ,98 ),
	glm::vec3( 16, 0 , -98),
	glm::vec3( 28, 0 ,-86 ),
	glm::vec3( -15, 0 ,-120 ),
	glm::vec3( -30, 0 ,-125 ),
	glm::vec3( -12, 0 ,18 ),
	glm::vec3( -84, 0 ,78 ),
	glm::vec3( -80, 0 , 26)
	
};
std::vector<float>  Rock1Orientacion= {
	-17.0, -82.67, 23.70
};
std::vector<glm::vec3> Rock2Position = {
	glm::vec3( 10, 0 ,15 ),
	glm::vec3( 62, 0 ,-82 ),
	glm::vec3( 80, 0 ,-84 ),
	glm::vec3( 28, 0 ,-98 ),
	//glm::vec3( -20, 0 ,-86 ),
	glm::vec3( -42, 0 ,-106 ),
	glm::vec3( -8, 0 , 26),
	glm::vec3( -85, 0 , 56),
	glm::vec3( -108, 0 ,18 )
	
};
std::vector<float>  Rock2Orientacion= {
	-17.0, -82.67, 23.70
};

std::vector<glm::vec3> casaPosition = {
	// costad
	glm::vec3(-172,0,70),
	glm::vec3(-172,0,62),
	glm::vec3(-172,0,54),
	glm::vec3(-172,0,46),
	// de frente 
	glm::vec3(-164,0,78),
	glm::vec3(-156,0,78),
	glm::vec3(-148,0,78),
	glm::vec3(-140,0,78),
	glm::vec3(-132,0,78),
	// ista
	glm::vec3(-156,0,60),
	glm::vec3(-148,0,60),
	glm::vec3(-140,0,60),
	glm::vec3(-132,0,60),
	glm::vec3(-124,0,60),
	// isla de frente 
	glm::vec3(-156,0,49),
	glm::vec3(-148,0,49),
	glm::vec3(-140,0,49),
	glm::vec3(-132,0,49),
	glm::vec3(-124,0,49)
	
};
std::vector<float>  casaOrientcion = {
	90,90,90,90,180,180,180,180,180,360,360,360,360,90,180,180,180,180,90
};
//+++++++++++++++++++++ Terminan Posiciones Rocas +++++++++++++++++++++++++++++++++++++
// Colliders
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB;
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB_P;
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > collidersSBB;
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4, char> > coll_Hongo_SBB;


// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);

void hongos(){

	// Render del hongo model

	/******      HONGOS GOLDEN      ******/
	for(int i = 0; i < hongo_G_pos.size(); i++){
		if(hongo_G_pos[i].w != false){	//El hongo esta inicializado por lo tanto = visible
			hongo_G_pos[i].y = terrain.getHeightTerrain(hongo_G_pos[i].x, hongo_G_pos[i].z);
			hongo_G.setPosition(glm::vec3(hongo_G_pos[i].x, hongo_G_pos[i].y, hongo_G_pos[i].z));
			hongo_G.setScale(glm::vec3(1.0));
			//hongo.setOrientation(glm::vec3(0, lamp1Orientation[i], 0));
			hongo_G.render();
		}
	}
	/******      HONGOS AMARILLOS      ******/
	for(int i = 0; i < hongo_A_pos.size(); i++){
		if(hongo_A_pos[i].w != false){	//El hongo esta inicializado por lo tanto = visible
			hongo_A_pos[i].y = terrain.getHeightTerrain(hongo_A_pos[i].x, hongo_A_pos[i].z);
			hongo.setPosition(glm::vec3(hongo_A_pos[i].x, hongo_A_pos[i].y, hongo_A_pos[i].z));
			hongo.setScale(glm::vec3(1.0));
			//hongo.setOrientation(glm::vec3(0, lamp1Orientation[i], 0));
			hongo.render();
		}

		char hongoColor = 'A';
		AbstractModel::SBB hongoCollider;
		glm::mat4 modelMatrixColliderHongo = glm::mat4(1.0);
		modelMatrixColliderHongo = glm::translate(modelMatrixColliderHongo, glm::vec3(hongo_A_pos[i].x, hongo_A_pos[i].y, hongo_A_pos[i].z));

		addOrUpdateColliders(coll_Hongo_SBB, "hongo-A-" + std::to_string(i), hongoCollider, modelMatrixColliderHongo, hongoColor);

		modelMatrixColliderHongo = glm::scale(modelMatrixColliderHongo, glm::vec3(1.0, 1.0, 1.0));
		modelMatrixColliderHongo = glm::translate(modelMatrixColliderHongo, hongo.getSbb().c);
		hongoCollider.c = glm::vec3(modelMatrixColliderHongo[3]);
	    hongoCollider.ratio = hongo.getSbb().ratio * 0.5;
		std::get<0>(coll_Hongo_SBB.find("hongo-A-" + std::to_string(i))->second) = hongoCollider;
	}



	/******      HONGOS MORADOS      ******/
	for(int i = 0; i < hongo_M_pos.size(); i++){
		if(hongo_M_pos[i].w != false){	//El hongo esta inicializado por lo tanto = visible
			hongo_M_pos[i].y = terrain.getHeightTerrain(hongo_M_pos[i].x, hongo_M_pos[i].z);
			hongo_M.setPosition(glm::vec3(hongo_M_pos[i].x, hongo_M_pos[i].y, hongo_M_pos[i].z));
			hongo_M.setScale(glm::vec3(1.0));
			//hongo.setOrientation(glm::vec3(0, lamp1Orientation[i], 0));
			hongo_M.render();
		}

		char hongoColor = 'M';
		AbstractModel::SBB hongoCollider;
		glm::mat4 modelMatrixColliderHongo = glm::mat4(1.0);
		modelMatrixColliderHongo = glm::translate(modelMatrixColliderHongo, glm::vec3(hongo_M_pos[i].x, hongo_M_pos[i].y, hongo_M_pos[i].z));

		addOrUpdateColliders(coll_Hongo_SBB, "hongo-M-" + std::to_string(i), hongoCollider, modelMatrixColliderHongo, hongoColor);

		modelMatrixColliderHongo = glm::scale(modelMatrixColliderHongo, glm::vec3(1.0, 1.0, 1.0));
		modelMatrixColliderHongo = glm::translate(modelMatrixColliderHongo, hongo_M.getSbb().c);
		hongoCollider.c = glm::vec3(modelMatrixColliderHongo[3]);
	    hongoCollider.ratio = hongo_M.getSbb().ratio * 0.5;
		std::get<0>(coll_Hongo_SBB.find("hongo-M-" + std::to_string(i))->second) = hongoCollider;
	}



	/******      HONGOS ROJOS      ******/
	for(int i = 0; i < hongo_R_pos.size(); i++){
		if(hongo_R_pos[i].w != false){	//El hongo esta inicializado por lo tanto = visible
			hongo_R_pos[i].y = terrain.getHeightTerrain(hongo_R_pos[i].x, hongo_R_pos[i].z);
			hongo_R.setPosition(glm::vec3(hongo_R_pos[i].x, hongo_R_pos[i].y, hongo_R_pos[i].z));
			hongo_R.setScale(glm::vec3(1.0));
			//hongo.setOrientation(glm::vec3(0, lamp1Orientation[i], 0));
			hongo_R.render();
		}

		char hongoColor = 'R';
		AbstractModel::SBB hongoCollider;
		glm::mat4 modelMatrixColliderHongo = glm::mat4(1.0);
		modelMatrixColliderHongo = glm::translate(modelMatrixColliderHongo, glm::vec3(hongo_R_pos[i].x, hongo_R_pos[i].y, hongo_R_pos[i].z));

		addOrUpdateColliders(coll_Hongo_SBB, "hongo-R-" + std::to_string(i), hongoCollider, modelMatrixColliderHongo, hongoColor);

		modelMatrixColliderHongo = glm::scale(modelMatrixColliderHongo, glm::vec3(1.0, 1.0, 1.0));
		modelMatrixColliderHongo = glm::translate(modelMatrixColliderHongo, hongo_R.getSbb().c);
		hongoCollider.c = glm::vec3(modelMatrixColliderHongo[3]);
	    hongoCollider.ratio = hongo_R.getSbb().ratio * 0.5;
		std::get<0>(coll_Hongo_SBB.find("hongo-R-" + std::to_string(i))->second) = hongoCollider;
	}

	/******      HONGOS VERDES      ******/
	for(int i = 0; i < hongo_V_pos.size(); i++){
		if(hongo_V_pos[i].w != false){	//El hongo esta inicializado por lo tanto = visible
			hongo_V_pos[i].y = terrain.getHeightTerrain(hongo_V_pos[i].x, hongo_V_pos[i].z);
			hongo_V.setPosition(glm::vec3(hongo_V_pos[i].x, hongo_V_pos[i].y, hongo_V_pos[i].z));
			hongo_V.setScale(glm::vec3(1.0));
			//hongo.setOrientation(glm::vec3(0, lamp1Orientation[i], 0));
			hongo_V.render();
		}

		char hongoColor = 'V';
		AbstractModel::SBB hongoCollider;
		glm::mat4 modelMatrixColliderHongo = glm::mat4(1.0);
		modelMatrixColliderHongo = glm::translate(modelMatrixColliderHongo, glm::vec3(hongo_V_pos[i].x, hongo_V_pos[i].y, hongo_V_pos[i].z));

		addOrUpdateColliders(coll_Hongo_SBB, "hongo-V-" + std::to_string(i), hongoCollider, modelMatrixColliderHongo, hongoColor);

		modelMatrixColliderHongo = glm::scale(modelMatrixColliderHongo, glm::vec3(1.0, 1.0, 1.0));
		modelMatrixColliderHongo = glm::translate(modelMatrixColliderHongo, hongo_V.getSbb().c);
		hongoCollider.c = glm::vec3(modelMatrixColliderHongo[3]);
	    hongoCollider.ratio = hongo_V.getSbb().ratio * 0.5;
		std::get<0>(coll_Hongo_SBB.find("hongo-V-" + std::to_string(i))->second) = hongoCollider;
	}


//+++++++++++++++++++++++++++++++++ Deteccion de colisiones con hongos ++++++++++++++++++++++++++++++++++++++++++++++++++++++
	int conta = 0;
	int cambio = 0;
	//Se hace un mapeo de todos los objetos que sean SBB del tipo Hongo
	for (std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4, char>>::iterator it = coll_Hongo_SBB.begin();
				it != coll_Hongo_SBB.end(); it++) {

		bool isCollision = false;
		for (std::map<std::string,	//Se hace un mapeo de todos los objetos que sean OBB (personaje)
			std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator jt = collidersOBB_P.begin(); jt != collidersOBB_P.end(); jt++) {

			//Desaparicion de Hongo

			if (testSphereOBox(std::get<0>(it->second), std::get<0>(jt->second)) && hongo_A_pos[conta].w != false && cambio == 0) {
				
				if(jt->first.compare("mayow") == 0){
					std::cout << "Hay colision del " << it->first << " y el modelo" <<
					jt->first << "  Hongo a desaparecer: " << conta <<std::endl;
					hongo_A_pos[conta].w = false; //Hongo deja de estar visible || No se renderiza el hongo
					coll_Hongo_SBB.erase(it->first);
					cont_Hongo_A++;
				}
			}else if(testSphereOBox(std::get<0>(it->second), std::get<0>(jt->second)) && hongo_M_pos[conta].w != false && cambio == 1){
				if(jt->first.compare("mayow") == 0){
					std::cout << "Hay colision del " << it->first << " y el modelo" <<
					jt->first << "  Hongo a desaparecer: " << conta <<std::endl;
					hongo_M_pos[conta].w = false; //Hongo deja de estar visible || No se renderiza el hongo
					coll_Hongo_SBB.erase(it->first);
					cont_Hongo_M++;
				}
			}else if(testSphereOBox(std::get<0>(it->second), std::get<0>(jt->second)) && hongo_R_pos[conta].w != false && cambio == 2){
				if(jt->first.compare("mayow") == 0){
					std::cout << "Hay colision del " << it->first << " y el modelo" <<
					jt->first << "  Hongo a desaparecer: " << conta <<std::endl;
					hongo_R_pos[conta].w = false; //Hongo deja de estar visible || No se renderiza el hongo
					coll_Hongo_SBB.erase(it->first);
					cont_Hongo_R++;
					vida--;
					if(vida == 0)
						exitApp = true;
				}
			}else if(testSphereOBox(std::get<0>(it->second), std::get<0>(jt->second)) && hongo_V_pos[conta].w != false && cambio == 3){
				if(jt->first.compare("mayow") == 0){
					std::cout << "Hay colision del " << it->first << " y el modelo" <<
					jt->first << "  Hongo a desaparecer: " << conta <<std::endl;
					hongo_V_pos[conta].w = false; //Hongo deja de estar visible || No se renderiza el hongo
					coll_Hongo_SBB.erase(it->first);
					cont_Hongo_V++;
					vida++;
					
				}
			}

			
		}

		//conta: ayuda a determinar el índice del hongo dependiendo el tipo de hongo || 0 >= conta <= hongo_A_pos.size(), hongo_M_pos.size()
		//***Funcionando para dos tipos de hongo***
		//esta función se debe cambiar si se requieren añadir más tipos de hongo
		conta ++;
		if(conta >= hongo_A_pos.size() && cambio == 0){ //si se excede de la cantidad de hongos amarillos, empieza conteo de hongos morados desde 0
			conta = 0;
			cambio++;
		}else if(conta >= hongo_M_pos.size() && cambio == 1){//Conteo hongos morados
			conta = 0;
			cambio++;
		}else if(conta >= hongo_R_pos.size() && cambio == 2){//Conteo hongos morados
			conta = 0;
			cambio++;
		}else if(conta >= hongo_V_pos.size() && cambio == 3){//Conteo hongos rojos
			conta = 0;
			cambio = 0;
		}
	}

	//Codigo para modificar rapidez de personaje al utilizar hongo Azul
	if(speedfast){
		speedP = speedRun;
		cont++;
		if(cont >= 300){
			cont = 0;
			speedP = speedWalk;
			speedfast = false;
			fastActive = false;
		}
	}
	
}
	//Codigo para modificar rapidez de personaje al utilizar hongo Azul
// 	if(speedfast){
// 		speedP = speedRun;
// 		cont++;
// 		if(cont >= 300){
// 			cont = 0;
// 			speedP = speedWalk;
// 			speedfast = false;
// 			fastActive = false;
// 		}
// 	}

// 	if(plusLife){
// 		idxPlusLife++;
// 		if(idxPlusLife >= 200){
// 			idxPlusLife = 0;
// 			plusLife = false;
// 		}
// 	}
	
// }

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen){

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

//+++++++++++++++++++++++++ Configuracion Pantalla +++++++++++++++++++++++++++++++
	screenWidth = width;
	screenHeight = height;

	GLFWmonitor* primMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(primMonitor);

	screenWidth = videoMode->width / 1.7;//Tamaño de ventana
	screenHeight = videoMode->height / 1.5;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen){	//Cuando fullscreen activado
		window = glfwCreateWindow(width, height, strTitle.c_str(),glfwGetPrimaryMonitor(), nullptr);
	}else
		window = glfwCreateWindow(screenWidth, screenHeight, strTitle.c_str(), nullptr,nullptr);

	if (window == nullptr) {
		std::cerr
				<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
				<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

//+++++++++++++++++++++++++ Termina Configuracion Pantalla +++++++++++++++++++++++++++++++


	// Inicialización de los shaders
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation.vs", "../Shaders/multipleLights.fs");
	shaderTerrain.initialize("../Shaders/terrain.vs", "../Shaders/terrain.fs");
	shaderTexture.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado.fs");
	shaderHongo.initialize("../Shaders/ilum_Hongo.vs", "../Shaders/hongo.fs");

	// Inicializacion de los objetos.
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));

	boxCollider.init();
	boxCollider.setShader(&shader);
	boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	sphereCollider.init();
	sphereCollider.setShader(&shader);
	sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	rayModel.init();
	rayModel.setShader(&shader);
	rayModel.setColor(glm::vec4(1.0));

	boxIntro.init();
	boxIntro.setShader(&shaderTexture);
	boxIntro.setScale(glm::vec3(2.0, 2.0, 1.0));

	boxMinLife.init();
	boxMinLife.setShader(&shaderTexture);
	boxMinLife.setScale(glm::vec3(1.0, 1.0, 1.0));

	boxMinLife2.init();
	boxMinLife2.setShader(&shaderTexture);
	boxMinLife2.setScale(glm::vec3(2.0, 2.0, 1.0));

	boxPlusLife.init();
	boxPlusLife.setShader(&shaderTexture);
	boxPlusLife.setScale(glm::vec3(0.5, 0.5, 1.0));

// +++++++++++++++++ Modelos Arboles ++++++++++++++++++++++++

	//Lamps models
	modelLamp1.loadModel("../models/arbol/arbol2.obj");
	modelLamp1.setShader(&shaderMulLighting);
	modelLamp2.loadModel("../models/arbol/arbol.obj");
	modelLamp2.setShader(&shaderMulLighting);
	modelcasa.loadModel("../models/casa/casa.obj");
	modelcasa.setShader(&shaderMulLighting);

	// modelos de rocas
	// NOTA Cambiar los shader de iluminacion por propios
	modelRock1.loadModel("../models/rock/rock.obj");
	modelRock1.setShader(&shaderMulLighting);
	modelRock1.loadModel("../models/rock/rock2.obj");
	modelRock1.setShader(&shaderMulLighting);

	// arboles con hojas
	modelArbol1.loadModel("../models/arboles/arboles1v2.obj");// 1v2
	modelArbol1.setShader(&shaderMulLighting);
	modelArbol2.loadModel("../models/arboles/arboles2v2.obj");// v2
	modelArbol2.setShader(&shaderMulLighting);
	modelArbol3.loadModel("../models/arbol/arbol3.obj");// excepcion de arbol en espera de nuevo modelo
	modelArbol3.setShader(&shaderMulLighting);
	// arboles con hojas
	modelarbolconhojas1.loadModel("../models/arbol/arbolv.obj");
	modelarbolconhojas1.setShader(&shaderMulLighting);
	modelarbolconhojas2.loadModel("../models/arbol/arbol2.obj");
	modelarbolconhojas2.setShader(&shaderMulLighting);
	// laberinto
	modelLaberinto.loadModel("../models/labe/labe.obj");
	modelLaberinto.setShader(&shaderMulLighting);
	modelArbolGigant.loadModel("../models/arbgrande/arbgrande.obj");
	modelArbolGigant.setShader(&shaderMulLighting);
	// Objeto de pruebas/ posiciones
	Position.loadModel("../models/Hongo/hongo.obj");
	Position.setShader(&shaderMulLighting);
// +++++++++++++++++ Terminan Modelos Arboles ++++++++++++++++++++++++

// ++++++++++++++++++++++ Modelos Varios ++++++++++++++++++++++++++++
	// Mayow
	mayowModelAnimate.loadModel("../models/mayow/personaje2.fbx");
	mayowModelAnimate.setShader(&shaderMulLighting);

	// Zombie Model
	zombie.loadModel("../models/zombie/zombie.obj");
	zombie.setShader(&shaderHongo);
	
	//Hongo Amarillo
	hongo.loadModel("../models/Hongo/hongo2.obj");
	hongo.setShader(&shaderHongo);

	//Hongo Golden
	hongo_G.loadModel("../models/Hongo/hongo.obj");
	hongo_G.setShader(&shaderHongo);

	//Hongo Morado
	hongo_M.loadModel("../models/Hongo/hongo_M.obj");
	hongo_M.setShader(&shaderHongo);
	
	//Hongo Rojo
	hongo_R.loadModel("../models/Hongo/hongo_R.obj");
	hongo_R.setShader(&shaderHongo);

	//Hongo Verde
	hongo_V.loadModel("../models/Hongo/hongo_V.obj");
	hongo_V.setShader(&shaderHongo);

	

	// //Laberinto
	// labirynth.loadModel("../models/Laberinto/labe.obj");
	// labirynth.setShader(&shaderMulLighting);

	// Terreno
	terrain.init();
	terrain.setShader(&shaderTerrain);
	terrain2.init();
	terrain2.setShader(&shaderTerrain);
	

	// Se inicializa el model de render text
	modelText = new FontTypeRendering::FontTypeRendering(screenWidth, screenHeight); //MOD 13/11/2024
	modelText->Initialize();

	std::cout << "screenWidth:" << screenWidth;

	camera->setPosition(glm::vec3(0.0, 3.0, 4.0));
	camera->setDistanceFromTarget(distanceFromTarget);
	camera->setSensitivity(1.0);
	
	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	// Tipo de textura CUBE MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		skyboxTexture = Texture(fileNames[i]);
		skyboxTexture.loadImage(true);
		if (skyboxTexture.getData()) {
			glTexImage2D(types[i], 0, skyboxTexture.getChannels() == 3 ? GL_RGB : GL_RGBA, skyboxTexture.getWidth(), skyboxTexture.getHeight(), 0,
			skyboxTexture.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, skyboxTexture.getData());
		} else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage();
	}

	// Definiendo la textura a utilizar
	Texture textureCesped("../Textures/grassy2.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureCesped.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureCespedID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureCespedID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureCesped.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		std::cout << "Numero de canales :=> " << textureCesped.getChannels() << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, textureCesped.getChannels() == 3 ? GL_RGB : GL_RGBA, textureCesped.getWidth(), textureCesped.getHeight(), 0,
		textureCesped.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureCesped.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureCesped.freeImage();


	// Defininiendo texturas del mapa de mezclas
	// Definiendo la textura
	Texture textureR("../Textures/NuevasTexturas/RProyecto.png");
	textureR.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainRID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureR.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureR.getChannels() == 3 ? GL_RGB : GL_RGBA, textureR.getWidth(), textureR.getHeight(), 0,
		textureR.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureR.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureR.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureG("../Textures/NuevasTexturas/G2.jpg");
	textureG.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainGID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureG.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureG.getChannels() == 3 ? GL_RGB : GL_RGBA, textureG.getWidth(), textureG.getHeight(), 0,
		textureG.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureG.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureG.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureB("../Textures/NuevasTexturas/R2.png");
	textureB.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainBID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureB.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureB.getChannels() == 3 ? GL_RGB : GL_RGBA, textureB.getWidth(), textureB.getHeight(), 0,
		textureB.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureB.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureB.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureBlendMap("../Textures/NuevasTexturas/Mapa3RGB2.png");
	textureBlendMap.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainBlendMapID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureBlendMap.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureBlendMap.getChannels() == 3 ? GL_RGB : GL_RGBA, textureBlendMap.getWidth(), textureBlendMap.getHeight(), 0,
		textureBlendMap.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureBlendMap.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureBlendMap.freeImage(); // Liberamos memoria

	
//++++++++++++++++++++++++++++++++++++++ Texturas Menu de Inicio +++++++++++++++++++++++++++++++++++++++++++++++++++++
	Texture textureIntro1("../Textures/NuevasTexturas/Intro1.png");// carga de menu personalisado 
	textureIntro1.loadImage(); // Cargar la textura
	glGenTextures(1, &textureInit1ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureInit1ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureIntro1.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureIntro1.getChannels() == 3 ? GL_RGB : GL_RGBA, textureIntro1.getWidth(), textureIntro1.getHeight(), 0,
		textureIntro1.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureIntro1.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureIntro1.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureIntro2("../Textures/NuevasTexturas/Intro2.png");// carga de menu personalizado
	textureIntro2.loadImage(); // Cargar la textura
	glGenTextures(1, &textureInit2ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureInit2ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureIntro2.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureIntro2.getChannels() == 3 ? GL_RGB : GL_RGBA, textureIntro2.getWidth(), textureIntro2.getHeight(), 0,
		textureIntro2.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureIntro2.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureIntro2.freeImage(); // Liberamos memoria


	Texture textureIntro3("../Textures/NuevasTexturas/Intro3.png");// Melu elegible para los datos 
	textureIntro3.loadImage(); // Cargar la textura
	glGenTextures(1, &textureInit3ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureInit3ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureIntro3.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureIntro3.getChannels() == 3 ? GL_RGB : GL_RGBA, textureIntro3.getWidth(), textureIntro3.getHeight(), 0,
		textureIntro3.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureIntro3.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureIntro3.freeImage(); // Liberamos memoria
	// 
	Texture textureIntro4("../Textures/NuevasTexturas/Datos.png");// Nombrs generales de los integrantes del equipo del proyecto
	textureIntro4.loadImage(); // Cargar la textura
	glGenTextures(1, &textureInit4ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureInit4ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureIntro4.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureIntro4.getChannels() == 3 ? GL_RGB : GL_RGBA, textureIntro4.getWidth(), textureIntro4.getHeight(), 0,
		textureIntro4.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureIntro4.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
		// another txture
	textureIntro4.freeImage(); // Liberamos memori
//++++++++++++++++++++++++++++++++++++++++++ Fin Texturas Menu de Inicio +++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++ Texturas Blood & Save +++++++++++++++++++++++++++++++++++++++++++++++++++++

	Texture textureBlood1("../Textures/NuevasTexturas/lvl_1.png");// carga de menu personalisado 
	textureBlood1.loadImage(); // Cargar la textura
	glGenTextures(1, &textureBlood1ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureBlood1ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureBlood1.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureBlood1.getChannels() == 3 ? GL_RGB : GL_RGBA, textureBlood1.getWidth(), textureBlood1.getHeight(), 0,
		textureBlood1.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureBlood1.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureBlood1.freeImage(); // Liberamos memoria

	


	Texture textureBlood2("../Textures/NuevasTexturas/lvl_2.png");// carga de menu personalisado 
	textureBlood2.loadImage(); // Cargar la textura
	glGenTextures(1, &textureBlood2ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureBlood2ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureBlood2.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureBlood2.getChannels() == 3 ? GL_RGB : GL_RGBA, textureBlood2.getWidth(), textureBlood2.getHeight(), 0,
		textureBlood2.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureBlood2.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureBlood2.freeImage(); // Liberamos memoria


	Texture textureBlood3("../Textures/NuevasTexturas/lvl_3.png");// carga de menu personalisado 
	textureBlood3.loadImage(); // Cargar la textura
	glGenTextures(1, &textureBlood3ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureBlood3ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureBlood3.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureBlood3.getChannels() == 3 ? GL_RGB : GL_RGBA, textureBlood3.getWidth(), textureBlood3.getHeight(), 0,
		textureBlood3.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureBlood3.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureBlood3.freeImage(); // Liberamos memoria

	Texture textureBlood4("../Textures/NuevasTexturas/lvl_4.png");// carga de menu personalisado 
	textureBlood4.loadImage(); // Cargar la textura
	glGenTextures(1, &textureBlood4ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureBlood4ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureBlood4.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureBlood4.getChannels() == 3 ? GL_RGB : GL_RGBA, textureBlood4.getWidth(), textureBlood4.getHeight(), 0,
		textureBlood4.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureBlood4.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureBlood4.freeImage(); // Liberamos memoria


	Texture textureSave("../Textures/bluewater.png");// carga de menu personalisado 
	textureSave.loadImage(); // Cargar la textura
	glGenTextures(1, &textureSaveID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureSaveID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureSave.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureSave.getChannels() == 3 ? GL_RGB : GL_RGBA, textureSave.getWidth(), textureSave.getHeight(), 0,
		textureSave.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureSave.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureSave.freeImage(); // Liberamos memoria


	Texture textureScreen("../Textures/Screen.png");// carga de menu personalisado 
	textureScreen.loadImage(); // Cargar la textura
	glGenTextures(1, &textureScreenID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureScreenID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureScreen.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureScreen.getChannels() == 3 ? GL_RGB : GL_RGBA, textureScreen.getWidth(), textureScreen.getHeight(), 0,
		textureScreen.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureScreen.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureScreen.freeImage(); // Liberamos memoria
//++++++++++++++++++++++++++++++++++++++ Fin Texturas Blood & Save +++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++ Texturas End +++++++++++++++++++++++++++++++++++++++++++++++++++++

	Texture textureEnd1("../Textures/NuevasTexturas/End1.png");// Panel de muerte 1 End o salir
	textureEnd1.loadImage(); // Cargar la textura
	glGenTextures(1, &textureEnd1ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureEnd1ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureEnd1.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureEnd1.getChannels() == 3 ? GL_RGB : GL_RGBA, textureEnd1.getWidth(), textureEnd1.getHeight(), 0,
		textureEnd1.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureEnd1.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
		// another txture
	textureEnd1.freeImage(); // Liberamos memori
	// 
	Texture textureEnd2("../Textures/NuevasTexturas/End2.png");// Panel de muerte 2 Resume
	textureEnd2.loadImage(); // Cargar la textura
	glGenTextures(1, &textureEnd2ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureEnd2ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureEnd2.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureEnd2.getChannels() == 3 ? GL_RGB : GL_RGBA, textureEnd2.getWidth(), textureEnd2.getHeight(), 0,
		textureEnd2.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureEnd2.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
		// another txture
	textureEnd2.freeImage(); // Liberamos memori

//++++++++++++++++++++++++++++++++++++++ Fin Texturas End +++++++++++++++++++++++++++++++++++++++++++++++++++++
}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Shaders Delete
	shader.destroy();
	shaderMulLighting.destroy();
	shaderSkybox.destroy();
	shaderTerrain.destroy();
	shaderHongo.destroy();

	// Basic objects Delete
	skyboxSphere.destroy();
	boxCesped.destroy();
	esfera1.destroy();
	boxCollider.destroy();
	sphereCollider.destroy();
	rayModel.destroy();
	boxIntro.destroy();
	boxMinLife.destroy();
	boxMinLife2.destroy();
	boxPlusLife.destroy();

	// Custom objects Delete
	hongo.destroy();
	hongo_G.destroy();
	hongo_M.destroy();
	hongo_R.destroy();
	hongo_V.destroy();
	mayowModelAnimate.destroy();
	zombie.destroy();

	// versiones sin hojas de arboles 
	modelArbol1.destroy();
	modelArbol2.destroy();
	modelArbol3.destroy();
	// modelos de arboles con hojas reducidos
	modelarbolconhojas1.destroy();
	modelarbolconhojas2.destroy();
	// rocas
	modelRock1.destroy();
	modelRock2.destroy();

	modelLaberinto.destroy();
	modelArbolGigant.destroy();
	modelcasa.destroy();

	// Terrains objects Delete
	terrain.destroy();
	terrain2.destroy();

	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureCespedID);
	glDeleteTextures(1, &textureTerrainBID);
	glDeleteTextures(1, &textureTerrainGID);
	glDeleteTextures(1, &textureTerrainRID);
	glDeleteTextures(1, &textureTerrainBlendMapID);
	glDeleteTextures(1, &textureInit1ID);
	glDeleteTextures(1, &textureInit2ID);
	glDeleteTextures(1, &textureInit3ID);
	glDeleteTextures(1, &textureInit4ID);
	glDeleteTextures(1, &textureScreenID);
	glDeleteTextures(1, &textureSaveID);
	glDeleteTextures(1, &textureBlood1ID);
	glDeleteTextures(1, &textureBlood2ID);
	glDeleteTextures(1, &textureBlood3ID);
	glDeleteTextures(1, &textureBlood4ID);
	glDeleteTextures(1, &textureEnd1ID);
	glDeleteTextures(1, &textureEnd2ID);
	

	// Cube Maps Delete
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDeleteTextures(1, &skyboxTextureID);
}

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset){
	distanceFromTarget -= yoffset;
	camera->setDistanceFromTarget(distanceFromTarget);
}

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}
	float movH = 0.8;
	// controles del menu de inicio del juego 
	if(!iniciaPartida ){
		bool presionarEnter = glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS;
		bool presionarsalir = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
		if(textureActivaID == textureInit1ID && presionarEnter){
			iniciaPartida = true;
			textureActivaID = textureScreenID;
		}
		else if(textureActivaID == textureInit2ID && presionarEnter){
			iniciaPartida = false;
			//textureActivaID = textureScreenID;
			glfwSetWindowShouldClose(window, GLFW_TRUE); // Cierra la ventana
		}
		else if(textureActivaID == textureInit3ID && presionarEnter){
			textureActivaID = textureInit4ID;
		}
		else if(textureActivaID == textureInit4ID && presionarsalir){
			textureActivaID = textureInit1ID;
		}
		else if(!presionarOpcion && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
			presionarOpcion = true;
			if(textureActivaID == textureInit1ID)
				textureActivaID = textureInit2ID;
			else if(textureActivaID == textureInit2ID)
				textureActivaID = textureInit3ID;
			else if(textureActivaID == textureInit3ID)
				textureActivaID = textureInit1ID;
		}
		else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE)
			presionarOpcion = false;
	}

	// controles del menu de termino del juego 
	// if(!terminarpartida ){
	// 	bool presionarEnter = glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS;
	// 	bool presionarsalir = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
	// 	if(textureActivaID == textureEnd1ID && presionarEnter){
	// 		terminarpartida = true;
	// 		textureActivaID = textureScreenID;
	// 	}
	// 	else if(textureActivaID == textureEnd2ID && presionarEnter){
	// 		terminarpartida = false;
	// 		//textureActivaID = textureScreenID;
	// 		glfwSetWindowShouldClose(window, GLFW_TRUE); // Cierra la ventana
	// 	}
	// 	else if(textureActivaID == textureInit3ID && presionarEnter){
	// 		textureActivaID = textureInit4ID;
	// 	}
	// 	else if(textureActivaID == textureInit4ID && presionarsalir){
	// 		textureActivaID = textureInit1ID;
	// 	}
		
	// 	else if(!presionarOpcion && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
	// 		presionarOpcion = true;
	// 		if(textureActivaID == textureEnd1ID)
	// 			textureActivaID = textureEnd2ID;
	// 		else if(textureActivaID == textureEnd2ID)
	// 			textureActivaID = textureEnd1ID;
			
	// 	}
	// 	else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE)
	// 		presionarOpcion = false;
			
	// }
	
	else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		Cordx += movH; 
		movx = Cordx;
			}
	else if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		Cordx -= movH; 
		movx = Cordx;
			}
	else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		Cordz += movH; 
		movz = Cordz;
			}
	else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		Cordz -= movH; 
		movz = Cordz;
			}

	if(glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS){
		std::cout << "Coordinates -> X: " << modelMatrixMayow[3][0] << ", Z: " << modelMatrixMayow[3][2] << std::endl;


	}

// ================ aqui terminan los controles de inicio
	if (glfwJoystickPresent(GLFW_JOYSTICK_1) == GL_TRUE) {
		std::cout << "Esta presente el joystick" << std::endl;
		int axesCount, buttonCount;
		const float * axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
		std::cout << "Número de ejes disponibles :=>" << axesCount << std::endl;
		// std::cout << "Left Stick X axis: " << axes[0] << std::endl;
		// std::cout << "Left Stick Y axis: " << axes[1] << std::endl;
		// std::cout << "Left Trigger/L2: " << axes[2] << std::endl;
		// std::cout << "Right Stick X axis: " << axes[3] << std::endl;
		// std::cout << "Right Stick Y axis: " << axes[4] << std::endl;
		// std::cout << "Right Trigger/R2: " << axes[5] << std::endl;
		//std::cout << "Coordinates -> X: " << Cordx << ", Y: " << Cordy << ", Z: " << Cordz << std::endl;

		if(fabs(axes[1]) > 0.2){
			modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(0, 0, -axes[1] * 0.1));
			animationMayowIndex = 0;
		}if(fabs(axes[0]) > 0.2){
			modelMatrixMayow = glm::rotate(modelMatrixMayow, glm::radians(-axes[0] * 0.5f), glm::vec3(0, 1, 0));
			animationMayowIndex = 0;
		}

		if(fabs(axes[3]) > 0.2){
			camera->mouseMoveCamera(axes[3], 0.0, deltaTime);
		}if(fabs(axes[4]) > 0.2){
			camera->mouseMoveCamera(0.0, axes[4], deltaTime);
		}

		const unsigned char * buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
		std::cout << "Número de botones disponibles :=>" << buttonCount << std::endl;
		if(buttons[0] == GLFW_PRESS)
			std::cout << "Se presiona x" << std::endl;

		if(!isJump && buttons[0] == GLFW_PRESS){
			isJump = true;
			startTimeJump = currTime;
			tmv = 0;
		}
	}

	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, 0.0, deltaTime);
	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		camera->mouseMoveCamera(0.0, offsetY, deltaTime);

	offsetX = 0;
	offsetY = 0;


	// Controles de mayow
	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
		modelMatrixMayow = glm::rotate(modelMatrixMayow, 0.04f, glm::vec3(0, 1, 0));
		animationMayowIndex = 0;
	} else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
		modelMatrixMayow = glm::rotate(modelMatrixMayow, -0.04f, glm::vec3(0, 1, 0));
		animationMayowIndex = 0;
	}
	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
		modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(0.0, 0.0, speedP));
		animationMayowIndex = 0;
	}
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
		modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(0.0, 0.0, -speedP_rev));
		animationMayowIndex = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && cont_Hongo_A > 0){
		
		if(fastActive == false){
			speedfast = true;	//Hacemos q personaje se desplace rapido
		
			cont_Hongo_A--;		//Disminuye en 1 cantidad de hongos Morados/Azules?
			fastActive = true;
		}

	}

	bool keySpaceStatus = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
	if(!isJump && keySpaceStatus){
		isJump = true;
		startTimeJump = currTime;
		tmv = 0;
	}

	glfwPollEvents();
	return continueApplication;
}


void applicationLoop() {
	bool psi = true;

	glm::vec3 axis;
	glm::vec3 target;
	float angleTarget;

// donde se esta genernado nuestro modelo
	modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(13.0f, 0.05f, -5.0f));
	modelMatrixMayow = glm::rotate(modelMatrixMayow, glm::radians(-90.0f), glm::vec3(0, 1, 0));
	
	modelMatrixZombie = glm::translate(modelMatrixZombie, glm::vec3(10.0f, 0.05f, -5.0f));

	lastTime = TimeManager::Instance().GetTime();

	textureActivaID = textureInit1ID;

	while (psi) {
		currTime = TimeManager::Instance().GetTime();
		if(currTime - lastTime < 0.016666667){
			glfwPollEvents();
			continue;
		}
		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(true);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = processInput(true);

		std::map<std::string, bool> collisionDetection;

		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
				(float) screenWidth / (float) screenHeight, 0.01f, 100.0f);

	//++++++++++++++++++++++++ Inicia Seleccion Personaje ++++++++++++++++++++++++++++++
		if(modelSelected == 0){
			axis = glm::axis(glm::quat_cast(modelMatrixMayow));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixMayow));
			target = modelMatrixMayow[3];
		}
	//++++++++++++++++++++++++ Termina Seleccion Personaje ++++++++++++++++++++++++++++++

	//++++++++++++++++++++++++ Inicia Posicion Camara ++++++++++++++++++++++++++++++
		if(std::isnan(angleTarget))
			angleTarget = 0.0;
		if(axis.y < 0)
			angleTarget = -angleTarget;
		if(modelSelected == 1)
			angleTarget -= glm::radians(90.0f);
		camera->setCameraTarget(target);
		camera->setAngleTarget(angleTarget);
		camera->updateCamera();
		glm::mat4 view = camera->getViewMatrix();
	//++++++++++++++++++++++++ Termina Posicion Camara ++++++++++++++++++++++++++++++

	//++++++++++++++++++++++++ Carga de Shaders ++++++++++++++++++++++++++++++++++++++
		// Settea la matriz de vista y projection al shader con solo color
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
				glm::value_ptr(glm::mat4(glm::mat3(view))));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderTerrain.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderTerrain.setMatrix4("view", 1, false,
				glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con multiples luces
		shaderHongo.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderHongo.setMatrix4("view", 1, false,
				glm::value_ptr(view));
	//++++++++++++++++++++++++ Termina Carga de Shaders ++++++++++++++++++++++++++++++++++++++

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		float amb = 0.2;
		float dif = 0.1;
		float spc = 0.5;

		glm::vec3 dir_Lt_am = glm::vec3(amb, amb, amb);
		glm::vec3 dir_Lt_dif = glm::vec3(dif, dif, dif);
		glm::vec3 dir_Lt_spc = glm::vec3(spc, spc, spc);

		float amb2 = 1.2;
		float dif2 = 0.07;
		float spc2 = 0.001;

		glm::vec3 dir_Lt_am2 = glm::vec3(amb2, amb2, amb2);
		glm::vec3 dir_Lt_dif2 = glm::vec3(dif2, dif2, dif2);
		glm::vec3 dir_Lt_spc2 = glm::vec3(spc2, spc2, spc2);

		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(dir_Lt_am));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(dir_Lt_dif));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(dir_Lt_spc));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));

		shaderHongo.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderHongo.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(dir_Lt_am2));
		shaderHongo.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(dir_Lt_dif2));
		shaderHongo.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(dir_Lt_spc2));
		shaderHongo.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));

		shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(dir_Lt_am));
		shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(dir_Lt_dif));
		shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(dir_Lt_spc));
		shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));

		/*******************************************
		 * Propiedades SpotLights
		 *******************************************/
		// shaderMulLighting.setInt("spotLightCount", 1);
		// shaderTerrain.setInt("spotLightCount", 1);
		// glm::vec3 spotPosition = glm::vec3(modelMatrixHeli * glm::vec4(0.0, 0.2, 1.75, 1.0));
		// shaderMulLighting.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
		// shaderMulLighting.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		// shaderMulLighting.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		// shaderMulLighting.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotPosition));
		// shaderMulLighting.setVectorFloat3("spotLights[0].direction", glm::value_ptr(glm::vec3(0, -1, 0)));
		// shaderMulLighting.setFloat("spotLights[0].constant", 1.0);
		// shaderMulLighting.setFloat("spotLights[0].linear", 0.07);
		// shaderMulLighting.setFloat("spotLights[0].quadratic", 0.03);
		// shaderMulLighting.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5f)));
		// shaderMulLighting.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0f)));
		// shaderTerrain.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
		// shaderTerrain.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		// shaderTerrain.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		// shaderTerrain.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotPosition));
		// shaderTerrain.setVectorFloat3("spotLights[0].direction", glm::value_ptr(glm::vec3(0, -1, 0)));
		// shaderTerrain.setFloat("spotLights[0].constant", 1.0);
		// shaderTerrain.setFloat("spotLights[0].linear", 0.07);
		// shaderTerrain.setFloat("spotLights[0].quadratic", 0.03);
		// shaderTerrain.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5f)));
		// shaderTerrain.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0f)));

		/*******************************************
		 * Propiedades PointLights
		 *******************************************/
		
		/************Render de imagen Menú**************/
		if(!iniciaPartida){
			shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
			shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureActivaID);
			shaderTexture.setInt("outTexture", 0);
			boxIntro.render();
			glfwSwapBuffers(window);
			continue;
		}
		
		

		/*******************************************
		 * Terrain Cesped
		 *******************************************/
		// Se activa la textura del agua
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureCespedID);
		shaderTerrain.setInt("backgroundTexture", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
		shaderTerrain.setInt("rTexture", 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
		shaderTerrain.setInt("gTexture", 2);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
		shaderTerrain.setInt("bTexture", 3);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
		shaderTerrain.setInt("blendMapTexture", 4);
		shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(80, 80)));
		terrain.setPosition(glm::vec3(100, 0, 100));
		terrain.render();
		terrain2.setPosition(glm::vec3(150, 0, 150));
		terrain2.render();
		shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(70, 70)));
		glBindTexture(GL_TEXTURE_2D, 0);

		/*******************************************
		 * Custom objects obj
		 *******************************************/


		glActiveTexture(GL_TEXTURE0);

	// ++++++++++++++++++++++++++++++++++++++ Render de arboles +++++++++++++++++++++++++++++++++++++++++++++

		// arboles 1 sinhojas posiciones 
		for(int i = 0; i < arboles1position.size(); i++){
			arboles1position[i].y = terrain.getHeightTerrain(arboles1position[i].x, arboles1position[i].z);
			modelArbol1.setPosition(arboles1position[i]);
			modelArbol1.setScale(glm::vec3(2.0));
			modelArbol1.setOrientation(glm::vec3(0, arboles1Orientacion[i], 0));
			modelArbol1.render();
		}
		// arboles 2 sin hojas posiciones 
		for(int i = 0; i < arboles2position.size(); i++){
			arboles2position[i].y = terrain.getHeightTerrain(arboles2position[i].x, arboles2position[i].z);
			modelArbol2.setPosition(arboles2position[i]);
			modelArbol2.setScale(glm::vec3(2.0));
			modelArbol2.setOrientation(glm::vec3(0, arboles2Orientacion[i], 0));
			modelArbol2.render();
		}
		// arboles 3 sin hojas posiciones
		for(int i = 0; i < arboles3position.size(); i++){
			arboles3position[i].y = terrain.getHeightTerrain(arboles3position[i].x, arboles3position[i].z);
			modelArbol3.setPosition(arboles3position[i]);
			modelArbol3.setScale(glm::vec3(1.5));
			modelArbol3.setOrientation(glm::vec3(0, arboles3Orientacion[i], 0));
			modelArbol3.render();
		}
		// casa positions
		for(int i = 0; i < casaPosition.size(); i++){
			casaPosition[i].y = terrain2.getHeightTerrain(casaPosition[i].x, casaPosition[i].z);
			modelcasa.setPosition(casaPosition[i]);
			modelcasa.setScale(glm::vec3(0.1));
			modelcasa.setOrientation(glm::vec3(0, casaOrientcion[i], 0));
			modelcasa.render();
		}
		// arboles con hojas 1 posiciones 
		for(int i = 0; i < arbolesConHojas1position.size(); i++){
			arbolesConHojas1position[i].y = terrain.getHeightTerrain(arbolesConHojas1position[i].x, arbolesConHojas1position[i].z);
			modelarbolconhojas1.setPosition(arbolesConHojas1position[i]);
			modelarbolconhojas1.setScale(glm::vec3(10.0));
			modelarbolconhojas1.setOrientation(glm::vec3(0, arbolesConHojas1Orientacion[i], 0));
			modelarbolconhojas1.render();
		}
		// arboles con hojas 2 posiciones  // revisar centro en blender vovler a exportar 
		for(int i = 0; i < arbolesConHojas2position.size(); i++){
			arbolesConHojas2position[i].y = terrain.getHeightTerrain(arbolesConHojas2position[i].x, arbolesConHojas2position[i].z);
			modelarbolconhojas2.setPosition(arbolesConHojas2position[i]);
			modelarbolconhojas2.setScale(glm::vec3(5.0));
			modelarbolconhojas2.setOrientation(glm::vec3(0, arbolesConHojas2Orientacion[i], 0));
			modelarbolconhojas2.render();
		}
		// Modelo de rocas 1 posiciones 
		for(int i = 0; i < Rock1Position.size(); i++){
			Rock1Position[i].y = terrain.getHeightTerrain(Rock1Position[i].x, Rock1Position[i].z);
			modelRock1.setPosition(Rock1Position[i]);
			modelRock1.setScale(glm::vec3(1.0));
			modelRock1.setOrientation(glm::vec3(0, Rock1Orientacion[i], 0));
			modelRock1.render();
		}
		// modelo de rockas 2 posiciones 
		for(int i = 0; i < Rock2Position.size(); i++){
			Rock2Position[i].y = terrain.getHeightTerrain(Rock2Position[i].x, Rock2Position[i].z);
			modelRock2.setPosition(Rock2Position[i]);
			modelRock2.setScale(glm::vec3(1.0));
			modelRock2.setOrientation(glm::vec3(0, Rock2Orientacion[i], 0));
			modelRock2.render();
		}
		// 

		glm::mat4 modelMatrixArbolGitant = glm::mat4(modelMatrixArrbolGrande);
		modelMatrixArbolGitant = glm::translate(modelMatrixArbolGitant, glm::vec3(-108.5, 7, -147));
		modelMatrixArbolGitant = glm::scale(modelMatrixArbolGitant, glm::vec3(5.0,3.0,5.0));
		modelArbolGigant.render(modelMatrixArbolGitant);
		//render laberinto
		glm::mat4 modelMatrixLaberinto = glm::mat4(modelMatrixLabe);
		modelMatrixLaberinto = glm::translate(modelMatrixLaberinto, glm::vec3(-108.5,0,-147));
		modelMatrixLaberinto = glm::scale(modelMatrixLaberinto, glm::vec3(1.01f));
		modelLaberinto.render(modelMatrixLaberinto);
		// pruebas de escritorio
		glm::mat4 modelMatrixprueba = glm::mat4(modelMatrixPosition);
		modelMatrixprueba = glm::translate(modelMatrixprueba, glm::vec3(movx, 0, movz));
		modelMatrixprueba = glm::scale(modelMatrixprueba, glm::vec3(3.0));
		Position.render(modelMatrixprueba);
	// ++++++++++++++++++++++++++++++++++++++ Fin Render de arboles +++++++++++++++++++++++++++++++++++++++++++++


		/*****************************************
		 * Objetos animados por huesos
		 * **************************************/
		// glm::vec3 ejey = glm::normalize(terrain.getNormalTerrain(modelMatrixMayow[3][0], modelMatrixMayow[3][2]));
		// glm::vec3 ejex = glm::vec3(modelMatrixMayow[0]);
		// glm::vec3 ejez = glm::normalize(glm::cross(ejex, ejey));
		// ejex = glm::normalize(glm::cross(ejey, ejez));
		// modelMatrixMayow[0] = glm::vec4(ejex, 0.0);
		// modelMatrixMayow[1] = glm::vec4(ejey, 0.0);
		// modelMatrixMayow[2] = glm::vec4(ejez, 0.0);
		modelMatrixMayow[3][1] = -GRAVITY * tmv * tmv + 3.5 * tmv + terrain.getHeightTerrain(modelMatrixMayow[3][0], modelMatrixMayow[3][2]);
		tmv = currTime - startTimeJump;
		if(modelMatrixMayow[3][1] < terrain.getHeightTerrain(modelMatrixMayow[3][0], modelMatrixMayow[3][2])){
			isJump = false;
			modelMatrixMayow[3][1] = terrain.getHeightTerrain(modelMatrixMayow[3][0], modelMatrixMayow[3][2]);
		}
		glm::mat4 modelMatrixMayowBody = glm::mat4(modelMatrixMayow);
		modelMatrixMayowBody = glm::scale(modelMatrixMayowBody, glm::vec3(0.021f));
		mayowModelAnimate.setAnimationIndex(animationMayowIndex);
		mayowModelAnimate.render(modelMatrixMayowBody);
		animationMayowIndex = 1;

		
		zombie.setScale(glm::vec3(3.0));
		modelMatrixZombie[3][1] = terrain.getHeightTerrain(modelMatrixZombie[3][0], modelMatrixZombie[3][2]);
		zombie.render(modelMatrixZombie);
	

		/*******************************************
		 * Skybox
		 *******************************************/
		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;
		// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
		shaderSkybox.setFloat("skybox", 0);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		glActiveTexture(GL_TEXTURE0);
		skyboxSphere.render();
		glCullFace(oldCullFaceMode);
		glDepthFunc(oldDepthFuncMode);

		/*******************************************
		 * Creacion de colliders
		 * IMPORTANT do this before interpolations
		 *******************************************/


	//+++++++++++++++++++++++++++++++++++++++ Colisiones de arboles ++++++++++++++++++++++++++++++++++++++++++++++++++ 
		for (int i = 0; i < arboles1position.size(); i++){
			AbstractModel::OBB Arbol1Collider;
			glm::mat4 modelMatrixColliderArbol1 = glm::mat4(1.0);
			modelMatrixColliderArbol1 = glm::translate(modelMatrixColliderArbol1, arboles1position[i]);
			modelMatrixColliderArbol1 = glm::rotate(modelMatrixColliderArbol1, glm::radians(arboles1Orientacion[i]),
					glm::vec3(0, 1, 0));
			addOrUpdateColliders(collidersOBB, " ArbolModel1-" + std::to_string(i), Arbol1Collider, modelMatrixColliderArbol1);
			// Set the orientation of collider before doing the scale
			Arbol1Collider.u = glm::quat_cast(modelMatrixColliderArbol1);
			modelMatrixColliderArbol1 = glm::scale(modelMatrixColliderArbol1, glm::vec3(0, 0.4,0));// posicion de la caja 
			modelMatrixColliderArbol1 = glm::translate(modelMatrixColliderArbol1, modelArbol1.getObb().c);
			Arbol1Collider.c = glm::vec3(modelMatrixColliderArbol1[3]);
			Arbol1Collider.e = modelArbol1.getObb().e * glm::vec3(0.05, 0.4, 0.05);  // tamaño de la caja de colision
			std::get<0>(collidersOBB.find(" ArbolModel1-" + std::to_string(i))->second) = Arbol1Collider;
		}
		// arboles 2 colisiones 
		for (int i = 0; i < arboles2position.size(); i++){
			AbstractModel::OBB Arbol2Collider;
			glm::mat4 modelMatrixColliderArbol2 = glm::mat4(1.0);
			modelMatrixColliderArbol2 = glm::translate(modelMatrixColliderArbol2, arboles2position[i]);
			modelMatrixColliderArbol2 = glm::rotate(modelMatrixColliderArbol2, glm::radians(arboles2Orientacion[i]),
					glm::vec3(0, 1, 0));
			addOrUpdateColliders(collidersOBB, "ArbolModel2-" + std::to_string(i), Arbol2Collider, modelMatrixColliderArbol2);
			// Set the orientation of collider before doing the scale
			Arbol2Collider.u = glm::quat_cast(modelMatrixColliderArbol2);
			modelMatrixColliderArbol2 = glm::scale(modelMatrixColliderArbol2, glm::vec3(0,0.4,0));// posicion de la caja deoluson modifica solo eje y para darle altura 
			modelMatrixColliderArbol2 = glm::translate(modelMatrixColliderArbol2, modelArbol2.getObb().c);
			Arbol2Collider.c = glm::vec3(modelMatrixColliderArbol2[3]);
			Arbol2Collider.e = modelArbol2.getObb().e * glm::vec3(0.08, 0.4, 0.15);  // tamaño de la caja de colision
			std::get<0>(collidersOBB.find("ArbolModel2-" + std::to_string(i))->second) = Arbol2Collider;
		}

		// colisiones de arboles con hojas 1 
		for (int i = 0; i < arbolesConHojas1position.size(); i++){
			AbstractModel::OBB ArbolesConHojasCollider;
			glm::mat4 modelMatrixCollidersArbolesConHojas1 = glm::mat4(1.0);
			modelMatrixCollidersArbolesConHojas1 = glm::translate(modelMatrixCollidersArbolesConHojas1, arbolesConHojas1position[i]);
			modelMatrixCollidersArbolesConHojas1 = glm::rotate(modelMatrixCollidersArbolesConHojas1, glm::radians(arbolesConHojas1Orientacion[i]),
					glm::vec3(0, 1, 0));
			addOrUpdateColliders(collidersOBB, "arbolconhjas1Model-" + std::to_string(i), ArbolesConHojasCollider, modelMatrixarbolconhojas1);
			// Set the orientation of collider before doing the scale
			ArbolesConHojasCollider.u = glm::quat_cast(modelMatrixCollidersArbolesConHojas1);
			modelMatrixCollidersArbolesConHojas1 = glm::scale(modelMatrixCollidersArbolesConHojas1, glm::vec3(0,0.4,0));// posicion de la caja deoluson modifica solo eje y para darle altura 
			modelMatrixCollidersArbolesConHojas1 = glm::translate(modelMatrixCollidersArbolesConHojas1, modelarbolconhojas1.getObb().c);
			ArbolesConHojasCollider.c = glm::vec3(modelMatrixCollidersArbolesConHojas1[3]);
			ArbolesConHojasCollider.e = modelarbolconhojas1.getObb().e * glm::vec3(0.3, 0.8, 0.3);  // tamaño de la caja de colision
			std::get<0>(collidersOBB.find("arbolconhjas1Model-" + std::to_string(i))->second) = ArbolesConHojasCollider;
		}
	//+++++++++++++++++++++++++++++++++++++++ Termina Colisiones de arboles ++++++++++++++++++++++++++++++++++++++++++++++++++

	//+++++++++++++++++++++++++++++++++++++++  Colisiones de casas ++++++++++++++++++++++++++++++++++++++++++++++++++
	
		for (int i = 0; i < casaPosition.size(); i++){
			AbstractModel::OBB CasaCollider;
			glm::mat4 modelMatrixColliderscasas = glm::mat4(1.0);
			modelMatrixColliderscasas = glm::translate(modelMatrixColliderscasas, casaPosition[i]);
			modelMatrixColliderscasas = glm::rotate(modelMatrixColliderscasas, glm::radians(casaOrientcion[i]),
					glm::vec3(0, 1, 0));
			addOrUpdateColliders(collidersOBB, "CasaModel-" + std::to_string(i), CasaCollider, modelMatrixcasa);
			// Set the orientation of collider before doing the scale
			CasaCollider.u = glm::quat_cast(modelMatrixColliderscasas);
			modelMatrixColliderscasas = glm::scale(modelMatrixColliderscasas, glm::vec3(0,0.1,0));// posicion de la caja deoluson modifica solo eje y para darle altura 
			modelMatrixColliderscasas = glm::translate(modelMatrixColliderscasas, modelcasa.getObb().c);
			CasaCollider.c = glm::vec3(modelMatrixColliderscasas[3]);
			CasaCollider.e = modelcasa.getObb().e * glm::vec3(0.07, 0.1, 0.06);  // tamaño de la caja de colision
			std::get<0>(collidersOBB.find("CasaModel-" + std::to_string(i))->second) = CasaCollider;
		}
	//+++++++++++++++++++++++++++++++++++++++ Termina Colisiones de casas ++++++++++++++++++++++++++++++++++++++++++++++++++

	//+++++++++++++++++++++++++++++++++++++++  Colisiones de casas ++++++++++++++++++++++++++++++++++++++++++++++++++
		// colisiones de rocas 1
		for (int i = 0; i < Rock1Position.size(); i++){
			AbstractModel::OBB Rock1Collider;
			glm::mat4 modelMatrixColliderRock1 = glm::mat4(1.0);
			modelMatrixColliderRock1 = glm::translate(modelMatrixColliderRock1, Rock1Position[i]);
			modelMatrixColliderRock1 = glm::rotate(modelMatrixColliderRock1, glm::radians(Rock1Orientacion[i]),
					glm::vec3(0, 1, 0));
			addOrUpdateColliders(collidersOBB, "RockModel-" + std::to_string(i), Rock1Collider, modelMatrixRock1);
			// Set the orientation of collider before doing the scale
			Rock1Collider.u = glm::quat_cast(modelMatrixColliderRock1);
			modelMatrixColliderRock1 = glm::scale(modelMatrixColliderRock1, glm::vec3(0,0.2,0));// posicion de la caja deoluson modifica solo eje y para darle altura 
			modelMatrixColliderRock1 = glm::translate(modelMatrixColliderRock1, modelRock1.getObb().c);
			Rock1Collider.c = glm::vec3(modelMatrixColliderRock1[3]);
			Rock1Collider.e = modelRock1.getObb().e * glm::vec3(0.6, 0.6, 0.6);  // tamaño de la caja de colision
			std::get<0>(collidersOBB.find("RockModel-" + std::to_string(i))->second) = Rock1Collider;
		}
	// colisiones de rocas 2 
		for (int i = 0; i < Rock2Position.size(); i++){
			AbstractModel::OBB Rock2Collider;
			glm::mat4 modelMatrixColliderRock2 = glm::mat4(1.0);
			modelMatrixColliderRock2 = glm::translate(modelMatrixColliderRock2, Rock2Position[i]);
			modelMatrixColliderRock2 = glm::rotate(modelMatrixColliderRock2, glm::radians(Rock2Orientacion[i]),
					glm::vec3(0, 1, 0));
			addOrUpdateColliders(collidersOBB, "Rock2Model-" + std::to_string(i), Rock2Collider, modelMatrixRock2);
			// Set the orientation of collider before doing the scale
			Rock2Collider.u = glm::quat_cast(modelMatrixColliderRock2);
			modelMatrixColliderRock2 = glm::scale(modelMatrixColliderRock2, glm::vec3(0,0.2,0));// posicion de la caja deoluson modifica solo eje y para darle altura 
			modelMatrixColliderRock2 = glm::translate(modelMatrixColliderRock2, modelRock2.getObb().c);
			Rock2Collider.c = glm::vec3(modelMatrixColliderRock2[3]);
			Rock2Collider.e = modelRock1.getObb().e * glm::vec3(0.6, 0.6, 0.6);  // tamaño de la caja de colision
			std::get<0>(collidersOBB.find("Rock2Model-" + std::to_string(i))->second) = Rock2Collider;
		}
	//+++++++++++++++++++++++++++++++++++++++ Termina Colisiones de casas ++++++++++++++++++++++++++++++++++++++++++++++++++


		// Collider de mayow
		AbstractModel::OBB mayowCollider;
		glm::mat4 modelmatrixColliderMayow = glm::mat4(modelMatrixMayow);
		modelmatrixColliderMayow = glm::rotate(modelmatrixColliderMayow,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		mayowCollider.u = glm::quat_cast(modelmatrixColliderMayow);
		modelmatrixColliderMayow = glm::scale(modelmatrixColliderMayow, glm::vec3(0.021, 0.021, 0.021));
		modelmatrixColliderMayow = glm::translate(modelmatrixColliderMayow,
				glm::vec3(mayowModelAnimate.getObb().c.x,
						mayowModelAnimate.getObb().c.y,
						mayowModelAnimate.getObb().c.z));
		mayowCollider.e = mayowModelAnimate.getObb().e * glm::vec3(0.021, 0.021, 0.021) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
		mayowCollider.c = glm::vec3(modelmatrixColliderMayow[3]);
		addOrUpdateColliders(collidersOBB_P, "mayow", mayowCollider, modelMatrixMayow);

		/*******************************************
		 * Render de colliders
		 *******************************************/
		for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
				collidersOBB.begin(); it != collidersOBB.end(); it++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			matrixCollider = matrixCollider * glm::mat4(std::get<0>(it->second).u);
			matrixCollider = glm::scale(matrixCollider, std::get<0>(it->second).e * 2.0f);
			boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
			boxCollider.enableWireMode();
			boxCollider.render(matrixCollider);
		}
		
		for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
				collidersOBB_P.begin(); it != collidersOBB_P.end(); it++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			matrixCollider = matrixCollider * glm::mat4(std::get<0>(it->second).u);
			matrixCollider = glm::scale(matrixCollider, std::get<0>(it->second).e * 2.0f);
			boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
			boxCollider.enableWireMode();
			boxCollider.render(matrixCollider);
		}

		for (std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
				collidersSBB.begin(); it != collidersSBB.end(); it++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			matrixCollider = glm::scale(matrixCollider, glm::vec3(std::get<0>(it->second).ratio * 2.0f));
			sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
			sphereCollider.enableWireMode();
			sphereCollider.render(matrixCollider);
		}

		hongos();

		/**********
		 * Render de las transparencias
		 */
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);
	
		glEnable(GL_CULL_FACE);
		glDisable(GL_BLEND);

		
		shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
		shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureActivaID);
		shaderTexture.setInt("outTexture", 0);
		glEnable(GL_BLEND);
		boxIntro.render();
		glDisable(GL_BLEND);

		if(vida < 5){
			
			if(vida == 4){
				textureActivaID = textureBlood1ID;
			}else if(vida == 3){
				textureActivaID = textureBlood2ID;
			}else if(vida == 2){
				textureActivaID = textureBlood3ID;
			}else if(vida == 1){
				textureActivaID = textureBlood4ID;
			}
			
			
		}else{
			textureActivaID = textureScreenID;
		}
		if(plusLife){
				textureActivaID = textureSaveID;
				// shaderTexture.setMatrix4("projection", 2, false, glm::value_ptr(glm::mat4(1.0)));
				// shaderTexture.setMatrix4("view", 2, false, glm::value_ptr(glm::mat4(1.0)));
				// glActiveTexture(GL_TEXTURE0);
				// glBindTexture(GL_TEXTURE_2D, textureSaveID);
				// shaderTexture.setInt("outTexture", 0);
				// glEnable(GL_BLEND);
				// boxPlusLife.render();
				// glDisable(GL_BLEND);
		}

		shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
		shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureActivaID);
		shaderTexture.setInt("outTexture", 0);
		glEnable(GL_BLEND);
		boxIntro.render();
		glDisable(GL_BLEND);

		/*********************Prueba de colisiones****************************/
		for (std::map<std::string,
			std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator it =
			collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator jt =
				collidersSBB.begin(); jt != collidersSBB.end(); jt++) {
				if (it != jt && testSphereSphereIntersection(
					std::get<0>(it->second), std::get<0>(jt->second))) {
					std::cout << "Hay collision entre " << it->first <<
						" y el modelo " << jt->first << std::endl;
					isCollision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator it =
			collidersOBB_P.begin(); it != collidersOBB_P.end(); it++) {
			bool isColision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator jt =
				collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
				if (it != jt && 
					testOBBOBB(std::get<0>(it->second), std::get<0>(jt->second))) {
					std::cout << "Hay colision entre " << it->first << " y el modelo" <<
						jt->first << std::endl;
					isColision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isColision);
		}

		
		
		
		std::map<std::string, bool>::iterator itCollision;
		for (itCollision = collisionDetection.begin(); itCollision != collisionDetection.end(); itCollision++) {
			
			std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator sbbBuscado = collidersSBB.find(itCollision->first);
			
			std::map<std::string, std::tuple<AbstractModel::OBB,glm::mat4, glm::mat4>>::iterator obbBuscado = collidersOBB_P.find(itCollision->first);
			
			if (sbbBuscado != collidersSBB.end()) {
				if (!itCollision->second) 
					addOrUpdateColliders(collidersSBB, itCollision->first);
			}
			if (obbBuscado != collidersOBB_P.end()) {
				if (!itCollision->second) 
					addOrUpdateColliders(collidersOBB_P, itCollision->first);
				else {
					if (itCollision->first.compare("mayow") == 0)
						modelMatrixMayow = std::get<1>(obbBuscado->second);
				}
			}
		}


	//++++++++++++++++++++++++++++++ Render Rayo Personaje ++++++++++++++++++++++++++++++++
		glm::mat4 modelMatrixRayMay = glm::mat4(modelMatrixMayow);
		modelMatrixRayMay = glm::translate(modelMatrixRayMay, glm::vec3(0, 1, 0));
		float maxDistanceRay = 10.0;
		glm::vec3 rayDirection = modelMatrixRayMay[2];
		glm::vec3 ori = modelMatrixRayMay[3];
		glm::vec3 rmd = ori + rayDirection * (maxDistanceRay / 2.0f);
		glm::vec3 targetRay = ori + rayDirection * maxDistanceRay;
		modelMatrixRayMay[3] = glm::vec4(rmd, 1.0);
		modelMatrixRayMay = glm::rotate(modelMatrixRayMay, glm::radians(90.0f), 
			glm::vec3(1, 0, 0));
		modelMatrixRayMay = glm::scale(modelMatrixRayMay, 
			glm::vec3(0.05, maxDistanceRay, 0.05));
		rayModel.render(modelMatrixRayMay);

		std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::
			iterator itSBB;
		for (itSBB = collidersSBB.begin(); itSBB != collidersSBB.end(); itSBB++) {
			float tRint;
			if (raySphereIntersect(ori, targetRay, rayDirection,
				std::get<0>(itSBB->second), tRint)) {
				std::cout << "Collision del rayo con el modelo " << itSBB->first 
				<< std::endl;
			}
		}
	//+++++++++++++++++++++++++++++ Termina Rayo Personaje +++++++++++++++++++++++++++++++++


		std::string hongos_Az = std::to_string(cont_Hongo_A);
		std::string hongos_M = std::to_string(cont_Hongo_M);
		std::string velocidad = std::to_string(speedP);
		std::string vidaP = std::to_string(vida);
		std::string plusL = std::to_string(plusLife);

		modelText->render("Az: "+ hongos_Az, -0.9, -0.9);
		modelText->render("Mor:"+ hongos_M, -0.5, -0.9);
		modelText->render("Plus:"+ plusL, -0.1, -0.9);
		modelText->render("Vida:"+ vidaP, 0.8, 0.9);

		glfwSwapBuffers(window);
	}
}

int main(int argc, char **argv) {
	init(1800, 1000, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}