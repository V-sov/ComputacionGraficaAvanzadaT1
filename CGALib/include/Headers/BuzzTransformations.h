#ifndef BUZZ_TRANSFORMATIONS_H
#define BUZZ_TRANSFORMATIONS_H

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
#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp> 
class DLL_PUBLIC BuzzTransformations {
public:
    // Método para capturar movimientos
    void captureMovements(std::vector<float>& matrixBuzzJoints,float rotBuzzHead, float rotBuzzLeftArm, 
float rotBuzzLeftForeArm, float rotBuzzRightForeArm,
float rotBuzzLeftHand, float rotBuzzRightArm, 
float rotBuzzRightHand, 
float rotBuzzLeftThigh, float rotBuzzRightThigh, 
float rotBuzzRightCalf, float rotBuzzLeftCalf,
float rotBuzzRightFoot, float rotBuzzLeftFoot);
private:
};
#endif // BUZZ_TRANSFORMATIONS_H