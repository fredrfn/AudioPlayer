#include "sounds/maths/complex.hpp"
#include "sounds/maths/commons.hpp"

Complex::Complex(double real, double imaginary) : real(real), imaginary(imaginary) {}

double Complex::modulus() const { 
    return sqrt(modulusSquared()); 
}

double Complex::modulusSquared() const { 
    return pow(real, 2) + pow(imaginary, 2); 
}

double Complex::argument() const { 
    return atan2(imaginary, real); 
}

Complex Complex::conjugate() const { 
    return {real, -imaginary}; 
}

Complex Complex::reciprocal() const { 
    return conjugate() / modulusSquared(); 
}

Complex Complex::operator+(Complex other) const { 
    return {real + other.real, imaginary + other.imaginary}; 
}

Complex Complex::operator-(Complex other) const { 
    return {real - other.real, imaginary - other.imaginary}; 
}

Complex Complex::operator*(Complex other) const { 
    return {
        (real * other.real) - (imaginary * other.imaginary), 
        (real * other.imaginary) + (imaginary * other.real) 
    };
}

Complex Complex::operator*(double other) const { 
    return { real * other, imaginary * other }; 
}

Complex Complex::operator/(double other) const { 
    return { real / other, imaginary / other }; 
}

Complex Complex::operator/(Complex other) const { 
    return (*this) * other.reciprocal(); 
}

Complex Complex::fromPolars(double modulus, double argument) { 
    return { modulus * cos(argument), modulus * sin(argument) }; 
}

Complex Complex::exponential(double argument) { 
    return fromPolars(1, argument); 
}
