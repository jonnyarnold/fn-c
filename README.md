# Fn

Fn is a experimental programming language aiming to do the following:
1. Teach me about how programming languages work internally.
2. (Optional) Be a functioning programming language.

Want to know more? [Take the tour!](tour.fn)

## Behind the Scenes

Fn uses [Flex](http://flex.sourceforge.net/) and [Bison](https://www.gnu.org/software/bison/) for parsing. To run, it uses its own [bytecode](src/bytecode.cpp) and [virtual machine](src/vm).
