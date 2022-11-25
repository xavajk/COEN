#include "poly.h"
#include <cmath>
#include <climits>
#include <iomanip>
#include <iostream>

using namespace std;

namespace coen79_lab4 {
    // The size of the maximum exponent permitted for a polynomial.
    const unsigned int polynomial::MAXIMUM_DEGREE;
    
    // HELPER MEMBER FUNCTIONS for the polynomial class
    void polynomial::update_current_degree() {
        /* POSTCONDITION: The current degree is set to the highest exponent
        with a non-zero coeficient. */
        for (size_type i = 0; i <= MAXIMUM_DEGREE; ++i) {
            if (poly[i] != 0) deg = i;
        }
    }


    // CONSTRUCTORS for the polynomial class
    polynomial::polynomial(double c, unsigned int exponent) {
        /* PRECONDITION: exponent <= MAXIMUM_DEGREE
        POSTCONDITION: This polynomial has been created with all zero
        coefficients, except for coefficient c for the specified exponent.
        When used as a default constructor (using default values for
        both arguments), the result is a polynomial with all zero
        coefficients. */
        assert(exponent <= MAXIMUM_DEGREE);
        for (size_type i = 0; i <= MAXIMUM_DEGREE; ++i) {
            if (i == exponent) poly[i] = c;
            else poly[i] = 0;
        }
        deg = exponent;
    }


    // MODIFICATION MEMBER FUNCTIONS for the polynomial class
    void polynomial::assign_coef(double coefficient, unsigned int exponent) {
        /* PRECONDITION: exponent <= MAXIMUM_DEGREE.
        POSTCONDITION: Sets the coefficient for the specified exponent. */
        assert(exponent <= MAXIMUM_DEGREE);
        poly[exponent] = coefficient;
        update_current_degree();
    }

    void polynomial::add_to_coef(double amount, unsigned int exponent) {
        /* PRECONDITION: exponent <= MAXIMUM_DEGREE.
        POSTCONDITION: Adds the given amount to the coefficient of the
        specified exponent. */
        assert(exponent <= MAXIMUM_DEGREE);
        poly[exponent] += amount;
        update_current_degree();
    }

    void polynomial::clear() {
        // POSTCONDITION: All coefficients of this polynomial are set to zero.
        for (size_type i = 0; i <= MAXIMUM_DEGREE; i++) {
            poly[i] = 0;
        }
    }


    // CONSTANT MEMBER FUNCTIONS for the polynomial class
    polynomial polynomial::antiderivative() const {
        /* PRECONDITION: degree() < MAXIMUM_DEGREE
        POSTCONDITION: The return value is the antiderivative (indefinite integral)
        of this polynomial.
        NOTE: The return polynomial always has a constant term of zero. */
        assert(degree() < MAXIMUM_DEGREE);
        polynomial ad;
        for (int i = degree() + 1; i > 0; --i) {
            ad.poly[i] = poly[i - 1] / i;
        }
        ad.poly[0] = 0;
        ad.deg = degree() + 1;
        return ad;
    }

    double polynomial::coefficient(unsigned int exponent) const {
        /* POSTCONDITION: Returns coefficient at specified exponent of this
        polynomial.
        NOTE: For exponents > MAXIMUM_DEGREE, the return value is always zero. */
        if (exponent > MAXIMUM_DEGREE) return 0.0;
        return poly[exponent];
    }

    double polynomial::definite_integral(double x0, double x1) const {
        /* POSTCONDITION: Returns the value of the definite integral computed from
        x0 to x1.  The answer is computed analytically. */
        double upper, lower;
        polynomial ad = antiderivative();
        upper = ad.eval(x1);
        lower = ad.eval(x0);
        return upper - lower;
    }

    unsigned int polynomial::degree() const {
        /* POSTCONDITION: The function returns the value of the largest exponent
        with a non-zero coefficient.
        If all coefficients are zero, then the function returns zero (even
        though, technically, this polynomial does not have a degree). */
        return deg;
    }

    polynomial polynomial::derivative() const {
        /* POSTCONDITION: The return value is the first derivative of this
        polynomial. */
        polynomial dv;
        for (size_type i = 0; i < MAXIMUM_DEGREE; ++i) {
            dv.poly[i] = poly[i + 1] * (i + 1);
        }
        dv.deg = degree() - 1;
        return dv;
    }

    double polynomial::eval(double x) const {
        /* POSTCONDITION: The return value is the value of this polynomial with the
        given value for the variable x. */
        double val = 0;
        for (size_type i = 0; i <= MAXIMUM_DEGREE; ++i) {
            val += poly[i] * pow(x, i);
        }
        return val;
    }

    bool polynomial::is_zero() const {
        /* POSTCONDITION: The return value is true if and only if the polynomial
        is the zero polynomial. */
        for (size_type i = 0; i <= MAXIMUM_DEGREE; ++i) {
            if (poly[i] != 0) {
                return false;
            }
        }
        return true;
    }

    unsigned int polynomial::next_term(unsigned int e) const {
        /* POSTCONDITION: The return value is the next exponent n which is LARGER
        than e such that coefficient(n) != 0.
        If there is no such term, then the return value is zero. */
        if (e > MAXIMUM_DEGREE - 1) return 0;
        for (size_type i = e + 1; i <= MAXIMUM_DEGREE; ++i) {
            if (poly[i] != 0) {
                return i;
            }
        }
        return 0;
    }

