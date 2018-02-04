#include "Collisions.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SOIL2.h>

// GLFW


#include "Shader.h"
#include "Rectangle.h"
#include "Window.h"

#include <GLFW/glfw3.h>


bool Collisions::intersectRayParallelepiped(Rectangle& rect, glm::vec3& cameraPos, Window& window,
											glm::mat4& modelMatrix, std::vector<glm::vec3>& normals,
											int xPos,int yPos, const glm::mat4& projMatrix, const glm::mat4& viewMatrix){

		glm::vec3& rayDir = transformMousePosToWorld(xPos, yPos, projMatrix, viewMatrix, window);
		glm::vec4 minVec = modelMatrix * glm::vec4(rect.getxMin(), rect.getyMin(), rect.getzMin(), 1);
		glm::vec4 maxVec = modelMatrix * glm::vec4(rect.getxMax(), rect.getyMax(), rect.getzMax(), 1);

		float d[6] = {-maxVec.x, -minVec.x,-maxVec.y,-minVec.y,-maxVec.z,-minVec.z };

		for(int i = 0; i < 6;i++){
			float t = -(glm::dot(cameraPos, normals[i]) + d[i])/glm::dot(rayDir, normals[i]);
			if(t > 0){
				glm::vec3 point = cameraPos  + (rayDir * t);
				switch(i){
					case 0:
						if(point[1] < maxVec.y && point[1] > minVec.y && point[2] < maxVec.z && point[2] > minVec.z){
							return true;
						}
						break;
					case 1:
						if(point[1] < maxVec.y && point[1] > minVec.y && point[2] < maxVec.z && point[2] > minVec.z){
							return true;
						}
						break;
					case 2:
						if(point[0] < maxVec.x && point[0] > minVec.x && point[2] < maxVec.z && point[2] > minVec.z){
							return true;
						}
						break;
					case 3:
						if(point[0] < maxVec.x && point[0] > minVec.x && point[2] < maxVec.z && point[2] > minVec.z){
							return true;
						}
						break;
					case 4:
						if(point[0] < maxVec.x && point[0] > minVec.x && point[1] < maxVec.y && point[1] > minVec.y){
							return true;
						}
						break;
					case 5:
						if(point[0] < maxVec.x && point[0] > minVec.x && point[1] < maxVec.y && point[1] > minVec.y){
							return true;
						}
						break;
				
				}
			}
		}

		return false;




}


glm::vec3 Collisions::transformMousePosToWorld(int xPos,int yPos, const glm::mat4& projMatrix, const glm::mat4& viewMatrix, Window& window ){
	float x = (2.0f * xPos) / window.getWidth() - 1.0f;
	float y = 1.0f - (2.0f * yPos) / window.getHeight();
	float z = 1.0f;
	glm::vec3 ray_nds(x, y, z);
	glm::vec4 ray_clip(ray_nds.x,ray_nds.y, -1.0, 1.0);
	glm::vec4 ray_eye = glm::inverse(projMatrix) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
	glm::vec3 ray_wor = glm::vec3((glm::inverse(viewMatrix) * ray_eye).x,(glm::inverse(viewMatrix) * ray_eye).y,
		(glm::inverse(viewMatrix) * ray_eye).z);
	// don't forget to normalise the vector at some point
	return ray_wor = glm::normalize(ray_wor);






}