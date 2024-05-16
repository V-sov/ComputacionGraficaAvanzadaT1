#include "Headers/BuzzTransformations.h"

void BuzzTransformations::captureMovements(std::vector<float>& matrixBuzzJoints,float rotBuzzHead, float rotBuzzLeftArm, 
float rotBuzzLeftForeArm, float rotBuzzRightForeArm,
float rotBuzzLeftHand, float rotBuzzRightArm, 
float rotBuzzRightHand, 
float rotBuzzLeftThigh, float rotBuzzRightThigh, 
float rotBuzzRightCalf, float rotBuzzLeftCalf,
float rotBuzzRightFoot, float rotBuzzLeftFoot){
    matrixBuzzJoints.push_back(rotBuzzHead);
	matrixBuzzJoints.push_back(rotBuzzLeftArm);
	matrixBuzzJoints.push_back(rotBuzzLeftForeArm);
	matrixBuzzJoints.push_back(rotBuzzLeftHand);
	matrixBuzzJoints.push_back(rotBuzzRightArm);
	matrixBuzzJoints.push_back(rotBuzzRightForeArm);
	matrixBuzzJoints.push_back(rotBuzzRightHand);
	matrixBuzzJoints.push_back(rotBuzzLeftThigh);
	matrixBuzzJoints.push_back(rotBuzzLeftCalf);
	matrixBuzzJoints.push_back(rotBuzzLeftFoot);
	matrixBuzzJoints.push_back(rotBuzzRightThigh);
	matrixBuzzJoints.push_back(rotBuzzRightCalf);
	matrixBuzzJoints.push_back(rotBuzzRightFoot);
}