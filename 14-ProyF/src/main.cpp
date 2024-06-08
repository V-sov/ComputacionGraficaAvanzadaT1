#define _USE_MATH_DEFINES


// Botones de acción
#define TRIANGLE_BUTTON 3
#define CIRCLE_BUTTON 1
#define CROSS_BUTTON 2
#define SQUARE_BUTTON 0

// Botones de los joysticks
#define JOYSTICK_LEFT_BUTTON 10
#define JOYSTICK_RIGHT_BUTTON 11

// Botones direccionales (D-pad)
#define DPAD_UP 12
#define DPAD_DOWN 13
#define DPAD_LEFT 14
#define DPAD_RIGHT 15

// Botones de los hombros
#define L1_BUTTON 4
#define R1_BUTTON 5
#define L2_BUTTON 6
#define R2_BUTTON 7

// Botones de opciones
#define START_BUTTON 8
#define SELECT_BUTTON 9

#define LEFT_JOYSTICK_HORIZONTAL_AXIS 0
#define LEFT_JOYSTICK_VERTICAL_AXIS 1

// Botón central (a menudo tiene un logo de la consola o del fabricante)
#define HOME_BUTTON 16

#include <cmath>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

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

// Font rendering include
#include "Headers/FontTypeRendering.h" 

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

// ShadowBox include
#include "Headers/ShadowBox.h"

// OpenAL include
#include <AL/alut.h>

// Sleep
#include <windows.h>  
#include <unistd.h>

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;
int buttonCount;

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

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

Shader shaderDepth;

Shader shaderViewDepth;

std::shared_ptr<Camera> camera(new ThirdPersonCamera()); //Intancia de la camara en tercera persona
std::shared_ptr<FirstPersonCamera> cameraFP(new FirstPersonCamera());
float distanceFromTarget = 15.0;

//Botón de pausa
bool pause = false;
bool pauseInicio = true;
//Activación de pantallas
bool iniciorel = true;
bool resumeAct = false;
bool keyrelease = true;
bool menuRelease = true;
bool ctrlRelease = true;
bool muerteRelease = true;
bool controlRelease = true;
bool checkRelease = true;

//Valores para checkpoint
// Pos. Arcos
float arco1x = 11.0;
float arco1y = 0.0;
float arco1z = 35.0;
// Vidas
int vida = 3;
int warco = 0;
float warcoy = 10.60f;
int starmur = 0;
bool chec = false;

Sphere skyboxSphere(20, 20);

Box boxCesped;
Box boxHighway;
Box boxLandingPad;
Box boxCollider;
Sphere sphereCollider(10, 10);

Box boxIntro;
Box boxViewDepth;

// Models complex instances
Model modelRock;
Model modelArc;
Model modelTower;
Model modelIsle;
Model modelBasePuente;
Model modelBordePuente;
Model modelPicasPuente;
Model modelCube;
Model modelCerbero;
// Lamps
Model modelLamp1;
Model modelLamp2;
Model modelLampPost2;
//FarosPropios
Model modelFaro;
// Modelos animados
// Main Character
Model heroeModelAnimate;

// Terrain model instance
Terrain terrain(-1, -1, 350, 87.0, "../Textures/heightmap5.png");

ShadowBox * shadowBox;

GLuint textureCespedID, textureWindowID, textureHighwayID, textureLandingPadID, textureArcID, textureIsleID;
GLuint textureTerrainRID, textureTerrainGID, textureTerrainBID, textureTerrainBlendMapID;
GLuint skyboxTextureID;
// Textura de inicio y pausa
GLuint textureStartID, textureResumeID, textureActivaID, textureMenuID, textureScreen2ID, textureControlID, textureKeysID;   
GLuint textureMuerteID;

//Proyecto

bool iniciaPartida = false, presionarOpcion = false, uno = false, muerte = false, isThirdCamera=true, changingCamera=false;;
// Modelos para el render del texto
FontTypeRendering::FontTypeRendering *modelTextMuerte;
FontTypeRendering::FontTypeRendering *modelTextRegresoInicio;
FontTypeRendering::FontTypeRendering *modelTextInicioPartida;
FontTypeRendering::FontTypeRendering *modelTextResume;
FontTypeRendering::FontTypeRendering *modelTextControles;


/*EMPIEZAN LAS DEFINICIONES DE LOS ARCHIVOS DE TEXTURA DEL SKY BOX*/
GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

//Prueba skybox 2
std::string fileNames[6] = { "../Textures/mp_proyf/redeclipse_ft.png",
		"../Textures/mp_proyf/redeclipse_bk.png",
		"../Textures/mp_proyf/redeclipse_up.png",
		"../Textures/mp_proyf/redeclipse_dn.png",
		"../Textures/mp_proyf/redeclipse_rt.png",
		"../Textures/mp_proyf/redeclipse_lf.png" };
bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

// Model matrix definitions
glm::mat4 matrixModelRock = glm::mat4(1.0);
glm::mat4 matrixModelArc = glm::mat4(1.0);
glm::mat4 matrixModelTower = glm::mat4(1.0);
glm::mat4 matrixModelIsle = glm::mat4(1.0);
glm::mat4 matrixModelBasePuente = glm::mat4(1.0);
glm::mat4 matrixModelBordePuente = glm::mat4(1.0);
glm::mat4 matrixModelCube = glm::mat4(1.0);
glm::mat4 matrixModelCerbero = glm::mat4(1.0);
glm::mat4 modelMatrixHeroe = glm::mat4(1.0f);

int animationHeroeIndex = 0;
int modelSelected = 0;
bool enableCountSelected = true;

// Variables to animations keyframes
bool saveFrame = false, availableSave = true;
std::ofstream myfile;
std::string fileName = "";
bool record = false;

// Lamps position

std::vector<glm::vec3> lamp2Position = {
	glm::vec3(-36.52, 0, -23.24),
	glm::vec3(-52.73, 0, -3.90)
};
std::vector<float> lamp2Orientation = {
	21.37 + 90, -65.0 + 90
};

//Lamps Position
std::vector<glm::vec3> FaroPosition = {
	glm::vec3(52.0, 0, 43.5), 
	glm::vec3(52.0, 0, 32),
	glm::vec3(52.0, 0, 20.5),
	glm::vec3(52.0, 0, 9),
	glm::vec3(52.0, 0, -3.5),
	glm::vec3(52.0, 0, -14.5),
	glm::vec3(52.0, 0, -26.0),
	glm::vec3(52.0, 0, -38.0),
	glm::vec3(52.0, 0, -49.5),
	glm::vec3(52.0, 0, -61.5),
	glm::vec3(52.0, 0, -73.0),
	glm::vec3(52.0, 0, -85.0),
	glm::vec3(52.0, 0, -96.5),
	glm::vec3(52.0, 0, -108.0),
	glm::vec3(52.0, 0, -120.0),
	glm::vec3(52.0, 0, -131.5),
	glm::vec3(52.0, 0, -143.2),
	glm::vec3(52.0, 0, -154.7),
	
	glm::vec3(40.0, 0, -143.2),
	glm::vec3(40.0, 0, -154.7),
};
std::vector<float> FaroOrientation = {
	-17.0, -82.67, 23.70, 23.70
}; 
//Puente
std::vector<glm::vec3> PuentePosition = {
	glm::vec3(0.0, 0.0, 0.0), 
	glm::vec3(0.0, 1.95, 0.0),
	glm::vec3(0.0, 3.9, 0.0),

	glm::vec3(0.0, 7.8, 0.0), 
	
	glm::vec3(0.0, 11.7, 0.0),
	glm::vec3(0.0, 13.65, 0.0),

	glm::vec3(0.0, 17.55, 0.0), 
	glm::vec3(0.0, 19.5, 0.0),
	glm::vec3(0.0, 21.45, 0.0),

	glm::vec3(0.0, 25.35, 0.5), 
	glm::vec3(0.0, 27.3, 0.5),
	glm::vec3(0.0, 29.25, 0.5),
	glm::vec3(0.0, 31.2, 0.5),
	
};
//Islas
std::vector<glm::vec3> IslaPosition = {
	glm::vec3(0.0, 0.0, 0.0), 
	glm::vec3(0.0, 5.0, 1.0),
	glm::vec3(7.0, 7.0, 2.0),
	glm::vec3(9.0, 12.0, 3.0),
	glm::vec3(7.0, 18.0, 4.0),
	glm::vec3(1.0, 22.0, 5.0),
	glm::vec3(-6.0, 21.0, 6.0),
	glm::vec3(-8.0, 15.0, 7.5),
	glm::vec3(-6.0, 8.0, 9.0),
	
};
//
std::vector<glm::vec3> ArcoPosition = {
	glm::vec3(0.0, 0.0, 0.0),
	glm::vec3(0.0, 0.0, -180.0),

};


double deltaTime;
double currTime, lastTime;


// Jump variables
bool isJump = false;
bool isOn = false;
float GRAVITY = 1.01;
double tmv = 0;
double startTimeJump = 0;

// Colliders
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB;
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > collidersSBB;

const float avance = 0.1;

// OpenAL Defines
#define NUM_BUFFERS 3
#define NUM_SOURCES 3
#define NUM_ENVIRONMENTS 1
// Listener
ALfloat listenerPos[] = { 0.0, 0.0, 0.0 };
ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
ALfloat listenerOri[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 0.0 };
// Source 0
ALfloat source0Pos[] = { 0.0, 0.0, 0.0 };
ALfloat source0Vel[] = { 0.0, 0.0, 0.0 };
// Source 1
ALfloat source1Pos[] = { 0.0, 0.0, 0.0 };
ALfloat source1Vel[] = { 0.0, 0.0, 0.0 };
// Source 2
ALfloat source2Pos[] = { 0.0, 0.0, 0.0 };
ALfloat source2Vel[] = { 0.0, 0.0, 0.0 };
// Buffers
ALuint buffer[NUM_BUFFERS];
ALuint source[NUM_SOURCES];
ALuint environment[NUM_ENVIRONMENTS];
// Configs
ALsizei size, freq;
ALenum format;
ALvoid *data;
int ch;
ALboolean loop;
std::vector<bool> sourcesPlay = {true, true, true};

// Framesbuffers
GLuint depthMap, depthMapFBO;

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

// Implementacion de todas las funciones.

// Ver lo de la caída
bool isHeroeOn(const AbstractModel::OBB& heroe, const AbstractModel::OBB& surf){
	// Posiciones mínimas y máximas de los OBB
	glm::vec3 surfMin = surf.c - surf.e;
    glm::vec3 surfMax = surf.c + surf.e;
	glm::vec3 heroeMin = heroe.c - heroe.e;
    glm::vec3 heroeMax = heroe.c + heroe.e;

	 // Verificamos si el héroe está dentro de los límites horizontales de la isla
    bool withinX = heroeMax.x >= surfMin.x && heroeMin.x <= surfMax.x;
    bool withinZ = heroeMax.z >= surfMin.z && heroeMin.z <= surfMax.z;

    // Verificamos si la base del héroe está tocando la parte superior de la isla
    bool onTop = heroeMin.y <= surfMax.y && heroeMax.y >= surfMax.y;

    return withinX && withinZ && onTop;


}

