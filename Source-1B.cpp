//********************************
//Αυτό το αρχείο θα το χρησιμοποιήσετε
// για να υλοποιήσετε την άσκηση 1Β της OpenGL
//
//ΑΜ:4723   Όνομα:ELEFTHERIOS-MARIOS MANIKAS
//ΑΜ:4829   Όνομα:STEFANOS FOTOPOULOS

//*********************************

// Include standard headers
#include <stdio.h>
#include <ctime>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
using namespace std;

//************************
// Βοηθητικές συναρτήσεις


float zoom = 45.0;//ftiaxame variable float zoom gia na kratisoume to arxiko radians tou ProjectionMatrix kai na to peirazoume analogws
glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}


///****************************************************************
//  Εδω θα υλοποιήσετε την συνάρτηση της κάμερας
//****************************************************************

void camera_function()
{
	// STRAFE STON AXONA X  UP
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		ViewMatrix = glm::rotate(ViewMatrix, radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	//STRAFE STON AXONA X Down 
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		ViewMatrix = glm::rotate(ViewMatrix, radians(1.0f), glm::vec3(-1.0f, 0.0f, 0.0f));

	}
	//STRAFE STON AXONA Z  Dexiostrofa
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		ViewMatrix = glm::rotate(ViewMatrix, radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	//STRAFE STON AXONA Z Aristerostrofa
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		ViewMatrix = glm::rotate(ViewMatrix, radians(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	}
	// ZOOM IN
	if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
		zoom = zoom - 0.1;
		if (zoom >= 0.1)
		{
			ProjectionMatrix = glm::perspective(glm::radians(zoom), float(1000 / 1000), 0.1f, 100.0f);
		}
	}
	//ZOOM OUT 
	if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
	{
		zoom = zoom + 0.1;
		if (zoom <= 100.0)
		{
			ProjectionMatrix = glm::perspective(glm::radians(zoom), float(1000 / 1000), 0.1f, 100.0f);
		}
	}
}


//************************************
// Η LoadShaders είναι black box για σας
//************************************

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}


