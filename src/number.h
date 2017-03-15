// Defines the Number type for the VM.
//
// The number type is a version of Douglas Crockford's decimal number type,
// but done badly.

#pragma once

#include <cstdint> // intXX_t
#include <cmath> // pow, abs, ...
#include <string> // std::string, std::string::npos
#include <iostream>

namespace fn {

  // Types for the exponent and coefficient of a Number.
  typedef int8_t Exponent;
  typedef int64_t Coefficient;

  class Number {
  public:
    Exponent exponent;
    Coefficient coefficient;

    Number(Exponent exponent, Coefficient coefficient) {
      this->exponent = exponent;
      this->coefficient = coefficient;
    }

    Number(std::string str) {
      // To work out what the representation is:
      // 1. The component is the number, without the ".",
      // 2. The exponent is the distance of the "." from the end of the string
      //    (or 0 if it's not there).
      size_t dotPos = str.find(".");
      if (dotPos == std::string::npos) {
        this->coefficient = atoi(str.c_str());
        this->exponent = 0;
      } else {
        // Here's an ASCII diagram to explain this one...
        //
        // -123.456 = -123456 * 10^(-3)
        //     ^                     ^
        // 01234                     ^
        //     3210                  ^
        //     ^---------------------^
        size_t dotPosFromEnd = (str.size() - 1) - dotPos;

        this->exponent = -1 * dotPosFromEnd;

        str.erase(dotPos, 1); // Remove the dot so we can parse the coefficient.

        this->coefficient = atoi(str.c_str());
      }


    }

    Number() = default;

    std::string toString() {
      std::string returnValue = std::to_string(this->coefficient);

      if (this->exponent != 0) {
        returnValue += " * 10^" + std::to_string(this->exponent);
      }
      
      return returnValue;
    }

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
      Exponent exponentDifference = lhs.exponent - rhs.exponent;
      Coefficient coefficientMultiplier = (int)pow(10, abs(exponentDifference));

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
      Exponent exponentDifference = lhs.exponent - rhs.exponent;
      Coefficient coefficientMultiplier = (int)pow(10, abs(exponentDifference));

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

    friend bool operator==(Number lhs, Number rhs) {
      // Get both numbers to the same base.
      //
      // Here's an example:
      // lhs = 200 * 10^3
      // rhs = 2   * 10^5
      // exponentDifference = 3 - 5 = -2
      // so we multiply rhs.coefficient by 100 and drop the coefficient...
      // lhs = 200 * 10^5
      // rhs = 200 * 10^5
      Exponent exponentDifference = lhs.exponent - rhs.exponent;
      Coefficient coefficientMultiplier = (int)pow(10, abs(exponentDifference));

      if (exponentDifference < 0) {
        rhs.coefficient *= coefficientMultiplier;
        rhs.exponent = lhs.exponent;
      } else {
        lhs.coefficient *= coefficientMultiplier;
        lhs.exponent = rhs.exponent;
      }

      return lhs.coefficient == rhs.coefficient
        && lhs.exponent == rhs.exponent;
    }
  };

}
