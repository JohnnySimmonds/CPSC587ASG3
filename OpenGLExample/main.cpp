// ==========================================================================
// Barebones OpenGL Core Profile Boilerplate
//    using the GLFW windowing system (http://www.glfw.org)
//
// Loosely based on
//  - Chris Wellons' example (https://github.com/skeeto/opengl-demo) and
//  - Camilla Berglund's example (http://www.glfw.org/docs/latest/quick.html)
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <algorithm>
#include <vector>
#include <cstdlib>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// specify that we want the OpenGL core profile before including GLFW headers
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "camera.h"
#include "Mass.h"
#include "Spring.h"

#define PI 3.14159265359

using namespace std;
using namespace glm;

//Forward definitions
bool CheckGLErrors(string location);
void QueryGLVersion();
string LoadSource(const string &filename);
GLuint CompileShader(GLenum shaderType, const string &source);
GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);


vec2 mousePos;
bool leftmousePressed = false;
bool rightmousePressed = false;
bool play = false;
bool springChain = false;
int scene = 0;
bool initSpringSys = false;
Camera* activeCamera;

GLFWwindow* window = 0;

mat4 winRatio = mat4(1.f);

// --------------------------------------------------------------------------
// GLFW callback functions

// reports GLFW errors
void ErrorCallback(int error, const char* description)
{
    cout << "GLFW ERROR " << error << ":" << endl;
    cout << description << endl;
}

// handles keyboard input events
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        {
			if(!play)
				play = true;
			else
				play = false;
		}
	
   if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
	
		scene += 1;
		if(scene == 4)
			scene = 0;
			
		if(!springChain)
			springChain = true;
		else
			springChain = false;
		
		initSpringSys = false;
		//initSpringSys = false;
	}
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if( (action == GLFW_PRESS) || (action == GLFW_RELEASE) ){
		if(button == GLFW_MOUSE_BUTTON_LEFT)
			leftmousePressed = !leftmousePressed;
		else if(button == GLFW_MOUSE_BUTTON_RIGHT)
			rightmousePressed = !rightmousePressed;
	}
}

void mousePosCallback(GLFWwindow* window, double xpos, double ypos)
{
	int vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);

	vec2 newPos = vec2(xpos/(double)vp[2], -ypos/(double)vp[3])*2.f - vec2(1.f);

	vec2 diff = newPos - mousePos;
	if(leftmousePressed){
		activeCamera->trackballRight(-diff.x);
		activeCamera->trackballUp(-diff.y);
	}
	else if(rightmousePressed){
		float zoomBase = (diff.y > 0) ? 1.f/2.f : 2.f;

		activeCamera->zoom(pow(zoomBase, abs(diff.y)));
	}

	mousePos = newPos;
}

void resizeCallback(GLFWwindow* window, int width, int height)
{
	int vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);

	glViewport(0, 0, width, height);

	float minDim = float(std::min(width, height));

	winRatio[0][0] = minDim/float(width);
	winRatio[1][1] = minDim/float(height);
}





//==========================================================================
// TUTORIAL STUFF


//vec2 and vec3 are part of the glm math library. 
//Include in your own project by putting the glm directory in your project, 
//and including glm/glm.hpp as I have at the top of the file.
//"using namespace glm;" will allow you to avoid writing everyting as glm::vec2

struct VertexBuffers{
	enum{ VERTICES=0, NORMALS, INDICES, COUNT};

	GLuint id[COUNT];
};
void deleteStuff(GLuint vao, VertexBuffers vbo, GLuint program);
//Describe the setup of the Vertex Array Object
bool initVAO(GLuint vao, const VertexBuffers& vbo)
{
	glBindVertexArray(vao);		//Set the active Vertex Array

	glEnableVertexAttribArray(0);		//Tell opengl you're using layout attribute 0 (For shader input)
	glBindBuffer( GL_ARRAY_BUFFER, vbo.id[VertexBuffers::VERTICES] );		//Set the active Vertex Buffer
	glVertexAttribPointer(
		0,				//Attribute
		3,				//Size # Components
		GL_FLOAT,	//Type
		GL_FALSE, 	//Normalized?
		sizeof(vec3),	//Stride
		(void*)0			//Offset
		);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo.id[VertexBuffers::NORMALS]);
	glVertexAttribPointer(
		1,				//Attribute
		3,				//Size # Components
		GL_FLOAT,	//Type
		GL_FALSE, 	//Normalized?
		sizeof(vec3),	//Stride
		(void*)0			//Offset
		);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo.id[VertexBuffers::INDICES]);

	return !CheckGLErrors("initVAO");		//Check for errors in initialize
}


