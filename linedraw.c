#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>

#define TRUE 1
#define FALSE 0


const char * lineshadervert = "\
	attribute float tcattrib;\n\
	attribute vec2 posattrib;\n\
	varying float tc;\n\
	void main(){\n\
		tc = tcattrib;\n\
		gl_Position = vec4(posattrib, 0.0, 1.0);\n\
	}\n\
";
const char * lineshaderfrag = "\
	varying float tc;\n\
	void main(){\n\
		gl_FragColor.r = step(1.0, mod(tc, 2.0));\n\
		gl_FragColor.gba = vec3(0.0);\n\
	}\n\
";

int shader_printProgramLogStatus(const int id){
	GLint blen = 0;
	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &blen);
	if(blen > 1){
		GLchar * log = (GLchar *) malloc(blen);
		glGetProgramInfoLog(id, blen, 0, log);
		printf("program log: \n%s\n", log);
		free(log);
	}
	return blen;
}
int shader_printShaderLogStatus(const int id){
	GLint blen = 0;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &blen);
	if(blen > 1){
		GLchar * log = (GLchar *) malloc(blen);
		glGetShaderInfoLog(id, blen, 0, log);
		printf("shader log: \n%s\n", log);
		free(log);
	}
	return blen;
}

GLuint shaderid = 0;
#define POSATTRIBLOC 0
#define TCATTRIBLOC 1

void drawlines(void){
}

int main(void){
	GLFWwindow * window;
	if(!glfwInit()) return -1;
	window 	= glfwCreateWindow(800, 600, "Linedraw", NULL, NULL);
	if (!window){
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	GLenum glewError = glewInit();
	if(glewError != GLEW_OK){
		printf( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
		return FALSE;
	}
	//gl and glew are good 2 go
	GLuint vertid;
	GLuint fragid;

	vertid = glCreateShader(GL_VERTEX_SHADER);
	fragid = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertid, 1, (const GLchar **)&lineshadervert, 0);
	glShaderSource(fragid, 1, (const GLchar **)&lineshaderfrag, 0);

	glCompileShader(vertid);
	glCompileShader(fragid);
	shader_printShaderLogStatus(vertid);
	shader_printShaderLogStatus(fragid);


	shaderid = glCreateProgram();
	if(!shaderid) printf("unable to greate program\n");
	glAttachShader(shaderid, vertid);
	glAttachShader(shaderid, fragid);

	glBindAttribLocation(shaderid, POSATTRIBLOC, "posattrib");
	glBindAttribLocation(shaderid, TCATTRIBLOC, "tcattrib");

	glLinkProgram(shaderid);
	glDeleteShader(vertid);
	glDeleteShader(fragid);




	shader_printProgramLogStatus(shaderid);
	glUseProgram(shaderid);


	GLfloat * vertices = malloc(200 * sizeof(GLfloat));
	GLfloat * texcoords = malloc(100 * sizeof(GLfloat));
	int i;
	for(i = 0; i < 100; i++){
		texcoords[i] = i * 0.4;
	}
	glEnableVertexAttribArray(POSATTRIBLOC);
	glVertexAttribPointer(POSATTRIBLOC, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), vertices);
	glEnableVertexAttribArray(TCATTRIBLOC);
	glVertexAttribPointer(TCATTRIBLOC, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(GLfloat), texcoords);

	while(!glfwWindowShouldClose(window)){
		glClear(GL_COLOR_BUFFER_BIT);
		double seconds = fmod(glfwGetTime() * 0.5, 2 * M_PI);
		float sway = sin(seconds) * 0.01;

		int z;
		for(z = 1; z < 5; z++){
			GLfloat myx= z /2.5 - 1.0, myy= -1.0;
			for(i = 0; i < 100; i++){
				vertices[i * 2 + 0] = myx = sin(i * sway)* 0.01 + myx;
				vertices[i * 2 + 1] = myy = cos(i * sway)* 0.01 + myy;
			}
			glDrawArrays(GL_LINE_STRIP, 0, 100);
		}
		glfwSwapBuffers(window);
	}
}
