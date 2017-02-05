// Defines the Number type for the VM.
//
// The number type is a version of Douglas Crockford's decimal number type,
// but done badly.

#pragma once

#include <cmath> // pow, abs, ...

// Types for the exponent and coefficient of a Number.
typedef signed char fnExponent;
typedef signed long long int fnCoefficient;

class fnVMNumber {
public:
  fnExponent exponent;
  fnCoefficient coefficient;

  fnVMNumber(fnExponent exponent, fnCoefficient coefficient) {
    this->exponent = exponent;
    this->coefficient = coefficient;
  }

  fnVMNumber() = default;

  friend fnVMNumber operator*(fnVMNumber lhs, fnVMNumber rhs) {
    fnVMNumber result;
    
    result.exponent = lhs.exponent + rhs.exponent;
    result.coefficient = lhs.coefficient * rhs.coefficient;

    // TODO: Detect overflow and correct

    return result;
  }

  friend fnVMNumber operator/(fnVMNumber lhs, fnVMNumber rhs) {
    fnVMNumber result;
    
    result.exponent = lhs.exponent - rhs.exponent;
    result.coefficient = lhs.coefficient / rhs.coefficient;

    // TODO: Divide by 0?

    return result;
  }

  friend fnVMNumber operator+(fnVMNumber lhs, fnVMNumber rhs) {
    fnVMNumber result;

    // Get both numbers to the same base.
    fnExponent exponentDifference = lhs.exponent - rhs.exponent;
    fnCoefficient coefficientMultiplier = (int)pow(10, abs(exponentDifference));

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

  friend fnVMNumber operator-(fnVMNumber lhs, fnVMNumber rhs) {
    fnVMNumber result;

    // Get both numbers to the same base.
    fnExponent exponentDifference = lhs.exponent - rhs.exponent;
    fnCoefficient coefficientMultiplier = (int)pow(10, abs(exponentDifference));

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