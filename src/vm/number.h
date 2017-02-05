// Defines the Number type for the VM.
// The number type is a version of Douglas Crockford's decimal number type,
// but done badly.

#pragma once

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
};