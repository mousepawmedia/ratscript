Syntax and Design
####################


Vision
======

The design of Ratscript's syntax has three goals:

* Explicit: Make behavior obvious from the syntax.
* Simple: Lower the learning curve for beginning users.
* Elegant: Maintain usability by experienced programmers.

Glossary
^^^^^^^^

* *compound statement*: a statement made up of one or more *clauses*. For
  example, an ``if`` statement.
* *clause*; a single header-suite pair in a compound statement, such as the
  ``else`` clause in an ``if``/``else`` compound statement.
* *expression*: a unit of code that can be evaluated to a value.
* *header*: the top part of a clause, usually defining how and when the clause
  will be executed.
* *name*: a textual reference to a value in memory, in the context of a
  variable. Names have both scope and type, and are *bound* to a value.
* *statement*: a single, executable unit of code.
* *suite*: the body part of a clause, subordinated to a header.
* *value*: any object or piece of data in memory, which can be bound to one or
  more *names*. Values have type, but no scope.
* *variable*: a value in memory associated with a name.

..  note::
    In Python, names have scope, but no type; values have type, but no scope.
    In Ratscript, names have both scope and type, while values only have scope.
    Ratscript names can be "rebound" to a new value, as long as that value is
    the same type.

Document Conventions
======================

..  note::
    Notes, TODOs, and proposed statements are formatted like this (blue block)
    to make them easier to find.


..  warning::
    **Design Principles:** These are notes about design principles.


..  code-block:: none

    # This is a code example.
