# The Fn VM (emuVM, or ΣμVM)

emuVM is a novel register-based virtual machine designed for use with the Fn programming language. Its design tradeoffs are:

- ???
- ???
- ???


## emuVM bytecode

emuVM bytecode contains a list of chunks. Each chunk is of the form

```
header
staticData
instructions
```

The header breaks down as follows:

```
arity[byte] numRegisters[byte]
```

- `arity` denotes the number of arguments to this chunk, and can be either `-1` (any number of arguments), `0`, `1` or `2`.
- `numRegisters` denotes the number of registers to assign for this chunk.

The header is of fixed size (2 bytes).


The static data section breaks down as follows:

```
staticDataLength[word] (...data)
```

The static data section is an array of data used by the chunk. It is used for data that does not fit into a value (see Values below),
such as strings.

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


## emuVM architecture

emuVM consists of an array of smaller VMs, with one VM per chunk. When a chunk is
defined, a specialised VM for that chunk is initialised. This allows each chunk to
manage its own registers and maintain lexical scoping easily.

Each MicroVM (or μVM) has the following structure:

```
{
  instructions : [Instruction]
  data: vector[byte]
  registers : [Value]
  counter : uint
  parent: *microVM
}
```

`instructions` contains the instructions for the chunk.

The `data` array is an expandable memory store. It is used for manipulating large values, like strings. It is initialised with the contents of `staticData`.

Registers are general-purpose value stores used for input, processing and output.

`counter` is used to keep track of where in the instruction set we are.

`parent` points to the parent VM. At the top level, this is empty. This is used for getting variables from elsewhere.

## emuVM values

Registers contain values. Each type of value in Fn is represented in 2 words. On a 64-bit machine,
this means every value is 16 bytes.

emuVM values are untyped; it is expected that all type checking has already been done by the compiler.

Booleans set all their bits to either all `1` (true) or all `0` (false).

Rationals use the first word for the numerator and the second word for the denominator. (Integers are Rationals with 1 as their denominator.)

Strings, blocks and lists use the first word to denote their size and the second word to point to the array index of the start of their data (defined in the `data` of the VM).

Functions only use the first word to point to the chunk index for the function.


## Special registers

While most registers are general purpose, each VM has a set of special registers, used for specific tasks:

- `ARG1`/`ARG2`: registers that hold the arguments passed to the block. For variadic blocks, `ARG1` is a list.
- `RETURN`: register for the return value of the VM.
- `SELF`: register for the current context.

## emuVM instructions

**Values**

- `SET registerId valueWord1 valueWord2`
- `EQ resultRegister inputRegister1 inputRegister2`
- `COPY destinationRegister sourceRegister`

**Booleans**

- `AND resultRegister inputRegister1 inputRegister2`
- `OR resultRegister inputRegister1 inputRegister2`
- `NOT resultRegister inputRegister`

**Rationals**

- `ADD resultRegister inputRegister1 inputRegister2`
- `SUB resultRegister inputRegister1 inputRegister2`
- `MUL resultRegister inputRegister1 inputRegister2`
- `DIV resultRegister inputRegister1 inputRegister2`
- `NEG resultRegister inputRegister`
- `MORETHAN resultRegister inputRegister1 inputRegister2` (r1 > r2)
- `LESSTHAN resultRegister inputRegister1 inputRegister2` (r1 < r2)

**Complex Data Structures**

- `GETDATA resultRegister pointerRegister offsetRegister` (result = pointer[offset])

**Strings**

- `CONCAT resultRegister inputRegister1 inputRegister2`
- `SLICE resultRegister inputRegister1 inputRegister2`

**Blocks**



**Lists**

- `APPEND resultRegister inputRegister1 inputRegister2`
- `GET

**Functions**

