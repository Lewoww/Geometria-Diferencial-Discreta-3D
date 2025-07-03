#ifndef EDGE3D_H
#define EDGE3D_H

#include "Vertex3d.h"
#include <string>

class Edge3d {
private:
    Vertex3d p1;
    Vertex3d p2;
    float _length;
    std::string color;

public:
    // Construtores
    Edge3d(const std::string& color = "", const Vertex3d& p1 = Vertex3d(), const Vertex3d& p2 = Vertex3d());
    Edge3d(const Vertex3d& p1, const Vertex3d& p2);

    // Métodos auxiliares
    Vertex3d getP1() const;
    void setP1(const Vertex3d& p1);

    Vertex3d getP2() const;
    void setP2(const Vertex3d& p2);

    std::string getColor() const;
    void setColor(const std::string& color);

    void computeLength(); // calcula _length com base nas posições atuais
    float getLength() const;
};

#endif // EDGE3D_H
