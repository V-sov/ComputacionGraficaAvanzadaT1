#ifndef BUZZMODELS_H
#define BUZZMODELS_H

#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_DLL
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllexport))
    #else
      #define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #else
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllimport))
    #else
      #define DLL_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #endif
  #define DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define DLL_PUBLIC __attribute__ ((visibility ("default")))
    #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define DLL_PUBLIC
    #define DLL_LOCAL
  #endif
#endif

#include "Model.h" // Incluir el archivo de encabezado necesario para los modelos
#include "Shader.h" // Incluir el archivo de encabezado necesario para los shaders

class DLL_PUBLIC BuzzModels {
public:
    //BuzzModels(); // Constructor
    // Inicialización de los modelos
    void initBuzzModels(Shader & shader); // Declaración de la función initBuzzModels
    //Liberación de la memoria
    void destruir();
    //~BuzzModels(); // Destructor

    // Declaraciones de los modelos como miembros de la clase
    Model modelBuzzTorso;
    Model modelBuzzHead;
    Model modelBuzzLeftArm;
    Model modelBuzzLeftForeArm;
    Model modelBuzzLeftHand;
    Model modelBuzzRightArm;
    Model modelBuzzRightForeArm;
    Model modelBuzzRightHand;
    Model modelBuzzHip;
    Model modelBuzzLeftThigh;
    Model modelBuzzLeftCalf;
    Model modelBuzzLeftFoot;
    Model modelBuzzRightThigh;
    Model modelBuzzRightCalf;
    Model modelBuzzRightFoot;
    Model modelBuzzLeftWing1;
    Model modelBuzzLeftWing2;
    Model modelBuzzRightWing1;
    Model modelBuzzRightWing2;

    glm::mat4 modelMatrixBuzz; // Matriz de modelo para Buzz

    
};

#endif // BUZZMODELS_H