#ifndef MATERIAL_H
#define MATERIAL_H

enum RGBComponent { RED, GREEN, BLUE, ALPHA };

class Material
{
    public:
        double GetDiffuse(int component, int x, int y, int z) const { return component == RED ? 1 : 0; }
        // double GetSpecular(int component, int x, int y, int z) const;
        // double GetAmbient(int component, int x, int y, int z) const;
        // int GetOwnLigth(int component, int x, int y, int z) const;

    private:
};

#endif
