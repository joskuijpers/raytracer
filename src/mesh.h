#pragma once

#include <vector>
#include <map>
#include <string>

#include "vertex.h"

//material class of the mesh
//while colors seem useful, also texture names are loaded
//texture coordinates are also supported,
//YOU DO NOT have to use textures, if you do not want to!
//The materials are loaded from the mesh's .mtl file.
//You can modify the materials yourself in a text editor
//try it for the dodge (car model) that is provided
class material
{
public:
    material() {
        cleanup();
    };

    material(const material &m) {
        *this = m;
    };

    material& operator=(const material &m) {
        Kd_ = m.Kd_;
        Kd_is_set_ = m.Kd_is_set_; // diffuse
        Ka_ = m.Ka_;
        Ka_is_set_ = m.Ka_is_set_; // ambient
        Ks_ = m.Ks_;
        Ks_is_set_ = m.Ks_is_set_; // specular

        Ns_ = m.Ns_;
        Ns_is_set_ = m.Ns_is_set_; // specular
        Ni_ = m.Ni_;
        Ni_is_set_ = m.Ni_is_set_; // specular

        Tr_ = m.Tr_;
        Tr_is_set_ = m.Tr_is_set_; // transparency (use this value to trade off reflection/refraction

        illum_ = m.illum_;
        name_ = m.name_;

        return (*this);
    };

    void cleanup()
    {
        Kd_is_set_ = false;
        Ka_is_set_ = false;
        Ks_is_set_ = false;
        Ns_is_set_ = false;
        Ni_is_set_ = false;
        Tr_is_set_ = false;
        illum_is_set_=false;
        name_="empty";
    }

    bool is_valid(void) const
    { return Kd_is_set_ || Ka_is_set_ || Ks_is_set_ || Tr_is_set_; }

    bool has_Kd(void) { return Kd_is_set_; }
    bool has_Ka(void) { return Ka_is_set_; }
    bool has_Ks(void) { return Ks_is_set_; }
    bool has_Ns(void) { return Ns_is_set_; }
    bool has_Ni(void) { return Ni_is_set_; }
    bool has_illum(void) { return illum_is_set_; }
    bool has_Tr(void) { return Tr_is_set_; }

    void set_Kd( float r, float g, float b )
    { Kd_=vector3f(r,g,b); Kd_is_set_=true; }

    void set_Ka( float r, float g, float b )
    { Ka_=vector3f(r,g,b); Ka_is_set_=true; }

    void set_Ks( float r, float g, float b )
    { Ks_=vector3f(r,g,b); Ks_is_set_=true; }

    void set_Ns( float r)
    { Ns_=r;    Ns_is_set_=true; }

    void set_Ni( float r)
    { Ni_=r;    Ni_is_set_=true; }

    void set_illum( int r)
    { illum_=r;    illum_is_set_=true; }

    void set_Tr( float t )
    { Tr_=t;            Tr_is_set_=true; }

    void set_textureName(const std::string & s)//name of the texture image file
    {
        textureName_=s;
    }

    void set_name(const std::string & s )
    {
        name_=s;
    }

    const vector3f& Kd( void ) const { return Kd_; } //diffuse
    const vector3f& Ka( void ) const { return Ka_; } //ambiant
    const vector3f& Ks( void ) const { return Ks_; } //specular
    float  Ni( void ) const { return Ni_; }
    float  Ns( void ) const { return Ns_; } //shininess
    int       illum(void)const { return illum_;}
    float  Tr( void ) const { return Tr_; }//can be hijacked, e.g., for transparency
    const std::string & textureName()//name of the texture image file
    {
        return textureName_;
    }

    const std::string & name()//name of the material
    {
        return name_;
    }

private:

    vector3f Kd_;         bool Kd_is_set_; // diffuse
    vector3f Ka_;         bool Ka_is_set_; // ambient
    vector3f Ks_;         bool Ks_is_set_; // specular
    float Ns_;                     bool Ns_is_set_;
    float Ni_;                     bool Ni_is_set_;
    int illum_;                     bool illum_is_set_;//illumination model
    float Tr_;         bool Tr_is_set_; // transperency
    std::string        name_;
    std::string        textureName_;
};


/**
 * Triangle class.
 *
 * A triangle contains 3 indices to refer to vertex positions
 * and 3 indices to refer to texture coordinates (optional)
 */
class triangle {
public:
    inline triangle () {
        v[0] = v[1] = v[2] = 0;
    }
    inline triangle (const triangle & t2) {
        v[0] = t2.v[0];
        v[1] = t2.v[1];
        v[2] = t2.v[2];

        t[0] = t2.t[0];
        t[1] = t2.t[1];
        t[2] = t2.t[2];

    }
    inline triangle (unsigned int v0, unsigned int t0, unsigned int v1, unsigned int t1, unsigned int v2, unsigned int t2) {
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;

        t[0] = t0;
        t[1] = t1;
        t[2] = t2;
    }
    inline virtual ~triangle () {}
    inline triangle & operator= (const triangle & t2) {
        v[0] = t2.v[0];
        v[1] = t2.v[1];
        v[2] = t2.v[2];
        t[0] = t2.v[0];
        t[1] = t2.v[1];
        t[2] = t2.v[2];
        return (*this);
    }
    //vertex position
    unsigned int v[3];
    //texture coordinate
    unsigned int t[3];
};

/**
 * Mesh class.
 */
class mesh {
public:
    mesh(){}
    inline mesh (const std::vector<vertex> & v, const std::vector<triangle> & t) : vertices (v), triangles (t)  {}
    bool loadmesh(const char * filename, bool randomizeTriangulation);
    bool loadMtl(const char * filename, std::map<std::string, unsigned int> & materialIndex);
    void computeVertexNormals ();
    void draw();
    void drawSmooth();

    //Vertices are the vertex positions, and normals of the mesh.
    std::vector<vertex> vertices;
    //texCoords are the texture coordinates, these are DIFFERENT indices in triangles.
    //in the current version, if you use textures, then you have to use texture coords everywhere
    //for convenience, vector3f is used, although only 2D tex coordinates are stored (x,y entry of the vector3f).
    std::vector<vector3f> texcoords;
    //triangles are the indices of the vertices involved in a triangle.
    //A triangle, thus, contains a triplet of values corresponding to the 3 vertices of a triangle.
    std::vector<triangle> triangles;
    //These are the material properties
    //each triangle (!), NOT (!) each vertex, has a material.
    //Use the triangle index to receive a material INDEX
    std::vector<unsigned int> trianglematerials;
    //using the material index, you can then recover the material from this vector
    //the class material is defined just above
    std::vector<material> materials;

    //As an example:
    //triangle triangles[i] has material index trianglematerials[i]
    //and uses material materials[trianglematerials[i]].
};