//Loads buffers with data
bool loadBuffer(const VertexBuffers& vbo, 
				const vector<vec3>& points, 
				const vector<vec3> normals, 
				const vector<unsigned int>& indices)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo.id[VertexBuffers::VERTICES]);
	glBufferData(
		GL_ARRAY_BUFFER,				//Which buffer you're loading too
		sizeof(vec3)*points.size(),		//Size of data in array (in bytes)
		&points[0],						//Start of array (&points[0] will give you pointer to start of vector)
		GL_STATIC_DRAW					//GL_DYNAMIC_DRAW if you're changing the data often
										//GL_STATIC_DRAW if you're changing seldomly
		);

	glBindBuffer(GL_ARRAY_BUFFER, vbo.id[VertexBuffers::NORMALS]);
	glBufferData(
		GL_ARRAY_BUFFER,				//Which buffer you're loading too
		sizeof(vec3)*normals.size(),	//Size of data in array (in bytes)
		&normals[0],					//Start of array (&points[0] will give you pointer to start of vector)
		GL_STATIC_DRAW					//GL_DYNAMIC_DRAW if you're changing the data often
										//GL_STATIC_DRAW if you're changing seldomly
		);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo.id[VertexBuffers::INDICES]);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(unsigned int)*indices.size(),
		&indices[0],
		GL_STATIC_DRAW
		);

	return !CheckGLErrors("loadBuffer");	
}

//Compile and link shaders, storing the program ID in shader array
GLuint initShader(string vertexName, string fragmentName)
{	
	string vertexSource = LoadSource(vertexName);		//Put vertex file text into string
	string fragmentSource = LoadSource(fragmentName);		//Put fragment file text into string

	GLuint vertexID = CompileShader(GL_VERTEX_SHADER, vertexSource);
	GLuint fragmentID = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
	
	return LinkProgram(vertexID, fragmentID);	//Link and store program ID in shader array
}

//Initialization
void initGL()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glClearColor(0.f, 0.f, 0.f, 0.f);		//Color to clear the screen with (R, G, B, Alpha)
}

bool loadUniforms(GLuint program, mat4 perspective, mat4 modelview)
{
	glUseProgram(program);

	glUniformMatrix4fv(glGetUniformLocation(program, "modelviewMatrix"),
						1,
						false,
						&modelview[0][0]);

	glUniformMatrix4fv(glGetUniformLocation(program, "perspectiveMatrix"),
						1,
						false,
						&perspective[0][0]);

	return !CheckGLErrors("loadUniforms");
}

//Draws buffers to screen
void renderLine(GLuint vao, int startElement, int numElements, GLuint program, VertexBuffers vbo, vector<vec3> points, vector<vec3>normals, vector<unsigned int> indices)
{
	
	
	glBindVertexArray(vao);		//Use the LINES vertex array
	glUseProgram(program);
	
	loadBuffer(vbo, points, normals, indices);
	
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(2.0f);
	
	glDrawElements(
			GL_LINES,		//What shape we're drawing	- GL_TRIANGLES, GL_LINES, GL_POINTS, GL_QUADS, GL_TRIANGLE_STRIP
			numElements,		//How many indices
			GL_UNSIGNED_INT,	//Type
			(void*)0			//Offset
			);
	glDisable(GL_LINE_SMOOTH);
 
	CheckGLErrors("render");
	glUseProgram(0);
	glBindVertexArray(0);


}
void renderPoints(GLuint vao, int startElement, int numElements, GLuint program, VertexBuffers vbo, vector<vec3> points, vector<vec3>normals, vector<unsigned int> indices)
{
	
	
	glBindVertexArray(vao);		//Use the LINES vertex array
	glUseProgram(program);
	
	loadBuffer(vbo, points, normals, indices);
	
	//glEnable(GL_LINE_SMOOTH);
	glPointSize(10.0f);
	
	glDrawElements(
			GL_POINTS,		//What shape we're drawing	- GL_TRIANGLES, GL_LINES, GL_POINTS, GL_QUADS, GL_TRIANGLE_STRIP
			numElements,		//How many indices
			GL_UNSIGNED_INT,	//Type
			(void*)0			//Offset
			);
	//glDisable(GL_LINE_SMOOTH);
 
	CheckGLErrors("render");
	glUseProgram(0);
	glBindVertexArray(0);


}