//Implementación para el botón la muerte
void accionMuerte(){
	//textureActivaID = textureStartID;
	textureActivaID = textureStartID;
	iniciaPartida = false;
	muerte = false;
}

void accionChec(){
    if(vida == 0){
		muerte = true;
		textureActivaID = textureMuerteID;
	}else{
		chec = true;
	}
}

void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
				glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
				nullptr);

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

	// Inicialización de los shaders
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation_shadow.vs", "../Shaders/multipleLights_shadow.fs");
	shaderTerrain.initialize("../Shaders/terrain_shadow.vs", "../Shaders/terrain_shadow.fs");
	shaderTexture.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado.fs");
	shaderViewDepth.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado_depth_view.fs");
	shaderDepth.initialize("../Shaders/shadow_mapping_depth.vs", "../Shaders/shadow_mapping_depth.fs");
	

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

	boxCesped.init();
	boxCesped.setShader(&shaderMulLighting);

	boxHighway.init();
	boxHighway.setShader(&shaderMulLighting);

	boxLandingPad.init();
	boxLandingPad.setShader(&shaderMulLighting);

	boxIntro.init();
	boxIntro.setShader(&shaderTexture);
	boxIntro.setScale(glm::vec3(2.0, 2.0, 1.0));

	boxViewDepth.init();
	boxViewDepth.setShader(&shaderViewDepth);

	modelRock.loadModel("../models/rock/rock.obj");
	modelRock.setShader(&shaderMulLighting);

	modelCube.loadModel("../models/cube/untitled.obj");
	modelCube.setShader(&shaderMulLighting);

	modelArc.loadModel("../models/Arco/Arco.fbx");
	modelArc.setShader(&shaderMulLighting);

	modelTower.loadModel("../models/Tower/Torre.fbx");
	modelTower.setShader(&shaderMulLighting);

	modelCerbero.loadModel("../models/Cerbero/Cerbero.fbx");
	modelCerbero.setShader(&shaderMulLighting);

	modelBasePuente.loadModel("../models/Puente/BasePuente.fbx");
	modelBasePuente.setShader(&shaderMulLighting);
	modelBordePuente.loadModel("../models/Puente/BordePuente.fbx");
	modelBordePuente.setShader(&shaderMulLighting);
	modelPicasPuente.loadModel("../models/Puente/PicasPuente.fbx");
	modelPicasPuente.setShader(&shaderMulLighting);

	modelIsle.loadModel("../models/Isla/Isla.fbx");
	modelIsle.setShader(&shaderMulLighting);
	//Lamps models
	modelLamp1.loadModel("../models/Street-Lamp-Black/objLamp.obj");
	modelLamp1.setShader(&shaderMulLighting);
	modelLamp2.loadModel("../models/Street_Light/Lamp.obj");
	modelLamp2.setShader(&shaderMulLighting);
	modelLampPost2.loadModel("../models/Street_Light/LampPost.obj");
	modelLampPost2.setShader(&shaderMulLighting);
	modelFaro.loadModel("../models/Farol/Faro.fbx");
	modelFaro.setShader(&shaderMulLighting);

	// Heroe model
	heroeModelAnimate.loadModel("../models/heroe/Heroe.fbx");
	heroeModelAnimate.setShader(&shaderMulLighting);

	// Terreno
	terrain.init();
	terrain.setShader(&shaderTerrain);

