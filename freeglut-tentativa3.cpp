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

//globals

unsigned int elefante;
vector<vector<float>> vertices;
vector<vector<float>> textures;
vector<vector<float>> normals;
float rot_ele;

// triangle faces indexes
vector<vector<int>> vertices_indexes;
vector<vector<int>> textures_indexes;
vector<vector<int>> normals_indexes;

int findIndexFromString(string &string, FaceIndexType type) {
    int substringStart = nthOccurrence(string, "/", type);
    int substringEnd = type < NORMAL ? nthOccurrence(string, "/", type + 1) : string.length() - 1;
    if (type > VERTICE) substringStart++;
    bool isValidSubstring = substringStart != -1 && substringEnd != -1;
    bool isEmptyValue = substringStart == substringEnd;

    if (isValidSubstring) {
        if (isEmptyValue) {
            return -1;
        }
        else {
            return stoi(string.substr(substringStart, substringEnd)) - 1;
        }
    }
    return -1;
}

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
    newFaceIndex(x, y, z, VERTICE); //cout << nthOccurrence(x, "/", 2);
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

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);

    glDepthFunc(GL_LEQUAL);

    glMatrixMode(GL_MODELVIEW);
}

void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Set up light properties
    GLfloat light_position[] = { 100.0, 100.0, 100.0, 1.0 };
    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}

void addGlVertex(int verticeIndex) {
    float vX = verticeIndex > -1 ? vertices[verticeIndex][0] : 0.0f;
    float vY = verticeIndex > -1 ? vertices[verticeIndex][1] : 0.0f;
    float vZ = verticeIndex > -1 ? vertices[verticeIndex][2] : 0.0f;
    glVertex3f(vX, vY, vZ);
}

void addGlNormal(int normalIndex) {
    float nX = normalIndex > -1 ? normals[normalIndex][0] : 0.0f;
    float nY = normalIndex > -1 ? normals[normalIndex][1] : 0.0f;
    float nZ = normalIndex > -1 ? normals[normalIndex][2] : 0.0f;
    glNormal3f(nX, nY, nZ);
}

void addGlTexCoord(int textureIndex) {
    float tU = textureIndex > -1 ? textures[textureIndex][0] : 0.0f;
    float tV = textureIndex > -1 ? textures[textureIndex][1] : 0.0f;
    glTexCoord2f(tU, tV);
}

void drawObject()
{
    glPushMatrix();
    glTranslatef(0, -40.00, -105);
    glColor3f(1.0, 0.23, 0.27);
    glScalef(0.4, 0.4, 0.4);
    glRotatef(rot_ele, 0, 1, 0);

    setupLighting();

    glEnable(GL_DEPTH_TEST);

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < vertices_indexes.size(); i++) {
        vector<int> verticesArray = vertices_indexes[i];
        vector<int> texturesArray = textures_indexes[i];
        vector<int> normalsArray = normals_indexes[i];

        for (int j = 0; j < 3; j++) {

            addGlVertex(verticesArray[j]);
            addGlNormal(normalsArray[j]);
            addGlTexCoord(texturesArray[j]);
        }
    }
    glEnd();

    glDisable(GL_DEPTH_TEST);

    glPopMatrix();
    rot_ele = rot_ele + 0.6;
    if (rot_ele > 360) rot_ele = rot_ele - 360;
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
    glutPostRedisplay();
    glutTimerFunc(10, timer, 0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 450);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Carregar OBJ");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutTimerFunc(10, timer, 0);
    loadObj("data/porsche.obj");

    glEnable(GL_DEPTH_TEST);

    glutMainLoop();
    return 0;
}
