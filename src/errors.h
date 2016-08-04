#pragma once

#include <exception>

// Denotes a location in the code.
class CodeLocation {
public:
  std::string fileName;
  int line;

  CodeLocation(std::string fileName, int line) {
    this->fileName = fileName;
    this->line = line;
  }

  std::string asString() {
    return this->fileName;
  }
};

// All expected exceptions from Fn
// should inherit from this type.
class FnError : public std::exception {
public:
  std::string errorText;
  CodeLocation* location;

  FnError(std::string errorText, CodeLocation* location) {
    this->errorText = errorText;
    this->location = location;
  }

  FnError(std::string errorText) {
    this->errorText = errorText;
  }

  // std::string getMessage() const {
  //   std::string result = this->errorText + "\n";
  //   if (this->location != NULL) {
  //     result += "@ " + this->location->asString() + "\n";
  //   }

  //   return result;
  // }

  virtual const char* what() const throw() {
    std::string result = this->errorText + "\n";
    if (this->location != NULL) {
      result += "@ " + this->location->asString() + "\n";
    }

    return result.c_str();
  }
  
};

// Called for issues when executing.
class FnRuntimeError : public FnError {
  using FnError::FnError;
};
