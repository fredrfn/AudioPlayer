#ifndef SOUNDS_MATHS_CARTHESIAN_COMPLEX_NUMBER_HPP
#define SOUNDS_MATHS_CARTHESIAN_COMPLEX_NUMBER_HPP

struct Complex {
    double real;
    double imaginary;
    Complex(double real = 0.0, double imaginary = 0.0);
    double modulus() const;
    double modulusSquared() const;
    double argument() const;
    Complex conjugate() const;
    Complex reciprocal() const;
    Complex operator+(Complex other) const;
    Complex operator-(Complex other) const;
    Complex operator*(Complex other) const;
    Complex operator*(double other) const;
    Complex operator/(double other) const;
    Complex operator/(Complex other) const;
    static Complex fromPolars(double modulus, double argument);
    static Complex exponential(double argument);
};

#endif