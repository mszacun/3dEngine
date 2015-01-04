#ifndef MATERIAL_H
#define MATERIAL_H

enum RGBComponent { RED, GREEN, BLUE, ALPHA };

class Material
{
    public:
        // 0 <= GetDiffuse() <= 1
        double GetDiffuse(int component, int x, int y, int z) const { return component == RED ? 1 : 0; }
        // The shininess specifies how small the highlights are: the shinier, the smaller the highlights.
        double GetShiness() const { return 200; };
        // 0 <= GetSpecular() <= 1
        double GetSpecular(int component, int x, int y, int z) const { return component == RED ? 0.1 : 0; }
        int GetOwnLigth(int component, int x, int y, int z) const { return component == BLUE ? 40 : 0; };

    private:
};

#endif
