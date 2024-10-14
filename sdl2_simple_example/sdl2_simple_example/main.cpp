#include <GL/glew.h>
#include <chrono>
#include <thread>
#include <exception>
#include <glm/glm.hpp>
#include <SDL2/SDL_events.h>
#include "MyWindow.h"
#include "imgui_impl_sdl2.h"

#include <stdio.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>


using namespace std;

using hrclock = chrono::high_resolution_clock;
using u8vec4 = glm::u8vec4;
using ivec2 = glm::ivec2;
using vec3 = glm::dvec3;

static const ivec2 WINDOW_SIZE(500, 500);
static const unsigned int FPS = 60;
static const auto FRAME_DT = 1.0s / FPS;


GLubyte checkerImage[256][256][4];

int _HEIGHT = 256;
int _WIDTH = 256;
GLuint textureID;

vector <GLfloat> uv;

void start() {
	
	
	for (int i = 0; i < _HEIGHT; i++) {
		for (int j = 0; j < _WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _WIDTH, _HEIGHT, 0,
	GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);
}
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

const aiScene* loadFBX(const std::string& filePath) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		printf("ERROR::ASSIMP::%s\n", importer.GetErrorString());
		return nullptr;
	}
	return scene;
}


void drawMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<float> vertices;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		vertices.push_back(mesh->mVertices[i].x);
		vertices.push_back(mesh->mVertices[i].y);
		vertices.push_back(mesh->mVertices[i].z);

		if (mesh->HasNormals()) {
			vertices.push_back(mesh->mNormals[i].x);
			vertices.push_back(mesh->mNormals[i].y);
			vertices.push_back(mesh->mNormals[i].z);
		}

		if (mesh->mTextureCoords[0]) { 
			uv.push_back(mesh->mTextureCoords[0][i].x);
			uv.push_back(mesh->mTextureCoords[0][i].y);
		}
	}
	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			unsigned int vertexIndex = face.mIndices[j];
			glTexCoord2f(uv[vertexIndex * 2], uv[vertexIndex * 2 + 1]); 
			glVertex3fv(&vertices[vertexIndex * 3]);
		}
	}
	glEnd();
}


void processNode(aiNode* node, const aiScene* scene) {
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		drawMesh(mesh, scene);
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}


const char* file = "ht.fbx";
const struct aiScene* scene = aiImportFile(file, aiProcess_Triangulate);

static void DrawFBX() {
	if (!scene) {
		fprintf(stderr, "Error al cargar el archivo: %s\n", aiGetErrorString());
		return;
	}

	for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[i];

		GLuint VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		float multi = 1;

		// Cargar los vértices
		GLfloat* vertices = new GLfloat[mesh->mNumVertices * 3];
		for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
			aiVector3D vertex = mesh->mVertices[v];
			vertices[v * 3] = vertex.x * multi;
			vertices[v * 3 + 1] = vertex.z * multi - 0.5;
			vertices[v * 3 + 2] = vertex.y * multi;
		}

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
		delete[] vertices;

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);

		

		// Dibuja la malla
		glDrawArrays(GL_TRIANGLES, 0, mesh->mNumVertices);


		// Limpiar
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);


	}
}

static void display_func(const aiScene* scene) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//draw_triangle(u8vec4(255, 0, 0, 255), vec3(0.0, 0.0, 0.0), 0.2);
	//draw_cube(u8vec4(255, 0, 0, 255), vec3(0.0, 0.0, 0.0), 1.0);
	glRotatef(0.8f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawFBX();
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

	const aiScene* scene = loadFBX("cube.fbx");

	start();

	while (processEvents()) {
		const auto t0 = hrclock::now();
		display_func(scene);  // Pasa la escena cargada
		window.swapBuffers();
		const auto t1 = hrclock::now();
		const auto dt = t1 - t0;
		if (dt < FRAME_DT) this_thread::sleep_for(FRAME_DT - dt);
	}
	return 0;
}
