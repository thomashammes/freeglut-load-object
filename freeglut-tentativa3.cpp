#include <iostream>
#include <fstream>
#include <vector>
#include <GL/freeglut.h>
#include <string>
using namespace std;

enum FaceIndexType {
    VERTICE,
    TEXTURE,
    NORMAL
};

// globals
vector<vector<float>> vertices;
vector<vector<float>> textures;
vector<vector<float>> normals;
float rot_ele;

// triangle faces indexes
vector<vector<int>> vertices_indexes;
vector<vector<int>> textures_indexes;
vector<vector<int>> normals_indexes;

// rotate, scale, translate
GLfloat scale = 0.5;

GLfloat translateX = 0;
GLfloat translateY = -40.00;
GLfloat translateZ = -105;

int xDegrees = 0, yDegrees = 0, zDegrees = 0;
int modDegree = 360;
bool xAxisActivated = false;
bool yAxisActivated = false;
bool zAxisActivated = false;

// lights
bool light1Enabled = true;
bool light2Enabled = true;
bool light3Enabled = true;


int nthOccurrence(const string& str, const string& findMe, int nth);

int findIndexFromString(string& string, FaceIndexType type);

void pushFaceToIndexesArray(int fp, int fs, int ft, FaceIndexType type) {
    vector<int> face;
    face.push_back(fp);
    face.push_back(fs);
    face.push_back(ft);
    switch (type) {
        case VERTICE:
            vertices_indexes.push_back(face);
            break;
        case TEXTURE:
            textures_indexes.push_back(face);
            break;
        case NORMAL:
            normals_indexes.push_back(face);
            break;
    }
}

void newFaceIndex(string x, string y, string z, FaceIndexType type) {
    int fp = findIndexFromString(x, type);
    int fs = findIndexFromString(y, type);
    int ft = findIndexFromString(z, type);
    pushFaceToIndexesArray(fp, fs, ft, type);
}

void newFaceFrom(istream& arquivo) {
    string x, y, z;
    arquivo >> x >> y >> z;
    newFaceIndex(x, y, z, VERTICE);
    newFaceIndex(x, y, z, TEXTURE);
    newFaceIndex(x, y, z, NORMAL);
}

void newVerticeFrom(istream& arquivo) {
    vector<float> vertice;
    float x, y, z;
    arquivo >> x >> y >> z;
    vertice.push_back(x);
    vertice.push_back(y);
    vertice.push_back(z);
    vertices.push_back(vertice);
}

void newTextureFrom(istream& arquivo) {
    vector<float> texture;
    float u, v;
    arquivo >> u >> v;
    texture.push_back(u);
    texture.push_back(v);
    textures.push_back(texture);
}

void newNormalFrom(istream& arquivo) {
    vector<float> normal;
    float x, y, z;
    arquivo >> x >> y >> z;
    normal.push_back(x);
    normal.push_back(y);
    normal.push_back(z);
    normals.push_back(normal);
}

void fillObjVectorsReadingFile(istream& arquivo) {
    string tipo;
    int count2 = 0;
    while (arquivo >> tipo)
    {
        if (tipo == "v")
        {
            newVerticeFrom(arquivo);
        }
        else if (tipo == "vt")
        {
            newTextureFrom(arquivo);
        }
        else if (tipo == "vn")
        {
            newNormalFrom(arquivo);
        }
        else if (tipo == "f")
        {
            newFaceFrom(arquivo);
        }
    }
}

void loadObj(string fname)
{
    ifstream arquivo(fname);
    if (!arquivo.is_open()) {
        cout << "arquivo nao encontrado";
        exit(1);
    }
    else {
        fillObjVectorsReadingFile(arquivo);
    }

    arquivo.close();
}

void setupLighting() {

    glEnable(GL_LIGHTING);
    if (light1Enabled) {
        glEnable(GL_LIGHT0);
        GLfloat light0_position[] = { 100.0, 100.0, 100.0, 1.0 };
        GLfloat light0_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
        GLfloat light0_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };
        glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
        glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    }
    else {
        glDisable(GL_LIGHT0);
    }

    if (light2Enabled) {
        glEnable(GL_LIGHT1);
        GLfloat light1_position[] = { -100.0, -100.0, 0.0, 1.0 };
        GLfloat light1_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
        GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
        glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
        glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    }
    else {
        glDisable(GL_LIGHT1);
    }

    if (light3Enabled) {
        glEnable(GL_LIGHT2);
        GLfloat light2_position[] = { 50.0, 500.0, 150.0, 1.0 };
        GLfloat light2_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
        GLfloat light2_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat light2_specular[] = { 1.0, 1.0, 1.0, 1.0 };
        glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
        glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
        glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
    }
    else {
        glDisable(GL_LIGHT2);
    }
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);

    glDepthFunc(GL_LEQUAL);

    glMatrixMode(GL_MODELVIEW);
}

void addGlVertex(int verticeIndex) {
    float vX = verticeIndex > -1 ? vertices[verticeIndex][0] : 0.0f;
    float vY = verticeIndex > -1 ? vertices[verticeIndex][1] : 0.0f;
    float vZ = verticeIndex > -1 ? vertices[verticeIndex][2] : 0.0f;
    glVertex3f(vX, vY, vZ);
}

