#ifndef VERTEX3D_H
#define VERTEX3D_H

#include "Point3d.h"
#include <vector>

class Vertex3d : public Point3d {
private:

public:
    // Construtores
    Vertex3d(dividing x = dividing(0, 1), dividing y = dividing(0, 1), dividing z = dividing(0, 1));
    Vertex3d(const std::string& color, dividing x, dividing y, dividing z);
    Vertex3d(const Point3d& point);

};

#endif // VERTEX3D_H
