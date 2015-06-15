#include "mesh.h"

#include "platform.h"

#include <cstdio>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>

#include "compiler_opt.h"
#include "raytracing.h"

using namespace std;

const unsigned int LINE_LEN = 256;

#pragma mark - Normal calculations

void Mesh::computeVertexNormals() {
    for (unsigned int i = 0; i < vertices.size (); ++i) {
        vertices[i].n = Vector3f (0.0, 0.0, 0.0);
    }

    // Sum up neighboring normals
    for (unsigned int i = 0; i < triangles.size (); ++i) {
        Vector3f edge01 = vertices[triangles[i].v[1]].p -  vertices[triangles[i].v[0]].p;
        Vector3f edge02 = vertices[triangles[i].v[2]].p -  vertices[triangles[i].v[0]].p;
        Vector3f n = edge01.cross(edge02);
        n.normalize ();
        for (unsigned int j = 0; j < 3; ++j) {
            vertices[triangles[i].v[j]].n += n;
        }
    }

    // Normalize
    for (unsigned int i = 0; i < vertices.size (); i++) {
        vertices[i].n.normalize ();
    }
}

#pragma mark - Ray tracing

void Mesh::createBoundingBox() {
    for(vertex vert : vertices) {
        boundingBox.extend(vert.p);
    }
}

enum intersection_result : int {
    DEGENERATE = -1, // triangle is a line or point
    DISJOINT = 0, // no intersection
    INTERSECT = 1,
    PARALLEL = 2,
    OUTSIDE = 3
};

Vector3f Mesh::normalOfFace(Triangle triangle, float s, float t) {
    Vector3f n0 = vertices[triangle.v[0]].n;
    Vector3f n1 = vertices[triangle.v[1]].n;
    Vector3f n2 = vertices[triangle.v[2]].n;

    Vector3f n;

    n = (1.f - (s + t)) * n0 + n1 * s + n2 * t;

    return n;
}

hit_result Mesh::hit(Ray ray, shared_ptr<SceneNode> skip [[gnu::unused]])
{
    hit_result result;
    size_t triangleIndex = 0;
    Triangle nearestTriangle;
    float nearestHitS = 0.f, nearestHitT = 0.f;

    // Store the viewer position for specular shading
    result.viewer = ray.origin;

    // Transform the ray, effectively transforming this object
    ray = ray.transform(ws_transformationMatrix);

    for(size_t it = 0; it < this->triangles.size(); ++it) {
        int intersectResult;
        Vector3f intPoint;
        float hit, hitS, hitT;
        Triangle t = this->triangles[it];

        intersectResult = rayTriangleIntersect(ray, t, intPoint, hit, hitS, hitT);

        if (likely(intersectResult != INTERSECT))
            continue;

        // Skip if not nearer than prev hit.
        if(hit >= result.depth)
            continue;

        result.depth = hit;
        nearestTriangle = t;
        triangleIndex = it;
        nearestHitS = hitS;
        nearestHitT = hitT;
    }

    // If no hit, return unhit result
    if(result.depth == FLT_MAX)
        return result;

    // Otherwise, mark as hit and add apply info.
    result.hit = true;
    result.node = shared_from_this();

    // Position the ray hit
    result.hitPosition = ray.origin + result.depth * ray.direction;

    // Interpolate normal with the 3 vertex normals
    result.normal = normalOfFace(nearestTriangle, nearestHitS, nearestHitT);

    // Material of the triangle
    result.material = materials[triangleMaterials[triangleIndex]];

    return result;
}

int Mesh::rayTriangleIntersect(Ray ray, Triangle triangle, Vector3f &point, float &hitDistance, float &s, float &t)
{
    Vector3f v0, v1, v2;
    Vector3f u, v, n;
    Vector3f w0, w; // a thing?
    float a, b;

    // get the vertices
    v0 = this->vertices[triangle.v[0]].p;
    v1 = this->vertices[triangle.v[1]].p;
    v2 = this->vertices[triangle.v[2]].p;

    u = v1 - v0;
    v = v2 - v0;

    n = u.cross(v);
    if(n.length() == 0)
        return DEGENERATE;

    w0 = ray.origin - v0;
    a = -(n.dot(w0));
    b = n.dot(ray.direction);

    // Angle is much much tiny: parallel
    if (unlikely(fabs(b) < 0.00000001f)) {
        if(a == 0.f)
            return PARALLEL;
        else
            return DISJOINT;
    }

    // get intersection point
    hitDistance = a / b;
    if (hitDistance < 0.f)
        return DISJOINT;

    point = ray.origin + hitDistance * ray.direction;

    // test if the point lies inside the triangle, not only in its plane

    // these dot products are used multiple times. Precompute them
    float uu, uv, vv, wu, wv, denominator;

    uu = u.dot(u);
    uv = u.dot(v);
    vv = v.dot(v);
    w = point - v0;
    wu = w.dot(u);
    wv = w.dot(v);
    denominator = uv * uv - uu * vv;

    s = (uv * wv - vv * wu) / denominator;
    if (s < 0.0 || s > 1.0)
        return OUTSIDE;

    t = (uv * wu - uu * wv) / denominator;
    if (t < 0.0 || (s + t) > 1.0)
        return OUTSIDE;
    
    return INTERSECT;
}