void addGlNormal(int normalIndex) {
    float nX = normals[normalIndex][0];
    float nY = normals[normalIndex][1];
    float nZ = normals[normalIndex][2];
    glNormal3f(nX, nY, nZ);
}

void addGlTexCoord(int textureIndex) {
    float tU = textures[textureIndex][0];
    float tV = textures[textureIndex][1];
    glTexCoord2f(tU, tV);
}

void drawObject()
{
    glPushMatrix();
    glColor3f(1.0, 0.23, 0.27);
    glTranslatef(translateX, translateY, translateZ);
    glScalef(scale, scale, scale);
    glRotatef(xDegrees, xAxisActivated || (!xAxisActivated && xDegrees > 0), false, false);
    glRotatef(yDegrees, false, yAxisActivated || (!yAxisActivated && yDegrees > 0), false);
    glRotatef(zDegrees, false, false, zAxisActivated || (!zAxisActivated && zDegrees > 0));

    setupLighting();

    glEnable(GL_DEPTH_TEST);

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < vertices_indexes.size(); i++) {
        
        for (int j = 0; j < 3; ++j)
        {
            int verticeIndex = vertices_indexes[i][j];
            int textureIndex = textures_indexes[i][j];
            int normalIndex = normals_indexes[i][j];

            if (textureIndex >= 0)
                addGlTexCoord(textureIndex);
            if (normalIndex >= 0)
                addGlNormal(normalIndex);
            if (verticeIndex >= 0)
                addGlVertex(verticeIndex);
        }
    }
    glEnd();

    glDisable(GL_DEPTH_TEST);

    glPopMatrix();
}

void display(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    drawObject();
    glutSwapBuffers();
}

void timer(int value) {
    if (xAxisActivated) {
        xDegrees += 1;
    }
    else if (!xAxisActivated && xDegrees > 0) {
        if (xDegrees >= modDegree) {
            xDegrees = xDegrees % modDegree;
        }
        xDegrees -= 1;
    }

    if (yAxisActivated) {
        if (yDegrees >= 360) {
            yDegrees = 0;
        }
        yDegrees += 1;
    }
    else if (!yAxisActivated && yDegrees > 0) {
        if (yDegrees >= modDegree) {
            yDegrees = yDegrees % modDegree;
        }
        yDegrees -= 1;
    }

    if (zAxisActivated) {
        zDegrees += 1;
    }
    else if (!zAxisActivated && zDegrees > 0) {
        if (zDegrees >= modDegree) {
            zDegrees = zDegrees % modDegree;
        }
        zDegrees -= 1;
    }
    glutPostRedisplay();
    glutTimerFunc(10, timer, 0);
}

void keyboardControl(unsigned char key, int x, int y) {
    switch (key) {
    case 27:
        exit(0);
        break;
    case 'x':
    case 'X':
        xAxisActivated = !xAxisActivated;
        break;
    case 'y':
    case 'Y':
        yAxisActivated = !yAxisActivated;
        break;
    case 'z':
    case 'Z':
        zAxisActivated = !zAxisActivated;
        break;
    case 'a':
    case 'A':
        translateX -= 1;
        break;
    case 'd':
    case 'D':
        translateX += 1;
        break;
    case 's':
    case 'S':
        translateY -= 1;
        break;
    case 'w':
    case 'W':
        translateY += 1;
        break;
    case 'q':
    case 'Q':
        translateZ -= 1;
        break;
    case 'e':
    case 'E':
        translateZ += 1;
        break;
    case 'f':
    case 'F':
        scale *= 0.9;
        break;
    case 'r':
    case 'R':
        scale *= 1.1;
        break;
    case '1':
        light1Enabled = !light1Enabled;
        break;
    case '2':
        light2Enabled = !light2Enabled;
        break;
    case '3':
        light3Enabled = !light3Enabled;
        break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 450);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Carregar OBJ");
    glutKeyboardFunc(keyboardControl);
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutTimerFunc(10, timer, 0);
    loadObj("data/mba1.obj");

    glEnable(GL_DEPTH_TEST);

    glutMainLoop();
    return 0;
}

int nthOccurrence(const string& str, const string& findMe, int nth)
{
    size_t  pos = 0;
    int     count = 0;

    while (count != nth)
    {
        pos += 1;
        pos = str.find(findMe, pos);
        if (pos == string::npos)
            return -1;
        count++;
    }
    return pos;
}

int findIndexFromString(string& string, FaceIndexType type) {
    int substringStart = nthOccurrence(string, "/", type);
    int substringLength = type < NORMAL ? nthOccurrence(string, "/", type + 1) : string.length();
    if (type > VERTICE) substringStart++;
    bool isValidSubstring = substringStart != -1 && substringLength != -1;
    bool isEmptyValue = substringStart == substringLength;

    if (isValidSubstring) {
        if (isEmptyValue) {
            return -1;
        }
        else {
            return stoi(string.substr(substringStart, substringLength - substringStart)) - 1;
        }
    }
    return -1;
}
