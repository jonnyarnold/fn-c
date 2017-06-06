The VM is a stack of *frames*. Each frame has its own:
- *Stack* of values.
- *Table* of symbols.
- *Return Index* (except for global).

In the rest of this section I will use:

- `[]` to denote the value stack (with the "top" value last); 
- `{}` to denote the table; 
- `[] {}` to denote a frame; and 
- `[] {} ; [] {}` to denote a VM with two frames (of which the first is the currently active frame).

## Constants

```
PUSH_FALSE(a) =  [a] {}
PUSH_TRUE
PUSH_NUMBER
PUSH_STRING
```

Push a constant value onto the value stack.

## Aliasing

```
NAME(#name) [a] {} = [a] {#name: a}
GET(#name) [] {} ; [a] {#name: a} = [a] {} ; [a] {#name: a}
```

Stores and retrieves names for values. Names do not have to belong to the current frame; name resolution is done along the frame stack until a name is found. If the name is not found, the VM terminates with an error.

## Built-In Operations

All operations destroy their arguments.

```
AND [a, b] {} = [a&&b] {}
OR
EQ
MUL
DIV
ADD
SUB
```

Binary operations destroy two arguments.

```
NOT [a] {} = [!a] {}
```

Unary operations replace the argument with its result.

## Functions

```
PUSH_DEF(numArgs) = [fn(arg1)] {}
CALL [fn(arg1), param1] {} = [param1] {#arg1: param1} ; [] {}
RETURN [a] {} ; [b] {} = [b, a] {}
```

- `PUSH_DEF` pushes a value onto the value stack that denotes the function definition: its position in the instruction set, number of arguments and argument names are stored.
- `CALL` creates a new frame and populates it with the given parameters. The function and its parameters are removed from the parent's value stack.
- `RETURN` takes the top of the current value stack and pushes it onto the value stack of the frame below. It also pops the current frame and replaces  off the frame stack and destroys it.

## Blocks

```
NEW_FRAME [] {} = [] {} ; [] {}
COMPRESS [a] {} ; [b] {} = [b, A] {} where A = [a] {}
EXPAND [b, A] {} = [a] {} ; [b] {}
```

- Blocks are essentially new frames. `NEW_FRAME` creates a new frame and puts it on top of the frame stack.
- `COMPRESS` stores the current frame and then pushes a reference to the it onto the value stack of the frame below. The frame is popped from the frame stack.
- `EXPAND` takes a reference to a frame from the value stack and pushes the frame onto the frame stack.

## Conditionals

`JUMP_IF_FALSE` advances the instruction counter if the top of the value stack is not true.

## TODO

- Variable arguments
- Lists
- Everything is a Block