#pragma mark - Drawing

void Mesh::draw() {
    SceneNode::draw();

    glBegin(GL_TRIANGLES);

    for (unsigned int i=0;i<triangles.size();++i) {
        Vector3f col = this->materials[triangleMaterials[i]].getKd();

        glColor3fv(col.pointer());
        for (int v = 0; v < 3; v++) {
            glNormal3f(vertices[triangles[i].v[v]].n[0],
                       vertices[triangles[i].v[v]].n[1],
                       vertices[triangles[i].v[v]].n[2]);

            glVertex3f(vertices[triangles[i].v[v]].p[0],
                       vertices[triangles[i].v[v]].p[1],
                       vertices[triangles[i].v[v]].p[2]);
        }
    }
    glEnd();
}

void Mesh::drawNotSmooth() {
    SceneNode::draw();

    glBegin(GL_TRIANGLES);

    for (unsigned int i = 0;i < triangles.size(); ++i) {
        Vector3f col, edge01, edge02, n;
        unsigned int triMat;

        triMat = triangleMaterials.at(i);
        col = this->materials.at(triMat).getKd();

        glColor3fv(col.pointer());

        edge01 = vertices[triangles[i].v[1]].p - vertices[triangles[i].v[0]].p;
        edge02 = vertices[triangles[i].v[2]].p - vertices[triangles[i].v[0]].p;
        n = edge01.cross(edge02);
        n.normalize ();
        glNormal3f(n[0],
                   n[1],
                   n[2]);

        for(int v = 0; v < 3; v++) {
            glVertex3f(vertices[triangles[i].v[v]].p[0],
                       vertices[triangles[i].v[v]].p[1],
                       vertices[triangles[i].v[v]].p[2]);
        }

    }

    glEnd();
}

#pragma mark - Loading

