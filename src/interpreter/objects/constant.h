#pragma once

// Represents any constant value in Fn.
// The type T is the C-type that the value
// belongs to.
template<class T>
class fnConstant : public fnValue {
public:

  // The C-value of the constant.
  T value;

  fnConstant(T value) { this->value = value; }

  virtual std::size_t hash() override {
    return (std::size_t)this->value;
  }

  virtual std::string asString(int indentationLevel) override = 0;
};
