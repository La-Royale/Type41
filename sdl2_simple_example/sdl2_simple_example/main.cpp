#include <GL/glew.h>
#include <chrono>
#include <thread>
#include <exception>
#include <glm/glm.hpp>
#include <SDL2/SDL_events.h>
#include "MyWindow.h"
#include "imgui_impl_sdl2.h"

using namespace std;

using hrclock = chrono::high_resolution_clock;
using u8vec4 = glm::u8vec4;
using ivec2 = glm::ivec2;
using vec3 = glm::dvec3;

static const ivec2 WINDOW_SIZE(512, 512);
static const unsigned int FPS = 60;
static const auto FRAME_DT = 1.0s / FPS;

static void init_openGL() {
	glewInit();
	if (!GLEW_VERSION_3_0) throw exception("OpenGL 3.0 API is not available");
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.5, 0.5, 0.5, 1.0);
}
static void draw_line(float x1, float y1, float z1, float x2, float y2, float z2) {
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y2, z2);

	glEnd();
}
static void draw_cube(const u8vec4& color, const vec3& center, double size) {
	//glColor4ub(color.r, color.g, color.b, color.a);
	
	double half = size / 2.0;
	vec3 vertices[8] = {
		vec3(center.x - half, center.y - half, center.z + half), // Vértice 0
		vec3(center.x + half, center.y - half, center.z + half), // Vértice 1
		vec3(center.x + half, center.y + half, center.z + half), // Vértice 2
		vec3(center.x - half, center.y + half, center.z + half), // Vértice 3
		vec3(center.x - half, center.y - half, center.z - half), // Vértice 4
		vec3(center.x + half, center.y - half, center.z - half), // Vértice 5
		vec3(center.x + half, center.y + half, center.z - half), // Vértice 6
		vec3(center.x - half, center.y + half, center.z - half)  // Vértice 7
	};

	glBegin(GL_TRIANGLES);
	// Frente Blanco
	glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3dv(&vertices[0].x); glVertex3dv(&vertices[1].x); glVertex3dv(&vertices[2].x);
	glVertex3dv(&vertices[2].x); glVertex3dv(&vertices[3].x); glVertex3dv(&vertices[0].x);
	// Derecha
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3dv(&vertices[1].x); glVertex3dv(&vertices[5].x); glVertex3dv(&vertices[6].x);
	glVertex3dv(&vertices[6].x); glVertex3dv(&vertices[2].x); glVertex3dv(&vertices[1].x);
	// Atrás
	glColor3f(.0f, 0.0f, 1.0f);
	glVertex3dv(&vertices[5].x); glVertex3dv(&vertices[4].x); glVertex3dv(&vertices[7].x);
	glVertex3dv(&vertices[7].x); glVertex3dv(&vertices[6].x); glVertex3dv(&vertices[5].x);
	// Izquierda Naranja
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3dv(&vertices[4].x); glVertex3dv(&vertices[0].x); glVertex3dv(&vertices[3].x);
	glVertex3dv(&vertices[3].x); glVertex3dv(&vertices[7].x); glVertex3dv(&vertices[4].x);
	// Arriba
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3dv(&vertices[3].x); glVertex3dv(&vertices[2].x); glVertex3dv(&vertices[6].x);
	glVertex3dv(&vertices[6].x); glVertex3dv(&vertices[7].x); glVertex3dv(&vertices[3].x);
	// Abajo
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3dv(&vertices[4].x); glVertex3dv(&vertices[5].x); glVertex3dv(&vertices[1].x);
	glVertex3dv(&vertices[1].x); glVertex3dv(&vertices[0].x); glVertex3dv(&vertices[4].x);
	glEnd();

}


static void draw_triangle(const u8vec4& color, const vec3& center, double size) {
	glColor4ub(color.r, color.g, color.b, color.a);
	glBegin(GL_TRIANGLES);
	glVertex3d(center.x, center.y + size, center.z);
	glVertex3d(center.x - size, center.y - size, center.z);
	glVertex3d(center.x + size, center.y - size, center.z);
	glEnd();
}


static void display_func() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//draw_triangle(u8vec4(255, 0, 0, 255), vec3(0.0, 0.0, 0.0), 0.2);
	draw_cube(u8vec4(255, 0, 0, 255), vec3(0.0, 0.0, 0.0), 1.0);
	glRotatef(0.8f, 1.0f, 1.0f, 0.0f);


}

static bool processEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return false;
			break;
		default:
			ImGui_ImplSDL2_ProcessEvent(&event);
		}
	}
	return true;
}

int main(int argc, char** argv) {
	MyWindow window("SDL2 Simple Example", WINDOW_SIZE.x, WINDOW_SIZE.y);

	init_openGL();

	while (processEvents()) {
		const auto t0 = hrclock::now();
		display_func();
		window.swapBuffers();
		const auto t1 = hrclock::now();
		const auto dt = t1 - t0;
		if(dt<FRAME_DT) this_thread::sleep_for(FRAME_DT - dt);
	}

	return 0;
}