bool Mesh::loadMesh(const char *filename, bool randomizeTriangulation) {
    Material defaultMat;
    map<string, unsigned int> materialIndex;
    char s[LINE_LEN];
    float x, y, z;
    std::string matname, path, realFilename, temp;
    std::vector<int> vhandles, texhandles, nhandles;
    size_t pos;
    FILE *in;

    vertices.clear();
    triangles.clear();
    texcoords.clear();

    if (randomizeTriangulation) {
        srand(0);
    }

    materials.clear();
    defaultMat.setKd(0.5f,0.5f,0.5f);
    defaultMat.setKa(0.f,0.f,0.f);
    defaultMat.setKs(0.5f,0.5f,0.5f);
    defaultMat.setNs(96.7f);
    //defaultMat.set_Ni();
    //defaultMat.set_Tr();
    defaultMat.setIl(2);
    defaultMat.setName(std::string("StandardMaterialInitFromTrimesh"));
    materials.push_back(defaultMat);

    //we replace the \ by /
    realFilename = std::string(filename);
    for (unsigned int i = 0; i < realFilename.length(); ++i) {
        if (realFilename[i]=='\\') {
            realFilename[i]='/';
        }
    }

    temp = std::string(realFilename);
    pos = temp.rfind("/");

    if (pos == string::npos) {
        path = "";
    } else {
        path = temp.substr(0, pos + 1);
    }

    memset(&s, 0, LINE_LEN);

    in = fopen(filename,"r");

    while(in && !feof(in) && fgets(s, LINE_LEN, in)) {
        // comment
        if (s[0] == '#' || isspace(s[0]) || s[0] == '\0')
            continue;

        // material file
        else if (strncmp(s, "mtllib ", 7) == 0) {
            std::string file;
            //            char mtlfile[128];
            char *p0 = s + 6, *p1;

            while(isspace(*++p0)) {};
            p1 = p0;

            std::string t = p1;
            size_t i;
            for (i = 0; i < t.length(); ++i) {
                if (t[i] < 32 || t[i] == (char)255)
                    break;
            }

            if (t.length() == i) {
                file = path.append(t);
            } else {
                file = path.append(t.substr(0, i));
            }

            printf("Load material file %s\n", file.c_str());
            loadMaterial(file.c_str(), materialIndex);
        }
        // usemtl
        else if (strncmp(s, "usemtl ", 7) == 0) {
            char *p0 = s + 6, *p1;

            while(isspace(*++p0)) {
            };

            p1 = p0;

            while(!isspace(*p1)) {
                ++p1;
            }

            *p1 = '\0';
            matname = p0;

            if (materialIndex.find(matname) == materialIndex.end()) {
                printf("Warning! Material '%s' not defined in material file. Taking default!\n", matname.c_str());
                matname = "";
            }
        }
        // vertex
        else if (strncmp(s, "v ", 2) == 0) {
            sscanf(s, "v %f %f %f", &x, &y, &z);
            vertices.push_back(Vector3f(x, y, z));
        }
        // texture coord
        else if (strncmp(s, "vt ", 3) == 0) {
            //we do nothing
            Vector3f texCoords(0, 0, 0);

            //we only support 2d tex coords
            sscanf(s, "vt %f %f", &texCoords[0], &texCoords[1]);
            texcoords.push_back(texCoords);
        }
        // normal
        else if (strncmp(s, "vn ", 3) == 0) {
            Vector3f normal(0, 0, 0);

            sscanf(s, "vn %f %f %f", &normal[0], &normal[1], &normal[2]);
            normals.push_back(normal);
        }
        // face
        else if (strncmp(s, "f ", 2) == 0) {
            int component(0), nV(0);
            bool endOfVertex(false);
            char *p0, *p1(s + 2); //place behind the "f "

            vhandles.clear();
            texhandles.clear();
            nhandles.clear();

            while (*p1 == ' ') {
                ++p1; // skip white-spaces
            }

            while (p1) {
                p0 = p1;

                // overwrite next separator

                // skip '/', '\n', ' ', '\0', '\r' <-- don't forget Windows
                while (*p1 != '/' && *p1 != '\r' &&
                       *p1 != '\n' &&
                       *p1 != ' ' && *p1 != '\0') {
                    ++p1;
                }

                // detect end of vertex
                if (*p1 != '/') endOfVertex = true;

                // replace separator by '\0'
                if (*p1 != '\0') {
                    *p1 = '\0';
                    p1++; // point to next token
                }

                // detect end of line and break
                if (*p1 == '\0' || *p1 == '\n') {
                    p1 = 0;
                }


                // read next vertex component
                if (*p0 != '\0') {
                    switch (component) {
                        case 0: { // vertex
                            int tmp = atoi(p0) - 1;
                            vhandles.push_back(tmp);
                        }
                            break;
                        case 1: { // texture coord
                            int tmp = atoi(p0) - 1;
                            texhandles.push_back(tmp);
                        }
                            break;
                        case 2: { // normal
                            //assert(!vhandles.empty());
                            //assert((unsigned int)(atoi(p0)-1) < normals.size());
                            //_bi.set_normal(vhandles.back(), normals[atoi(p0)-1]);
                            int tmp = atoi(p0) - 1;
                            nhandles.push_back(tmp);
                        }
                            break;
                    }
                }

                ++component;

                if (endOfVertex) {
                    component = 0;
                    nV++;
                    endOfVertex = false;
                }
            }

            if (vhandles.size() != texhandles.size()) {
                texhandles.resize(vhandles.size(),0);
            }

            if (vhandles.size() > 3) {
                //model is not triangulated, so let us do this on the fly...
                //to have a more uniform mesh, we add randomization
                unsigned int k = (false) ? (rand() % vhandles.size()) : 0;
                for (unsigned int i = 0; i < vhandles.size() - 2; ++i) {
                    const int v0 = (k + 0) % vhandles.size();
                    const int v1 = (k + i + 1) % vhandles.size();
                    const int v2 = (k + i + 2) % vhandles.size();

                    const int t0 = (k + 0) % vhandles.size();
                    const int t1 = (k + i + 1) % vhandles.size();
                    const int t2 = (k + i + 2) % vhandles.size();

                    const int m  = (materialIndex.find(matname))->second;

                    triangles.push_back(Triangle(vhandles[v0], texhandles[t0],
                                                 vhandles[v1], texhandles[t1],
                                                 vhandles[v2], texhandles[t2]));
                    triangleMaterials.push_back(m);
                }
            } else if (vhandles.size() == 3) {
                Triangle t(vhandles[0], texhandles[0],
                           vhandles[1], texhandles[1],
                           vhandles[2], texhandles[2]);

                /*if(nhandles.size() == 3 && normals.size() > 0) {
                    t.n[0] = nhandles[0];
                    t.n[1] = nhandles[1];
                    t.n[2] = nhandles[2];
                }*/

                triangles.push_back(t);

                triangleMaterials.push_back((materialIndex.find(matname))->second);
            } else {
                fprintf(stderr, "Trimesh::LOAD: Unexpected number of face vertices (<3). Ignoring face.\n");
            }
        }
        memset(&s, 0, LINE_LEN);
    }

    fclose(in);
    return true;
}

