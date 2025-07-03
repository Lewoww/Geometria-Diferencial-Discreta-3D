#include <iostream>
#include "dividing.h"

dividing::dividing(float x, float y) : x(x), y(y) {}

#include <iostream>

static std::ostream& operator<<(std::ostream& os, const dividing& d) {
    os << "dividing(" << d.get_numerator() << ", " << d.get_denominator() << ")";
    return os;
}


//Sobrecarga dos operadores
dividing dividing::operator + (const dividing& other) const {
    float newX = this->x * other.y + other.x * this->y;
    float newY = this->y * other.y;
    return dividing(newX, newY);
}

dividing dividing::operator - (const dividing& other) const {
    float newX = this->x * other.y - other.x * this->y;
    float newY = this->y * other.y;
    return dividing(newX, newY);
}

dividing dividing::operator * (const dividing& other) const {
    float newX = this->x * other.x;
    float newY = this->y * other.y;
    return dividing(newX, newY);
}

dividing dividing::operator / (const dividing& other) const {
    float newX = this->x * other.y;
    float newY = this->y * other.x;
    return dividing(newX, newY);
}

dividing& dividing::operator = (const dividing& other) {
    if (this != &other) {
        this->x = other.x;
        this->y = other.y;
    }
    return *this;
}

bool dividing::operator<(const dividing& other) {
    return (x * other.y) < (other.x * y);
}

bool dividing::operator>(const dividing& other) {
    return (x * other.y) > (other.x * y);
}

bool dividing::operator == (const dividing& other) {
    return this->x * other.y == this->y * other.x;
}

bool dividing::operator!=(const dividing& other) {
    return !(*this == other); // Inverte o resultado da comparação de igualdade
}

dividing dividing::abs()
{
    // Verifica se o valor real da fração é negativo e inverte o sinal do numerador
    if (this->calculate() < 0) {
        return dividing(-this->get_numerator(), this->get_denominator());
    }
    else {
        return *this; // Se já for positivo, retorna o valor como está
    }
}


// Métodos auxiliares
float dividing::get_numerator() const {
    return x;
}

float dividing::get_denominator() const {
    return y;
}

void dividing::set_numerator(const float x) {
    this->x = x;
}

void dividing::set_denominator(const float y) {
    this->y = y;
}

float dividing::calculate() const {
    if (y != 0) {
        return x / y;
    }
    else {
        return 0;
    }
}