int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1000, 1000, u8"Εργασία 1Β – Τραπεζοειδές Πρίσμα", NULL, NULL);

	float h = 0;
	float min = 2.0;
	float max = 10.0;
	srand((unsigned)time(NULL));
	h = (min)+(((float)rand()) / (float)RAND_MAX) * (max - min);
	cout << h;

	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the space key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//background color
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);//0.4 dark blue
	glEnable(GL_DEPTH_TEST);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders

	GLuint programID = LoadShaders("ProjBVertexShader.vertexshader", "ProjBFragmentShader.fragmentshader");

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");


	//********************
	// **Δείγμα κλήσης των συναρτήσεων για δημιουργία του MVP - είναι τυχαίες οι ρυθμίσεις και δεν ανταποκρίνονται στην άσκησή σας

	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), float(1000 / 1000), 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(10.0f, 50.0f, 0.0f),//topothesia kameras erotimatos 
		glm::vec3(0.0f, 0.0f, 0.0f),// kentro 
		glm::vec3(0.0f, 0.0f, 1.0f)//anion dianysma
	);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around
	ViewMatrix = View;//thetw to ViewMatrix = View gia na mporw na xrhsimopoihsw thn getViewMatrix() sthn camera_function()
	ProjectionMatrix = Projection;//thetw to ProjectionMatrix = Projection gia na mporw na xrhsimopoihsw thn getProjectionMatrix() sthn camera_function()
	//syntetagmenes koryfwn prismatos
	static const GLfloat g_vertex_buffer_data[] = {

		//anw vasi me z=h/2
		-1.0f,3.0f,h / 2,
		1.0f,3.0f,h / 2,
		4.0f,-3.0f,h / 2,

		-1.0f,3.0f,h / 2,
		-4.0f,-3.0f,h / 2,
		4.0f,-3.0f,h / 2,
		//katw vasi me z=-h/2
		-1.0f,3.0f,-h / 2,
		1.0f,3.0f,-h / 2,
		4.0f,-3.0f,-h / 2,

		-1.0f,3.0f,-h / 2,
		-4.0f,-3.0f,-h / 2,
		4.0f,-3.0f,-h / 2,

		//messaio orthogwnio
		 -4.0f,-3.0f, h / 2,// h/2
		 4.0f,-3.0f, h / 2,// h/2
		 4.0f,-3.0f, -h / 2,// -h/2

		 -4.0f,-3.0f, h / 2,// h/2
		 4.0f,-3.0f,-h / 2,// -h/2
		 -4.0f,-3.0f, -h / 2,// -h/2

		 //aristero orthogwnio
		 -4.0f,-3.0f, h / 2,// h/2
		 -1.0f,3.0f, h / 2,// h/2
		 -4.0f,-3.0f, -h / 2,// -h/2

		 -1.0f,3.0f, h / 2,// h/2
		 -1.0f,3.0f, -h / 2,// -h/2
		 -4.0f,-3.0f, -h / 2,// -h/2
		 //deksi orthogwnio
		  4.0f,-3.0f, h / 2,// h/2
		  1.0f,3.0f, -h / 2,// -h/2
		  4.0f,-3.0f, -h / 2,// -h/2

		  1.0f,3.0f, h / 2,// h/2
		  1.0f,3.0f, -h / 2,// -h/2
		  4.0f,-3.0f, h / 2,// h/2
		  //mprostino orthogwnio
		  -1.0f,3.0f, h / 2,// h/2
		  1.0f,3.0f, h / 2,// h/2
		  1.0f,3.0f, -h / 2,// -h/2

		  -1.0f,3.0f, h / 2,// h/2
		  -1.0f,3.0f,-h / 2,// -h/2
		  1.0f,3.0f, -h / 2,// -h/2


	};
	// One color for each vertex. They were generated by us.
	static const GLfloat g_color_buffer_data[] = {
		1.0f,  0.0f,  1.0f,
		1.0f,  0.0f,  1.0f,
		1.0f,  0.0f,  1.0f,

		1.0f,  0.0f,  1.0f,
		1.0f,  0.0f,  1.0f,
		1.0f,  0.0f,  1.0f,

		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,

		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,

		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,

		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,

		0.0f,  0.5f,  1.0f,
		0.0f,  0.5f,  1.0f,
		0.0f,  0.5f,  1.0f,

		0.0f,  0.5f,  1.0f,
		0.0f,  0.5f,  1.0f,
		0.0f,  0.5f,  1.0f,

		0.0f,  1.0f,  0.5f,
		0.0f,  1.0f,  0.5f,
		0.0f,  1.0f,  0.5f,

		0.0f,  1.0f,  0.5f,
		0.0f,  1.0f,  0.5f,
		0.0f,  1.0f,  0.5f,

		1.5f,  0.5f,  0.5f,
		1.5f,  0.5f,  0.5f,
		1.5f,  0.5f,  0.5f,

		1.5f,  0.5f,  0.5f,
		1.5f,  0.5f,  0.5f,
		1.5f,  0.5f,  0.5f,

	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		//*************************************************
		// Να προστεθεί κώδικας για τον υπολογισμό του νέο MVP
		camera_function();//Kalw thn camera_function() h opoia allazei to ViewMatrix  & to projectionMatrix 
		View = getViewMatrix();//eisagw sthn view to allagmeno ViewMatrix an patithike kapoio apo ta apodekta plhktra
		Projection = getProjectionMatrix(); //eisagw sthn Projection to allagmeno ProjectionMatrix an patithike kapoio apo ta apodekta plhktra
		
		
		MVP = Projection * View * Model;// ypologizw to kainourgio mvp symfwna me ta kainourgia dedomena 

		// **************************************************/
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);


		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		
		//Scaling
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)//elegxos gia to patima koumpiou
		{
			Model = glm::scale(Model, glm::vec3(1.0f, 1.0f, 1.2f));//kanw scale ton monadiaio pinaka Model me to vector 3(x,y,z) 
		}
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)//elegxos gia to patima koumpiou
		{
			Model = glm::scale(Model, glm::vec3(1.0f, 1.0f, 0.8f));//kanw scale ton monadiaio pinaka Model me to vector 3(x,y,z)
		}
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 3 indices starting at 0 -> 1 triangle


		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();


	} 
	// Check if the SPACE key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