bool Mesh::loadMaterial(const char *filename, std::map<string, unsigned int> &materialIndex)
{
    FILE *in;

    in = fopen(filename, "r" );
    if (!in) {
        printf("  Warning! Material file '%s' not found!\n", filename);
        return false;
    }

    char line[LINE_LEN];
    std::string textureName;
    std::string key;
    Material mat;
    float f1,f2,f3;
    bool indef = false;

    memset(line, 0, LINE_LEN);
    while(in && !feof(in)) {
        fgets(line, LINE_LEN, in);

        if (line[0] == '#') // skip comments
        {
            memset(line, 0, LINE_LEN);
            continue;
        } else if( isspace(line[0])||line[0]=='\0') {
            if (indef && !key.empty() && mat.isValid()) {
                if (materialIndex.find(key) == materialIndex.end()) {
                    mat.setName(key);
                    materials.push_back(mat);
                    materialIndex[key] = (unsigned int)materials.size() - 1;
                }
                mat.cleanup();
            }

            if (line[0]=='\0') {
                break;
            }
        }
        else if (strncmp(line, "newmtl ", 7)==0) // begin new material definition
        {
            char *p0 = line + 6, *p1;

            while(isspace(*++p0)) {};
            p1=p0;

            while(!isspace(*p1)) {
                ++p1;
            }

            *p1 = '\0';
            key = p0;
            indef = true;
        }
        else if (strncmp(line, "Kd ", 3) == 0) // diffuse color
        {
            sscanf(line, "Kd %f %f %f", &f1, &f2, &f3);
            mat.setKd(f1, f2, f3);
        }
        else if (strncmp(line, "Ka ", 3) == 0) // ambient color
        {
            sscanf(line, "Ka %f %f %f", &f1, &f2, &f3);
            mat.setKa(f1, f2, f3);
        }
        else if (strncmp(line, "Ks ", 3) == 0) // specular color
        {
            sscanf(line, "Ks %f %f %f", &f1, &f2, &f3);
            mat.setKs(f1, f2, f3);
        }
        else if (strncmp(line, "Ns ", 3) == 0) // Shininess [0..200]
        {
            sscanf(line, "Ns %f", &f1);
            mat.setNs(f1);
        }
        else if (strncmp(line, "Ni ", 3) == 0) // Shininess [0..200]
        {
            sscanf(line, "Ni %f", &f1);
            mat.setNi(f1);
        }
        else if (strncmp(line, "illum ", 6) == 0) // diffuse/specular shading model
        {
            int illum = -1;
            sscanf(line, "illum %i", &illum);
            mat.setIl(illum);
        }
        else if (strncmp(line, "map_Kd ",7) == 0) // map images
        {
            std::string t = &(line[7]);
            if (!t.empty() && t[t.length() - 1] == '\n') {
                t.erase(t.length() - 1);
            }

            // map_Kd, diffuse map
            // map_Ks, specular map
            // map_Ka, ambient map
            // map_Bump, bump map
            // map_d,  opacity map
            // just skip this
            mat.setTextureName(t);

        }
        else if (strncmp(line, "Tr ", 3) == 0 ) // transparency value
        {
            sscanf(line, "Tr %f", &f1);
            mat.setTr(f1);
        }
        else if (strncmp(line, "d ", 2) == 0 ) // transparency value
        {
            sscanf(line, "d %f", &f1);
            mat.setTr(f1);
        }
        
        if (feof(in) && indef && mat.isValid() && !key.empty()) {
            if (materialIndex.find(key)==materialIndex.end()) {
                mat.setName(key);
                materials.push_back(mat);
                materialIndex[key] = (unsigned int)materials.size() - 1;
            }
        }
        memset(line,0,LINE_LEN);
    }
    unsigned int msize = (unsigned int)materials.size();
    printf("%u  materials loaded.\n", msize);
    
    fclose(in);
    
    return true;
}
