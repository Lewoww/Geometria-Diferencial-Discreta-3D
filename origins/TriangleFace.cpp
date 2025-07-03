#include "TriangleFace.h"

/*TriangleFace::TriangleFace(Vertex3d* a, Vertex3d* b, Vertex3d* c, float r, float, g, float b)
    : v1(a), v2(b), v3(c)}
 */

TriangleFace::TriangleFace(Vertex3d* a, Vertex3d* b, Vertex3d* c)
    : v1(a), v2(b), v3(c){
    Vertex3d V(10, 8, 7);

    Vertex3d u(v2->getX() - v1->getX(), v2->getY() -v1->getY(), v2->getZ() - v1->getZ());
    Vertex3d v(v3->getX() - v1->getX(), v3->getY() - v1->getY(), v3->getZ() - v1->getZ());
    Vertex3d w(dividing(0,1) - v1->getX(), dividing(0,1) - v1->getY(), dividing(20,1) - v1->getZ());

    Vertex3d p(u.getY() * v.getZ() - u.getZ() * v.getY(), u.getZ() * v.getX() - u.getX() * v.getZ(), u.getX() * v.getY() - u.getY() * v.getX());

    Vertex3d menp(dividing(-1)*p.getX(), dividing(-1) * p.getY(), dividing(-1) * p.getZ());

    p = menp;
    
    float prod = (p.getX() * w.getX() + p.getY() * w.getY() + p.getZ() * w.getZ()).calculate();

    dividing normP2 = p.getX() * p.getX() + p.getY() * p.getY() + p.getZ() * p.getZ();
    dividing normW2= w.getX() * w.getX() + w.getY() * w.getY() + w.getZ() * w.getZ();

    float normP = sqrt(normP2.calculate());
    float normW = sqrt(normW2.calculate());

    float cos_PW = prod / (normP * normW);
    /*
    if (cos_PW == 0) {
        r = 0;
        g = 1;
        k = 0;
    }
    if (cos_PW > 0) {
        r = cos_PW;
        g = 1;
        k = cos_PW;
    }
    else {
        r = 0;
        g = -cos_PW;
        k = 0;
    }
    */

    float qcgraus = sqrt(2) / 2;



    if (cos_PW == qcgraus) {
        r = 0;
        g = 1;
        k = 0;
    }
    if (cos_PW > qcgraus) {
        r = (cos_PW - qcgraus)/(1 - qcgraus);
        g = 1;
        k = (cos_PW - qcgraus)/ (1 - qcgraus);
    }
    if (cos_PW < qcgraus) {
        r = 0;
        g = cos_PW/qcgraus;
        k = 0;
    }
    if (cos_PW < 0) {
        r = 0;
        g = 0;
        k = 0;
    }


    /*
    r = static_cast<float>(rand()) / RAND_MAX;
    g = static_cast<float>(rand()) / RAND_MAX;
    k = static_cast<float>(rand()) / RAND_MAX;
    */
}



Vertex3d TriangleFace::get_V1() const {
    return *v1;
}

Vertex3d TriangleFace::get_V2() const {
    return *v2;
}

Vertex3d TriangleFace::get_V3() const {
    return *v3;
}

float TriangleFace::get_r() const
{
    return r;
}
float TriangleFace::get_g() const
{
    return g;
}
float TriangleFace::get_b() const
{
    return k;
}
