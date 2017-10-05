# The Fn VM (uVM)

uVM is a novel register-based virtual machine designed for use with the Fn programming language. Its design tradeoffs are:

- ???
- ???
- ???

## uVM bytecode

uVM bytecode contains a list of blocks. Each block is of the form

```
header
staticData
instructions
```

The header breaks down as follows:

```
arity[byte] numRegisters[byte]
```

- `arity` denotes the number of arguments to this block, and can be either `-1` (any number of arguments), `0`, `1`, `2`.
- `numRegisters` denotes the number of registers to assign for this block.

The header is of fixed size (2 bytes).


The static data section breaks down as follows:

```
staticDataLength[word] (...data)
```

The static data section is an array of `staticDataLength` values. Each data element is a value (see the Values section).


The instruction section is similar to the static data section as it is an array, but instead of values we store instructions:

```
instructionLength[word] (...instructions)
```

Each bytecode instruction is of the form:

```
instructionId[byte] operand1[word] operand2[word] operand3[word]
```

On a 64-bit machine, this means that each instruction is 25 bytes.

When an instruction is read, the `instructionId` is decoded to determine what to do. The instruction determines the meaning of the 3 general-purpose `operand` words that follow.

## uVM architecture


# uVM values


## uVM instructions

