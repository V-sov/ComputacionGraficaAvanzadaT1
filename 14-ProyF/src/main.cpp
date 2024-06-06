#define _USE_MATH_DEFINES
#define TRIANGLE_BUTTON 3
#define CIRCLE_BUTTON 1
#define JOYSTICK_LEFT_BUTTON 0 // Suponiendo que el botón izquierdo del joystick tiene el índice 0

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
bool resumeAct = false;
bool keyrelease = true;
bool menuRelease = true;
bool ctrlRelease = true;
bool muerteRelease = true;
bool controlRelease = true;
bool checkRelease = true;
bool isThirdCamera = true;
bool changingCamera = false;

//Valores para checkpoint
// Pos. Arcos
float arco1x = 11.0;
float arco1y = 0.0;
float arco1z = 35.0;
// Vidas
int vida = 2;
int warco = 0;
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
Terrain terrain(-1, -1, 250, 10, "../Textures/heightmap3.png");

ShadowBox * shadowBox;

GLuint textureCespedID, textureWindowID, textureHighwayID, textureLandingPadID, textureArcID, textureIsleID;
GLuint textureTerrainRID, textureTerrainGID, textureTerrainBID, textureTerrainBlendMapID;
GLuint skyboxTextureID;
// Textura de inicio y pausa
GLuint textureStartID, textureResumeID, textureActivaID, textureMenuID, textureScreen2ID, textureControlID, textureKeysID;   
GLuint textureMuerteID;

//Proyecto

bool iniciaPartida = false, presionarOpcion = false, uno = false, muerte = false;
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
	glm::vec3(0.0, 5.0, 10.0),
	glm::vec3(7.0, 7.0, 11.5),
	glm::vec3(8.0, 12.0, 12.5),
	glm::vec3(7.0, 18.0, 13.5),	
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
ALfloat listenerPos[] = { 0.0, 0.0, 4.0 };
ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
ALfloat listenerOri[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 0.0 };
// Source 0
ALfloat source0Pos[] = { -2.0, 0.0, 0.0 };
ALfloat source0Vel[] = { 0.0, 0.0, 0.0 };
// Source 1
ALfloat source1Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source1Vel[] = { 0.0, 0.0, 0.0 };
// Source 2
ALfloat source2Pos[] = { 2.0, 0.0, 0.0 };
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