void generateSquare(vector<vec3>* vertices, vector<vec3>* normals, 
					vector<unsigned int>* indices, float width)
{
	vertices->push_back(vec3(-width*0.5f, -width*0.5f, 0.f));
	vertices->push_back(vec3(width*0.5f, -width*0.5f, 0.f));
	vertices->push_back(vec3(width*0.5f, width*0.5f, 0.f));
	vertices->push_back(vec3(-width*0.5f, width*0.5f, 0.f));

	normals->push_back(vec3(0.f, 0.f, 1.f));
	normals->push_back(vec3(0.f, 0.f, 1.f));
	normals->push_back(vec3(0.f, 0.f, 1.f));
	normals->push_back(vec3(0.f, 0.f, 1.f));

	//First triangle
	indices->push_back(0);
	indices->push_back(1);
	indices->push_back(2);
	//Second triangle
	indices->push_back(2);
	indices->push_back(3);
	indices->push_back(0);
}

GLFWwindow* createGLFWWindow()
{
	// initialize the GLFW windowing system
    if (!glfwInit()) {
        cout << "ERROR: GLFW failed to initialize, TERMINATING" << endl;
        return NULL;
    }
    glfwSetErrorCallback(ErrorCallback);

    // attempt to create a window with an OpenGL 4.1 core profile context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(1024, 1024, "OpenGL Example", 0, 0);
    if (!window) {
        cout << "Program failed to create GLFW window, TERMINATING" << endl;
        glfwTerminate();
        return NULL;
    }

    // set keyboard callback function and make our context current (active)
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, mousePosCallback);
    glfwSetWindowSizeCallback(window, resizeCallback);
    glfwMakeContextCurrent(window);

    return window;
}



// ==========================================================================
// PROGRAM ENTRY POINT
void printVec3(vec3 toPrint)
{
	cout << "X: " << toPrint.x << endl;
	cout << "Y: " << toPrint.y << endl;
	cout << "Z: " << toPrint.z << endl;
}


void createSpringChain(vector<Spring*> *springs, int numSprings)
{

	Spring *springRoot = new Spring(vec3(0.0f, 2.0f, 0.0f), vec3(0.0f, -2.0f, 0.0f), true, false);
	springs->push_back(springRoot);


	float setXMassB = 4.0f;
	float setYMassB = 2.0f;
	bool zigZag = false;
	Spring *springPrev = springRoot;
	for(int i = 0; i < numSprings-1; i++)
	{
		Spring *springNew = new Spring(springPrev->getMassB()->getPosition(), vec3(setXMassB , setYMassB, 0.0f), false, false);
		springNew->setMassA(springPrev->getMassB());
		springs->push_back(springNew);
		springPrev = springNew;
		setXMassB += 2.0f;
		if(zigZag)
		{
			setYMassB += 1.0f;
			zigZag = false;
		}
		else
		{
			setYMassB -= 1.0f;
			zigZag = true;
		}
	}

}

/*
 * vertices->push_back(vec3(1.0f, 1.0f, 1.f)); //0
vertices->push_back(vec3(1.0f, -1.0f, 1.f)); //1
vertices->push_back(vec3(-1.0f, -1.0f, 1.f)); //2
vertices->push_back(vec3(-1.0f, 1.0f, 1.f)); //3
vertices->push_back(vec3(-1.0f, -1.0f, -1.f)); //4
vertices->push_back(vec3(-1.0f, 1.0f, -1.f)); //5
vertices->push_back(vec3(1.0f, 1.0f, -1.f)); //6
vertices->push_back(vec3(1.0f, -1.0f, -1.f)); //7
 * */
 
 
void createCube(vector<Spring*> *springs)
{
	//Spring *springRoot = new Spring(vec3(-1.0f,1.0f,1.0f), vec3(-1.0f,-1.0f,1.0f), false,false);
	//springs->push_back(springRoot);
	
	Spring *springRoot = new Spring(vec3(1.0f,1.0f,1.0f), vec3(-1.0f,1.0f,1.0f), false,false);
	springs->push_back(springRoot);
	Spring *springTest = new Spring(springRoot->getMassA()->getPosition(), vec3(1.0f,-1.0f,1.0f), false,false);
	springTest->setMassA(springRoot->getMassA());
	springs->push_back(springTest);
	Spring *springTest2 = new Spring(vec3(-1.0f,-1.0f,1.0f), springTest->getMassB()->getPosition(), false,false);
	springTest2->setMassB(springTest->getMassB());
	springs->push_back(springTest2);
	Spring *springTest3 = new Spring(springTest2->getMassA()->getPosition(), springRoot->getMassB()->getPosition(), false,false);
	springTest3->setMassA(springTest2->getMassA());
	springTest3->setMassB(springRoot->getMassB());
	springs->push_back(springTest3);
	
	Spring *springCross = new Spring (springRoot->getMassA()->getPosition(), springTest2->getMassA()->getPosition(), false, false);
	springCross->setMassA(springRoot->getMassA());
	springCross->setMassB(springTest2->getMassA());
	springs->push_back(springCross);
	
	Spring *springCross2 = new Spring (springRoot->getMassB()->getPosition(), springTest2->getMassB()->getPosition(), false, false);
	springCross2->setMassA(springRoot->getMassB());
	springCross2->setMassB(springTest2->getMassB());
	springs->push_back(springCross2);
	
} 



