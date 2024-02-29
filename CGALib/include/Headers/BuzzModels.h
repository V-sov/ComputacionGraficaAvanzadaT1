#ifndef BUZZ_MODELS_H
#define BUZZ_MODELS_H

#include "Model.h" // Incluir el archivo de encabezado necesario para los modelos
#include "Shader.h" // Incluir el archivo de encabezado necesario para los shaders
#include <glm/glm.hpp> // Incluir el archivo de encabezado necesario para glm

class BuzzModels {
public:
    BuzzModels(); // Constructor
    ~BuzzModels(); // Destructor

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

    // Inicialización de los modelos
    void initBuzzModels(Shader & shader); // Declaración de la función initBuzzModels
};

#endif // BUZZ_MODELS_H