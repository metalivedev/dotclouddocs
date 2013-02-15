:title: SSH Access
:description: How to SSH to a Service on the dotCloud platform.
:keywords: dotCloud documentation, quick start guide, SSH to service

SSH Access
==========

.. include:: ../dotcloud2note.inc

How to SSH to a Service
-----------------------

You can SSH to any service by running the ``dotcloud run`` command like so::

    dotcloud run www

For scaled services ``dotcloud run`` connects to the first instance but you can
connect to a specific instance by appending its number::

    dotcloud run www.1

The first instance is ``.0``.

How to Run a Single Command
---------------------------

You can run a single command in a service with the ``dotcloud run`` command::

    dotcloud run www -- ls -la

Like for ssh you can append the instance number to run a command on a specific
instance.

.. note::

   Note the double-dash after the service name. This will make sure that
   any extra dash-argument will be passed *as is* to the service, instead
   of being interpreted by the CLI.