void createJelloCube(vector<Spring*> *springs)
{
	
	int numSpringsPerFace = 0;
	Spring *springNew;

	/*Creating the Top row of the cube*/

	Spring *springRoot = new Spring(vec3(1.0f, 1.0f, 1.0f), vec3(0.0f, 1.0f, 1.0f), false, false);
	springs->push_back(springRoot);
	Spring *springPrev = springRoot;
	float moveMassBX = -1.0f;
	for(int i = 0; i < numSpringsPerFace; i++)
	{
		springNew = new Spring(springPrev->getMassB()->getPosition(), vec3(moveMassBX, 1.0f, 1.0f), false, false);
		springNew->setMassA(springPrev->getMassB());
		springs->push_back(springNew);
		springPrev = springNew;
		moveMassBX -= 1.0f;
		
	}

	/* Create Left column*/
	

	float currSpotX = springPrev->getMassB()->getPosition().x;
	//Spring *springYUp = new Spring(vec3(currSpotX, currSpotY, 1.0f), vec3(currSpotX, currSpotY+2.0f, 1.0f), false, false);
	Spring *springYUp = new Spring(springPrev->getMassB()->getPosition(), vec3(currSpotX, 0.0f, 1.0f), false, false);
	springYUp->setMassA(springPrev->getMassB());
	//springYUp->getMassA()->setIsFixed(true);
	springs->push_back(springYUp);
	springPrev = springYUp;
	float moveMassBY = -1.0f;
	//y = springPrev->getMassB()->getPosition().y;
	for(int i = 0; i < numSpringsPerFace; i++)
	{
		
		springNew = new Spring(springPrev->getMassB()->getPosition(), vec3(currSpotX, moveMassBY, 1.0f), false, false);
		springNew->setMassA(springPrev->getMassB());
		springs->push_back(springNew);
		springPrev = springNew;
		moveMassBY -= 1.0f; 
	}
	Spring *springBotLeft = springPrev;

	/* Creating right column of the cube*/

	//Spring *springY = new Spring(vec3(1.0f, 1.0f, 1.0f), vec3(1.0f, -1.0f, 1.0f), false, false);
	Spring *springY = new Spring(springRoot->getMassA()->getPosition(), vec3(1.0f, 0.0f, 1.0f), false, false);
	springY->setMassA(springRoot->getMassA());
	//springY->getMassA()->setIsFixed(true);
	springs->push_back(springY);
	springPrev = springY;
	moveMassBY = -1.0f;
	for(int i = 0; i < numSpringsPerFace; i++)
	{
		springNew = new Spring(springPrev->getMassB()->getPosition(), vec3(1.0f, moveMassBY, 1.0f), false, false);
		springNew->setMassA(springPrev->getMassB());
		springs->push_back(springNew);
		springPrev = springNew;
		moveMassBY -= 1.0f; 
	}
	
	/* Creating the bottom row of the cube*/

	float currSpotY = springPrev->getMassB()->getPosition().y;//-1.0 - ((numSpringsPerFace-2) * 2.0f);
	//Spring *springXY = new Spring(vec3(1.0f, currSpotY, 1.0f), vec3(-1.0f, currSpotY, 1.0f), false, false);
	Spring *springXY = new Spring(springPrev->getMassB()->getPosition(), vec3(0.0f, currSpotY, 1.0f), false, false);
	springXY->setMassA(springPrev->getMassB());
	//springXY->getMassA()->setIsFixed(true);
	springs->push_back(springXY);
	springPrev = springXY;
	float moveMassBXY = -1.0f;
	//float y = springPrev->getMassB()->getPosition().y;
	for(int i = 0; i < numSpringsPerFace; i++)
	{
		springNew = new Spring(springPrev->getMassB()->getPosition(), vec3(moveMassBXY, currSpotY, 1.0f), false, false);
		springNew->setMassA(springPrev->getMassB());
		if(i+1 == numSpringsPerFace)
		{
			springNew->setMassB(springBotLeft->getMassB());
		}
			
		springs->push_back(springNew);
		springPrev = springNew;
		moveMassBXY -= 1.0f; 
	}
	/*
	Spring *springFinish = new Spring (springPrev->getMassB()->getPosition() , springBotLeft->getMassB()->getPosition(), false, false);
	springFinish->setMassA(springPrev->getMassB());
	springFinish->setMassB(springBotLeft->getMassB());
	//springFinish->getMassB()->setIsFixed(true);
	springs->push_back(springFinish);
	*/
}

