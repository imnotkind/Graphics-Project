#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#define _CRT_SECURE_NO_WARNINGS

// Include all GLM core / GLSL features
#include <glm/glm.hpp> // vec2, vec3, mat4, radians

// Include all GLM extensions
#include <glm/ext.hpp> // perspective, translate, rotate
#include <glm/gtc/matrix_transform.hpp>
#include "OBJ_Loader.h"
#include "objloader.hpp"


#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "Opengl32.lib")


using namespace std;
using namespace glm;

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>

GLuint VertexShaderID;
GLuint FragmentShaderID;

GLuint programID;
GLuint vertexLoc;
GLuint colorLoc;


GLuint vertexbuffer;
GLuint vertexbuffer2;
GLuint vertexbuffer3;

GLuint VertexArrayID;
GLuint VertexArrayID2;
GLuint VertexArrayID3;

GLuint MatrixID;

// Read our .obj file
std::vector<glm::vec3> vertices;
std::vector<glm::vec2> uvs;
std::vector<glm::vec3> normals; // Won't be used at the moment.

objl::Loader loader;

int meshcount = 0;

set<int> group_info[1] = {
	{ 0, 1, 2, 3, 4, 5, 6, 7 },
};

vec3 group_translation[1] = {
	{ 0,0, 0 }
};

pair<float, vec3> group_rotation[1] = {
	{ 0,{ 1,0,0 } }
};

int parent_info[1] = {
	-1
};

pair<float, vec3> parent_rotation[1] = {
	{ 0,{ 0,0,1 } }
};

vec3 parent_translation[1] = {
	{ 0,0,0 }
};


GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {

	// Create the shaders
	VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
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
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
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
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
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

	glBindFragDataLocation(ProgramID, 0, "color");
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





void reshape1(int w, int h) {
	cout << "reshapefunc1" << endl;
	glViewport(0, 0, w, h);
}

void idle1()
{
	glutPostRedisplay();
}


void display1() {

	static double k = 0.0;
	k += 0.01;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(programID);



	glm::mat4 Projection = glm::perspective(glm::radians(100.0f), 1.0f, 0.1f, 1000.0f);
	glm::mat4 View = glm::lookAt(glm::vec3(30, 30, 30), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 mvp = Projection * View * Model;

	glm::mat4 iden = glm::mat4(1.0f);


	//y axis
	Model = glm::mat4(1.0f);
	Model = glm::scale(Model, glm::vec3(1.0f, 1000.0f, 1.0f));
	mvp = Projection * View * Model;

	glBindVertexArray(VertexArrayID2);
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
	glUniform4f(colorLoc, 0.0, 1.0, 0.0, 1.0);
	glDrawArrays(GL_LINES, 0, 2);

	//z axis
	Model = glm::mat4(1.0f);
	Model = glm::rotate(Model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	Model = glm::scale(Model, glm::vec3(1.0f, 1000.0f, 1.0f));
	mvp = Projection * View * Model;

	glBindVertexArray(VertexArrayID2);
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
	glUniform4f(colorLoc, 0.0, 0.0, 1.0, 1.0);
	glDrawArrays(GL_LINES, 0, 2);

	//x axis
	Model = glm::mat4(1.0f);
	Model = glm::rotate(Model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	Model = glm::scale(Model, glm::vec3(1.0f, 1000.0f, 1.0f));
	mvp = Projection * View * Model;

	glBindVertexArray(VertexArrayID2);
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
	glUniform4f(colorLoc, 1.0, 0.0, 0.0, 1.0);
	glDrawArrays(GL_LINES, 0, 2);

	//our object
	Model = glm::mat4(1.0f);
	//Model = glm::scale(glm::mat4(1.0f), glm::vec3(10, 10, 10));
	mvp = Projection * View * Model;

	glBindVertexArray(VertexArrayID3);
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
	glUniform4f(colorLoc, 1.0, 1.0, 0.0, 1.0);
	glDrawArrays(GL_LINES, 0, vertices.size());

	/*
	Model = glm::scale(glm::mat4(1.0f), glm::vec3(10, 10, 10));
	mvp = Projection * View * Model;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
	glUniform4f(colorLoc, 1.0, 0.0, 0.0, 1.0);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	*/





	glutSwapBuffers();
}

void translatetoorigin(int p)
{
	

	set<int> a = group_info[p];

	for (int q : a)
	{
		objl::Mesh m = loader.LoadedMeshes[q];
		cout << m.MeshName << endl;

		glm::mat4 M = glm::mat4(1.0f);
		M = glm::rotate(M, glm::radians(group_rotation[p].first), group_rotation[p].second);
		M = glm::translate(M, group_translation[p]);

		for (unsigned int t : m.Indices)
		{
			objl::Vertex v = m.Vertices[t];
			glm::vec4 gv;
			gv.x = v.Position.X;
			gv.y = v.Position.Y;
			gv.z = v.Position.Z;
			gv.w = 1;

			vertices.push_back(M * gv);
		}
	}

}

void translatetoparent(int p)
{
	set<int> a = group_info[p];

	for (int q : a)
	{
		objl::Mesh m = loader.LoadedMeshes[q];
		cout << m.MeshName << endl;

		glm::mat4 P = glm::mat4(1.0f);
		P = glm::translate(P, parent_translation[p]);
		P = glm::rotate(P, glm::radians(parent_rotation[p].first), parent_rotation[p].second);

		glm::mat4 M = glm::mat4(1.0f);
		M = glm::rotate(M, glm::radians(group_rotation[p].first), group_rotation[p].second);
		M = glm::translate(M, group_translation[p]);

		for (unsigned int t : m.Indices)
		{
			objl::Vertex v = m.Vertices[t];
			glm::vec4 gv;
			gv.x = v.Position.X;
			gv.y = v.Position.Y;
			gv.z = v.Position.Z;
			gv.w = 1;

			vertices.push_back(P * M * gv);
		}
	}
}


void init_shader(int p)
{
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};

	static const GLfloat g_vertex_buffer_data2[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

	};


	programID = LoadShaders("ver.glsl", "frag.glsl");

	MatrixID = glGetUniformLocation(programID, "trans");
	vertexLoc = glGetAttribLocation(programID, "position");
	colorLoc = glGetUniformLocation(programID, "vicolor");

	//https://stackoverflow.com/questions/45860198/glgenvertexarrays-and-glgenbuffers-arguments
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);



	glGenVertexArrays(1, &VertexArrayID2);
	glBindVertexArray(VertexArrayID2);
	glGenBuffers(1, &vertexbuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data2), g_vertex_buffer_data2, GL_STATIC_DRAW);



	glBindVertexArray(VertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glEnableVertexAttribArray(vertexLoc);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(VertexArrayID2);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
	glEnableVertexAttribArray(vertexLoc);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


	vertices.clear();

	if (p == -1)
	{
		for (int i = 0; i < 8; i++)
		{
			objl::Mesh m = loader.LoadedMeshes[i];
			cout << m.MeshName << endl;
			for (unsigned int t : m.Indices)
			{
				objl::Vertex v = m.Vertices[t];
				glm::vec3 gv;
				gv.x = v.Position.X;
				gv.y = v.Position.Y;
				gv.z = v.Position.Z;
				vertices.push_back(gv);
			}
		}
	}
	else
	{

		translatetoorigin(p);
		
	}



	// Load it into a VBO
	glGenVertexArrays(1, &VertexArrayID3);
	glBindVertexArray(VertexArrayID3);

	glGenBuffers(1, &vertexbuffer3);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer3);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);


	glBindVertexArray(VertexArrayID3);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer3);
	glEnableVertexAttribArray(vertexLoc);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

}




void key1(unsigned char key, int x, int y)
{

	init_shader(meshcount);
	meshcount++;
	if (meshcount > 29) //15 groups
		meshcount = -1;


}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowPosition(400, 400);
	glutInitWindowSize(800, 800);
	cout << glutCreateWindow("Hello OpenGL") << endl;


	glutReshapeFunc(reshape1);
	glutDisplayFunc(display1);
	glutIdleFunc(idle1);
	glutKeyboardFunc(key1);

	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	// Dark blue background
	glClearColor(0.15f, 0.15f, 0.15f, 0.0f);

	// Create and compile our GLSL program from the shaders
	loader.LoadFile("OBJ files/Skeleton.obj");
	init_shader(-1);



	glutMainLoop();


	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);



	return 0;
}

