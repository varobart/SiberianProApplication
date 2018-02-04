#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods


// Default camera values
	const float YAW        = -90.0f;
	const float PITCH      =  0.0f;
	const float SPEED      =  10.0f;
	const float SENSITIVTY =  0.7f;
	const float ZOOM       =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Camera Attributes

	enum CameraMovement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};


    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float dyaw = YAW, float dpitch = PITCH);

    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    inline glm::mat4 getViewMatrix(){ 
		return glm::lookAt(Position, Position + Front, Up);
    }

	inline glm::vec3 getPosition(){ 
		return Position;
    }

	inline glm::vec3 getFront(){ 
		return Front;
    }

	inline glm::vec3 getUp(){ 
		return Up;
    }

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void processKeyboard(CameraMovement direction, float deltaTime);
    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void processMouseScroll(float yoffset);

private:

	glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Eular Angles
    float yaw;
    float pitch;
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    // Calculates the front vector from the Camera's (updated) Eular Angles
    void updateCameraVectors();
};
#endif