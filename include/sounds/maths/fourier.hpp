#ifndef SOUNDS_MATHS_FOURIER_HPP
#define SOUNDS_MATHS_FOURIER_HPP

#include <vector>
#include "sounds/maths/complex.hpp"

std::vector<Complex> fft(const std::vector<float>& input);
std::vector<Complex> fft(const std::vector<Complex>& input, bool isLengthPowerOfTwo = false);
std::vector<Complex> ifft(const std::vector<Complex>& input);

class FourierSeries {
protected:
    double pulse;
    std::vector<double> _an;
    std::vector<double> _bn;
public:
    FourierSeries(double period, std::vector<double> an, std::vector<double> bn);
    unsigned long long termsCount() const;
    virtual double an(unsigned long long n) const;
    virtual double bn(unsigned long long n) const;
    virtual double amplitude(unsigned long long n) const;
    virtual double phase(unsigned long long n) const;
    virtual double evaluate(double t) const;
    double period() const;
    double frequency() const;
};

class FourierSeriesAmplitudePhase : public FourierSeries {
public:
    using FourierSeries::FourierSeries;
    virtual double amplitude(unsigned long long n) const override;
    virtual double phase(unsigned long long n) const override;
    virtual double an(unsigned long long n) const override;
    virtual double bn(unsigned long long n) const override;
    virtual double evaluate(double t) const override;
    FourierSeries toFourierSeries();
    static FourierSeriesAmplitudePhase fromFourierSeries(const FourierSeries& series);
};

#endif