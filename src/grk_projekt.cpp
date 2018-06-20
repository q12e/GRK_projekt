#include "glew.h"
#include "freeglut.h"
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <cstdlib>

#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "Camera.h"
#include "Texture.h"

#define ILOSCRYB 800
#define ILOSCWODOROSTOW 300

GLuint programColor;
GLuint programTexture;

Core::Shader_Loader shaderLoader;

float timeF;
float timeFish;
float timeShark;

obj::Model shipModel;
obj::Model sharkModel;
obj::Model fishModel;
obj::Model plantModel;
obj::Model groundModel;
obj::Model sphereModel;
float cameraAngle = 0;
float cameraAngle1 = 0;
glm::vec3 cameraPos = glm::vec3(0, -100, 5);
glm::vec3 cameraDir; // Wektor "do przodu" kamery
glm::vec3 cameraSide; // Wektor "w bok" kamery
glm::vec3 cameraVer; // Wektor "w góre" kamery
GLuint sharkTex;
GLuint fishTex;
GLuint plantTex;
GLuint asteroidTex;
GLuint groundTex;
GLuint skyTex;

glm::mat4 cameraMatrix, perspectiveMatrix;

glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, -0.9f, -1.0f));
glm::vec3 lightDir1 = glm::normalize(glm::vec3(-1.0f, 0.9f, 1.0f));
glm::vec3 lightDir2 = glm::normalize(glm::vec3(-1.0f, -0.9f, -1.0f));
glm::vec3 lightDir3 = glm::normalize(glm::vec3(1.0f, -0.9f, 1.0f));

void keyboard(unsigned char key, int x, int y)
{
	float angleSpeed = 0.1f;
	float angleSpeed1 = 0.5f;
	float moveSpeed = 0.7f;
	switch (key)
	{
	case 'z': cameraAngle -= angleSpeed; break;
	case 'x': cameraAngle += angleSpeed; break;
	case 'q': cameraPos -= cameraVer * moveSpeed; break;
	case 'e': if(cameraPos.y > -111) cameraPos += cameraVer * moveSpeed; break;
	case 'w': cameraPos += cameraDir * moveSpeed; break;
	case 's': cameraPos -= cameraDir * moveSpeed; break;
	case 'd': cameraPos += cameraSide * moveSpeed; break;
	case 'a': cameraPos -= cameraSide * moveSpeed; break;
	}
}

glm::mat4 createCameraMatrix()
{
	cameraDir = glm::vec3(cosf(cameraAngle - glm::radians(90.0f)), 0.0f, sinf(cameraAngle - glm::radians(90.0f)));
	glm::vec3 up = glm::vec3(0, 1, 0);
	cameraVer = glm::vec3(0.0f, sinf(cameraAngle1 - glm::radians(90.0f)), 0.0f);
	cameraSide = glm::cross(cameraDir, up);

	return Core::createViewMatrix(cameraPos, cameraDir, -cameraVer);
}

void drawObjectColor(obj::Model * model, glm::mat4 modelMatrix, glm::vec3 color)
{
	GLuint program = programColor;

	glUseProgram(program);

	glUniform3f(glGetUniformLocation(program, "objectColor"), color.x, color.y, color.z);
	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModel(model);

	glUseProgram(0);
}

void drawObjectProceduralTexture(obj::Model * model, glm::mat4 modelMatrix)
{

	GLuint program = programTexture;

	glUseProgram(program);
	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModel(model);

	glUseProgram(0);





}


void drawObjectTexture(obj::Model * model, glm::mat4 modelMatrix, GLuint textureId)
{
	GLuint program = programTexture;

	glUseProgram(program);

	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);
	glUniform3f(glGetUniformLocation(program, "lightDir1"), lightDir1.x, lightDir1.y, lightDir1.z);
	glUniform3f(glGetUniformLocation(program, "lightDir2"), lightDir2.x, lightDir2.y, lightDir2.z);

	Core::SetActiveTexture(textureId, "textureSampler", program, 0);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModel(model);

	glUseProgram(0);
}

class Ryba
{
public:
	Ryba();
	Ryba(obj::Model *model, GLuint *tex);
	void Render();

private:
	obj::Model *model;
	GLuint *tex;
	glm::vec3 position;
	glm::vec3 direction;
	GLfloat random;
	void Move();
	bool trigger;
	GLfloat moveSpeed;
	GLfloat slowSpeed;
	GLfloat scale;
	glm::vec3 temp;
	//float angle;
};

Ryba::Ryba()
{
	position = glm::ballRand(115.0f);
	direction = glm::ballRand(1.0f);
	//std::cout << "xD " + glm::to_string(this->direction) << std::endl;
	random = (std::rand() % 200) * 1.0f + glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	//this->moveSpeed = 1.0f;
	scale = 1.0f;
}

Ryba::Ryba(obj::Model *mdl, GLuint *txt)
{
	model = mdl;
	tex = txt;
	position = glm::sphericalRand(106.0f);
	random = (std::rand() % 200) * 1.0f + glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	//glm::vec3 lookat = glm::sphericalRand(1.0f);
	//float dot = position.x*lookat.x + position.z*lookat.z;
	//float det = position.x*lookat.z - position.z*lookat.x;
	//this->angle = atan2(dot, det);
	direction = glm::vec3(cosf(random), 0.0f, sinf(random));
	trigger = false;
	moveSpeed = 1.0f;
	slowSpeed = 0.05f;
	scale = (int)random % 75 / 100.0f + 0.25f;
}