void setupDraw(vector<vec3> *mass, vector<unsigned int> *massInds, vector<vec3> *massColor, 
vector<vec3> *spring, vector<unsigned int> *springsInd, vector<vec3> *springColor, Spring *springOne)
{

	//if(!springOne->getMassA()->isDrawn())
	//{
		
		spring->push_back(springOne->getMassA()->getPosition());
		springsInd->push_back(springsInd->size());
		springColor->push_back(vec3(0.0f, 1.0f, 1.0f));

		mass->push_back(springOne->getMassA()->getPosition());
		massInds->push_back(massInds->size());
		massColor->push_back(vec3(1.0f, 1.0f, 1.0f));
		
		
		
		springOne->getMassA()->setIsDrawn(true);

	
	//}
//	if(!springOne->getMassB()->isDrawn())
	//{
		spring->push_back(springOne->getMassB()->getPosition());
		springsInd->push_back(springsInd->size());
		springColor->push_back(vec3(0.0f, 1.0f, 1.0f));
		
		mass->push_back(springOne->getMassB()->getPosition());
		massInds->push_back(massInds->size());
		massColor->push_back(vec3(1.0f, 1.0f, 1.0f));
		
		springOne->getMassB()->setIsDrawn(true);
//	}
}

/*
void createSpringBox(vector<vec3> box, vector<unsigned int> indices, vector<Spring*> *springs, vector<Mass*> *masses)
{
	//vector<Mass*> masses;
	for(int i = 0; i < box.size(); i++)
	{
		Mass *massNew = new Mass();
		massNew->setPosition(box[i]);
		masses->push_back(massNew);
	}
	int ind1, ind2;
	for(unsigned int i = 0; i < indices.size()-1; i++)
	{
		ind1 = indices[i];
		ind2 = indices[i+1];
		
		Spring* springNew = new Spring(masses->at(ind1),masses->at(ind2), false, false);
		//springNew->setMassA(masses->at(ind1));
		//springNew->setMassB(masses->at(ind2));
		springs->push_back(springNew);
	}
}
* */
void createClothSprings(vector<vec3> cloth, vector<unsigned int> inds, vector<Spring*> *springs, vector<Mass*> *masses)
{
	
	for(int i = 0; i < cloth.size(); i++)
	{
		Mass *massNew = new Mass();
		massNew->setPosition(cloth[i]);
		massNew->setNewPos(cloth[i]);
		if(i == 0 || i == 2)
			massNew->setIsFixed(true);
		masses->push_back(massNew);
	}
	int ind1, ind2;
	for(unsigned int i = 0; i < inds.size(); i+=2)
	{
		ind1 = inds[i];
		ind2 = inds[i+1];
		
		Spring* springNew = new Spring(cloth[ind1],cloth[ind2], false, false);
		springNew->setMassA(masses->at(ind1));
		springNew->setMassB(masses->at(ind2));
		//springNew->getMassA()->setIsFixed(true);
		springs->push_back(springNew);
	}
	
}
void createCloth(vector<vec3> *cloth, vector<vec3> *normals, vector<unsigned int> *indices)
{
	cloth->push_back(vec3(1.0f,1.0f,1.0f));
	cloth->push_back(vec3(1.0f,-1.0f,1.0f));
	cloth->push_back(vec3(-1.0f,1.0f,1.0f));
	cloth->push_back(vec3(-1.0f,-1.0f,1.0f));
	
	
	
	indices->push_back(0);
	indices->push_back(1);
	
	indices->push_back(0);
	indices->push_back(2);
	
	indices->push_back(1);
	indices->push_back(3);
	
	indices->push_back(2);
	indices->push_back(3);
	
	normals->push_back(vec3(1.0f,1.0f,1.0f));
	normals->push_back(vec3(1.0f,1.0f,1.0f));
	normals->push_back(vec3(1.0f,1.0f,1.0f));
	normals->push_back(vec3(1.0f,1.0f,1.0f));
}

