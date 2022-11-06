//#include "stdafx.h"
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <iostream>

// Kody shaderow
const GLchar* vertexSource = R"glsl(
 #version 150 core
 in vec3 position;
 in vec3 color;
 out vec3 Color;
 void main(){
 Color = color;
 gl_Position = vec4(position, 1.0);
 }
 )glsl";

const GLchar* fragmentSource = R"glsl(
 #version 150 core
 in vec3 Color;
 out vec4 outColor;
 void main()
 {
 outColor = vec4(Color, 1.0);
 }
 )glsl";


int main()
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 2;

	//Rendering
	sf::Window window(sf::VideoMode(800, 600, 32), "OpenGL", sf::Style::Titlebar | sf::Style::Close, settings);

	//Inicjalizacja GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	//Utworzenie VAO (Vertex Array Object)
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Utworzenie VBO (Vertex Buffer Object)
	// i skopiowanie do niego danych wierzchoÅ‚kowych
	GLuint vbo;
	glGenBuffers(1, &vbo);

	// współrzedna x, współrzedna y, współrzędna z, RGB
	GLfloat vertices[] =
	{ -0.7f, 0.7f, 0.0f, 0.60f, 0.00f, 0.30f,
	0.7f, 0.7f, 0.1f, 0.30f, 0.42f, 1.00f,
	-0.7f, -0.7f, 0.2f, 0.30f, 0.42f, 1.00f,

	0.7f, -0.7f, 0.0f, 0.60f, 0.00f, 0.30f,
	0.7f, 0.7f, 0.1f, 0.30f, 0.42f, 1.00f,
	-0.7f, -0.7f, 0.2f, 0.30f, 0.42f, 1.00f,
	};
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Utworzenie i skompilowanie shadera wierzchołków
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	// Utworzenie i skompilowanie shadera fragmentów
	GLuint fragmentShader =	glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	// Zlinkowanie obu shaderów w jeden wspólny program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Specifikacja formatu danych wierzchołkowych
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	// Rozpoczęcie pętli zdarzeń,
	int prymityw = GL_TRIANGLES;
	bool running = true;

	while (running) {
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent)) {
			switch (windowEvent.type) {
			case sf::Event::Closed:
				running = false;
				break;
			}
		}
		sf::Event::KeyPressed;
		switch (windowEvent.key.code)
		{
		case sf::Keyboard::Num1:
			prymityw = GL_POINTS;
			break;

		case sf::Keyboard::Num2:
			prymityw = GL_LINES;
			break;

		case sf::Keyboard::Num3:
			prymityw = GL_LINE_STRIP;
			break;

		case sf::Keyboard::Num4:
			prymityw = GL_LINE_LOOP;
			break;

		case sf::Keyboard::Num5:
			prymityw = GL_TRIANGLES;
			break;

		case sf::Keyboard::Num6:
			prymityw = GL_TRIANGLE_STRIP;
			break;

		case sf::Keyboard::Num7:
			prymityw = GL_TRIANGLE_FAN;
			break;

		case sf::Keyboard::Num8:
			prymityw = GL_QUADS;
			break;

			prymityw = GL_QUAD_STRIP;
		case sf::Keyboard::Num9:
			break;

		case sf::Keyboard::Num0:
			prymityw = GL_POLYGON;
			break;

		case sf::Keyboard::Escape:
			running = false;
			break;
		}

		//kolor sceny
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Narysowanie trójkąta na podstawie 3 wierzchołków
		glDrawArrays(prymityw, 0, sizeof(vertices)); //wieloktotnośc 3 - rysowanie odpowiedniej ilości trójkątów
	   // Wymiana buforów tylni/przedni
		window.display();
	}
	// Kasowanie programu i czyszczenie buforów
	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	// Zamknięcie okna renderingu
	window.close();
	return 0;
}