// Se inicializa los modelos de render text
	modelTextMuerte = new FontTypeRendering::FontTypeRendering(screenWidth, screenHeight);
	modelTextMuerte->Initialize();
	modelTextRegresoInicio = new FontTypeRendering::FontTypeRendering(screenWidth, screenHeight);
	modelTextRegresoInicio->Initialize();
	modelTextInicioPartida = new FontTypeRendering::FontTypeRendering(screenWidth, screenHeight);
	modelTextInicioPartida->Initialize();
	modelTextResume = new FontTypeRendering::FontTypeRendering(screenWidth, screenHeight);
	modelTextResume->Initialize();
 	modelTextControles = new FontTypeRendering::FontTypeRendering(screenWidth, screenHeight);
	modelTextControles->Initialize();


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
	Texture textureCesped("../Textures/rocaN.png");
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

	// Definiendo la textura a utilizar
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Creando la textura con id 1
	// Enlazar esa textura a una tipo de textura de 2D.
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura

	// Definiendo la textura a utilizar
	Texture textureWindow("../Textures/ventana.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureWindow.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureWindowID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureWindowID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureWindow.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, textureWindow.getChannels() == 3 ? GL_RGB : GL_RGBA, textureWindow.getWidth(), textureWindow.getHeight(), 0,
		textureWindow.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureWindow.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureWindow.freeImage();

	// Definiendo la textura a utilizar
	Texture textureHighway("../Textures/highway.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureHighway.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureHighwayID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureHighwayID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureHighway.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, textureHighway.getChannels() == 3 ? GL_RGB : GL_RGBA, textureHighway.getWidth(), textureHighway.getHeight(), 0,
		textureHighway.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureHighway.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureHighway.freeImage();

	// Definiendo la textura
	Texture textureLandingPad("../Textures/landingPad.jpg");
	textureLandingPad.loadImage(); // Cargar la textura
	glGenTextures(1, &textureLandingPadID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureLandingPadID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureLandingPad.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureLandingPad.getChannels() == 3 ? GL_RGB : GL_RGBA, textureLandingPad.getWidth(), textureLandingPad.getHeight(), 0,
		textureLandingPad.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureLandingPad.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureLandingPad.freeImage(); // Liberamos memoria

	//Arco
	Texture textureArc("../models/Arco/Arco_CGA.png");
	textureArc.loadImage(); // Cargar la textura
	glGenTextures(1, &textureArcID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureArcID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureArc.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureArc.getChannels() == 3 ? GL_RGB : GL_RGBA, textureArc.getWidth(), textureArc.getHeight(), 0,
		textureArc.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureArc.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureArc.freeImage(); // Liberamos memoria
	//Isla
	Texture textureIsle("../models/Isla/PiedraPasto.png");
	textureIsle.loadImage(); // Cargar la textura
	glGenTextures(1, &textureIsleID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureIsleID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureIsle.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureIsle.getChannels() == 3 ? GL_RGB : GL_RGBA, textureIsle.getWidth(), textureIsle.getHeight(), 0,
		textureIsle.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureIsle.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureIsle.freeImage(); // Liberamos memoria

	// Defininiendo texturas del mapa de mezclas
	// Definiendo la textura
	Texture textureR("../Textures/rocaT.png");
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
	Texture textureG("../Textures/cloud2.png");
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
	Texture textureB("../Textures/rocaA.png");
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
	Texture textureBlendMap("../Textures/blendMap2.png");
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
// Definiendo la textura
	Texture textureIntro1("../Textures/Intro1Cerbero.png");
	textureIntro1.loadImage(); // Cargar la textura
	glGenTextures(1, &textureStartID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureStartID); // Se enlaza la textura
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
	Texture textureIntro2("../Textures/Intro2Cerbero.png");
	textureIntro2.loadImage(); // Cargar la textura
	glGenTextures(1, &textureResumeID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureResumeID); // Se enlaza la textura
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

	// Definiendo la textura
	Texture textureMenu("../Textures/Menu.png");
	textureMenu.loadImage(); // Cargar la textura
	glGenTextures(1, &textureMenuID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureMenuID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureMenu.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureMenu.getChannels() == 3 ? GL_RGB : GL_RGBA, textureMenu.getWidth(), textureMenu.getHeight(), 0,
		textureMenu.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureMenu.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureMenu.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureScreen2("../Textures/Screen2.png");
	textureScreen2.loadImage(); // Cargar la textura
	glGenTextures(1, &textureScreen2ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureScreen2ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureScreen2.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureScreen2.getChannels() == 3 ? GL_RGB : GL_RGBA, textureScreen2.getWidth(), textureScreen2.getHeight(), 0,
		textureScreen2.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureScreen2.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureScreen2.freeImage(); // Liberamos memoria

	// Definiendo la textura Muerte
	Texture textureMuerte("../Textures/Skull.png");
	textureMuerte.loadImage(); // Cargar la textura
	glGenTextures(1, &textureMuerteID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureMuerteID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureMuerte.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureMuerte.getChannels() == 3 ? GL_RGB : GL_RGBA, textureMuerte.getWidth(), textureMuerte.getHeight(), 0,
		textureMuerte.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureMuerte.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureMuerte.freeImage(); // Liberamos memoria

	// Definiendo la textura 
	Texture textureControles("../Textures/ControlesSF.png");
	textureControles.loadImage(); // Cargar la textura
	glGenTextures(1, &textureControlID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureControlID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureControles.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureControles.getChannels() == 3 ? GL_RGB : GL_RGBA, textureControles.getWidth(), textureControles.getHeight(), 0,
		textureControles.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureControles.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureControles.freeImage(); // Liberamos memoria

	// Definiendo la textura textureKeysID
	Texture textureKeys("../Textures/KeyControls.png");
	textureKeys.loadImage(); // Cargar la textura
	glGenTextures(1, &textureKeysID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureKeysID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureKeys.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureKeys.getChannels() == 3 ? GL_RGB : GL_RGBA, textureKeys.getWidth(), textureKeys.getHeight(), 0,
		textureKeys.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureKeys.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureKeys.freeImage(); // Liberamos memoria

	// Definiendo la textura
	/*Texture textureParticlesFountain("../Textures/bluewater.png");
	textureParticlesFountain.loadImage(); // Cargar la textura
	glGenTextures(1, &textureParticleFountainID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureParticleFountainID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureParticlesFountain.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureParticlesFountain.getChannels() == 3 ? GL_RGB : GL_RGBA, textureParticlesFountain.getWidth(), textureParticlesFountain.getHeight(), 0,
		textureParticlesFountain.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureParticlesFountain.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureParticlesFountain.freeImage(); // Liberamos memoria
*/

	/*******************************************
	 * OpenAL init
	 *******************************************/
	alutInit(0, nullptr);
	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);
	alGetError(); // clear any error messages
	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating buffers !!\n");
		exit(1);
	}
	else {
		printf("init() - No errors yet.");
	}
	// Generate buffers, or else no sound will happen!
	alGenBuffers(NUM_BUFFERS, buffer);
	buffer[0] = alutCreateBufferFromFile("../sounds/LeaveBeforeDark.wav");
	buffer[1] = alutCreateBufferFromFile("../sounds/screams.wav");
	buffer[2] = alutCreateBufferFromFile("../sounds/scary_cry.wav");
	int errorAlut = alutGetError();
	if (errorAlut != ALUT_ERROR_NO_ERROR){
		printf("- Error open files with alut %d !!\n", errorAlut);
		exit(2);
	}

	alGetError(); /* clear error */
	alGenSources(NUM_SOURCES, source);

	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating sources !!\n");
		exit(2);
	}
	else {
		printf("init - no errors after alGenSources\n");
	}
	alSourcef(source[0], AL_PITCH, 1.0f);
	alSourcef(source[0], AL_GAIN, 20.0f);
	alSourcefv(source[0], AL_POSITION, source0Pos);
	alSourcefv(source[0], AL_VELOCITY, source0Vel);
	alSourcei(source[0], AL_BUFFER, buffer[0]);
	alSourcei(source[0], AL_LOOPING, AL_TRUE);
	alSourcef(source[0], AL_MAX_DISTANCE, 100000);

	alSourcef(source[1], AL_PITCH, 1.0f);
	alSourcef(source[1], AL_GAIN, 5.0f);
	alSourcefv(source[1], AL_POSITION, source1Pos);
	alSourcefv(source[1], AL_VELOCITY, source1Vel);
	alSourcei(source[1], AL_BUFFER, buffer[1]);
	alSourcei(source[1], AL_LOOPING, AL_TRUE);
	alSourcef(source[1], AL_MAX_DISTANCE, 1000);

	alSourcef(source[2], AL_PITCH, 1.0f);
	alSourcef(source[2], AL_GAIN, 4.0f);
	alSourcefv(source[2], AL_POSITION, source2Pos);
	alSourcefv(source[2], AL_VELOCITY, source2Vel);
	alSourcei(source[2], AL_BUFFER, buffer[2]);
	alSourcei(source[2], AL_LOOPING, AL_TRUE);
	alSourcef(source[2], AL_MAX_DISTANCE, 1000);

	/*******************************************
	 * Inicializacion del framebuffer para
	 * almacenar el buffer de profunidadad
	 *******************************************/
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
				 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

	// Basic objects Delete
	skyboxSphere.destroy();
	boxCesped.destroy();
	boxHighway.destroy();
	boxLandingPad.destroy();
	boxCollider.destroy();
	sphereCollider.destroy();
	boxIntro.destroy();
	boxViewDepth.destroy();

	// Custom objects Delete
	modelRock.destroy();
	modelArc.destroy();
	modelTower.destroy();
	modelBasePuente.destroy();
	modelBordePuente.destroy();
	modelPicasPuente.destroy();
	modelIsle.destroy();
	modelLamp1.destroy();
	modelLamp2.destroy();
	modelLampPost2.destroy();
	heroeModelAnimate.destroy();
	modelFaro.destroy();
	modelCerbero.destroy();
	modelCube.destroy();

	// Terrains objects Delete
	terrain.destroy();

	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureCespedID);
	glDeleteTextures(1, &textureArcID);
	glDeleteTextures(1, &textureIsleID);
	glDeleteTextures(1, &textureWindowID);
	glDeleteTextures(1, &textureHighwayID);
	glDeleteTextures(1, &textureLandingPadID);
	glDeleteTextures(1, &textureTerrainBID);
	glDeleteTextures(1, &textureTerrainGID);
	glDeleteTextures(1, &textureTerrainRID);
	glDeleteTextures(1, &textureTerrainBlendMapID);
	glDeleteTextures(1, &textureStartID);
	glDeleteTextures(1, &textureResumeID);
	glDeleteTextures(1, &textureScreen2ID);
	glDeleteTextures(1, &textureMuerteID);
	glDeleteTextures(1, &textureMenuID);
	glDeleteTextures(1, &textureControlID); 
	glDeleteTextures(1, &textureKeysID);


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
		case GLFW_KEY_TAB:
				bool uno =processInput(false);
				if(uno){
					processInput(true);
				}
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
	
    if (!iniciaPartida) {
        const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

        bool presionarEnter = (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) || 
                              (glfwJoystickPresent(GLFW_JOYSTICK_1) && buttons && buttons[CIRCLE_BUTTON] == GLFW_PRESS);
                              (glfwJoystickPresent(GLFW_JOYSTICK_1) && buttons && buttons[CIRCLE_BUTTON] == GLFW_PRESS);
        int p3 = glfwGetKey(window, GLFW_KEY_ENTER);
        if (textureActivaID == textureScreen2ID || textureActivaID == textureMuerteID) {
            ctrlRelease = false;
        } else {
            ctrlRelease = true;
        }
        if (textureActivaID == textureStartID && presionarEnter && muerteRelease && controlRelease && keyrelease && menuRelease) {
            iniciaPartida = true;
            pause = false;
            pauseInicio = false;
            muerte = false;
			warco = 0;     
			warcoy = 10.60f;
			textureActivaID = textureScreen2ID;  
			if (starmur == 0){
				vida = 0;
				textureActivaID = textureMuerteID;  
			}else{ 
				vida = 3;
			}
        } else if (textureActivaID == textureResumeID && presionarEnter) {
            iniciaPartida = true;
            pause = false;
            pauseInicio = false;
            muerte = false;
            textureActivaID = textureScreen2ID;
        }
        else if (textureActivaID == textureControlID && presionarEnter) {
            controlRelease = false;
            textureActivaID = textureKeysID;        
        }
        else if (textureActivaID == textureKeysID && presionarEnter && controlRelease) {
            keyrelease = false;
            textureActivaID = textureStartID;        
        }
        else if (!presionarOpcion && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || (glfwJoystickPresent(GLFW_JOYSTICK_1) && buttons && buttons[CROSS_BUTTON] == GLFW_PRESS)) {
            presionarOpcion = true;
        const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
            glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);
            (glfwJoystickPresent(GLFW_JOYSTICK_1) && buttons && buttons[CROSS_BUTTON]);
            if (textureActivaID == textureStartID && !muerte && ctrlRelease) {
                textureActivaID = textureControlID;
            }
            else if (textureActivaID == textureControlID && ctrlRelease) {
                textureActivaID = textureStartID;
            }
            else if (textureActivaID == textureMenuID && !pauseInicio && ctrlRelease) {
                textureActivaID = textureResumeID;            
            } else if (textureActivaID == textureResumeID && ctrlRelease) {
                textureActivaID = textureMenuID;
            }
            else if (textureActivaID == textureMuerteID && muerte ) {
                textureActivaID = textureMuerteID;
            }
            else if (textureActivaID == textureKeysID && ctrlRelease) {
                textureActivaID = textureKeysID;
            }
        }
        else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE || (glfwJoystickPresent(GLFW_JOYSTICK_1) && buttons && buttons[CROSS_BUTTON] == GLFW_RELEASE)) {
            presionarOpcion = false;
        }
        if (textureActivaID == textureMenuID && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS || (textureActivaID == textureMenuID && (glfwJoystickPresent(GLFW_JOYSTICK_1) && buttons && buttons[CIRCLE_BUTTON] == GLFW_PRESS))) {
            textureActivaID = textureStartID;
            menuRelease = false;
            iniciaPartida = false;
            pauseInicio = true;
        }
		if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE && (glfwJoystickPresent(GLFW_JOYSTICK_1) && buttons && buttons[CIRCLE_BUTTON] == GLFW_RELEASE)){
			iniciorel = true;
		}
    } else if (muerte == true && iniciaPartida == true && iniciorel) {
		const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
        if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS || (glfwJoystickPresent(GLFW_JOYSTICK_1) && buttons && buttons[CIRCLE_BUTTON] == GLFW_PRESS)){
			if(starmur == 0){
				starmur = 1;
			}
				modelMatrixHeroe = glm::translate(modelMatrixHeroe, glm::vec3(45.0f, 00.05f, 40.0f));
				modelMatrixHeroe = glm::rotate(modelMatrixHeroe, glm::radians(180.0f), glm::vec3(0, 1, 0));
				iniciaPartida = false;
				muerteRelease = false;
				textureActivaID = textureMenuID;
				accionMuerte();
			}
		else{
            textureActivaID = textureMuerteID;
			modelMatrixHeroe = glm::translate(modelMatrixHeroe, glm::vec3(45.0f, 00.05f, 40.0f));
			modelMatrixHeroe = glm::rotate(modelMatrixHeroe, glm::radians(180.0f), glm::vec3(0, 1, 0));
		}
    }
    // Pantalla STOP
    if (enableCountSelected && (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS || 
        (glfwJoystickPresent(GLFW_JOYSTICK_1) && glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount)[TRIANGLE_BUTTON] == GLFW_PRESS))) {
        if (!pause && !pauseInicio) {
            if (!muerte) {
                pause = true;
                textureActivaID = textureResumeID;
                iniciaPartida = false;
            }
        } else if (muerte || pause || pauseInicio) {
            textureActivaID = textureActivaID;
			std::cout << textureActivaID <<"1111110\n";
        }
    }

    if (enableCountSelected && glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS) {
        if (iniciaPartida) {
            if(vida == 0){
				muerte = true;
				textureActivaID = textureMuerteID;
			}else{
				chec = true;
			}
			checkRelease = false;
        }
    }
    if (enableCountSelected && glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_RELEASE) {
        checkRelease = true;
    }
	const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE && (glfwJoystickPresent(GLFW_JOYSTICK_1) && buttons && buttons[CIRCLE_BUTTON] == GLFW_RELEASE)) {
        if (!controlRelease) {
            controlRelease = true;
        } else if (!keyrelease) {
            keyrelease = true;
        } else if (!menuRelease) {
            menuRelease = true;
        } else if (!muerteRelease) {
            muerteRelease = true;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE || (glfwJoystickPresent(GLFW_JOYSTICK_1) && buttons && buttons[CROSS_BUTTON] == GLFW_RELEASE)) {
        ctrlRelease = true;
    }

    if (pause == false && iniciaPartida && !muerte) {
        if (glfwJoystickPresent(GLFW_JOYSTICK_1) == GL_TRUE) {
            //std::cout << "Está presente el joystick" << std::endl;
            int axesCount, buttonCount;
            const float * axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
            //std::cout << "Right Stick X axis: " << axes[2] << std::endl;
            //std::cout << "Right Stick Y axis: " << axes[3] << std::endl;
            if (fabs(axes[1]) > 0.2) {
                modelMatrixHeroe = glm::translate(modelMatrixHeroe, glm::vec3(0, 0, -axes[1] * 0.1));
                animationHeroeIndex = 2;
            }
            if (fabs(axes[0]) > 0.2) {
                modelMatrixHeroe = glm::rotate(modelMatrixHeroe, glm::radians(-axes[0] * 0.5f), glm::vec3(0, 1, 0));
                animationHeroeIndex = 2;
            }
			/*
            if (fabs(axes[2]) > 0.2) {
                camera->mouseMoveCamera(-axes[2], 0.0, deltaTime);
            }
            if (fabs(axes[3]) > 0.2) {
                camera->mouseMoveCamera(0.0, -axes[3], deltaTime);
            }
			*/

            const unsigned char * buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
            if (buttons[SQUARE_BUTTON] == GLFW_PRESS)
                std::cout << "Se presiona x" << std::endl;

            if (!isJump && buttons[SQUARE_BUTTON] == GLFW_PRESS) {
                isJump = true;
                startTimeJump = currTime;
                tmv = 0;
            }
        }
		GLFWgamepadstate gamepadState;
		if (isThirdCamera)
		{
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
				camera->mouseMoveCamera(offsetX, offsetY, deltaTime);
		}
		else
		{
			glm::vec3 heroView = glm::vec3(modelMatrixHeroe[3]);
			float yOffset = 5.0f;
			float rotationSpeed = 1.14592f;
			glm::vec3 cameraPosition = heroView + glm::vec3(0.0f, yOffset, -1.0f);
			// Establecer la posición de la cámara
			cameraFP->setPosition(cameraPosition);
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				cameraFP->moveFrontCamera(true, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				cameraFP->moveFrontCamera(false, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				cameraFP->rotateLeftRight(rotationSpeed, true); // Rotación hacia la izquierda
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				cameraFP->rotateLeftRight(rotationSpeed, false); // Rotación hacia la derecha
			if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT)== GLFW_PRESS)
				cameraFP->mouseMoveCamera(offsetX, offsetY, deltaTime);

			if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {
				int axisCount;
				const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axisCount);
				const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

				float leftJoystickHorizontal = axes[LEFT_JOYSTICK_HORIZONTAL_AXIS];
				float leftJoystickVertical = axes[LEFT_JOYSTICK_VERTICAL_AXIS];

				// Movimiento de la cámara hacia adelante y hacia atrás
				if (leftJoystickVertical < -0.1f) {
					cameraFP->moveFrontCamera(true, deltaTime);
				}
				if (leftJoystickVertical > 0.1f) {
					cameraFP->moveFrontCamera(false, deltaTime);
				}
				// Rotación izquierda y derecha con los botones R1 y R2
				if (buttons[R2_BUTTON] == GLFW_PRESS)
					cameraFP->rotateLeftRight(rotationSpeed, false); // Rotación hacia la derecha
				if (buttons[L2_BUTTON] == GLFW_PRESS)
					cameraFP->rotateLeftRight(rotationSpeed, true); // Rotación hacia la izquierda
			}
		}
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		{
			changingCamera = true;
		}
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE)
		{
			if (changingCamera)
			{
				isThirdCamera = !isThirdCamera;
				std::cout << "Changed Camera" << std::endl;
			}
			changingCamera = false;
		}
		if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {
			int buttonCount;
			const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

			if (buttons[JOYSTICK_RIGHT_BUTTON] == GLFW_PRESS) {
				changingCamera = true;
			}
			if (buttons[JOYSTICK_RIGHT_BUTTON] == GLFW_RELEASE) {
				if (changingCamera) {
					isThirdCamera = !isThirdCamera;
					std::cout << "Changed Camera" << std::endl;
				}
				changingCamera = false;
			}
		}
			
		//if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		//	camera->mouseMoveCamera(0, offsetY, deltaTime);
		offsetX = 0;
		offsetY = 0;

        // Guardar key frames
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            record = true;
            if (myfile.is_open())
                myfile.close();
            myfile.open(fileName);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            record = false;
            myfile.close();
            std::cout << "";
        }
        if (availableSave && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
            saveFrame = true;
            availableSave = false;
        }
        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
            availableSave = true;

        // Controles de Heroe
        if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            modelMatrixHeroe = glm::rotate(modelMatrixHeroe, 0.02f, glm::vec3(0, 1, 0));
            animationHeroeIndex = 2;
        } else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            modelMatrixHeroe = glm::rotate(modelMatrixHeroe, -0.02f, glm::vec3(0, 1, 0));
            animationHeroeIndex = 2;
        }
        if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            modelMatrixHeroe = glm::translate(modelMatrixHeroe, glm::vec3(0.0, 0.0, 0.2));
            animationHeroeIndex = 2;
        } else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            modelMatrixHeroe = glm::translate(modelMatrixHeroe, glm::vec3(0.0, 0.0, -0.2));
            animationHeroeIndex = 2;
        }
        bool keySpaceStatus = (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);
        if (!isJump && keySpaceStatus) {
            isJump = true;
            startTimeJump = currTime;
            tmv = 0;
            animationHeroeIndex = 1;
        } else if (isJump) {
            animationHeroeIndex = 1;
        }
    }

    glfwPollEvents();
    return continueApplication;
}


