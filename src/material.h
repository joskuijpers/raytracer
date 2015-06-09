#pragma once

/**
 * Material class.
 *
 * A material is the CAS colors, plus the texture name and tex coords.
 */
class Material
{
public:
    Material() {
        cleanup();
    };

    Material(const Material &m) {
        *this = m;
    };

    Material& operator=(const Material &m) {
        Kd = m.Kd;
        Kd_is_set = m.Kd_is_set; // diffuse
        Ka = m.Ka;
        Ka_is_set = m.Ka_is_set; // ambient
        Ks = m.Ks;
        Ks_is_set = m.Ks_is_set; // specular

        Ns = m.Ns;
        Ns_is_set = m.Ns_is_set; // specular
        Ni = m.Ni;
        Ni_is_set = m.Ni_is_set; // illumination

        Tr = m.Tr;
        Tr_is_set = m.Tr_is_set; // transparency (use this value to trade off reflection/refraction

        Il = m.Il;
        name = m.name;

        return (*this);
    };

    void cleanup()
    {
        Kd_is_set = false;
        Ka_is_set = false;
        Ks_is_set = false;
        Ns_is_set = false;
        Ni_is_set = false;
        Tr_is_set = false;
        Il_is_set = false;

        textureName = "empty";
    }

    bool isValid(void) const {
        return Kd_is_set || Ka_is_set || Ks_is_set || Tr_is_set;
    }

    bool hasKd(void) { return Kd_is_set; }
    bool hasKa(void) { return Ka_is_set; }
    bool hasKs(void) { return Ks_is_set; }
    bool hasNs(void) { return Ns_is_set; }
    bool hasNi(void) { return Ni_is_set; }
    bool hasIl(void) { return Il_is_set; }
    bool hasTr(void) { return Tr_is_set; }

    void setKd(float r, float g, float b) {
        Kd = vector3f(r,g,b);
        Kd_is_set = true;
    }

    void setKa(float r, float g, float b) {
        Ka = vector3f(r,g,b);
        Ka_is_set = true;
    }

    void setKs(float r, float g, float b) {
        Ks = vector3f(r,g,b);
        Ks_is_set = true;
    }

    void setNs(float r) {
        Ns = r;
        Ns_is_set = true;
    }

    void setNi(float r) {
        Ni = r;
        Ni_is_set = true;
    }

    void setIl(int r) {
        Il = r;
        Il_is_set = true;
    }

    void setTr(float t) {
        Tr = t;
        Tr_is_set = true;
    }

    void setTextureName(const std::string &s) {
        textureName = s;
    }

    void setName(const std::string &s) {
        name = s;
    }

    const vector3f& getKd(void) const { return Kd; }
    const vector3f& getKa(void) const { return Ka; }
    const vector3f& getKs(void) const { return Ks; }

    float getNi(void) const { return Ni; }
    float getNs(void) const { return Ns; } // shininess

    int getIl(void) const { return Il; }
    float getTr(void) const { return Tr; } //can be hijacked, e.g., for transparency

    const std::string& getTextureName() { // name of the texture image file
        return textureName;
    }

    const std::string& getName() { // name of the material
        return name;
    }

private:

    // diffuse
    vector3f Kd;
    bool Kd_is_set;

    // ambient
    vector3f Ka;
    bool Ka_is_set;

    // specular
    vector3f Ks;
    bool Ks_is_set;

    // shininess
    float Ns;
    bool Ns_is_set;

    float Ni;
    bool Ni_is_set;

    // illumination model
    int Il;
    bool Il_is_set;
    
    // transperency
    float Tr;
    bool Tr_is_set;
    
    std::string name;
    std::string textureName;
    
    friend class mesh;
};
