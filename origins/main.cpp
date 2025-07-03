#include <GLFW/glfw3.h>
#include <iostream>
#include "Vertex3d.h"
#include "Edge3d.h"
#include "TriangleFace.h"
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>  // rand, srand
#include <ctime>    // time
using namespace std;

GLFWwindow* window;
vector<TriangleFace> faces;
float cameraX = 0.0f, cameraY = 0.0f, cameraZ = 0.0f; // posição da câmera
float cameraYaw = 0.0f;   // Rotação em torno do eixo Y
float cameraPitch = 0.0f;   // Rotação em torno do eixo X
float rotationSpeed = 1.5f; // Velocidade de rotação
float cameraSpeed = 1.5f; // velocidade de movimento

bool keys[1024]; // estados das teclas

bool mousePressed = false;
double lastX = 400, lastY = 300; // valores iniciais (meio da tela)
float sensitivity = 0.1f;

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            mousePressed = true;
            glfwGetCursorPos(window, &lastX, &lastY); // salva posição inicial
        }
        else if (action == GLFW_RELEASE) {
            mousePressed = false;
        }
    }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (mousePressed) {
        float xoffset = xpos - lastX;
        float yoffset = ypos - lastY;

        lastX = xpos;
        lastY = ypos;

        cameraYaw += xoffset * sensitivity;
        cameraPitch += yoffset * sensitivity;

        if (cameraPitch > 89.0f) cameraPitch = 89.0f;
        if (cameraPitch < -89.0f) cameraPitch = -89.0f;
    }
}


// Callback de teclado
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

static void processInput() {
    if (keys[GLFW_KEY_W])
        cameraZ -= cameraSpeed;
    if (keys[GLFW_KEY_S])
        cameraZ += cameraSpeed;
    if (keys[GLFW_KEY_A])
        cameraX -= cameraSpeed;
    if (keys[GLFW_KEY_D])
        cameraX += cameraSpeed;
    if (keys[GLFW_KEY_Q])
        cameraY += cameraSpeed;
    if (keys[GLFW_KEY_E])
        cameraY -= cameraSpeed;

    // Rotação da câmera
    if (keys[GLFW_KEY_LEFT])
        cameraYaw += rotationSpeed;
    if (keys[GLFW_KEY_RIGHT])
        cameraYaw -= rotationSpeed;
    if (keys[GLFW_KEY_UP])
        cameraPitch += rotationSpeed;
    if (keys[GLFW_KEY_DOWN])
        cameraPitch -= rotationSpeed;
}

vector<TriangleFace> loadSTLFile(const string& filename, vector<Vertex3d>& outVertices) {
    vector<TriangleFace> faces;
    ifstream infile(filename);

    if (!infile.is_open()) {
        cerr << "Erro ao abrir o arquivo STL: " << filename << endl;
        return faces;
    }

    string line;
    while (getline(infile, line)) {
        // Ignora espaços em branco no começo da linha
        stringstream ss(line);
        string word;
        ss >> word;

        if (word == "facet") {
            // Pula a linha "facet normal ..."
            getline(infile, line); // outer loop

            Vertex3d* vertices[3];
            for (int i = 0; i < 3; ++i) {
                getline(infile, line); // vertex x y z
                stringstream vertexLine(line);
                string label;
                double x, y, z;
                vertexLine >> label >> x >> y >> z;
                outVertices.emplace_back("", x, y, z);
                vertices[i] = &outVertices.back();
            }

            // Pula "endloop" e "endfacet"
            getline(infile, line);
            getline(infile, line);

            // Cria a face com os 3 vértices
            faces.emplace_back(vertices[0], vertices[1], vertices[2]);
        }
    }

    infile.close();
    return faces;
}