void prepareScene(){
	terrain.setShader(&shaderTerrain);
	modelRock.setShader(&shaderMulLighting);
	modelArc.setShader(&shaderMulLighting);
	modelTower.setShader(&shaderMulLighting);
	modelBasePuente.setShader(&shaderMulLighting);
	modelBordePuente.setShader(&shaderMulLighting);
	modelPicasPuente.setShader(&shaderMulLighting);
	modelIsle.setShader(&shaderMulLighting);
	modelCerbero.setShader(&shaderMulLighting);
	modelCube.setShader(&shaderMulLighting);

	//Lamp models
	modelLamp1.setShader(&shaderMulLighting);
	modelLamp2.setShader(&shaderMulLighting);
	modelLampPost2.setShader(&shaderMulLighting);
	modelFaro.setShader(&shaderMulLighting);
	//Heroe
	heroeModelAnimate.setShader(&shaderMulLighting);

}

void prepareDepthScene(){

	terrain.setShader(&shaderDepth);
	
	modelRock.setShader(&shaderDepth);
	modelArc.setShader(&shaderMulLighting);
	modelTower.setShader(&shaderDepth);
	modelBasePuente.setShader(&shaderDepth);
	modelBordePuente.setShader(&shaderDepth);
	modelPicasPuente.setShader(&shaderDepth);
	modelIsle.setShader(&shaderDepth);
	modelCerbero.setShader(&shaderDepth);
	modelCube.setShader(&shaderDepth);
	//Lamp models
	modelLamp1.setShader(&shaderDepth);
	modelLamp2.setShader(&shaderDepth);
	modelLampPost2.setShader(&shaderDepth);
	modelFaro.setShader(&shaderDepth);
	//Heroe
	heroeModelAnimate.setShader(&shaderDepth);
}

void renderSolidScene(){
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
	terrain.enableWireMode();
	//terrain.render();
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
	glBindTexture(GL_TEXTURE_2D, 0);
	
	
	/*******************************************
	 * Custom objects obj
	 *******************************************/
	//Rock render
	matrixModelRock[3][1] = terrain.getHeightTerrain(matrixModelRock[3][0], matrixModelRock[3][2]);
	matrixModelCube[3][1] = terrain.getHeightTerrain(matrixModelCube[3][0], matrixModelCube[3][2]);

	for(int i = 0; i < ArcoPosition.size(); i++){
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureArcID);
		shaderMulLighting.setInt("texture1", 0);
		matrixModelArc[3][1] = terrain.getHeightTerrain(matrixModelArc[3][0], matrixModelArc[3][2]);
		glm::mat4 modelMatrixArc = glm::mat4(matrixModelArc); 
		modelMatrixArc = glm::translate(modelMatrixArc, ArcoPosition[i]);
		modelArc.render(modelMatrixArc);
	}

	// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
	//matrixModelTower[3][1] = terrain.getHeightTerrain(matrixModelTower[3][0], matrixModelTower[3][2]);
	modelTower.render(matrixModelTower);
	//matrixModelIsle[3][1] = terrain.getHeightTerrain(matrixModelIsle[3][0], matrixModelIsle[3][2]);
	for(int i = 0; i < IslaPosition.size(); i++){
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureIsleID);
		shaderMulLighting.setInt("texture1", 0);
		glm::mat4 modelMatrixIsle = glm::mat4(matrixModelIsle);
		modelMatrixIsle = glm::translate(modelMatrixIsle, IslaPosition[i]);
		modelIsle.render(modelMatrixIsle);
		
	}
		
	glActiveTexture(GL_TEXTURE0);
	matrixModelCerbero[3][1] = terrain.getHeightTerrain(matrixModelCerbero[3][0], matrixModelCerbero[3][2]);
	modelCerbero.render(matrixModelCerbero);
	// Render lamp
	for(int i = 0; i < FaroPosition.size(); i++){
		FaroPosition[i].y = terrain.getHeightTerrain(FaroPosition[i].x, FaroPosition[i].z);
		modelFaro.setPosition(FaroPosition[i]);
		modelFaro.setScale(glm::vec3(1.5));
		modelFaro.setOrientation(glm::vec3(0, FaroOrientation[i], 0));
		modelFaro.render();
	}
	

	//Render Puente
	for(int i = 0; i < PuentePosition.size(); i++){
		glm::mat4 modelMatrixPuente = glm::mat4(matrixModelBasePuente);
		modelMatrixPuente = glm::translate(modelMatrixPuente, PuentePosition[i]);
		modelBasePuente.render(modelMatrixPuente);
		modelBordePuente.render(modelMatrixPuente);
		modelPicasPuente.render(modelMatrixPuente);
	}
	
	/*glm::mat4 modelMatrixPuente2 = glm::mat4(matrixModelBordePuente);
	modelBasePuente.render(modelMatrixPuente2);
	modelBordePuente.render(modelMatrixPuente2);
	modelPicasPuente.render(modelMatrixPuente2);*/
	glEnable(GL_CULL_FACE);

	/*****************************************
		 * Objetos animados por huesos
		 * **************************************/
		glm::vec3 ejey = glm::normalize(terrain.getNormalTerrain(modelMatrixHeroe[3][0], modelMatrixHeroe[3][2]));
		glm::vec3 ejex = glm::vec3(modelMatrixHeroe[0]);
		glm::vec3 ejez = glm::normalize(glm::cross(ejex, ejey));
		ejex = glm::normalize(glm::cross(ejey, ejez));
		modelMatrixHeroe[0] = glm::vec4(ejex, 0.0);
		modelMatrixHeroe[1] = glm::vec4(ejey, 0.0);
		modelMatrixHeroe[2] = glm::vec4(ejez, 0.0);
		modelMatrixHeroe[3][1] = -GRAVITY * tmv * tmv + 3.5 * tmv + terrain.getHeightTerrain(modelMatrixHeroe[3][0], modelMatrixHeroe[3][2]);
		tmv = currTime - startTimeJump;

		if(modelMatrixHeroe[3][1] < terrain.getHeightTerrain(modelMatrixHeroe[3][0], modelMatrixHeroe[3][2])){
			isJump = false;
			modelMatrixHeroe[3][1] = terrain.getHeightTerrain(modelMatrixHeroe[3][0], modelMatrixHeroe[3][2]);
		}
		glm::mat4 modelMatrixHeroeBody = glm::mat4(modelMatrixHeroe);
		modelMatrixHeroeBody = glm::scale(modelMatrixHeroeBody, glm::vec3(0.021f));
		heroeModelAnimate.setAnimationIndex(animationHeroeIndex);
		heroeModelAnimate.render(modelMatrixHeroeBody);
		//animationHeroeIndex = 0;
	

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
}