//Implementación para el botón la muerte
void accionMuerte(){
	//textureActivaID = textureStartID;
	textureActivaID = textureStartID;
	iniciaPartida = false;
	muerte = false;
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

	modelArc.loadModel("../models/Arco/Arco.fbx");
	modelArc.setShader(&shaderMulLighting);

	modelTower.loadModel("../models/Tower/Torre.fbx");
	modelTower.setShader(&shaderMulLighting);

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

	camera->setPosition(glm::vec3(-15.0f, 3.0f, 0.0f));
	camera->setDistanceFromTarget(distanceFromTarget);
	camera->setSensitivity(1.0);
	
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
	buffer[0] = alutCreateBufferFromFile("../sounds/fountain.wav");
	buffer[1] = alutCreateBufferFromFile("../sounds/fire.wav");
	buffer[2] = alutCreateBufferFromFile("../sounds/darth_vader.wav");
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
	alSourcef(source[0], AL_GAIN, 3.0f);
	alSourcefv(source[0], AL_POSITION, source0Pos);
	alSourcefv(source[0], AL_VELOCITY, source0Vel);
	alSourcei(source[0], AL_BUFFER, buffer[0]);
	alSourcei(source[0], AL_LOOPING, AL_TRUE);
	alSourcef(source[0], AL_MAX_DISTANCE, 2000);

	alSourcef(source[1], AL_PITCH, 1.0f);
	alSourcef(source[1], AL_GAIN, 0.5f);
	alSourcefv(source[1], AL_POSITION, source1Pos);
	alSourcefv(source[1], AL_VELOCITY, source1Vel);
	alSourcei(source[1], AL_BUFFER, buffer[1]);
	alSourcei(source[1], AL_LOOPING, AL_TRUE);
	alSourcef(source[1], AL_MAX_DISTANCE, 1000);

	alSourcef(source[2], AL_PITCH, 1.0f);
	alSourcef(source[2], AL_GAIN, 0.3f);
	alSourcefv(source[2], AL_POSITION, source2Pos);
	alSourcefv(source[2], AL_VELOCITY, source2Vel);
	alSourcei(source[2], AL_BUFFER, buffer[2]);
	alSourcei(source[2], AL_LOOPING, AL_TRUE);
	alSourcef(source[2], AL_MAX_DISTANCE, 2000);

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
	glDeleteTextures(1, &textureMenuID);
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
        int p3 = glfwGetKey(window, GLFW_KEY_ENTER);
        if (textureActivaID == textureScreen2ID || textureActivaID == textureMuerteID) {
            ctrlRelease = false;
        } else {
            ctrlRelease = true;
        }
        if (textureActivaID == textureStartID && presionarEnter && muerteRelease 
            && controlRelease && keyrelease && menuRelease) {
            iniciaPartida = true;
            pause = false;
            pauseInicio = false;
            muerte = false;
            textureActivaID = textureScreen2ID;        
            vida = 2;
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
        else if (!presionarOpcion && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            presionarOpcion = true;
            glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);
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
            else if (textureActivaID == textureMuerteID && muerte && ctrlRelease) {
                textureActivaID = textureMuerteID;
            }
            else if (textureActivaID == textureKeysID && ctrlRelease) {
                textureActivaID = textureKeysID;
            }
        }
        else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE) {
            presionarOpcion = false;
        }
        if (textureActivaID == textureMenuID && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
            textureActivaID = textureStartID;
            menuRelease = false;
            iniciaPartida = false;
            pauseInicio = true;
        }
    } else if (muerte == true && iniciaPartida == true) {
        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
            iniciaPartida = false;
            muerteRelease = false;
            textureActivaID = textureMenuID;
            accionMuerte();
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
        }
    }

    if (enableCountSelected && glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS) {
        if (iniciaPartida) {
            if (vida == 0) {
                muerte = true;
                textureActivaID = textureMuerteID;
            } else {
                chec = true;
            }
            checkRelease = false;
        }
    }
    if (enableCountSelected && glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_RELEASE) {
        checkRelease = true;
    }

    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE) {
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
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE) {
        ctrlRelease = true;
    }

    if (pause == false && iniciaPartida && !muerte) {
        if (glfwJoystickPresent(GLFW_JOYSTICK_1) == GL_TRUE) {
            std::cout << "Está presente el joystick" << std::endl;
            int axesCount, buttonCount;
            const float * axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
            std::cout << "Right Stick X axis: " << axes[2] << std::endl;
            std::cout << "Right Stick Y axis: " << axes[3] << std::endl;

            if (fabs(axes[1]) > 0.2) {
                modelMatrixHeroe = glm::translate(modelMatrixHeroe, glm::vec3(0, 0, -axes[1] * 0.1));
                animationHeroeIndex = 2;
            }
            if (fabs(axes[0]) > 0.2) {
                modelMatrixHeroe = glm::rotate(modelMatrixHeroe, glm::radians(-axes[0] * 0.5f), glm::vec3(0, 1, 0));
                animationHeroeIndex = 2;
            }

            if (fabs(axes[2]) > 0.2) {
                camera->mouseMoveCamera(-axes[2], 0.0, deltaTime);
            }
            if (fabs(axes[3]) > 0.2) {
                camera->mouseMoveCamera(0.0, -axes[3], deltaTime);
            }

            const unsigned char * buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
            if (buttons[0] == GLFW_PRESS)
                std::cout << "Se presiona x" << std::endl;

            if (!isJump && buttons[0] == GLFW_PRESS) {
                isJump = true;
                startTimeJump = currTime;
                tmv = 0;
            }
        }
		GLFWgamepadstate gamepadState;
		if (!isThirdCamera) {
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
				camera->mouseMoveCamera(offsetX, offsetY, deltaTime);

			// Integración de la rotación del joystick derecho
			if (glfwGetGamepadState(GLFW_JOYSTICK_1, &gamepadState) == GLFW_PRESS) {
				float rightThumbX = gamepadState.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
				float rightThumbY = gamepadState.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];
				if (fabs(rightThumbX) > 0.1 || fabs(rightThumbY) > 0.1) {
					// Lógica para manejar la rotación del joystick derecho
					// Por ejemplo:
					camera->mouseMoveCamera(rightThumbX, rightThumbY, deltaTime);
				}
			}
		} else {
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				cameraFP->moveFrontCamera(true, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				cameraFP->moveFrontCamera(false, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				cameraFP->moveRightCamera(false, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				cameraFP->moveRightCamera(true, deltaTime);
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
				cameraFP->mouseMoveCamera(offsetX, offsetY, deltaTime);
		}


		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS || (glfwGetGamepadState(GLFW_JOYSTICK_1, &gamepadState) == GLFW_PRESS &&
			gamepadState.buttons[GLFW_GAMEPAD_BUTTON_LEFT_THUMB] == GLFW_PRESS)) {
			changingCamera = true;
		}
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE || (glfwGetGamepadState(GLFW_JOYSTICK_1, &gamepadState) == GLFW_PRESS &&
			gamepadState.buttons[GLFW_GAMEPAD_BUTTON_LEFT_THUMB] == GLFW_RELEASE)) {
			if (changingCamera) {
				isThirdCamera = !isThirdCamera;
				std::cout << "Changed Camera" << std::endl;
			}
			changingCamera = false;
		}

        offsetX = 0;
        offsetY = 0;

        // Seleccionar modelo
        if (enableCountSelected && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
            enableCountSelected = false;
            modelSelected++;
            if (modelSelected > 4)
                modelSelected = 0;
            if (modelSelected == 1)
                modelSelected = 0;
            if (modelSelected == 2)
                modelSelected = 0;
            if (modelSelected == 3)
                modelSelected = 0;
            if (modelSelected == 4)

            std::cout << "modelSelected: " << modelSelected << std::endl;
        } else if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE) {
            enableCountSelected = true;
        }

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
        if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
            animationHeroeIndex = 3;
        }
        if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            animationHeroeIndex = 1;
        }
        bool keySpaceStatus = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
        if (!isJump && keySpaceStatus) {
            isJump = true;
            startTimeJump = currTime;
            tmv = 0;
            animationHeroeIndex = 1;
        } else if (isJump) {
            animationHeroeIndex = 1;
        }

        glm::vec3 heroPosition = glm::vec3(modelMatrixHeroe[3]);
        glm::vec3 cameraOffset = glm::vec3(-5.0f, 2.0f, 0.0f); // Ajusta estos valores según sea necesario
        glm::vec3 fixedCameraPosition = heroPosition + cameraOffset;
        camera->setPosition(fixedCameraPosition);
        camera->setCameraTarget(heroPosition);
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
	terrain.render();
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
	glBindTexture(GL_TEXTURE_2D, 0);
	
	
	/*******************************************
	 * Custom objects obj
	 *******************************************/
	//Rock render
	matrixModelRock[3][1] = terrain.getHeightTerrain(matrixModelRock[3][0], matrixModelRock[3][2]);
	modelRock.render(matrixModelRock);

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
	matrixModelTower[3][1] = terrain.getHeightTerrain(matrixModelTower[3][0], matrixModelTower[3][2]);
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
	
	// Render lamp
	for(int i = 0; i < FaroPosition.size(); i++){
		FaroPosition[i].y = terrain.getHeightTerrain(FaroPosition[i].x, FaroPosition[i].z);
		modelFaro.setPosition(FaroPosition[i]);
		modelFaro.setScale(glm::vec3(1.5));
		modelFaro.setOrientation(glm::vec3(0, FaroOrientation[i], 0));
		modelFaro.render();
	}
	for(int i = 0; i < lamp2Position.size(); i++){
		lamp2Position[i].y = terrain.getHeightTerrain(lamp2Position[i].x, lamp2Position[i].z);
		modelLamp2.setPosition(lamp2Position[i]);
		modelLamp2.setScale(glm::vec3(0.5));
		modelLamp2.setOrientation(glm::vec3(0, lamp2Orientation[i], 0));
		//modelLamp2.render();
		modelLampPost2.setPosition(lamp2Position[i]);
		modelLampPost2.setScale(glm::vec3(0.5));
		modelLampPost2.setOrientation(glm::vec3(0, lamp2Orientation[i], 0));
		//modelLampPost2.render();
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
	float angleTarget;

	int state = 0;

	matrixModelRock = glm::translate(matrixModelRock, glm::vec3(arco1x, arco1y, arco1z));
	matrixModelArc = glm::scale(matrixModelArc, glm::vec3(2.0, 2.0, 1.0));
	matrixModelArc = glm::translate(matrixModelArc, glm::vec3(arco1x, arco1y, arco1z));
	matrixModelArc = glm::rotate(matrixModelArc, glm::radians(-90.0f), glm::vec3(0, 0, 1));

	matrixModelTower = glm::scale(matrixModelTower, glm::vec3(9.0, 9.0, 9.0));
	matrixModelTower = glm::translate(matrixModelTower, glm::vec3(5.0, 0.0, -23.0));
	matrixModelTower = glm::rotate(matrixModelTower, glm::radians(-90.0f), glm::vec3(1, 0, 0));

	matrixModelBasePuente = glm::scale(matrixModelBasePuente, glm::vec3(6.0, 6.0, 6.0));
	matrixModelBasePuente = glm::translate(matrixModelBasePuente, glm::vec3(8.0, 1.0, 7.0));
	matrixModelBasePuente = glm::rotate(matrixModelBasePuente, glm::radians(-90.0f), glm::vec3(1, 0, 0));

	matrixModelBordePuente = glm::scale(matrixModelBordePuente, glm::vec3(6.0, 6.0, 6.0));
	matrixModelBordePuente = glm::translate(matrixModelBordePuente, glm::vec3(8.0, 2.0, 7.0));
	matrixModelBordePuente = glm::rotate(matrixModelBordePuente, glm::radians(-90.0f), glm::vec3(1, 0, 0));

	matrixModelIsle = glm::scale(matrixModelIsle, glm::vec3(3.0, 3.0, 3.0));
	matrixModelIsle = glm::translate(matrixModelIsle, glm::vec3(15.5, 4.0, -55.0));
	matrixModelIsle = glm::rotate(matrixModelIsle, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	
 	modelMatrixHeroe = glm::translate(modelMatrixHeroe, glm::vec3(13.0f, 0.05f, -150.0f));
	modelMatrixHeroe = glm::rotate(modelMatrixHeroe, glm::radians(-20.0f), glm::vec3(0, 1, 0));

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

		if(modelSelected == 1){
			axis = glm::axis(glm::quat_cast(modelMatrixHeroe));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixHeroe));
			target = modelMatrixHeroe[3];
		}
		else{
			axis = glm::axis(glm::quat_cast(modelMatrixHeroe));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixHeroe));
			target = modelMatrixHeroe[3];
		}

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
			boxIntro.render();
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
				std::cout << "x y z = " << arco1x  << " " << arco1y << " "  << arco1z << std::endl;
				modelMatrixHeroe = glm::mat4(1);
				modelMatrixHeroe = glm::translate(modelMatrixHeroe, glm::vec3(arco1x+13, arco1y, arco1z));
				modelMatrixHeroe = glm::rotate(modelMatrixHeroe, glm::radians(-20.0f), glm::vec3(0, 1, 0));
				modelMatrixHeroe = glm::translate(modelMatrixHeroe, glm::vec3(0, 0, 0));
				vida = vida-1;
				chec = false;
			}
		}else if(vida == 0 && muerte && checkRelease){
			modelMatrixHeroe = glm::mat4(1);
 			modelMatrixHeroe = glm::translate(modelMatrixHeroe, glm::vec3(13.0f, 0.05f, -5.0f));
			modelMatrixHeroe = glm::rotate(modelMatrixHeroe, glm::radians(-20.0f), glm::vec3(0, 1, 0));
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

		//Collider del la rock
		AbstractModel::SBB rockCollider;
		glm::mat4 modelMatrixColliderRock= glm::mat4(matrixModelRock);
		modelMatrixColliderRock = glm::scale(modelMatrixColliderRock, glm::vec3(1.0, 1.0, 1.0));
		modelMatrixColliderRock = glm::translate(modelMatrixColliderRock, modelRock.getSbb().c);
		rockCollider.c = glm::vec3(modelMatrixColliderRock[3]);
		rockCollider.ratio = modelRock.getSbb().ratio * 1.0;
		addOrUpdateColliders(collidersSBB, "rock", rockCollider, matrixModelRock);

		// Collider de Heroe
		AbstractModel::OBB heroeCollider;
		glm::mat4 modelmatrixColliderHeroe = glm::mat4(modelMatrixHeroe);
		modelmatrixColliderHeroe = glm::rotate(modelmatrixColliderHeroe,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		heroeCollider.u = glm::quat_cast(modelmatrixColliderHeroe);
		modelmatrixColliderHeroe = glm::scale(modelmatrixColliderHeroe, glm::vec3(0.021, 0.021, 0.021));
		modelmatrixColliderHeroe = glm::translate(modelmatrixColliderHeroe,
				glm::vec3(heroeModelAnimate.getObb().c.x,
						heroeModelAnimate.getObb().c.y,
						heroeModelAnimate.getObb().c.z));
		heroeCollider.e = heroeModelAnimate.getObb().e * glm::vec3(0.021, 0.021, 0.021) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
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
			sphereCollider.render(matrixCollider);
		}

		animationHeroeIndex = 0;

	/************Render de imagen de frente Texto Muerte**************/
		shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
		shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureActivaID);
		shaderTexture.setInt("outTexture", 0);
		glEnable(GL_BLEND);
		boxIntro.render();
		glDisable(GL_BLEND);
		if(textureActivaID == textureMuerteID){
			modelTextMuerte->modFuente(120, 0.850f, 0.850f, 0.850f, 1.0f);
			modelTextMuerte->render("GAME OVER",-0.78,0.0);
		}

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

		for (std::map<std::string,
			std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator it =
			collidersOBB.begin(); it != collidersOBB.end(); it++) {
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

		for (std::map<std::string,
			std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator it =
			collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator jt =
				collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
				if (testSphereOBox(std::get<0>(it->second), std::get<0>(jt->second))) {
					std::cout << "Hay colision del " << it->first << " y el modelo" <<
						jt->first << std::endl;
					isCollision = true;
					addOrUpdateCollisionDetection(collisionDetection, jt->first, true);
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		std::map<std::string, bool>::iterator itCollision;
		for (itCollision = collisionDetection.begin(); 
			itCollision != collisionDetection.end(); itCollision++) {
			std::map<std::string, std::tuple<AbstractModel::SBB, 
				glm::mat4, glm::mat4>>::iterator sbbBuscado = 
				collidersSBB.find(itCollision->first);
			std::map<std::string, std::tuple<AbstractModel::OBB,
				glm::mat4, glm::mat4>>::iterator obbBuscado =
				collidersOBB.find(itCollision->first);
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
		/*std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::
			iterator itOBB;
		for (itOBB = collidersOBB.begin(); itOBB != collidersOBB.end(); itOBB++) {
			if (testRayOBB(ori, targetRay, std::get<0>(itOBB->second))) {
				std::cout << "Collision del rayo con el modelo " << itOBB->first
					<< std::endl;
			}
		}*/

		// Esto es para ilustrar la transformacion inversa de los coliders
		/*glm::vec3 cinv = glm::inverse(heroeCollider.u) * glm::vec4(rockCollider.c, 1.0);
		glm::mat4 invColliderS = glm::mat4(1.0);
		invColliderS = glm::translate(invColliderS, cinv);
		invColliderS =  invColliderS * glm::mat4(heroeCollider.u);
		invColliderS = glm::scale(invColliderS, glm::vec3(rockCollider.ratio * 2.0, rockCollider.ratio * 2.0, rockCollider.ratio * 2.0));
		sphereCollider.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));
		sphereCollider.enableWireMode();
		sphereCollider.render(invColliderS);
		glm::vec3 cinv2 = glm::inverse(heroeCollider.u) * glm::vec4(heroeCollider.c, 1.0);
		glm::mat4 invColliderB = glm::mat4(1.0);
		invColliderB = glm::translate(invColliderB, cinv2);
		invColliderB = glm::scale(invColliderB, heroeCollider.e * 2.0f);
		boxCollider.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0)); f
		boxCollider.enableWireMode();
		boxCollider.render(invColliderB);
		// Se regresa el color blanco
		sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
		boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));*/
		
		

		glfwSwapBuffers(window);
	}
}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}