    unsigned int polynomial::previous_term(unsigned int e) const {
        /* POSTCONDITION: The return value is the next exponent n which is SMALLER
        than e such that coefficient(n) != 0.
        If there is no such term, then the return value is UINT_MAX
        from <climits>. */
        if (e < 1) return UINT_MAX; 
        for (int i = e - 1; i >= 0; --i) {
            if (poly[i] != 0) {
                return i;
            }
        }
        return UINT_MAX;
    }


    // NON-MEMBER BINARY OPERATORS
    polynomial operator +(const polynomial& p1, const polynomial& p2) {
        /* POSTCONDITION: return-value is a polynomial with each coefficient
        equal to the sum of the coefficients of p1 & p2 for any given
        exponent. */
        polynomial sum;
        for (polynomial::size_type i = 0; i <= polynomial::MAXIMUM_DEGREE; ++i) {
            sum.poly[i] = p1.poly[i] + p2.poly[i];
        }
        sum.update_current_degree();
        return sum;
    }

    polynomial operator -(const polynomial& p1, const polynomial& p2) {
        /* POSTCONDITION: return-value is a polynomial with each coefficient
        equal to the difference of the coefficients of p1 & p2 for any given
        exponent. */
        polynomial dif;
        for (polynomial::size_type i = 0; i <= polynomial::MAXIMUM_DEGREE; ++i) {
            dif.poly[i] = p1.poly[i] - p2.poly[i];
        }
        dif.update_current_degree();
        return dif;
    }

    polynomial operator *(const polynomial& p1, const polynomial& p2) {
        /* PRECONDITION: p1.degree() + p2.degree() <= MAXIMUM_DEGREE.
        POSTCONDITION: Each term of p1 has been multiplied by each term of p2,
        and the answer is the sum of all these term-by-term products.
        For example, if p1 is 2x^2 + 3x + 4 and p2 is 5x^2 - 1x + 7, then the
        return value is 10x^4 + 13x^3 + 31x^2 + 17x + 28. */
        assert(p1.degree() + p2.degree() <= polynomial::MAXIMUM_DEGREE);
        polynomial pd;
        // use nested loops by distributive property
        for (int i = 0; i <= p1.degree(); ++i) {
            for (int j = 0; j <= p2.degree(); ++j) {
                // check special conditions 
                if (i == 1 || j == 1) {
                    pd.poly[i*j + 1] += p1.poly[i] * p2.poly[j];
                    continue;
                } else if (i == 0) {
                    pd.poly[j] += p1.poly[i] * p2.poly[j];
                    continue;
                } else if (j == 0) {
                    pd.poly[i] += p1.poly[i] * p2.poly[j];
                    continue;
                }
                // else simply multiply
                pd.poly[i*j] += p1.poly[i] * p2.poly[j];
            }
        }
        pd.update_current_degree();
        return pd;
    }

    
    // NON-MEMBER OUTPUT FUNCTIONS
    ostream& operator << (ostream& out, const polynomial& p) {
        /* POSTCONDITION: The polynomial has been printed to ostream out, which,
        in turn, has been returned to the calling function.
        The format of the polynomial when printed should be as follows:
                -2.5x^2 - 4x + 5.9
        1) There should be exactly 1 space between the numbers and the operators.
        2) The constant coeficient should be printed by itself: 5.3, not 5.3x^0
        3) The first coeficient chould be followed by a plain x: 4x, not 4x^1
        4) The highest degree's coeficient should have a sign: -2.5x^2, but the
            following values should all be separated by appropriate operators
            and then printed unsigned: -2.5x^2 - 4x...
        5) If you manipulate any settings of the outstream inside of this,
            such as the precision, they should be restored to what they were
            at the start of the function before returning. */
        unsigned int hd = p.degree();
        streamsize ss = out.precision();
        string pos = " + ";
        string neg = " - ";

        // if polynomial is zero
        if (p.is_zero()) {
            out << "0.0";
            return out;
        }

        // else print all non zero terms starting with hd
        bool first = true;
        for (int i = hd; i >= 0; --i) {
            if (p.poly[i] == 0) continue;
            if (!first) {
                if (i == 1) {
                    out << (p.poly[i] < 0 ? neg : pos);
                    out << abs(p.poly[i]) << "x";
                } else if (i == 0) {
                    out << (p.poly[i] < 0 ? neg : pos);
                    out << abs(p.poly[i]);
                } else {
                    out << (p.poly[i] < 0 ? neg : pos);
                    out << abs(p.poly[i]) << "x^" << i;
                }
            } else {
                if (i == 1) {
                    out << fixed << setprecision(1) << p.poly[i] << "x";
                    first = false;
                } else if (i == 0) {
                    out << fixed << setprecision(1) << p.poly[i];
                    first = false;
                } else {
                    out << fixed << setprecision(1) << p.poly[i] << "x^" << i;
                    first = false;
                }
            }
        }
        out << setprecision(ss);
        return out;   
    }
}