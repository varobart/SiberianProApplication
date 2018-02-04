#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>

using namespace std;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#include <SOIL2.h>

// GLFW
#include <GLFW/glfw3.h>

#include "headers/Shader.h"
#include "headers/Rectangle.h"
#include "headers/ObjectLoader.h"
#include "headers/Camera.h"
#include "headers/Window.h"
#include "headers/RawModel.h"
#include "headers/Texture.h"
#include "headers/CubeMap.h"
#include "headers/Collisions.h"


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

bool intersectRayParallelepiped(Rectangle& rect, glm::vec3& cameraPos, glm::vec3& rayDir,
								glm::mat4& modelMatrix, std::vector<glm::vec3>& normals);
void do_movement();

glm::vec3 transformMousePosToWorld(int xPos,int yPos, const glm::mat4& projMatrix, const glm::mat4& viewMatrix);


unsigned int loadTexture(char const * path);
unsigned int loadCubemap(vector<std::string> faces);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
GLfloat lastTime = glfwGetTime();
GLfloat deltaTime = 0;
GLfloat lastX = 400, lastY = 300;
bool keys[1024];
bool firstMouse = false;



double xPos, yPos;
Camera camera;




std::vector<glm::vec3> normals;


int main()
{
	glfwInit();

    // Create a GLFWwindow object that we can use for GLFW's functions
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);  


	Window window(WIDTH, HEIGHT);
	if(window.init() == -1){
		return -1;
	}
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the required callback functions
	glfwSetKeyCallback(window.getGLFWwindow(), key_callback);
	glfwSetCursorPosCallback(window.getGLFWwindow(), mouse_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }


	Rectangle rect("C:/BlenderObjects/ObjectRectangles/rectangles.txt","pawn_rectangle:");
	RawModel pawnModel("C:/BlenderObjects/pawnblend.obj");
	pawnModel.load();

	RawModel cube("cube.txt");
	cube.load(false);

	CubeMap cubeMap("cube_map_vertices.txt","cube_map/orbital-element_ft.tga","cube_map/orbital-element_bk.tga",
		"cube_map/orbital-element_up.tga","cube_map/orbital-element_dn.tga",
		"cube_map/orbital-element_rt.tga","cube_map/orbital-element_lf.tga");
	cubeMap.load();

	


	Texture texture1("textures/container.jpg");
	Texture texture2("textures/awesomeface.png");
	Texture texture3("textures/white_chessman_texture.jpg");



    // Define the viewport dimensions
    int width, height;
	glfwGetFramebufferSize(window.getGLFWwindow(), &width, &height);  
    glViewport(0, 0, width, height);


	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	glm::vec3 cubePositions[] = {
	  glm::vec3( 0.0f,  0.0f,  -3.0f), 
	  glm::vec3( 2.0f,  5.0f, -15.0f), 
	  glm::vec3(-1.5f, -2.2f, -2.5f),  
	  glm::vec3(-3.8f, -2.0f, -12.3f),  
	  glm::vec3( 2.4f, -0.4f, -3.5f),  
	  glm::vec3(-1.7f,  3.0f, -7.5f),  
	  glm::vec3( 1.3f, -2.0f, -2.5f),  
	  glm::vec3( 1.5f,  2.0f, -4.5f), 
	  glm::vec3( 1.5f,  0.2f, -1.5f), 
	  glm::vec3(-1.3f,  1.0f, -9.5f)  
	};

	glm::vec3 cubePositions2[] = {
	  glm::vec3( 0.0f,  0.0f,  -30.0f), 
	  glm::vec3( 2.0f,  5.0f, -30.0f), 
	};

	normals.push_back(glm::vec3(1, 0, 0));
	normals.push_back(glm::vec3(-1, 0, 0));
	normals.push_back(glm::vec3(0, 1, 0));
	normals.push_back(glm::vec3(0, -1, 0));
	normals.push_back(glm::vec3(0, 0, 1));
	normals.push_back(glm::vec3(0, 0, -1));



	Shader myShader("shaders/vertex_shader.txt", "shaders/fragment_shader.txt");
	Shader objectShader("shaders/objects_vertex_shader.txt", "shaders/objects_fragment_shader.txt");
	Shader cubeMapShader("shaders/cube_map_vertex_shader.txt", "shaders/cube_map_fragment_shader.txt");


	objectShader.use();
	objectShader.setFloat("material.shininess", 76.8f);

	objectShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	objectShader.setVec3("dirLight.ambient", 0.05f, 0.0f, 0.0f);
	objectShader.setVec3("dirLight.diffuse", 0.4f, 0.0f, 0.0f);
	objectShader.setVec3("dirLight.specular", 0.5f, 0.0f, 0.0f);

	objectShader.setVec3("pointLight.position", 5, 5, -4.5);
	objectShader.setVec3("pointLight.ambient", 0.05f, 0.05f, 0.05f);
	objectShader.setVec3("pointLight.diffuse", 0.0f, 0.0f, 0.8f);
	objectShader.setVec3("pointLight.specular", 0.0f, 0.0f, 1.0f);
	objectShader.setFloat("pointLight.constant", 1.0f);
	objectShader.setFloat("pointLight.linear", 0.09f);
	objectShader.setFloat("pointLight.quadratic", 0.032f);

	objectShader.setVec3("spotlight.ambient", 0.0f, 0.0f, 0.0f);
	objectShader.setVec3("spotlight.diffuse", 0.0f, 1.0f, 0.0f);
	objectShader.setVec3("spotlight.specular", 0.0f, 1.0f, 0.0f);
	objectShader.setFloat("spotlight.innerCutOff", glm::cos(glm::radians(12.5f)));
	objectShader.setFloat("spotlight.outerCutOff", glm::cos(glm::radians(17.5f)));


	glm::mat4 projection;
	glm::mat4 view;


	projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);


	int j = 0;
    // Game loop
	while (!window.shouldClose())
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
		do_movement();

			

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		view = camera.getViewMatrix();
		



		objectShader.use();
		objectShader.setVec3("spotlight.position", camera.getPosition());
		objectShader.setVec3("spotlight.direction", camera.getFront());
		glm::mat4 pawnMod;
		//pawnMod = glm::scale(pawnMod, glm::vec3(20,20,20));
		pawnMod = translate(pawnMod, glm::vec3(5,5,-5));
		objectShader.setMat4("projection", projection);
		objectShader.setMat4("view", view);
		objectShader.setMat4("model", pawnMod);
		texture3.passToShader(objectShader.getID(), "material.diffuse", 0);
		texture3.passToShader(objectShader.getID(), "material.specular", 1);
		pawnModel.draw();



		myShader.use();
		myShader.setMat4("projection", projection);
		myShader.setMat4("view", view);
		texture1.passToShader(myShader.getID(), "ourTexture1", 0);
		texture2.passToShader(myShader.getID(), "ourTexture2", 1);
		//glBindVertexArray(VAO);
		glm::mat4 model;
		for(glm::vec3 pos : cubePositions){	
			model = glm::translate(model, pos);
			myShader.setMat4("model", model);
			cube.draw();
		}
	
		for(glm::vec3 pos : cubePositions2){
			model = glm::translate(model, pos);
			myShader.setMat4("model", model);
			cube.draw();
		}


        cubeMapShader.use();
        view = glm::mat4(glm::mat3(camera.getViewMatrix())); // remove translation from the view matrix
		cubeMapShader.setMat4("projection", projection);
		cubeMapShader.setMat4("view", view);
		cubeMap.draw();


		bool intersection = Collisions::intersectRayParallelepiped(rect, camera.getPosition(), window, pawnMod, normals,
			xPos,yPos, projection, view);

		//j++;
		//if(intersection && j%10 == 0){
		//	cout << "intersection \n";
		//}
        // Swap the screen buffers
		glfwSwapBuffers(window.getGLFWwindow());
		
    }

    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}