vector<TriangleFace> loadBinarySTLFile(const string& filename, vector<Vertex3d>& outVertices) {
    vector<TriangleFace> faces;
    ifstream infile(filename, ios::in | ios::binary);

    if (!infile) {
        cerr << "Erro ao abrir o arquivo STL binário: " << filename << endl;
        return faces;
    }

    // Pula cabeçalho de 80 bytes
    char header[80];
    infile.read(header, 80);

    // Lê número de triângulos (4 bytes)
    uint32_t numTriangles = 0;
    infile.read(reinterpret_cast<char*>(&numTriangles), sizeof(uint32_t));

    for (uint32_t i = 0; i < numTriangles; ++i) {
        float normal[3]; // pode ser ignorada
        float vertices[9]; // 3 vértices x 3 coordenadas

        infile.read(reinterpret_cast<char*>(normal), 3 * sizeof(float));
        infile.read(reinterpret_cast<char*>(vertices), 9 * sizeof(float));

        Vertex3d* v[3];
        for (int j = 0; j < 3; ++j) {
            float x = vertices[j * 3];
            float y = vertices[j * 3 + 1];
            float z = vertices[j * 3 + 2];
            outVertices.emplace_back("", x, y, z);
            v[j] = &outVertices.back();
        }

        faces.emplace_back(v[0], v[1], v[2]);

        // Pula os 2 bytes de atributo
        char attributeBytes[2];
        infile.read(attributeBytes, 2);
    }

    infile.close();
    return faces;
}

vector<TriangleFace> loadMeshFromFile(const string& filename, vector<Vertex3d>& outVertices) {
    vector<TriangleFace> faces;
    ifstream infile(filename);

    if (!infile.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << filename << endl;
        return faces;
    }

    string line;
    while (getline(infile, line)) {
        stringstream ss(line);
        string type;
        ss >> type;

        if (type == "v") {
            double x, y, z;
            ss >> x >> y >> z;
            outVertices.push_back(Vertex3d("", x, y, z));
        }
        else if (type == "f") {
            int i1, i2, i3;
            ss >> i1 >> i2 >> i3;

            Vertex3d* v1 = &outVertices[i1 - 1];
            Vertex3d* v2 = &outVertices[i2 - 1];
            Vertex3d* v3 = &outVertices[i3 - 1];

            faces.push_back(TriangleFace(v1, v2, v3));
        }
    }

    infile.close();
    return faces;
}

void normalizeMesh(vector<Vertex3d>& verts) {
    if (verts.empty()) return;

    float minX = verts[0].getX().calculate(), maxX = minX;
    float minY = verts[0].getY().calculate(), maxY = minY;
    float minZ = verts[0].getZ().calculate(), maxZ = minZ;

    // encontra bounding box
    for (auto& v : verts) {
        float x = v.getX().calculate();
        float y = v.getY().calculate();
        float z = v.getZ().calculate();
        minX = std::min(minX, x); maxX = std::max(maxX, x);
        minY = std::min(minY, y); maxY = std::max(maxY, y);
        minZ = std::min(minZ, z); maxZ = std::max(maxZ, z);
    }

    // centro da caixa
    float centerX = (minX + maxX) * 0.5f;
    float centerY = (minY + maxY) * 0.5f;
    float centerZ = (minZ + maxZ) * 0.5f;

    // opcional: escala para caber em ±size (por ex. size = 2.0f)
    float sizeX = maxX - minX, sizeY = maxY - minY, sizeZ = maxZ - minZ;
    float maxDim = std::max(sizeX, std::max(sizeY, sizeZ));
    float scale = (maxDim > 0.0f) ? (4.0f / maxDim) : 1.0f;

    // ajusta todos os vértices
    for (auto& v : verts) {
        float x = (v.getX().calculate() - centerX) * scale;
        float y = (v.getY().calculate() - centerY) * scale;
        float z = (v.getZ().calculate() - centerZ) * scale;
        v.setX(x);
        v.setY(y);
        v.setZ(z);
    }
}


