#ifndef SOUNDS_MATHS_CARTHESIAN_COMPLEX_NUMBER_HPP
#define SOUNDS_MATHS_CARTHESIAN_COMPLEX_NUMBER_HPP

struct Complex {
    double real;
    double imaginary;
    Complex() : real(0.0), imaginary(0.0) {}
    explicit Complex(double real) : real(real), imaginary(0.0) {}
    explicit Complex(float real) : real(real), imaginary(0.0) {}
    Complex(double real, double imaginary);
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