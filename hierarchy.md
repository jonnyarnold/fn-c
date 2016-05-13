# Built-in object zoo.
# This is the ideal case...

Object = (obj) {

  # Returns a reference to itself.
  this = <builtin>

  # Returns true if two objects have the same value.
  eq = (other) { <builtin> }

  # Returns a string representation of the object.
  asString = () { <builtin> }

  # Returns an object with this object's properties,
  # then the given object's properties.
  merge = (other) { <builtin> }

  # In-place version of `merge()`.
  merge! = (other) { <builtin> }

  # This function is run when the object is referenced like:
  # obj(1)
  # By default, it returns the property given by the argument.
  call = (propertyName) { <builtin> }

  # Iterates over the properties of the object,
  # calling the given function for each one.
  each = (f) { <builtin> }
}


Value : {eq, asString, properties, arity, call, merge, this}
  + Bool : {and, or}
  + Number : {+, -, *, /, lessThan, moreThan}
    + Int : { mod }
    + Float : {}
  + String : {}
  + Block
    + TopBlock : {print, import, not}
  + Def : {}
  + List : {each}