void do_movement()
{
	// Camera controls'
	GLfloat currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	if(keys[GLFW_KEY_W])
		camera.processKeyboard(Camera::CameraMovement::FORWARD, deltaTime);
	 if(keys[GLFW_KEY_S])
  		camera.processKeyboard(Camera::CameraMovement::BACKWARD, deltaTime);
	if(keys[GLFW_KEY_A])
  		camera.processKeyboard(Camera::CameraMovement::LEFT, deltaTime);
	if(keys[GLFW_KEY_D])
	 	camera.processKeyboard(Camera::CameraMovement::RIGHT, deltaTime);
}




// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

	 
	 if(key == GLFW_KEY_W){
		if(action == GLFW_PRESS)
			keys[key] = true;
		else if(action == GLFW_RELEASE)
			keys[key] = false;
	 }

	 if(key == GLFW_KEY_S){
		if(action == GLFW_PRESS)
			keys[key] = true;
		else if(action == GLFW_RELEASE)
			keys[key] = false;
	 }

	if(key == GLFW_KEY_A){
		if(action == GLFW_PRESS)
			keys[key] = true;
		else if(action == GLFW_RELEASE)
			keys[key] = false;
	 }

	if(key == GLFW_KEY_D){
		if(action == GLFW_PRESS)
			keys[key] = true;
		else if(action == GLFW_RELEASE)
			keys[key] = false;
	 }

}

int j = 0;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

	j++;
	if(j%10 == 0){
		xPos = xpos;
		yPos = ypos;
		cout << xpos << "   " << ypos << "\n";
	}

  
    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

	camera.processMouseMovement(xoffset, yoffset);

}