void createBox(vector<vec3> *box, vector<vec3> *normals, vector<unsigned int> *indices)
{
	box->push_back(vec3(1.0f, 1.0f, 1.f)); //0
	box->push_back(vec3(1.0f, -1.0f, 1.f)); //1
	box->push_back(vec3(-1.0f, -1.0f, 1.f)); //2
	box->push_back(vec3(-1.0f, 1.0f, 1.f)); //3
	box->push_back(vec3(-1.0f, -1.0f, -1.f)); //4
	box->push_back(vec3(-1.0f, 1.0f, -1.f)); //5
	box->push_back(vec3(1.0f, 1.0f, -1.f)); //6
	box->push_back(vec3(1.0f, -1.0f, -1.f)); //7
	
	indices->push_back(0);
	indices->push_back(1);
	
	indices->push_back(0);
	indices->push_back(6);
	
	indices->push_back(0);
	indices->push_back(7);
	
	indices->push_back(0);
	indices->push_back(3);
	
	indices->push_back(0);
	indices->push_back(5);
	
	indices->push_back(0);
	indices->push_back(2);
	
	indices->push_back(1);
	indices->push_back(2);
	
	indices->push_back(1);
	indices->push_back(4);
	
	indices->push_back(1);
	indices->push_back(7);
	
	indices->push_back(1);
	indices->push_back(6);
	
	indices->push_back(1);
	indices->push_back(3);
	
	indices->push_back(2);
	indices->push_back(3);
	
	indices->push_back(2);
	indices->push_back(4);
	
	indices->push_back(2);
	indices->push_back(5);
	
	indices->push_back(2);
	indices->push_back(7);
	
	indices->push_back(3);
	indices->push_back(5);
	
	//indices->push_back(3);
	//indices->push_back(6);
	
	indices->push_back(3);
	indices->push_back(4);
	
	indices->push_back(4);
	indices->push_back(5);
	
	indices->push_back(4);
	indices->push_back(6);
	
	indices->push_back(4);
	indices->push_back(7);
	
	indices->push_back(5);
	indices->push_back(7);
	
	indices->push_back(5);
	indices->push_back(6);
	
	indices->push_back(6);
	indices->push_back(7);
	
	

	/*colors of each point*/

	normals->push_back(vec3(1.0f, 0.0f, 0.f));
	normals->push_back(vec3(0.0f, 1.0f, 0.f));
	normals->push_back(vec3(0.0f, 0.0f, 1.f));
	normals->push_back(vec3(1.0f, 1.0f, 0.f));

	normals->push_back(vec3(1.0f, 0.0f, 1.f));
	normals->push_back(vec3(0.0f, 1.0f, 1.f));
	normals->push_back(vec3(0.5f, 0.5f, 0.5f));
	normals->push_back(vec3(0.0f, 0.0f, 0.f));
}

