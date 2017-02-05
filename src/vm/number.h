// Defines the Number type for the VM.
//
// The number type is a version of Douglas Crockford's decimal number type,
// but done badly.

#pragma once

#include <cmath> // pow, abs, ...

namespace fn { namespace vm {

  namespace number {
    // Types for the exponent and coefficient of a Number.
    typedef signed char Exponent;
    typedef signed long long int Coefficient;
  }

  class Number {
  public:
    number::Exponent exponent;
    number::Coefficient coefficient;

    Number(number::Exponent exponent, number::Coefficient coefficient) {
      this->exponent = exponent;
      this->coefficient = coefficient;
    }

    Number() = default;

    friend Number operator*(Number lhs, Number rhs) {
      Number result;
      
      result.exponent = lhs.exponent + rhs.exponent;
      result.coefficient = lhs.coefficient * rhs.coefficient;

      // TODO: Detect overflow and correct

      return result;
    }

    friend Number operator/(Number lhs, Number rhs) {
      Number result;
      
      result.exponent = lhs.exponent - rhs.exponent;
      result.coefficient = lhs.coefficient / rhs.coefficient;

      // TODO: Divide by 0?

      return result;
    }

    friend Number operator+(Number lhs, Number rhs) {
      Number result;

      // Get both numbers to the same base.
      number::Exponent exponentDifference = lhs.exponent - rhs.exponent;
      number::Coefficient coefficientMultiplier = (int)pow(10, abs(exponentDifference));

      if (exponentDifference < 0) {
        rhs.coefficient *= coefficientMultiplier;
        result.exponent = lhs.exponent;
      } else {
        lhs.coefficient *= coefficientMultiplier;
        result.exponent = rhs.exponent;
      }
      
      result.coefficient = lhs.coefficient + rhs.coefficient;

      // TODO: Detect overflow and correct

      return result;
    }

    friend Number operator-(Number lhs, Number rhs) {
      Number result;

      // Get both numbers to the same base.
      number::Exponent exponentDifference = lhs.exponent - rhs.exponent;
      number::Coefficient coefficientMultiplier = (int)pow(10, abs(exponentDifference));

      if (exponentDifference < 0) {
        rhs.coefficient *= coefficientMultiplier;
        result.exponent = lhs.exponent;
      } else {
        lhs.coefficient *= coefficientMultiplier;
        result.exponent = rhs.exponent;
      }
      
      result.coefficient = lhs.coefficient - rhs.coefficient;

      // TODO: Detect overflow and correct

      return result;
    }
  };
  

}}


