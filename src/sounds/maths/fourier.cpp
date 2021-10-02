#include "sounds/maths/fourier.hpp"
#include "sounds/maths/commons.hpp"

FourierSeries::FourierSeries(
    double period, 
    std::vector<double> an, 
    std::vector<double> bn
) : pulse(2.0 * PI / period), _an(std::move(an)), _bn(std::move(bn)) {}

unsigned long long FourierSeries::termsCount() const { 
    return __min(_an.size(), _bn.size()); 
}

double FourierSeries::an(unsigned long long n) const { 
    return n < termsCount() ? _an[n] : 0.0f; 
}

double FourierSeries::bn(unsigned long long n) const {
    return n < termsCount() ? _bn[n] : 0.0f; 
}

double FourierSeries::amplitude(unsigned long long n) const { 
    return Complex(an(n), bn(n)).modulus(); 
}

double FourierSeries::phase(unsigned long long n) const { 
    return -atan2(bn(n), an(n)); 
}

double FourierSeries::period() const { 
    return pulse != 0.0f ? 2.0 * PI / pulse : 0.0f; 
}

double FourierSeries::frequency() const { 
    return period() != 0.0f ? 1.0f / period() : 0.0f; 
}

double FourierSeriesAmplitudePhase::amplitude(unsigned long long n) const { 
    return n < termsCount() ? _an[n] : 0.0f; 
}

double FourierSeriesAmplitudePhase::phase(unsigned long long n) const { 
    return n < termsCount() ? _bn[n] : 0.0f; 
}

double FourierSeriesAmplitudePhase::an(unsigned long long n) const { 
    return sqrt(pow(amplitude(n), 2) - pow(bn(n), 2)); 
}

double FourierSeriesAmplitudePhase::bn(unsigned long long n) const { 
    return amplitude(n) / sqrt(1 + pow(tan(phase(n)), 2)); 
}

double FourierSeries::evaluate(double t) const {
    unsigned long long count = termsCount();
    if (count == 0) { return 0.0f; }
    double result = an(0)/2;
    for (unsigned long long n = 1; n < count; n++) {
        double x = pulse * n * t;
        result += (an(n) * cos(x)) + (bn(n) * sin(x));
    }
    return result;
}

double FourierSeriesAmplitudePhase::evaluate(double t) const { 
    unsigned long long count = termsCount();
    if (count == 0) { return 0.0f; }
    double result = amplitude(0)/2;
    for (unsigned long long n = 1; n < count; n++) {
        result += (amplitude(n) * cos((pulse * n * t) - phase(n)));
    }
    return result;
}

/**
 *  FFT: Fast Fourier Transform, based on the Cooley-Tukey algorithm
 *  NOTE: the implementation proposed below is NOT very optimized, it is designed to reflect accurately
 *  the Cooley Tukey algorithm and the "butterflies" concept. It is still perfectly usable.
 * 
 *  The discrete fourier transform of a discrete signal f of length N has the following expression: 
 *  DFT(f)(k) = SUM(from n = 0 to N-1) of f(n)(exp(-iwnk)
 *  Where w = 2PI/N
 *  The output of the DFT is a set of complex numbers, indexed by k above.
 * 
 *  A naive approach of this is doable, but with O(n2) complexity. Introducing FFT.
 * 
 *  Cooley-Tukey are very smart people. They observed that if you separate odd- and even-indexed samples
 *  you obtain the following result:
 * 
 *  DFT(f)(k) = DFT(evens)(k) + exp(-iwk)DFT(odds)(k)
 *  DFT(f)(k + N/2) = DFT(evens)(k) - exp(-iwk)DFT(odds)(k)
 * 
 *  That's very powerful and saves lots of computations: the benefits compounds itself if a divide and
 *  conquer approch is implemented to compute only size-2 DFTs, called butterflies and summing them.
*   
 *  Proof obtained thanks to:
 *  - elemental exp maths factorisaion
 *  - periodicity of the complex exponential (2PI-periodic, forming circle of magnitude 1 around (0,0))
 *      -> understood easily with the Euler's formula exp(iX) = cos X + isin X
 *          + Pythagorean circle equation x2 + y2 = R, and trigonometric identification
 *
 * 
 *  For optimization ideas:
 *  https://www.ti.com/lit/an/spra291/spra291.pdf
 **/

std::vector<Complex> fft(const std::vector<Complex>& input, bool isLengthPowerOfTwo) {
    if (input.size() < 2) {
        return input;
    }

    if (!isLengthPowerOfTwo) {
        unsigned long long nonPaddedSize = input.size();
        unsigned int result = 0;
        while (nonPaddedSize >>= 1) result++; 
        if (nonPaddedSize != pow(2, result)) {
            std::vector<Complex> resized = input;
            resized.resize((unsigned long long) pow(2, result + 1), Complex(0, 0));
            return fft(resized, true);
        }
    }

    unsigned long long N = input.size();

    std::vector<Complex> output;
    std::vector<Complex> evens;
    std::vector<Complex> odds;
    output.reserve(N);
    evens.reserve(N/2);
    odds.reserve(N/2);
    for (unsigned long j = 0; j < N; j++) {
        j % 2 ? evens.push_back(input[j]) : odds.push_back(input[j]);
    }

    std::vector<Complex> evenDFT = fft(evens, true);
    std::vector<Complex> oddDFT = fft(odds, true);

    for(unsigned long k = 0; k < N/2; k++) {
        // Compute the so-called "butterflies"
        Complex oddDFTPart = Complex::exponential(-2 * PI * k / N) * oddDFT[k];
        output[k] = evenDFT[k] + oddDFTPart;
        output[k + N/2] = evenDFT[k] - oddDFTPart;
    }

    return output;
}

// Proof can be found here: https://adamsiembida.com/how-to-compute-the-ifft-using-only-the-forward-fft/
// IFFT(f)(k) = (1/N) * conjugate(DFT(f)(conjugate(k)))
std::vector<Complex> ifft(const std::vector<Complex>& input) {
    std::vector<Complex> output;
    output.reserve(input.size());
    
    for (const auto& value: input) { output.push_back(value.conjugate()); }
    output = fft(output);
    for (auto& value: output) { value = (value.conjugate()/(double)output.size()); }
    
    return output;
}

FourierSeries FourierSeriesAmplitudePhase::toFourierSeries() { 
    std::vector<double> anCoeffs;
    std::vector<double> bnCoeffs;
    anCoeffs.reserve(termsCount());
    bnCoeffs.reserve(termsCount());
    for (unsigned long long i = 0; i < termsCount(); i++) { 
        anCoeffs.push_back(an(i));
        bnCoeffs.push_back(bn(i));
    }
    return FourierSeries(period(), anCoeffs, bnCoeffs); 
}

FourierSeriesAmplitudePhase FourierSeriesAmplitudePhase::fromFourierSeries(const FourierSeries& series) {
    std::vector<double> amplitudes;
    std::vector<double> phases;
    amplitudes.reserve(series.termsCount());
    phases.reserve(series.termsCount());
    for (unsigned long long i = 0; i < series.termsCount(); i++) { 
        amplitudes.push_back(series.amplitude(i));
        phases.push_back(series.phase(i));
    } 
    return FourierSeriesAmplitudePhase(series.period(), amplitudes, phases);
}