#ifndef DIVIDING_H
#define DIVIDING_H

class dividing
{
private:
    float x;
    float y;

public:
    dividing(float x = 0, float y = 1);

    // Sobrecarga dos operadores
    dividing operator + (const dividing& other) const;
    dividing operator - (const dividing& other) const;
    dividing operator * (const dividing& other) const;
    dividing operator / (const dividing& other) const;
    dividing& operator = (const dividing& other);
    bool operator < (const dividing& other);
    bool operator > (const dividing& other);
    bool operator == (const dividing& other);
    bool operator != (const dividing& other);
    dividing abs();

    // Métodos auxiliares
    float get_numerator() const;
    float get_denominator() const;
    void set_numerator(const float x);
    void set_denominator(const float y);
    float calculate() const;
};

#endif
