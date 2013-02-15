:title: PostgreSQL + PostGIS Service
:description: dotCloud features PostgreSQL 9.0. You can also request a PostGIS enabled PostgreSQL for use in your geographic applications.
:keywords: dotCloud documentation, dotCloud service, PostgreSQL, PostGis, geographic application

PostgreSQL and PostGIS
======================

.. include:: hawarning.inc
.. include:: ../dotcloud2note.inc

dotCloud features `PostgreSQL <http://www.postgresql.org/>`_ 9.0.
You can also request a `PostGIS <http://postgis.refractions.net/>`_ enabled
PostgreSQL for use in your geographic applications.


Basics
------

If your application requires a PostgreSQL database, all you need to do is
add the following lines to your dotcloud.yml :doc:`../guides/build-file`:

.. code-block:: yaml

   data:
     type: postgresql

If you want to have a PostgreSQL server with PostGIS extensions, specify
``postgis`` instead:

.. code-block:: yaml

   data:
     type: postgis

.. include:: database.inc

::

  dotcloud info data


You can use the "root" user, or create your own users if you prefer.
If you want to create the user "joe", run the following command::

  dotcloud run data -- createuser joe --pwprompt

This will create the user "joe" and prompt you for a password for this
new user. Be sure to pick a secure password.

You can also decide to use the default "template1" database, or create
your own. If you want to create your own database, you can run the
following command::

  dotcloud run data -- createdb mydb

And, if you want it to belong to the "joe" user created in the previous step::

  dotcloud run data -- createdb -O joe mydb

.. include:: environment-json.inc

Your mynicedb.data service will expose the following variables:

* ``DOTCLOUD_DATA_SQL_HOST``
* ``DOTCLOUD_DATA_SQL_LOGIN``
* ``DOTCLOUD_DATA_SQL_PASSWORD``
* ``DOTCLOUD_DATA_SQL_PORT``
* ``DOTCLOUD_DATA_SQL_URL``

Note that DATA will be replaced by the (upper-cased) name of your service.
