Language Structure
######################


Ratscript draws inspiration from many languages, but it has no direct parents.
It is most closely comparable with Python, although many of its core features
differentiate it even there.

..  warning::
    **Design Principle:** Syntax is chosen for suitability and ease-of-use,
    *not* for familiarity or tradition.

Whitespace
==========

The Ratscript language largely ignores whitespace, except as important for
determining the boundaries of names.

We will formalize a :ref:`style`, which will include whitespace recommendations.

Line Termination
^^^^^^^^^^^^^^^^

All lines of code in Ratscript are simply terminated by the newline character
``\n``.

..  code-block:: ratscript

    print("Hello, world!")
    print("I am Ratscript.")

    #> Hello, world!
    #> I am Ratscript.


If the user wants to continue typing beyond one line, they can do so with the
ellipsis ``...`` token, as in the following example.

..  code-block:: ratscript

    print("Hello, world! I...
    am Ratscript.")

    #> Hello, world! I am Ratscript.


However, the ``...`` must be the last characters (except whitespace) on a line
for line continuation. The following would throw an error.

..  code-block:: ratscript

    COUNTEREXAMPLE
    print("My name is Bond... James
          Bond.")


The line continuation should work in all situations, as long as it's at the end
of a line.


..  code-block:: ratscript

    let x = 5 + 7 ...
            + 13 + 9

    x
    #> 34


Comments
========

Line and Inline Comments
^^^^^^^^^^^^^^^^^^^^^^^^

A line comment is denoted with the hash ``#`` at the start of the comment. This
symbol tells the compiler that everything until the newline is a comment,
allowing for both line and inline comments.

..  code-block:: ratscript

    # This is a single line comment.
    print("Hello, world!") # This is an inline comment.


Multiline Comments
^^^^^^^^^^^^^^^^^^

..  code-block:: ratscript

    ##
    This is a multiline comment.
    A multiline comment is denoted with `##` at the start,
    and `##` at the end.
    ##


After the initial ``##``, all subsequent ``#`` (and whitespace) are ignored
until another character is encountered. This is a valid multiline comment then:

..  code-block:: ratscript

    ### ### ### ### ###
    FANCY
    BANNER
    HERE
    ### ### ### ### ###



Documentation Commenting
^^^^^^^^^^^^^^^^^^^^^^^^

Documentation comments are denoted with ``#!``.

..  code-block:: ratscript

    #! This is a documentation comment.


..  code-block:: ratscript

    ##!
    You can create multiline documentation comment like this.
    The closing tag is the same as a normal multiline comment.
    ##


Terminal Output
^^^^^^^^^^^^^^^

All command line output begins with ``#>`` to make it a valid line comment, for
convenience when copy-pasting.

Error messages are also preceded with ``/!\`` to make them easier to spot.

..  code-block:: ratscript

    #> This is command line output.
    #> /!\ This is an error message.



Subordination
=============

Ratscript uses a very unique way of defining "blocks": the
**subordination operator**. While this is unusual (hey, brackets were too,
once!), it offers a few advantages, as you'll see.

Subordination Operator
^^^^^^^^^^^^^^^^^^^^^^

The subordination operator is the semicolon ``;``, chosen because of its
location on the home row on QWERTY keyboards.

The whitespace around the ``;`` operator is ignored, so you can use traditional
indenting as it suits your needs and preferences. The recommended style is used
herein, and outlined in :ref:`style`.

Here's the subordination operator in use:

..  code-block:: ratscript

    if foo
    ;  if bar
    ;  ;  if baz
    ;  ;  ; do_thing()
    ;  ;  else
    ;  ;  ; do_other_thing()
    ;  ;  end if
    ;  end if
    end if


This offers the unique advantage of being able to see how deeply nested ANY line
is out of context, merely by the number of ``;`` operators before it. What's
further, when space-padded in the recommended style depicted, it aids the eye in
drawing a direct line between statements at the same level. Third, it's more
visible than whitespace indentation.

By being able to "subordinate" any line to any other line, we can add additional
information to virtually any line. (See :ref:`attributes`.)

..  code-block:: ratscript

    make chinese_name as string
    ; <encoding "utf-16">


..  code-block:: ratscript

    make fibonacci(num as integer!)
    ;  <recursion MAX>
    ;  <return as integer>
    ;  if num < 2
    ;  ; return num
    ;  else
    ;  ; return fibonacci(num-1) + fibonacci(n-2)
    ;  end if


Multi-lining Statements
~~~~~~~~~~~~~~~~~~~~~~~

The subordination operator may appear in the middle of a line, for creating
one-line statements. (This is one other reason we chose the semicolon.)

For example, the following...

..  code-block:: ratscript

    if answer == 42
    ;  print("What's the question?")


...could be rewritten as...

..  code-block:: ratscript

    if answer == 42 ; print("What's the question?")


The depth of nesting still matters however, as seen here:

..  code-block:: ratscript

    if searching
    ; if answer == 42 ;; print("What's the question?")


We have to use two ``;;`` to separate, otherwise the suite will become
disconnected from its header.

Here are some more examples:

..  code-block:: ratscript

    if foo == 42; do_thing(); do_other_thing()

    if foo == 42
    ;  do_thing()
    ;  do_other_thing()

    make name = "Jason" ; <encoding=utf-8>


These, however, do not work:

..  code-block:: ratscript

    do_thing_one(); do_thing_two()  # invalid!
    do_thing_one();  # invalid, don't end with a semicolon


This helps enforce good style, as it's already considered bad practice in nearly
all languages to combine two disparate statements on the same line.

Attributes
==========

Ratscript allows applying attributes to anything. (This can be used for many
things.)

Attributes are defined in corner brackets ``< >``, and are subordinated to the
definition of whatever they modify.

..  code-block:: ratscript

    make name = "Jason"
    ;  <encoding="utf-8">

    make num = 65.9
    ;  <precision="double">

    make do_thing()
    ;  <recursion=100>
    ;  print("Hi!")
    ;  doThing() # this would stop recursing after a depth of 100


These are basically just compile-time variables/properties.

Built-In Attributes
^^^^^^^^^^^^^^^^^^^

Some attributes are built-in. You can also define a custom attribute on a class or function. (See :ref:`classes`)

..  list-table::
    :header-rows: 1

    * - Class Attributes
      -
    * - ``<key=member>``
      - The key member of a class, used for casting and hashing
    * - ``<private>``
      - A class member that is only visible from within the class

    * - ``<protected>``
      - A class member that is only writable from within the class

    * - ``<static>``
      - A static class member

..  list-table::
    :header-rows: 1

    * - Function Attributes
      -
    * - ``<throw>``
      - Function has the potential to throw an error



..  list-table::
    :header-rows: 1

    * - Variable Attributes
      -
    * - ``<encoding="utf-8">``
      - ``string`` encoding
    * - ``<precision="double">``
      - ``float`` precision


Keywords
==========


The following are keywords in Ratscript. Actual behavior will typically be
described elsewhere.

* ``assert``
* ``and``
* ``break``
* ``case``
* ``class``
* ``continue``
* ``define``
* ``elif`` or ``else if``
* ``else``
* ``end``
* ``fall``
* ``false``
* ``finally``
* ``for``
* ``if``
* ``let``
* ``make``
* ``nil``
* ``or``
* ``panic``
* ``pass``
* ``print``
* ``return``
* ``switch``
* ``test``
* ``this``
* ``until``
* ``warn``
* ``while``
