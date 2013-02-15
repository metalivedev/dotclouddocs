:title: Ruby Worker Service
:description: dotCloud's Ruby Worker service allows you to run code outside of a single HTTP request: in the background, or at set times via cron.
:keywords: dotCloud documentation, dotCloud service, Ruby worker

Ruby Worker
===========

.. include:: ../dotcloud2note.inc

.. include:: worker.inc


.. include:: service-boilerplate.inc

.. code-block:: yaml

   worker:
     type: ruby-worker

With your dotCloud Build File in hands you are ready to :ref:`define your
daemons <guides-define-daemons>`.

Dependencies
------------

As in the :doc:`Ruby service <ruby>` you can specify your
external dependencies in a *Gemfile*. For example:

.. code-block:: ruby

   source :rubygems
   gem 'redis'


.. include:: ruby-unsupported-gems.inc

.. include:: ruby-cron.inc

.. include:: ruby-custom-version.inc
