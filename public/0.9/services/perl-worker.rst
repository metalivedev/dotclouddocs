:title: Perl Worker Service
:description: dotCloud's Perl Worker service allows you to run code outside of a single HTTP request: in the background, or at set times via cron.
:keywords: dotCloud documentation, dotCloud service, PERL, perl worker, worker service

Perl Worker
===========

.. include:: ../dotcloud2note.inc
.. include:: worker.inc

.. include:: service-boilerplate.inc

.. code-block:: yaml

   worker:
     type: perl-worker

With your dotCloud build file in hands you are ready to :ref:`define your
daemons <guides-define-daemons>`.

Perl Versions
-------------

The perl-worker service supports three different branches of Perl (5.12.x,
5.14.x, 5.16.x), it will default to Perl 5.12.x unless you specify otherwise.
The current versions of each Perl branch are listed in the table below. Pick the
branch that works best for you.

+---------+---------+-----------+
| branch  | version | variable  |
+=========+=========+===========+
| 5.12.x* | 5.12.4  |  v5.12.x  |
+---------+---------+-----------+
| 5.14.x  | 5.14.2  |  v5.14.x  |
+---------+---------+-----------+
| 5.16.x  | 5.16.0  |  v5.16.x  |
+---------+---------+-----------+

\* Perl 5.12.x is the default

To configure your service to use a version of Perl other than 5.12.x, you will
set the ``perl_version`` config option in your ``dotcloud.yml``

For example, this is what you would have if you wanted Perl 5.14.x:

.. code-block:: yaml

   www:
     type: perl-worker
     approot: helloperl
     config:
       perl_version: v5.14.x

.. include:: /guides/config-change.inc

.. include:: perl-cron.inc

Specify Perl Dependencies
-------------------------

Just like for the :doc:`Perl service <perl>`, you can specify external
dependencies in a Makefile.PL or directly in your dotcloud.yml file with a
requirement list.