void Ryba::Move()
{
	if (trigger == true) {
		position += direction * moveSpeed;
	}
	else
	{
		position += direction * slowSpeed;
	}
	temp = glm::vec3(position);
	temp.y -= 1.2*sinf(glutGet(GLUT_ELAPSED_TIME) / 500.0f - random);
	if (trigger == false && sqrt((cameraPos.x - temp.x)*(cameraPos.x - temp.x) + (cameraPos.y - temp.y)*(cameraPos.y - temp.y) + (cameraPos.z - temp.z)*(cameraPos.z - temp.z)) < 20.0f)
	{
		std::cout << "BLISKO" << std::endl;
		trigger = true;
	}
}

void Ryba::Render()
{
	Move();
	glm::mat4 ModelMatrix = glm::translate(temp + direction * 0.5f + glm::vec3(0, -0.25f, 0)) * glm::rotate(-random + glm::radians(90.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(scale));
	drawObjectTexture(model, ModelMatrix, *tex);
	if (abs(sqrt((0 - position.x)*(0 - position.x)+ (0 - position.z)*(0 - position.z))) > 120.0f) *this = Ryba(this->model, this->tex);
}

class Wodorost
{
public:
	Wodorost();
	void Render();

private:
	glm::vec3 position;
	GLfloat random;
	glm::vec3 direction;
};

Wodorost::Wodorost()
{
	random = (std::rand() % 200) * 1.0f + glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	direction = glm::vec3(cosf(random), 0.0f, sinf(random));
	position = glm::ballRand(150.0f);
	position.y = -115;
}

void Wodorost::Render()
{
	drawObjectTexture(&plantModel, glm::translate(position + direction * 0.5f), plantTex);
}

Ryba fish[ILOSCRYB];
Wodorost plant[ILOSCWODOROSTOW];

void renderScene()
{
	//debug pozycji
	//std::cout << glm::to_string(cameraPos) << std::endl;

	cameraMatrix = createCameraMatrix();
	perspectiveMatrix = Core::createPerspectiveMatrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.005f, 0.4f, 0.6f, 1.0f);


	glm::mat4 shipInitialTransformation = glm::translate(glm::vec3(0, -2.25f, -5)) * glm::rotate(glm::radians(180.0f), glm::vec3(0, 1, -0.1)) * glm::scale(glm::vec3(0.25f));
	glm::mat4 shipModelMatrix = glm::translate(cameraPos + cameraDir * 0.5f) * glm::rotate(-cameraAngle, glm::vec3(0, 1, 0)) * shipInitialTransformation;
	//glm::mat4 shipModelMatrix = glm::translate(cameraPos + cameraDir * 0.5f + glm::vec3(0,-0.25f,0)) * glm::rotate(-cameraAngle + glm::radians(90.0f), glm::vec3(0,1,0)) * glm::scale(glm::vec3(0.25f));
	//drawObjectColor(&shipModel, shipModelMatrix, glm::vec3(0.6f));
	drawObjectTexture(&sharkModel, shipModelMatrix, sharkTex);
	drawObjectTexture(&groundModel, glm::translate(glm::vec3(0, -115, 0)), groundTex);
	drawObjectTexture(&sphereModel, glm::translate(glm::vec3(0.0f, -130, 0.0f)) * glm::scale(glm::vec3(95.0f)), skyTex);

	timeF = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

	for (int i = 0; i < ILOSCRYB; i++)
	{
		fish[i].Render();
	}

	for (int i = 0; i < ILOSCWODOROSTOW; i++)
	{
		plant[i].Render();
	}

	glutSwapBuffers();
}

void init()
{
	glEnable(GL_DEPTH_TEST);

	for (int i = 0; i < ILOSCRYB; i++)
	{
		Ryba* temporary = new Ryba(&fishModel, &fishTex);
		fish[i] = *temporary;
	}

	for (int i = 0; i < ILOSCWODOROSTOW; i++)
	{
		Wodorost* temporary = new Wodorost();
		plant[i] = *temporary;
	}

	programColor = shaderLoader.CreateProgram("shaders/shader_color.vert", "shaders/shader_color.frag");
	programTexture = shaderLoader.CreateProgram("shaders/shader_tex.vert", "shaders/shader_tex.frag");
	groundModel = obj::loadModelFromFile("models/Rockwall.obj");
	shipModel = obj::loadModelFromFile("models/spaceship.obj");
	sharkModel = obj::loadModelFromFile("models/shark.obj");
	sphereModel = obj::loadModelFromFile("models/sphere.obj");
	sharkTex = Core::LoadTexture("Textures/shark.png");
	fishModel = obj::loadModelFromFile("models/fish.obj");
	fishTex = Core::LoadTexture("textures/fish.png");
	plantModel = obj::loadModelFromFile("models/plant.obj");
	plantTex = Core::LoadTexture("textures/plant.png");
	asteroidTex = Core::LoadTexture("textures/asteroid.png");
	groundTex = Core::LoadTexture("textures/ground.png");
	skyTex = Core::LoadTexture("textures/Skybox.png");
}

void shutdown()
{
	shaderLoader.DeleteProgram(programColor);
	shaderLoader.DeleteProgram(programTexture);
}

void idle()
{
	glutPostRedisplay();
}

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(1024, 800);
	glutCreateWindow("GRK Plywajac z rybkami");
	glewInit();

	init();
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(renderScene);
	glutIdleFunc(idle);

	glutMainLoop();

	shutdown();

	return 0;
}