int main(int argc, char *argv[])
{   
    window = createGLFWWindow();
    if(window == NULL)
    	return -1;

    //Initialize glad
    if (!gladLoadGL())
	{
		cout << "GLAD init failed" << endl;
		return -1;
	}

    // query and print out information about our OpenGL environment
    QueryGLVersion();

	initGL();

	//Initialize shader
	GLuint program = initShader("vertex.glsl", "fragment.glsl");

	GLuint vao;
	VertexBuffers vbo;

	//Generate object ids
	glGenVertexArrays(1, &vao);
	glGenBuffers(VertexBuffers::COUNT, vbo.id);

	initVAO(vao, vbo);

	//Geometry information
	vector<vec3> points, normals;
	vector<unsigned int> indices;

	generateSquare(&points, &normals, &indices, 1.f);

	vector<Spring*> multipleSprings;

	Camera cam = Camera(vec3(0, 0, -1), vec3(0, 0, 20));
	activeCamera = &cam;
	//float fovy, float aspect, float zNear, float zFar
	mat4 perspectiveMatrix = perspective(radians(80.f), 1.f, 0.1f, 400.f);

	//Spring spring = Spring(vec3(0.0f, 5.0f, 0.0f), vec3(0.0f, -5.0f, 0.0f));
	int numSprings = 6;
/*
	
	if(springChain)
		createSpringChain(&multipleSprings, numSprings);
	else
		createCube(&multipleSprings);
		//createJelloCube(&multipleSprings);
	*/
	float dt = 0.01f;
	float time = 0.0f;

	vec3 gravity = vec3(0.0f, -9.81f, 0.0f);
    vec3 force;
    mat4 moveObj;
    
    vector<vec3> masses;
    vector<unsigned int> massInd;
    vector<vec3> colorMass;

    
    vector<vec3> springs;
    vector<unsigned int> springInd;
    vector<vec3> colorSpring;
    
    
    vector<vec3> massFixed;
    vector<unsigned int> massFixedInd;
    vector<vec3> colorMassFixed;
    
    vector<vec3> box;
    vector<vec3> boxColor;
    vector<unsigned int> boxInds;

    vector<vec3> cloth;
    vector<vec3> clothColor;
    vector<unsigned int> clothInds;
    
    vector<Mass*> massObjs;
    float minHeight = -20.0f;
	float extraTime = 0.0f;

    // run an event-triggered main loop

    while (!glfwWindowShouldClose(window))
    {
		if(!initSpringSys)
		{
			masses.clear();
			massInd.clear();
			colorMass.clear();
	
				
			springs.clear();
			springInd.clear();
			colorSpring.clear();
			
			
			massFixed.clear();
			massFixedInd.clear();
			colorMassFixed.clear();
			
			multipleSprings.clear();	
			massObjs.clear();	
			
			cloth.clear();
			clothInds.clear();
			
			switch(scene)
			{
				case 0:
					createSpringChain(&multipleSprings, 1);
				break;
				
				case 1:
					createSpringChain(&multipleSprings, numSprings);
				break;
				
				case 2:
				{
					createBox(&box, &boxColor, &boxInds);
					createCube(&multipleSprings);
					for(int i = 0; i < multipleSprings.size(); i++)
					{
						multipleSprings[i]->getMassA()->setIsCube(true);
						multipleSprings[i]->getMassB()->setIsCube(true);
					}
					//createSpringBox(box, boxInds, &multipleSprings, &massObjs);
				}
				//	createCube(&multipleSprings);
					//createJelloCube(&multipleSprings);
				
				break;
				case 3:
				{
					createCloth(&cloth, &clothColor, &clothInds);
					createClothSprings(cloth, clothInds, &multipleSprings, &massObjs);
				}
				break;
				
				default:
				break;
				
			}
			/*
			if(springChain)
			{
				multipleSprings.clear();
				createSpringChain(&multipleSprings, numSprings);
			
			}
			else
			{
				multipleSprings.clear();
				createCube(&multipleSprings);
				
			}
			* */
			initSpringSys = true;
			for(int i = 0; i < multipleSprings.size(); i++)
			{
				setupDraw(&masses, &massInd, &colorMass, &springs, &springInd, &colorSpring, multipleSprings[i]);

			}
		}
		//createJelloCube(&multipleSprings);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//Clear color and depth buffers (Haven't covered yet)
		dt = 0.003f;
		dt += extraTime;
	
		if(play)
			{
				float timeStep = 1.0f / 1000.0f;
				
				while (dt >= timeStep)
					{
						masses.clear();
						massInd.clear();
						colorMass.clear();

						
						springs.clear();
						springInd.clear();
						colorSpring.clear();
						
						
						massFixed.clear();
						massFixedInd.clear();
						colorMassFixed.clear();
						
						force = gravity;
						for( int i = 0; i < multipleSprings.size(); i++)
						{
							multipleSprings[i]->zeroForce();
						}
						for(int j = 0; j < multipleSprings.size(); j++)
						{
							multipleSprings[j]->applyForce(force, dt);
							//setupDraw(&masses, &massInd, &colorMass, &springs, &springInd, &colorSpring, multipleSprings[j]);
						}
						for(int k = 0; k < multipleSprings.size(); k++)
						{
							
							if(!multipleSprings[k]->getMassA()->getCalced())
							{
								multipleSprings[k]->getMassA()->setForce((multipleSprings[k]->getMassA()->getForce() + gravity*multipleSprings[k]->getMassA()->getMass()));
								multipleSprings[k]->getMassA()->resolveForces(dt);
								//multipleSprings[k]->getMassA()->setCalced(true);	
							}
							if(!multipleSprings[k]->getMassB()->getCalced())
							{
								multipleSprings[k]->getMassB()->setForce((multipleSprings[k]->getMassB()->getForce() + gravity*multipleSprings[k]->getMassB()->getMass()));
								multipleSprings[k]->getMassB()->resolveForces(dt);
								//multipleSprings[k]->getMassB()->setCalced(true);
							}
								
						}
					//	cout << multipleSprings.size() << endl;
						for( int i = 0; i < multipleSprings.size(); i++)
						{
							//printVec3(multipleSprings[i]->getMassB()->getForce());
							//printVec3(multipleSprings[i]->getMassA()->getForce());
							multipleSprings[i]->getMassA()->setPosition(multipleSprings[i]->getMassA()->getNewPos());
							multipleSprings[i]->getMassB()->setPosition(multipleSprings[i]->getMassB()->getNewPos());
							
							setupDraw(&masses, &massInd, &colorMass, &springs, &springInd, &colorSpring, multipleSprings[i]);
							//cout << masses.size() << endl;
							multipleSprings[i]->unCalced(); 
						}
					
						/*Add the forces being appleied to the masses here*/
						/*Apply gravity to each individual mass once then resolve forces*/
						
						/*------------------------------------------------*/
					
						extraTime = dt;
						dt -= timeStep;
				
					
					}
			}
					
		//	cout << "Spring Inds Size: " << springInd.size() << endl;		
			loadUniforms(program, winRatio*perspectiveMatrix*cam.getMatrix(), mat4(1.0f));
			renderLine(vao, 0, springInd.size(), program, vbo, springs, colorSpring, springInd); 

			loadUniforms(program, winRatio*perspectiveMatrix*cam.getMatrix(), mat4(1.0f));
			renderPoints(vao, 0, massInd.size(), program, vbo, masses, colorMass, massInd);
			
		//	loadUniforms(program, winRatio*perspectiveMatrix*cam.getMatrix(), mat4(1.0f));
		//	renderLine(vao, 0, clothInds.size(), program, vbo, cloth, clothColor, clothInds);
			
			//glfwSwapInterval(1);
			glfwSwapBuffers(window);// scene is rendered to the back buffer, so swap to front for display
			glfwPollEvents(); // sleep until next event before drawing again
			if(play)
			{
				masses.clear();
				massInd.clear();
				colorMass.clear();

				
				springs.clear();
				springInd.clear();
				colorSpring.clear();
				
				
				massFixed.clear();
				massFixedInd.clear();
				colorMassFixed.clear();
			}
		
	}

	// clean up allocated resources before exit
	
	deleteStuff(vao, vbo, program);
	
	return 0;
}

