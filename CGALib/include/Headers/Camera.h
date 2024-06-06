#ifndef CAMERA_H
#define CAMERA_H

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

#define SPEED 3.0f
#define SENSITIVITY 25.0f

#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

class DLL_PUBLIC Camera
{
public:
    virtual void mouseMoveCamera(float xoffset, float yoffset, float dt) = 0;
    virtual void scrollMoveCamera(float soffset, float dt) = 0;
    virtual void updateCamera() = 0;

    glm::mat4 getViewMatrix() {
        return glm::lookAt(position, position + front, worldUp);
    }

    void setPosition(const glm::vec3& position) {
        this->position = position;
        updateCamera();
    }

    glm::vec3 getPosition() const {
        return this->position;
    }

    glm::vec3 getUp() const {
        return this->up;
    }

    void setUp(const glm::vec3& up){
        this->up = up;
    }

    glm::vec3 getFront() const {
        return this->front;
    }

    glm::vec3 getRight() const {
        return this->right;
    }

    float getSensitivity() const {
        return this->sensitivity;
    }

    void setSensitivity(float sensitivity) {
        this->sensitivity = sensitivity;
    }

    float getSpeed() const {
        return this->speed;
    }

    void setSpeed(float speed) {
        this->speed = speed;
    }

    void setCameraTarget(const glm::vec3& cameraTarget){
        this->cameraTarget = cameraTarget;
        updateCamera();
    }

    glm::vec3 getCameraTarget() const {
        return this->cameraTarget;
    }

    void setAngleTarget(float angleTarget) {
        this->angleTarget = angleTarget;
    }

    float getAngleTarget() const {
        return this->angleTarget;
    }

    void setDistanceFromTarget(float distanceFromTarget) {
        this->distanceFromTarget = distanceFromTarget;
    }

protected:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    glm::vec3 cameraTarget;
    float angleTarget;
    float distanceFromTarget;
    float angleAroundTarget;

    float yaw;
    float pitch;
    float sensitivity;
    float speed;
};

#endif // CAMERA_H
