:title: Python Worker Service
:description: dotCloud's Python Worker service allows you to run code outside of a single HTTP request: in the background, or at set times via cron.
:keywords: dotCloud documentation, dotCloud service, python worker

Python worker
=============

.. include:: worker.inc

.. include:: service-boilerplate.inc

.. code-block:: yaml

   worker:
     type: python-worker

With your DotCloud build file in hands you are ready to :ref:`define your
daemons <guides-define-daemons>`.

Specify Python dependencies
---------------------------

Just like for the :doc:`Python service <python>`, you can specify
external dependencies in a file called *requirements.txt*. For example::

  redis
  restkit == 2.3.3

If you specify a Python package installing executable scripts or binaries,
they will be correctly installed in the $PATH and you will be able
to run them without specifying their full path.


Python Versions
---------------

The python-worker service supports four different branches of Python (2.6, 2.7,
3.1, 3.2), it will default to Python 2.6 unless you specify otherwise. The
current versions of each Python branch are listed in the table below. Pick the
branch that works best for you.

+--------+---------+----------+
| branch | version | variable |
+========+=========+==========+
| 2.6*   | 2.6.5   |  v2.6    |
+--------+---------+----------+
| 2.7    | 2.7.2   |  v2.7    |
+--------+---------+----------+
| 3.1    | 3.1.2   |  v3.1    |
+--------+---------+----------+
| 3.2    | 3.2.2   |  v3.2    |
+--------+---------+----------+

\* Python 2.6 is the default

To configure your service to use a version of Python other than 2.6, you will
set the ``python_version`` config option in your ``dotcloud.yml``.

For example, this is what you would have if you wanted Python 2.7.x:

.. code-block:: yaml

   www:
     type: python-worker
     config:
       python_version: v2.7