static vector<TriangleFace> createSurface() {
    vector<TriangleFace> surface;

    // Triângulo da base inferior (v1, v2, v3)
    static vector<Vertex3d> vertices = {
        Vertex3d("", 0, 0, 0), // v1
        Vertex3d("", 1, 0, 0), // v2
        Vertex3d("", 1, 1, 0), // v3
        Vertex3d("", 0, 1, 0), // v4

        Vertex3d("", 0, 0, 1), // v5
        Vertex3d("", 1, 0, 1), // v6
        Vertex3d("", 1, 1, 1), // v7
        Vertex3d("", 0, 1, 1)  // v8
    };

    Vertex3d* v1 = &vertices[0];
    Vertex3d* v2 = &vertices[1];
    Vertex3d* v3 = &vertices[2];
    Vertex3d* v4 = &vertices[3];
    Vertex3d* v5 = &vertices[4];
    Vertex3d* v6 = &vertices[5];
    Vertex3d* v7 = &vertices[6];
    Vertex3d* v8 = &vertices[7];
    /*
    // Face inferior (base): v1, v2, v3, v4
    faces.push_back(TriangleFace(v1, v2, v3));
    faces.push_back(TriangleFace(v1, v3, v4));

    // Face superior (topo): v5, v6, v7, v8
    faces.push_back(TriangleFace(v5, v6, v7));
    faces.push_back(TriangleFace(v5, v7, v8));
    */
    // Face frontal: v1, v2, v6, v5
    faces.push_back(TriangleFace(v1, v2, v6));
    faces.push_back(TriangleFace(v1, v6, v5));

    // Face traseira: v4, v3, v7, v8
    faces.push_back(TriangleFace(v4, v3, v7));
    faces.push_back(TriangleFace(v4, v7, v8));

    // Face direita: v2, v3, v7, v6
    faces.push_back(TriangleFace(v2, v3, v7));
    faces.push_back(TriangleFace(v2, v7, v6));

    // Face esquerda: v1, v4, v8, v5
    faces.push_back(TriangleFace(v1, v4, v8));
    faces.push_back(TriangleFace(v1, v8, v5));

    return surface;
}

// Callback de redimensionamento
static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (float)width / (float)height;
    float size = 5.0f;

    if (aspect >= 1.0f)
        glOrtho(-size * aspect, size * aspect, -size, size, -10.0f, 10.0f);
    else
        glOrtho(-size, size, -size / aspect, size / aspect, -10.0f, 10.0f);

    glMatrixMode(GL_MODELVIEW);
}

static void updateProjection() {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    framebuffer_size_callback(window, width, height);
}

static void drawAxes() {
    float axisLength = 40.0f;

    glLineWidth(2.0f);
    glBegin(GL_LINES);

    // Eixo X - vermelho
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-axisLength, 0.0f, 0.0f);
    glVertex3f(axisLength, 0.0f, 0.0f);

    // Eixo Y - verde
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -axisLength, 0.0f);
    glVertex3f(0.0f, axisLength, 0.0f);

    // Eixo Z - azul
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -axisLength);
    glVertex3f(0.0f, 0.0f, axisLength);

    glEnd();
}

static void drawTriangleFaces(const std::vector<TriangleFace>& faces) {
    glBegin(GL_TRIANGLES);


    for (const auto& face : faces) {
        Vertex3d p1 = face.get_V1();
        Point3d p2 = face.get_V2();
        Point3d p3 = face.get_V3();

        glColor3f(face.get_r(), face.get_g(), face.get_b());
        glVertex3f(p1.getX().calculate(), p1.getY().calculate(), p1.getZ().calculate());
        glVertex3f(p2.getX().calculate(), p2.getY().calculate(), p2.getZ().calculate());
        glVertex3f(p3.getX().calculate(), p3.getY().calculate(), p3.getZ().calculate());
    }
    glEnd();
}




int main() {
    if (!glfwInit()) return -1;

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    window = glfwCreateWindow(mode->width, mode->height, "Visualizador 3D", monitor, nullptr);
    if (!window) { glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);


    updateProjection();

    vector<Vertex3d> loadedVertices;
    vector<TriangleFace> faces = loadMeshFromFile("C:/Users/leois/Downloads/teapot.txt", loadedVertices);
    //vector<TriangleFace> faces = createSurface();


    normalizeMesh(loadedVertices);


    while (!glfwWindowShouldClose(window)) {
        processInput();
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetCursorPosCallback(window, cursor_position_callback);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        glRotatef(cameraPitch, 1.0f, 0.0f, 0.0f);
        glRotatef(cameraYaw, 0.0f, 1.0f, 0.0f);
        glTranslatef(-cameraX, -cameraY, -cameraZ);

        drawAxes();

        
        drawTriangleFaces(faces);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
