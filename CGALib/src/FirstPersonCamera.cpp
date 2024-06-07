#include "Headers/FirstPersonCamera.h"

FirstPersonCamera::FirstPersonCamera(glm::vec3 position, glm::vec3 worldUp,
	glm::vec3 front, float yaw, float pitch, float speed, float sensitivity){
	this->yaw = yaw;
	this->pitch = pitch;
	this->speed = speed;
	this->sensitivity = sensitivity;
	this->position = position;
	this->front = front;
	this->worldUp = worldUp;
	updateCamera();
}

void FirstPersonCamera::mouseMoveCamera(float xoffset, float yoffset, float dt) {
	xoffset *= dt * sensitivity;
	yoffset *= -1 * dt * sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	updateCamera();
}

void FirstPersonCamera::scrollMoveCamera(float soffset, float dt) {
}

void FirstPersonCamera::updateCamera() {
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	this->front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->front.y = sin(glm::radians(pitch));
	this->front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->front = glm::normalize(this->front);
	this->right = glm::normalize(glm::cross(this->front, this->worldUp));
	this->up = glm::normalize(glm::cross(this->right, this->front));
}

void FirstPersonCamera::moveFrontCamera(bool dir, float dt) {
	if (dir)
		this->position += (float)dt * speed * this->front;
	else
		this->position -= (float)dt * speed * this->front;
	updateCamera();
}

// Modificado para realizar una rotación alrededor del eje Y
void FirstPersonCamera::rotateLeftRight(float angle, bool left) {
    // Asumiendo que 'angle' es en grados y queremos convertirlo a radianes
    float radiansAngle = glm::radians(angle);

    if(left) {
        // Rotación hacia la izquierda
        this->yaw -= radiansAngle;
    } else {
        // Rotación hacia la derecha
        this->yaw += radiansAngle;
    }

    updateCamera(); // Actualizar la cámara después de la rotación
}
