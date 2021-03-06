.. _syntax:

Syntax and grammar
==================

.. note:: The below is still a work-in-progress

.. note::

    The Blackbird used *directly* inside a Strawberry Fields Engine context
    is a **superset** of the Blackbird quantum assembly language described here,
    as it is embedded in a Python environment.

    You can think of it as 'Python-enhanced Blackbird'. The reason for this is twofold:

    1. We are already in a Python environment, we might as well allow the user
       to use Python functions and constructs, even if they are not 'officially'
       part of the Blackbird spec.

    .. 

    2. It would be near impossible to restrict the code inside the
       Strawberry Fields Engine context anyway. We could force the user to provide
       a string containing the Blackbird code, but this is ugly and not elegent.

    Currently, I like the SF Blackbird approach. It is close enough to
    Blackbird to look qualitatively the same in most cases.


Introduction
------------

In this section, we define the structure, syntax, and grammar of Blackbird code.

Philosophy
~~~~~~~~~~

Blackbird was designed from the ground up adhering to the following philosophies:

**It should encapsulate the universal ability of continuous-variable (CV) quantum computation.**

**It should be clear, concise, and simple to read and follow.** This simplicity should allow for both

* **Human readability** - operations and expressions should correspond to
  existing conventions, and allude to common notation in quantum computing.

* **Hardware execution** - code should be un-ambiguous, with one quantum operation per line.


**It should be easy to learn, using constructs and operators familiar in scientific computation**


**A Blackbird script should contain only one quantum algorithm or simulation,
making it an ideal format for saving and loading CV quantum algorithms.**

Similarity to Python
~~~~~~~~~~~~~~~~~~~~

To satisfy points 2 and 3, Blackbird deliberately is Python-like, inheriting
the following:

* Case sensitivity.

.. 

* ``#`` for line comments.

.. 

* Newlines at the same indentation level indicate the end of a statement.

.. 

* Indentation is used to denote programmatic 'blocks'.

.. 

* Operators and literals are similar to their Python equivalents.

.. 

* The ``with`` statement is used to denote the quantum device used to execute operations.

.. 

* After measurement, quantum modes are automatically and implicitly converted into
  classical registers.

.. 

* The resulting output is implicitly determined by the presence of measurement statements.

Differences to Python
~~~~~~~~~~~~~~~~~~~~~

Contrary to Python, however, we also introduce the following restrictions,
to enable Blackbird to function as a quantum assembly language across
a wide array of quantum hardware:

* All Blackbird scripts are separated into two main sections;

  - Variable declarations (optional)
  - The quantum program, denoted by the ``with`` statement (required)

.. 

* Statically typed - you must declare the variable type, and variables
  and arguments of conflicting types are **not** automatically cast to the correct type.

.. 

* Array variables may be declared, but Blackbird does not support array manipulation.


Variable declarations
---------------------

Variable may be optionally defined at the top of a Blackbird script, prior to the quantum program.

The syntax for defining variables is as follows:

.. code-block:: python

  type name = expression

with the following types supported:

* ``int``: ``0``, ``1``, ``5``
* ``float``: ``8.0``, ``0.43``, ``-0.123``, ``89.23e-10``
* ``complex``: ``0+5j``, ``8-1j``, ``0.54+0.21j``
* ``bool``: ``True``, ``False``
* ``str``: any ASCII string surrounded by quotes, ``"hello world"``

.. note::

    * When using a float, you must provide the full decimal. I.e., ``8`` and ``8.``
      are not valid floats, but ``8.0`` is.

    * When using a complex, you must provide both real and imaginary parts.
      I.e., ``8`` and ``2j`` are not valid complex literals, but ``8+0j`` is.

Examples:

.. code-block:: python

    int n = +5
    int k = n

    float m = -0.5432
    float alpha = 0.5432
    float x = 0.5+0.1
    float Delta = 0.543

    complex beta = 5.21
    complex y = -0.43e-4+0.912j
    complex z = +0.43e-4-0.912j

    bool flag = True
    str name = "program1"

Operators
~~~~~~~~~

Blackbird allows expressions using the following operators:

* ``+``: addition
* ``-``: subtraction, unary negation
* ``*``: multiplication
* ``/``: division
* ``**``: right-associative exponentiation.

.. note::

    * Blackbird will attempt to dynamically cast variables where it makes sense.
      For example, consider the following:

      .. code-block:: python

        int n = 2
        float x = 5.0**n

      Blackbird will automatically cast variable ``n`` to a float to perform the calculation.
      However, note that literals will not be automatically cast - ``float x = 5**n`` would
      return an error, as ``5`` is an ``int`` and not a float.

    * No matrix operations are defined; if the expression includes arrays, these operators will act in an elementwise manner.

Functions
~~~~~~~~~

Blackbird also supports the intrinsic functions

* ``exp()``
* ``sin()``
* ``cos()``
* ``sqrt()``

and the intrinsic constant

* ``pi``

You can also use previously defined variable names in your expressions:

.. code-block:: python

    float gamma = 2.0*cos(alpha*pi)
    float test = n**2.0

Arrays
~~~~~~

To define arrays, specify ``'array'`` after the variable type.
Each row of the array is then defined on an indented line, with
columns separated by commas.

.. code-block:: python

    float array A =
        -1.0, 2.0
        -0.1, 0.2

    complex array U[3, 3] =
        -0.23191638+0.17828953j,  0.58457815+0.41415933j, -0.05795454-0.46965132j
        +0.42259383+0.56368926j, -0.42219920+0.04735544j, -0.18902308-0.01590913j
        -0.02396850+0.64301446j,  0.09918161+0.36797446j,  0.26993055+0.30341975j


.. note::

    For additional array validation, you can specify the *shape* of the array using square
    brackets directly after the variable name (i.e. ``U[3, 3]``)
    but this is optional.

Quantum program
---------------

The ``with`` statement indicates the device to run the program on,
as well as providing device-specific options.

Inside the indented ``with`` block, all operations are queued
to be executed on the device, in the order they appear.

For example:

.. code-block:: python

    with fock(num_subsystems=1, cutoff_dim=7, shots=10000):
        # Statements within the 'with' block have the following form:
        Operation(parameters) | modes

        # Depending on the operation, parameters may be optional
        # Parameters can be variables of literals or expressions
        Coherent(alpha**2, Delta*sqrt(pi)) | 0

        # Multiple modes are specified by comma separated integers
        Interferometer(U) | [0, 1, 2, 3]

        # Finish with measurements
        MeasureFock() | 0

Currently, the device always accepts keyword arguments, and operations always accept
positional arguments.

After running a Blackbird program, the user should expect to receive the results
as an array:

* each column is a measurement result, corresponding to the measurements in the order they appear in the blackbird program,
* each row represents a shot/run.