/*
        */

// ==========================================================================
// SUPPORT FUNCTION DEFINITIONS

// --------------------------------------------------------------------------
// OpenGL utility functions

void deleteStuff(GLuint vao, VertexBuffers vbo, GLuint program)
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(VertexBuffers::COUNT, vbo.id);
	glDeleteProgram(program);
	

	glfwDestroyWindow(window);
	glfwTerminate();
}
void QueryGLVersion()
{
    // query opengl version and renderer information
    string version  = reinterpret_cast<const char *>(glGetString(GL_VERSION));
    string glslver  = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
    string renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));

    cout << "OpenGL [ " << version << " ] "
         << "with GLSL [ " << glslver << " ] "
         << "on renderer [ " << renderer << " ]" << endl;
}

bool CheckGLErrors(string location)
{
    bool error = false;
    for (GLenum flag = glGetError(); flag != GL_NO_ERROR; flag = glGetError())
    {
        cout << "OpenGL ERROR:  ";
        switch (flag) {
        case GL_INVALID_ENUM:
            cout << location << ": " << "GL_INVALID_ENUM" << endl; break;
        case GL_INVALID_VALUE:
            cout << location << ": " << "GL_INVALID_VALUE" << endl; break;
        case GL_INVALID_OPERATION:
            cout << location << ": " << "GL_INVALID_OPERATION" << endl; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            cout << location << ": " << "GL_INVALID_FRAMEBUFFER_OPERATION" << endl; break;
        case GL_OUT_OF_MEMORY:
            cout << location << ": " << "GL_OUT_OF_MEMORY" << endl; break;
        default:
            cout << "[unknown error code]" << endl;
        }
        error = true;
    }
    return error;
}

// --------------------------------------------------------------------------
// OpenGL shader support functions

// reads a text file with the given name into a string
string LoadSource(const string &filename)
{
    string source;

    ifstream input(filename.c_str());
    if (input) {
        copy(istreambuf_iterator<char>(input),
             istreambuf_iterator<char>(),
             back_inserter(source));
        input.close();
    }
    else {
        cout << "ERROR: Could not load shader source from file "
             << filename << endl;
    }

    return source;
}

// creates and returns a shader object compiled from the given source
GLuint CompileShader(GLenum shaderType, const string &source)
{
    // allocate shader object name
    GLuint shaderObject = glCreateShader(shaderType);

    // try compiling the source as a shader of the given type
    const GLchar *source_ptr = source.c_str();
    glShaderSource(shaderObject, 1, &source_ptr, 0);
    glCompileShader(shaderObject);

    // retrieve compile status
    GLint status;
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint length;
        glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
        string info(length, ' ');
        glGetShaderInfoLog(shaderObject, info.length(), &length, &info[0]);
        cout << "ERROR compiling shader:" << endl << endl;
        cout << source << endl;
        cout << info << endl;
    }

    return shaderObject;
}

// creates and returns a program object linked from vertex and fragment shaders
GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader)
{
    // allocate program object name
    GLuint programObject = glCreateProgram();

    // attach provided shader objects to this program
    if (vertexShader)   glAttachShader(programObject, vertexShader);
    if (fragmentShader) glAttachShader(programObject, fragmentShader);

    // try linking the program with given attachments
    glLinkProgram(programObject);

    // retrieve link status
    GLint status;
    glGetProgramiv(programObject, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint length;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &length);
        string info(length, ' ');
        glGetProgramInfoLog(programObject, info.length(), &length, &info[0]);
        cout << "ERROR linking shader program:" << endl;
        cout << info << endl;
    }

    return programObject;
}


// ==========================================================================
