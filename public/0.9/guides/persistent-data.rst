:title: Persistent Data
:description: How to store arbitrary persistent data on a dotCloud service
:keywords: dotCloud documentation, quick start guide, persistent data, data directory

Persistent Data
===============

You can store arbitrary persistent data on a *stateless* dotCloud
service. Any service that runs application code is considered stateless
by dotCloud. For example, :doc:`Python </services/python>`, :doc:`Ruby
</services/ruby>`, :doc:`NodeJS </services/nodejs>` as well as the
:doc:`worker services </guides/daemons>` are considered stateless.

dotCloud recognizes only one directory as persistent on a service,
this directory is: ``~/data/`` [#]_. If you want to store any kind of
file on the local file system from your application, then ``~/data/`` is
the directory to use. Any other directory is considered *volatile* and
will *not* be persisted across pushes.

Moreover, there are two things you need to know when you use the
``~/data/`` directory:

- The ``~/data/`` directory is not shared across your services nor your
  services instances. If you :ref:`horizontally scale <scaling_horizontally>`
  your application, then the content of the ``~/data/`` directory will
  *not* be replicated;
- You will no longer have “zero-downtime pushes”, since the platform
  will have to stop your old services and move the ``~/data/`` directory
  before it can start the new version of your application.

That is why we don't encourage the use of a ``~/data/`` directory and
rely on a third party service to store anything (that could be a
database running on dotCloud, a blob store like `Amazon S3 <http://aws.amazon.com/s3/>`_…).

----

.. [#] For most services this will translate to ``/home/dotcloud/data/``.