void renderAlphaScene(bool render = true){
}

void renderScene(){
	renderSolidScene();
	//renderAlphaScene(false);
}


void applicationLoop() {
	bool psi = true;

	glm::vec3 axis;
	glm::vec3 target;
	glm::mat4 view;
	float angleTarget;

	int state = 0;

	matrixModelRock = glm::translate(matrixModelRock, glm::vec3(arco1x, arco1y, arco1z));
	matrixModelCube = glm::translate(matrixModelCube, glm::vec3(35,0,0));

	matrixModelArc = glm::scale(matrixModelArc, glm::vec3(2.0, 2.0, 1.0));
	matrixModelArc = glm::translate(matrixModelArc, glm::vec3(arco1x, arco1y, arco1z));
	matrixModelArc = glm::rotate(matrixModelArc, glm::radians(-90.0f), glm::vec3(0, 0, 1));

	matrixModelTower = glm::scale(matrixModelTower, glm::vec3(9.0, 4.0, 9.0));
	matrixModelTower = glm::translate(matrixModelTower, glm::vec3(5.0, 2.7, -23.0));
	matrixModelTower = glm::rotate(matrixModelTower, glm::radians(-90.0f), glm::vec3(1, 0, 0));

	matrixModelCerbero = glm::translate(matrixModelCerbero, glm::vec3(45.0, 0.0, -210.0));
	matrixModelCerbero = glm::rotate(matrixModelCerbero, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	
	matrixModelBasePuente = glm::scale(matrixModelBasePuente, glm::vec3(6.0, 6.0, 6.0));
	matrixModelBasePuente = glm::translate(matrixModelBasePuente, glm::vec3(8.0, 1.0, 7.0));
	matrixModelBasePuente = glm::rotate(matrixModelBasePuente, glm::radians(-90.0f), glm::vec3(1, 0, 0));

	matrixModelBordePuente = glm::scale(matrixModelBordePuente, glm::vec3(6.0, 6.0, 6.0));
	matrixModelBordePuente = glm::translate(matrixModelBordePuente, glm::vec3(8.0, 2.0, 7.0));
	matrixModelBordePuente = glm::rotate(matrixModelBordePuente, glm::radians(-90.0f), glm::vec3(1, 0, 0));

	matrixModelIsle = glm::scale(matrixModelIsle, glm::vec3(3.0, 3.0, 3.0));
	matrixModelIsle = glm::translate(matrixModelIsle, glm::vec3(15.5, 4.0, -55.0));
	matrixModelIsle = glm::rotate(matrixModelIsle, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	
 	modelMatrixHeroe = glm::translate(modelMatrixHeroe, glm::vec3(45.0f, 00.05f, 40.0f));
 	//modelMatrixHeroe = glm::translate(modelMatrixHeroe, glm::vec3(45.0f, 00.05f, -190.0f));
	modelMatrixHeroe = glm::rotate(modelMatrixHeroe, glm::radians(180.0f), glm::vec3(0, 1, 0));

	lastTime = TimeManager::Instance().GetTime();

	textureActivaID = textureStartID;

	glm::vec3 lightPos = glm::vec3(10.0, 10.0, -10.0);

	shadowBox = new ShadowBox(-lightPos, camera.get(), 15.0f, 0.1f, 45.0f);

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

		// Variables donde se guardan las matrices de cada articulacion por 1 frame
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
				(float) screenWidth / (float) screenHeight, 0.01f, 100.0f);

		if(std::isnan(angleTarget))
			angleTarget = 0.0;
		if(axis.y < 0)
			angleTarget = -angleTarget;
		if(modelSelected == 1)
			angleTarget -= glm::radians(90.0f);

		//configurando el viewMatrix:
		if (isThirdCamera)
		{
			axis = glm::axis(glm::quat_cast(modelMatrixHeroe));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixHeroe));
			target = modelMatrixHeroe[3];
			camera->setAngleTarget(angleTarget);
			camera->setCameraTarget(target);
			camera->updateCamera();
			camera->setDistanceFromTarget(distanceFromTarget);
			view = camera->getViewMatrix();
		}
		else
		{
			view = cameraFP->getViewMatrix();
		}

		shadowBox->update(screenWidth, screenHeight);
		glm::vec3 centerBox = shadowBox->getCenter();

		// Projection light shadow mapping
		glm::mat4 lightProjection = glm::mat4(1.0f), lightView = glm::mat4(1.0f);
		glm::mat4 lightSpaceMatrix;
		lightProjection[0][0] = 2.0f / shadowBox->getWidth();
		lightProjection[1][1] = 2.0f / shadowBox->getHeight();
		lightProjection[2][2] = -2.0f / shadowBox->getLength();
		lightProjection[3][3] = 1.0f;
		lightView = glm::lookAt(centerBox, centerBox + glm::normalize(-lightPos), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		shaderDepth.setMatrix4("lightSpaceMatrix", 1, false, glm::value_ptr(lightSpaceMatrix));


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
		shaderMulLighting.setMatrix4("lightSpaceMatrix", 1, false,
				glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderTerrain.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderTerrain.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		shaderTerrain.setMatrix4("lightSpaceMatrix", 1, false,
				glm::value_ptr(lightSpaceMatrix));

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.7, 0.7, 0.9)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.8, 0.8, 0.9)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.7)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.7, 0.7, 0.9)));
		shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.8, 0.8, 0.9)));
		shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.7)));
		shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		/*******************************************
		 * Propiedades SpotLights
		 *******************************************/
		
		shaderMulLighting.setInt("spotLightCount", 1);
		shaderTerrain.setInt("spotLightCount", 1);

		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));

		shaderMulLighting.setFloat("spotLights[0].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[0].linear", 0.07);
		shaderMulLighting.setFloat("spotLights[0].quadratic", 0.03);
		shaderMulLighting.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5f)));
		shaderMulLighting.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0f)));
		
shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
		shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));
		shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));

		shaderTerrain.setFloat("spotLights[0].constant", 1.0);
		shaderTerrain.setFloat("spotLights[0].linear", 0.07);
		shaderTerrain.setFloat("spotLights[0].quadratic", 0.03);
		shaderTerrain.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5f)));
		shaderTerrain.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0f)));

		/*******************************************
		 * Propiedades PointLights
		 *******************************************/
		shaderMulLighting.setInt("pointLightCount", FaroPosition.size() + lamp2Position.size());
		shaderTerrain.setInt("pointLightCount", FaroPosition.size() + lamp2Position.size());
		for(int i = 0; i < FaroPosition.size(); i++){
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, FaroPosition[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(FaroOrientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(0.2));
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0.0, 8.35, 0));
			glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.2, 0.7)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 1.0)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.5, 0.5, 1.0)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.02);
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.2, 0.7)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 1.0)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.5, 0.5, 1.0)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.02);
		}
		for(int i = 0; i < lamp2Position.size(); i++){
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, lamp2Position[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lamp2Orientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(1.0));
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0.75, 5.0, 0));
			glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(FaroPosition.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(FaroPosition.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(FaroPosition.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(FaroPosition.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderMulLighting.setFloat("pointLights[" + std::to_string(FaroPosition.size() + i) + "].constant", 1.0);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(FaroPosition.size() + i) + "].linear", 0.09);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(FaroPosition.size() + i) + "].quadratic", 0.02);
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(FaroPosition.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(FaroPosition.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(FaroPosition.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(FaroPosition.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderTerrain.setFloat("pointLights[" + std::to_string(FaroPosition.size() + i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(FaroPosition.size() + i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(FaroPosition.size() + i) + "].quadratic", 0.02);
		}

		/************Render de imagen de frente**************/
		if(!iniciaPartida || resumeAct){
			resumeAct = false;
			shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
			shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureActivaID);
			shaderTexture.setInt("outTexture", 0);
			glEnable(GL_BLEND);
			boxIntro.render();
			glDisable(GL_BLEND);
			if(textureActivaID==textureMenuID && pause){
				/************Render de imagen de frente Texto Menú**************/
				modelTextInicioPartida->modFuente(65, 0.10f, 0.10f, 0.150f, 1.0f);
				modelTextInicioPartida->render("MENU",-0.950,-0.15);
			}else if(textureActivaID==textureStartID){
				/************Render de imagen de frente Texto Inicio**************/
				modelTextInicioPartida->modFuente(130, 0.910f, 0.910520f, 0.9261580f, 1.0f);
				modelTextInicioPartida->render("START",-0.48,-0.550);
			}else if(textureActivaID == textureControlID){
				/************Render de imagen de frente Texto Controles**************/
				modelTextInicioPartida->modFuente(100, 0.01714280f, 0.03402590f, 0.10f, 1.0f);
				modelTextInicioPartida->render("CONTROLS",-0.6,-0.80);
			}
			if(textureActivaID==textureResumeID){
				/************Render de imagen de frente Texto Resume**************/
				modelTextInicioPartida->modFuente(130, 0.910f, 0.910520f, 0.9261580f, 0.60f);
				modelTextInicioPartida->render("RESUME",-0.58,-0.550);
			}
			glfwSwapBuffers(window);
			continue;
		}

		/**
		 * CHECK POINTS LOGIC
		*/
		if(vida > 0 && chec && checkRelease){
			if(warco == 0 ){
				modelMatrixHeroe = glm::mat4(1);
				modelMatrixHeroe = glm::translate(modelMatrixHeroe, glm::vec3(45.0f, 00.05f, 40.0f));
				modelMatrixHeroe = glm::rotate(modelMatrixHeroe, glm::radians(180.0f), glm::vec3(0, 1, 0));
				modelMatrixHeroe = glm::translate(modelMatrixHeroe, glm::vec3(0, 0, 0));
				vida = vida-1;
				chec = false;
			}
			else if(warco == 1){
				modelMatrixHeroe = glm::mat4(1);
				modelMatrixHeroe = glm::translate(modelMatrixHeroe, glm::vec3(45.0f, 00.05f, -150.0f));
				modelMatrixHeroe = glm::rotate(modelMatrixHeroe, glm::radians(180.0f), glm::vec3(0, 1, 0));
				modelMatrixHeroe = glm::translate(modelMatrixHeroe, glm::vec3(0, 0, 0));
				vida = vida-1;
				chec = false;
			}
		}else if(vida == 0 && muerte && checkRelease){
			modelMatrixHeroe = glm::mat4(1);
 			modelMatrixHeroe = glm::translate(modelMatrixHeroe, glm::vec3(45.0f, 00.05f, 40.0f));
			modelMatrixHeroe = glm::rotate(modelMatrixHeroe, glm::radians(180.0f), glm::vec3(0, 1, 0));
			modelMatrixHeroe = glm::translate(modelMatrixHeroe, glm::vec3(0, 0, 0));
		}

		
		/*******************************************
		 * 1.- We render the depth buffer
		 *******************************************/
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render scene from light's point of view
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_FRONT);
		prepareDepthScene();
		renderScene();
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

/*************************/


		/*******************************************
		 * 2.- We render the normal objects
		 *******************************************/
		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		prepareScene();
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		shaderMulLighting.setInt("shadowMap", 10);
		shaderTerrain.setInt("shadowMap", 10);
		renderSolidScene();


		/*******************************************
		 * Skybox
		 *******************************************/
		/*GLint oldCullFaceMode;
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
*/
		/*******************************************
		 * Creacion de colliders
		 * IMPORTANT do this before interpolations
		 *******************************************/

		/************Render de imagen de frente Texto Muerte**************/
		if (starmur == 0){
			textureActivaID == textureStartID;
		}
		shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
		shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureActivaID);
		shaderTexture.setInt("outTexture", 0);
		glEnable(GL_BLEND);
		boxIntro.render();
		glDisable(GL_BLEND);
		if(textureActivaID == textureMuerteID){
			std::cout << textureActivaID;
			modelTextMuerte->modFuente(120, 0.850f, 0.850f, 0.850f, 1.0f);
			modelTextMuerte->render("GAME OVER",-0.78,0.0);
		}
		
		//Colliders puentes 
		for(int i = 0; i < PuentePosition.size(); i++){
			AbstractModel::OBB puenteCollider;
			glm::mat4 modelMatrixColliderPuente = glm::mat4(1.0);
			modelMatrixColliderPuente = glm::translate(matrixModelBasePuente, PuentePosition[i]+glm::vec3(0.0,0.70,01.0));
			modelMatrixColliderPuente = glm::rotate(modelMatrixColliderPuente, 
				glm::radians(90.0f), glm::vec3(1, 0, 0));
			addOrUpdateColliders(collidersOBB, "puenteder-" + std::to_string(i), puenteCollider,
				modelMatrixColliderPuente);
			puenteCollider.u = glm::quat_cast(modelMatrixColliderPuente);
			modelMatrixColliderPuente = glm::scale(modelMatrixColliderPuente, glm::vec3(0,05.5,9.10));
			modelMatrixColliderPuente = glm::translate(modelMatrixColliderPuente, modelBasePuente.getObb().c);
			puenteCollider.c = modelMatrixColliderPuente[3];
			puenteCollider.e = modelBasePuente.getObb().e*glm::vec3(0.0f,05.5f,9.10f);
			//puenteCollider.e = modelBasePuente.getObb().e * glm::vec3(0.8f,0.50f,01.10f);
			std::get<0>(collidersOBB.find("puenteder-" + std::to_string(i))->second) = puenteCollider;
		}

		//Colliders puentes izq
		for(int i = 0; i < PuentePosition.size(); i++){
			AbstractModel::OBB puenteCollider2;
			glm::mat4 modelMatrixColliderPuente2 = glm::mat4(1.0);
			modelMatrixColliderPuente2 = glm::translate(matrixModelBasePuente, PuentePosition[i]+glm::vec3(-0.90,0.7,1.0));
			modelMatrixColliderPuente2 = glm::rotate(modelMatrixColliderPuente2, 
				glm::radians(90.0f), glm::vec3(1, 0, 0));
			addOrUpdateColliders(collidersOBB, "picas-" + std::to_string(i), puenteCollider2,
				modelMatrixColliderPuente2);
			puenteCollider2.u = glm::quat_cast(modelMatrixColliderPuente2);
			modelMatrixColliderPuente2 = glm::scale(modelMatrixColliderPuente2, glm::vec3(0,05.5,9.10));
			modelMatrixColliderPuente2 = glm::translate(modelMatrixColliderPuente2, modelBasePuente.getObb().c);
			puenteCollider2.c = modelMatrixColliderPuente2[3];
			puenteCollider2.e = modelBasePuente.getObb().e*glm::vec3(0.0f,05.5f,9.10f);
			//puenteCollider2.e = modelBasePuente.getObb().e * glm::vec3(0.8f,0.50f,01.10f);
			std::get<0>(collidersOBB.find("picas-" + std::to_string(i))->second) = puenteCollider2;
		}

		AbstractModel::OBB cube;
		glm::mat4 modelMatrixColliderCube = glm::mat4(matrixModelCube);
			modelMatrixColliderCube = glm::translate(matrixModelCube, glm::vec3(012.0,5.90,2.8));
			//modelMatrixColliderCube = glm::rotate(modelMatrixColliderCube, glm::radians(90.0f), glm::vec3(1, 0, 0));
			addOrUpdateColliders(collidersOBB, "cubo-1", cube,modelMatrixColliderCube);
			cube.u = glm::quat_cast(modelMatrixColliderCube);
			modelMatrixColliderCube = glm::scale(modelMatrixColliderCube, glm::vec3(20,2,8.8));
			modelMatrixColliderCube = glm::translate(modelMatrixColliderCube, modelBasePuente.getObb().c);
			cube.c = modelMatrixColliderCube[3];
			cube.e = modelBasePuente.getObb().e*glm::vec3(20,2,8.8);
			//cube.e = modelBasePuente.getObb().e * glm::vec3(0.8f,0.50f,01.10f);

		AbstractModel::OBB cube2;
		glm::mat4 modelMatrixColliderCube2 = glm::mat4(matrixModelCube);
			modelMatrixColliderCube2 = glm::translate(matrixModelCube, glm::vec3(012.0,5.90,-20.5));
			//modelMatrixColliderCube2 = glm::rotate(modelMatrixColliderCube2, glm::radians(90.0f), glm::vec3(1, 0, 0));
			addOrUpdateColliders(collidersOBB, "cubo-2", cube2,modelMatrixColliderCube2);
			cube2.u = glm::quat_cast(modelMatrixColliderCube2);
			modelMatrixColliderCube2 = glm::scale(modelMatrixColliderCube2, glm::vec3(20,2,8.8));
			modelMatrixColliderCube2 = glm::translate(modelMatrixColliderCube2, modelBasePuente.getObb().c);
			cube2.c = modelMatrixColliderCube2[3];
			cube2.e = modelBasePuente.getObb().e*glm::vec3(20,2,8.8);
			//cube2.e = modelBasePuente.getObb().e * glm::vec3(0.8f,0.50f,01.10f);

		AbstractModel::OBB cube3;
		glm::mat4 modelMatrixColliderCube3 = glm::mat4(matrixModelCube);
			modelMatrixColliderCube3 = glm::translate(matrixModelCube, glm::vec3(012.0,5.90,-55.6));
			//modelMatrixColliderCube3 = glm::rotate(modelMatrixColliderCube3, glm::radians(90.0f), glm::vec3(1, 0, 0));
			addOrUpdateColliders(collidersOBB, "cubo-3", cube3,modelMatrixColliderCube3);
			cube3.u = glm::quat_cast(modelMatrixColliderCube3);
			modelMatrixColliderCube3 = glm::scale(modelMatrixColliderCube3, glm::vec3(20,2,8.7));
			modelMatrixColliderCube3 = glm::translate(modelMatrixColliderCube3, modelBasePuente.getObb().c);
			cube3.c = modelMatrixColliderCube3[3];
			cube3.e = modelBasePuente.getObb().e*glm::vec3(20,2,8.7);
			//cube3.e = modelBasePuente.getObb().e * glm::vec3(0.8f,0.50f,01.10f);

		AbstractModel::OBB cube4;
		glm::mat4 modelMatrixColliderCube4 = glm::mat4(matrixModelCube);
			modelMatrixColliderCube4 = glm::translate(matrixModelCube, glm::vec3(012.0,7.0,-102.280));
			modelMatrixColliderCube4 = glm::rotate(modelMatrixColliderCube4, glm::radians(27.0f), glm::vec3(1, 0, 0));
			//modelMatrixColliderCube4 = glm::translate(matrixModelCube, glm::vec3(012.0,5.90,-103.0));
			addOrUpdateColliders(collidersOBB, "cubo-4", cube4,modelMatrixColliderCube4);
			cube4.u = glm::quat_cast(modelMatrixColliderCube4);
			modelMatrixColliderCube4 = glm::scale(modelMatrixColliderCube4, glm::vec3(20,2,6.0));
			modelMatrixColliderCube4 = glm::translate(modelMatrixColliderCube4, modelBasePuente.getObb().c);
			cube4.c = modelMatrixColliderCube4[3];
			cube4.e = modelBasePuente.getObb().e*glm::vec3(20,2,6.0);
			//cube4.e = modelBasePuente.getObb().e * glm::vec3(0.8f,0.50f,01.10f);

		AbstractModel::OBB cubo5;
		glm::mat4 modelMatrixColliderCube5 = glm::mat4(matrixModelCube);
			modelMatrixColliderCube5 = glm::translate(matrixModelCube, glm::vec3(012.0,10.60,-106.40));
			//modelMatrixColliderCube5 = glm::rotate(modelMatrixColliderCube5, glm::radians(27.0f), glm::vec3(1, 0, 0));
			//modelMatrixColliderCube5 = glm::translate(matrixModelCube, glm::vec3(012.0,5.90,-103.0));
			addOrUpdateColliders(collidersOBB, "cubo-5", cubo5,modelMatrixColliderCube5);
			cubo5.u = glm::quat_cast(modelMatrixColliderCube5);
			modelMatrixColliderCube5 = glm::scale(modelMatrixColliderCube5, glm::vec3(20,0.05,01.70));
			modelMatrixColliderCube5 = glm::translate(modelMatrixColliderCube5, modelBasePuente.getObb().c);
			cubo5.c = modelMatrixColliderCube5[3];
			cubo5.e = modelBasePuente.getObb().e*glm::vec3(20,0.05,01.70);
			//cubo5.e = modelBasePuente.getObb().e * glm::vec3(0.8f,0.50f,01.10f);

		AbstractModel::OBB cubo6;
		glm::mat4 modelMatrixColliderCube6 = glm::mat4(matrixModelCube);
			modelMatrixColliderCube6 = glm::translate(matrixModelCube, glm::vec3(012.0,10.60,-165.0));
			//modelMatrixColliderCube6 = glm::rotate(modelMatrixColliderCube6, glm::radians(27.0f), glm::vec3(1, 0, 0));
			//modelMatrixColliderCube6 = glm::translate(matrixModelCube, glm::vec3(012.0,5.90,-103.0));
			addOrUpdateColliders(collidersOBB, "cubo-6", cubo6,modelMatrixColliderCube6);
			cubo6.u = glm::quat_cast(modelMatrixColliderCube6);
			modelMatrixColliderCube6 = glm::scale(modelMatrixColliderCube6, glm::vec3(26,0.05,013.300));
			modelMatrixColliderCube6 = glm::translate(modelMatrixColliderCube6, modelBasePuente.getObb().c);
			cubo6.c = modelMatrixColliderCube6[3];
			cubo6.e = modelBasePuente.getObb().e*glm::vec3(26,0.05,013.300);
			//cubo6.e = modelBasePuente.getObb().e * glm::vec3(0.8f,0.50f,01.10f);

		AbstractModel::OBB cubo7;
		glm::mat4 modelMatrixColliderCube7 = glm::mat4(matrixModelCube);
			modelMatrixColliderCube7 = glm::translate(matrixModelCube, glm::vec3(012.0,13.600,-172.70));
			modelMatrixColliderCube7 = glm::rotate(modelMatrixColliderCube7, glm::radians(35.0f), glm::vec3(1, 0, 0));
			//modelMatrixColliderCube7 = glm::translate(matrixModelCube, glm::vec3(012.0,5.90,-103.0));
			addOrUpdateColliders(collidersOBB, "cubo-7", cubo7,modelMatrixColliderCube7);
			cubo7.u = glm::quat_cast(modelMatrixColliderCube7);
			modelMatrixColliderCube7 = glm::scale(modelMatrixColliderCube7, glm::vec3(20,0.05,02.40));
			modelMatrixColliderCube7 = glm::translate(modelMatrixColliderCube7, modelBasePuente.getObb().c);
			cubo7.c = modelMatrixColliderCube7[3];
			cubo7.e = modelBasePuente.getObb().e*glm::vec3(20,0.05,02.40);
			//cubo7.e = modelBasePuente.getObb().e * glm::vec3(0.8f,0.50f,01.10f);

		AbstractModel::OBB cubo8;
		glm::mat4 modelMatrixColliderCube8 = glm::mat4(matrixModelCube);
			modelMatrixColliderCube8 = glm::translate(matrixModelCube, glm::vec3(021.0,15.800,-180.70));
			modelMatrixColliderCube8 = glm::rotate(modelMatrixColliderCube8, glm::radians(25.0f), glm::vec3(0, 0, 1));
			//modelMatrixColliderCube8 = glm::translate(matrixModelCube, glm::vec3(012.0,5.90,-103.0));
			addOrUpdateColliders(collidersOBB, "cubo-8", cubo8,modelMatrixColliderCube8);
			cubo8.u = glm::quat_cast(modelMatrixColliderCube8);
			modelMatrixColliderCube8 = glm::scale(modelMatrixColliderCube8, glm::vec3(07.0,0.05,06.40));
			modelMatrixColliderCube8 = glm::translate(modelMatrixColliderCube8, modelBasePuente.getObb().c);
			cubo8.c = modelMatrixColliderCube8[3];
			cubo8.e = modelBasePuente.getObb().e*glm::vec3(7,0.05,06.40);
			//cubo8.e = modelBasePuente.getObb().e * glm::vec3(0.8f,0.50f,01.10f);

		AbstractModel::OBB cubo9;
		glm::mat4 modelMatrixColliderCube9 = glm::mat4(matrixModelCube);
			modelMatrixColliderCube9 = glm::translate(matrixModelCube, glm::vec3(035.40,19.700,-194.20));
			modelMatrixColliderCube9 = glm::rotate(modelMatrixColliderCube9, glm::radians(20.0f), glm::vec3(1, 0, 0));
			modelMatrixColliderCube9 = glm::rotate(modelMatrixColliderCube9, glm::radians(-15.0f), glm::vec3(0, 1, 0));
			//modelMatrixColliderCube9 = glm::rotate(modelMatrixColliderCube9, glm::radians(-10.0f), glm::vec3(0, 1, 0));
			//modelMatrixColliderCube9 = glm::translate(matrixModelCube, glm::vec3(012.0,5.90,-103.0));
			addOrUpdateColliders(collidersOBB, "cubo-9", cubo9,modelMatrixColliderCube9);
			cubo9.u = glm::quat_cast(modelMatrixColliderCube9);
			modelMatrixColliderCube9 = glm::scale(modelMatrixColliderCube9, glm::vec3(012.0,0.05,03.2070));
			modelMatrixColliderCube9 = glm::translate(modelMatrixColliderCube9, modelBasePuente.getObb().c);
			cubo9.c = modelMatrixColliderCube9[3];
			cubo9.e = modelBasePuente.getObb().e*glm::vec3(12,0.05,03.2070);
			//cubo9.e = modelBasePuente.getObb().e * glm::vec3(0.8f,0.50f,01.10f);

		AbstractModel::OBB cubo10;
		glm::mat4 modelMatrixColliderCube10 = glm::mat4(matrixModelCube);
			modelMatrixColliderCube10 = glm::translate(matrixModelCube, glm::vec3(035.0,23.100,-209.80));
			modelMatrixColliderCube10 = glm::rotate(modelMatrixColliderCube10, glm::radians(20.0f), glm::vec3(1, 0, 0));
			modelMatrixColliderCube10 = glm::rotate(modelMatrixColliderCube10, glm::radians(05.0f), glm::vec3(0, 1, 0));
			//modelMatrixColliderCube10 = glm::translate(matrixModelCube, glm::vec3(012.0,5.90,-103.0));
			addOrUpdateColliders(collidersOBB, "cubo-10", cubo10,modelMatrixColliderCube10);
			cubo10.u = glm::quat_cast(modelMatrixColliderCube10);
			modelMatrixColliderCube10 = glm::scale(modelMatrixColliderCube10, glm::vec3(017.0,0.05,05.0));
			modelMatrixColliderCube10 = glm::translate(modelMatrixColliderCube10, modelBasePuente.getObb().c);
			cubo10.c = modelMatrixColliderCube10[3];
			cubo10.e = modelBasePuente.getObb().e*glm::vec3(17.0,0.05,05.0);
			//cubo10.e = modelBasePuente.getObb().e * glm::vec3(0.8f,0.50f,01.10f);

		AbstractModel::OBB cubo11;
		glm::mat4 modelMatrixColliderCube11 = glm::mat4(matrixModelCube);
			modelMatrixColliderCube11 = glm::translate(matrixModelCube, glm::vec3(026.0,26.200,-227.70));
			modelMatrixColliderCube11 = glm::rotate(modelMatrixColliderCube11, glm::radians(50.0f), glm::vec3(0, 1, 0));
			modelMatrixColliderCube11 = glm::rotate(modelMatrixColliderCube11, glm::radians(13.0f), glm::vec3(01, 0, 0));
			//modelMatrixColliderCube11 = glm::rotate(modelMatrixColliderCube11, glm::radians(40.0f), glm::vec3(0, 0, 01));
			//modelMatrixColliderCube11 = glm::translate(matrixModelCube, glm::vec3(012.0,5.90,-103.0));
			addOrUpdateColliders(collidersOBB, "cubo-11", cubo11,modelMatrixColliderCube11);
			cubo11.u = glm::quat_cast(modelMatrixColliderCube11);
			modelMatrixColliderCube11 = glm::scale(modelMatrixColliderCube11, glm::vec3(8.0,0.05,04.50));
			modelMatrixColliderCube11 = glm::translate(modelMatrixColliderCube11, modelCube.getObb().c);
			cubo11.c = modelMatrixColliderCube11[3];
			cubo11.e = modelCube.getObb().e*glm::vec3(8,0.05,04.50);
			//cubo11.e = modelCube.getObb().e * glm::vec3(0.8f,0.50f,01.10f);

		AbstractModel::OBB cubo12;
		glm::mat4 modelMatrixColliderCube12 = glm::mat4(matrixModelCube);
			modelMatrixColliderCube12 = glm::translate(matrixModelCube, glm::vec3(04.0,28.2500,-230.70));
			modelMatrixColliderCube12 = glm::rotate(modelMatrixColliderCube12, glm::radians(95.0f), glm::vec3(0, 1, 0));
			modelMatrixColliderCube12 = glm::rotate(modelMatrixColliderCube12, glm::radians(22.0f), glm::vec3(1, 0, 0));
			//modelMatrixColliderCube12 = glm::translate(matrixModelCube, glm::vec3(012.0,5.90,-103.0));
			addOrUpdateColliders(collidersOBB, "cubo-12", cubo12,modelMatrixColliderCube12);
			cubo12.u = glm::quat_cast(modelMatrixColliderCube12);
			modelMatrixColliderCube12 = glm::scale(modelMatrixColliderCube12, glm::vec3(07.0,0.5,04.10));
			modelMatrixColliderCube12 = glm::translate(modelMatrixColliderCube12, modelCube.getObb().c);
			cubo12.c = modelMatrixColliderCube12[3];
			cubo12.e = modelCube.getObb().e*glm::vec3(7,0.5,04.10);
			//cubo12.e = modelCube.getObb().e * glm::vec3(0.8f,0.50f,01.10f);

		AbstractModel::OBB cubo13;
		glm::mat4 modelMatrixColliderCube13 = glm::mat4(matrixModelCube);
			modelMatrixColliderCube13 = glm::translate(matrixModelCube, glm::vec3(-13.0,32.800,-219.70));
			modelMatrixColliderCube13 = glm::rotate(modelMatrixColliderCube13, glm::radians(-25.0f), glm::vec3(1, 0, 0));
			modelMatrixColliderCube13 = glm::rotate(modelMatrixColliderCube13, glm::radians(-5.0f), glm::vec3(0, 1, 0));
			//modelMatrixColliderCube13 = glm::translate(matrixModelCube, glm::vec3(012.0,5.90,-103.0));
			addOrUpdateColliders(collidersOBB, "cubo-13", cubo13,modelMatrixColliderCube13);
			cubo13.u = glm::quat_cast(modelMatrixColliderCube13);
			modelMatrixColliderCube13 = glm::scale(modelMatrixColliderCube13, glm::vec3(09.0,0.05,03.10));
			modelMatrixColliderCube13 = glm::translate(modelMatrixColliderCube13, modelCube.getObb().c);
			cubo13.c = modelMatrixColliderCube13[3];
			cubo13.e = modelCube.getObb().e*glm::vec3(9,0.05,03.10);
			//cubo13.e = modelCube.getObb().e * glm::vec3(0.8f,0.50f,01.10f);

		AbstractModel::OBB cubo14;
		glm::mat4 modelMatrixColliderCube14 = glm::mat4(matrixModelCube);
			modelMatrixColliderCube14 = glm::translate(matrixModelCube, glm::vec3(-13.0,37.800,-198.70));
			modelMatrixColliderCube14 = glm::rotate(modelMatrixColliderCube14, glm::radians(-25.0f), glm::vec3(1, 0, 0));
			//modelMatrixColliderCube14 = glm::translate(matrixModelCube, glm::vec3(012.0,5.90,-103.0));
			addOrUpdateColliders(collidersOBB, "cubo-14", cubo14,modelMatrixColliderCube14);
			cubo14.u = glm::quat_cast(modelMatrixColliderCube14);
			modelMatrixColliderCube14 = glm::scale(modelMatrixColliderCube14, glm::vec3(08.0,0.5,04.50));
			modelMatrixColliderCube14 = glm::translate(modelMatrixColliderCube14, modelCube.getObb().c);
			cubo14.c = modelMatrixColliderCube14[3];
			cubo14.e = modelCube.getObb().e*glm::vec3(8,0.5,04.50);
			//cubo14.e = modelCube.getObb().e * glm::vec3(0.8f,0.50f,01.10f);

		// CUBOS DEL TERRENO

		AbstractModel::OBB cubo15;
		glm::mat4 modelMatrixColliderCube15 = glm::mat4(matrixModelCube);
			modelMatrixColliderCube15 = glm::translate(matrixModelCube, glm::vec3(012.0,14.00,-188.00));
			//modelMatrixColliderCube15 = glm::rotate(modelMatrixColliderCube15, glm::radians(-27.0f), glm::vec3(1, 0, 0));
			//modelMatrixColliderCube15 = glm::translate(matrixModelCube, glm::vec3(012.0,5.90,-103.0));
			addOrUpdateColliders(collidersOBB, "cubo-15", cubo15,modelMatrixColliderCube15);
			cubo15.u = glm::quat_cast(modelMatrixColliderCube15);
			modelMatrixColliderCube15 = glm::scale(modelMatrixColliderCube15, glm::vec3(033.0,0.05,022.50));
			modelMatrixColliderCube15 = glm::translate(modelMatrixColliderCube15, modelBasePuente.getObb().c);
			cubo15.c = modelMatrixColliderCube15[3];
			cubo15.e = modelBasePuente.getObb().e*glm::vec3(33,0.05,022.50);

		AbstractModel::OBB cubo16;
		glm::mat4 modelMatrixColliderCube16 = glm::mat4(matrixModelCube);
			modelMatrixColliderCube16 = glm::translate(matrixModelCube, glm::vec3(010.0,24.60,-206.0));
			//modelMatrixColliderCube16 = glm::rotate(modelMatrixColliderCube16, glm::radians(27.0f), glm::vec3(1, 0, 0));
			//modelMatrixColliderCube16 = glm::translate(matrixModelCube, glm::vec3(012.0,5.90,-103.0));
			addOrUpdateColliders(collidersOBB, "cubo-16", cubo16,modelMatrixColliderCube16);
			cubo16.u = glm::quat_cast(modelMatrixColliderCube16);
			modelMatrixColliderCube16 = glm::scale(modelMatrixColliderCube16, glm::vec3(30,010.0,022.300));
			modelMatrixColliderCube16 = glm::translate(modelMatrixColliderCube16, modelBasePuente.getObb().c);
			cubo16.c = modelMatrixColliderCube16[3];
			cubo16.e = modelBasePuente.getObb().e*glm::vec3(30,10.0,022.300);
			//cubo16.e = modelBasePuente.getObb().e * glm::vec3(0.8f,0.50f,01.10f);

		AbstractModel::OBB chec;
		glm::mat4 modelMatrixColliderChec = glm::mat4(matrixModelCube);
			modelMatrixColliderChec = glm::translate(matrixModelCube, glm::vec3(010.0,warcoy,-151.40));
			//modelMatrixColliderChec = glm::translate(matrixModelCube, glm::vec3(012.0,5.90,-103.0));
			addOrUpdateColliders(collidersOBB, "warco", chec,modelMatrixColliderChec);
			chec.u = glm::quat_cast(modelMatrixColliderChec);
			modelMatrixColliderChec = glm::scale(modelMatrixColliderChec, glm::vec3(01.30,05.0,0.0));
			modelMatrixColliderChec = glm::translate(modelMatrixColliderChec, modelCube.getObb().c);
			chec.c = modelMatrixColliderChec[3];
			chec.e = modelCube.getObb().e*glm::vec3(1.3,5.0,0.0);


		// Collider de Heroe
		AbstractModel::OBB heroeCollider;
		glm::mat4 modelmatrixColliderHeroe = glm::mat4(modelMatrixHeroe);
		modelmatrixColliderHeroe = glm::rotate(modelmatrixColliderHeroe,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		heroeCollider.u = glm::quat_cast(modelmatrixColliderHeroe);
		modelmatrixColliderHeroe = glm::scale(modelmatrixColliderHeroe, glm::vec3(0.3,0.15,0.01));
		modelmatrixColliderHeroe = glm::translate(modelmatrixColliderHeroe,
				glm::vec3(heroeModelAnimate.getObb().c.x,
						heroeModelAnimate.getObb().c.y,
						heroeModelAnimate.getObb().c.z));
		heroeCollider.e = heroeModelAnimate.getObb().e * glm::vec3(0.3f,0.15f,0.01f);
		heroeCollider.c = glm::vec3(modelmatrixColliderHeroe[3]);
		addOrUpdateColliders(collidersOBB, "heroe", heroeCollider, modelMatrixHeroe);

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

		for (std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
				collidersSBB.begin(); it != collidersSBB.end(); it++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			matrixCollider = glm::scale(matrixCollider, glm::vec3(std::get<0>(it->second).ratio * 2.0f));
			sphereCollider.setColor(glm::vec4(0.1, 0.1, 0.1, 0.1));
			sphereCollider.enableWireMode();
			//sphereCollider.render(matrixCollider);
		}

		animationHeroeIndex = 0;
		

		/**
		 * Colisiones de Oriented Bounding Box - Oriented Bounding Box
		 */

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
					//std::cout << "Hay collision entre " << it->first << " y el modelo " << jt->first << std::endl;
					isCollision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		for (std::map<std::string,
			std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator it =
			collidersOBB.begin(); it != collidersOBB.end(); it++) {
			int i = 0;
			int j = 0;
			bool isColision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator jt =
				collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
				if (it != jt && 
					testOBBOBB(std::get<0>(it->second), std::get<0>(jt->second))) {
					std::cout << "Hay colision entre " << it->first << " y el modelo" <<	jt->first << std::endl;
					isColision = true;

					//if((it->first.find("cubo-") && jt->first == "heroe") || (jt->first.find("cubo-") && it->first == "heroe")){
					if(((it->first.find("cubo-")!= std::string::npos )&& (jt->first.find("heroe")!= std::string::npos))|| ((jt->first.find("cubo-")!= std::string::npos) && (it->first.find("heroe")!= std::string::npos))){
						accionChec();
					}else if(((it->first.find("warco")!= std::string::npos) && (jt->first.find("heroe")!= std::string::npos)) || (jt->first.find("warco")!= std::string::npos && (it->first.find("heroe")!= std::string::npos))){
						warco = 1;
						warcoy = -10.0f;
						//isColision = false;
					}
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isColision);
		}

		for (std::map<std::string,
			std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator it =
			collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator jt =
				collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
				if (testSphereOBox(std::get<0>(it->second), std::get<0>(jt->second))) {
					//std::cout << "Hay colision del " << it->first << " y el modelo" <<	jt->first << std::endl;
					isCollision = true;
					addOrUpdateCollisionDetection(collisionDetection, jt->first, true);
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		std::map<std::string, bool>::iterator itCollision;
		for (itCollision = collisionDetection.begin(); itCollision != collisionDetection.end(); itCollision++) {
			std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator sbbBuscado = collidersSBB.find(itCollision->first);
			std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator obbBuscado = collidersOBB.find(itCollision->first);
			if (sbbBuscado != collidersSBB.end()) {
				if (!itCollision->second) 
					addOrUpdateColliders(collidersSBB, itCollision->first);
			}
			if (obbBuscado != collidersOBB.end()) {
				if (!itCollision->second) 
					addOrUpdateColliders(collidersOBB, itCollision->first);
				else {
					if (itCollision->first.compare("heroe") == 0)
						modelMatrixHeroe = std::get<1>(obbBuscado->second);
				}
			}
		}
		std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::
			iterator itSBB;

		glfwSwapBuffers(window);
		
		//OpenAL Sound data
		source0Pos[0] = modelMatrixHeroe[3].x;
		source0Pos[1] = modelMatrixHeroe[3].y;
		source0Pos[2] = modelMatrixHeroe[3].z;
		alSourcefv(source[0], AL_POSITION, source0Pos);
		
		source1Pos[0] = matrixModelTower[3].x;
		source1Pos[1] = matrixModelTower[3].y;
		source1Pos[2] = matrixModelTower[3].z;
		alSourcefv(source[1], AL_POSITION, source1Pos);

		source2Pos[0] = matrixModelArc[3].x;
		source2Pos[1] = matrixModelArc[3].y;
		source2Pos[2] = matrixModelArc[3].z;
		alSourcefv(source[2], AL_POSITION, source2Pos);

		listenerPos[0] = modelMatrixHeroe[3].x;
		listenerPos[1] = modelMatrixHeroe[3].y;
		listenerPos[2] = modelMatrixHeroe[3].z;
		alListenerfv(AL_POSITION, listenerPos);

		glm::vec3 upModel = glm::normalize(modelMatrixHeroe[1]);
		glm::vec3 frontModel = glm::normalize(modelMatrixHeroe[2]);

		listenerOri[0] = frontModel.x;
		listenerOri[1] = frontModel.y;
		listenerOri[2] = frontModel.z;
		listenerOri[3] = upModel.x;
		listenerOri[4] = upModel.y;
		listenerOri[5] = upModel.z;

		for(unsigned int i = 0; i < sourcesPlay.size(); i++){
			if(sourcesPlay[i]){
				alSourcePlay(source[i]);
				sourcesPlay[i] = false;
			}
		}
		isOn = true;

	}
}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}