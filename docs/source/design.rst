Design
#########################

These are the core guiding principles to the continued development of Ratscript.
This is not a scratchpad. Careful consideration should be made before adding,
revising, or removing points herein.

.. NOTE:: The Ratscript examples here should be kept up to date with the syntax.

Syntax
==================

Alignment
------------------

Code blocks should be denoted with a preceding character on each line, defined
herein as a "subordination operator". The depth of "subordination" should
directly correlate with the number of subordination operators preceding the
line.

Whitespace has long been used, both formally (Python) and informally (C) to
define code blocks and their depth. This can make determining the depth of a
particular line difficult without context, and leaves the code vulnerable
to indentation-related errors and formatting problems.

Example::

    if foo == true
    :if bar == 42
    ::print("The answer!")
    :else
    ::print("Hello, world.")
    :end if

Algebraic Conformity
----------------------

All operators and syntax choices should be consistent with standard algebraic
syntax and behavior, as far as is possible and practical. This includes all
mathematical operators, function calls, and operator precedence.

Example::

    print(5+3) #Addition
    print(5-3) #Subtraction
    print(5*3) #Multiplication
    print(5/3) #Division
    print(5^3) #Exponent
    print(log(3)) #Logarithm
