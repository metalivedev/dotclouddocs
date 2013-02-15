:title: MySQL Service
:description: How to install a MySQL database that is highly available.
:keywords: dotCloud documentation, dotCloud service, MySQL, MySQL 5.1, environment.json, master/slave

MySQL
=====

.. include:: ../dotcloud2note.inc

dotCloud features `MySQL <http://dev.mysql.com/doc/>`_ 5.1 and uses the
InnoDB storage engine by default.

Basics
------

If your application requires a MySQL database, all you need to do is
to add the following lines to your dotcloud.yml :doc:`../guides/build-file`:

.. code-block:: yaml

   data:
     type: mysql

.. include:: database.inc

::

  dotcloud info data

If you plan to use this database service for only one thing in your app,
you can opt to directly use the system database called "mysql".
If you plan to store multiple things (e.g. a WordPress blog and a Django-based
CMS) and want to configure different access rights for them, or if you don't
feel comfortable with storing your data directly in the "mysql" system
database, here is how to set this up.

First, get a shell on the database::

  dotcloud run data -- mysql

You will be prompted by MySQL::

    Welcome to the MySQL monitor.  Commands end with ; or \g.
    Your MySQL connection id is 105
    Server version: 5.1.41-3ubuntu12.7 (Ubuntu)

    Type 'help;' or '\h' for help. Type '\c' to clear the current input
    statement.

    mysql> 

Let's create a new database called "wordpress"::

    CREATE DATABASE wordpress;

Now create a user "joe" with access rights on this database::

    GRANT ALL ON wordpress.* TO 'joe'@'%' IDENTIFIED BY 'opensesame';

Of course, you should provide a better password than 'opensesame'!
You can exit your MySQL shell by typing ``exit``.

.. include:: environment-json.inc

Your ramen.data service will expose the following variables:

* ``DOTCLOUD_DATA_MYSQL_HOST``
* ``DOTCLOUD_DATA_MYSQL_LOGIN``
* ``DOTCLOUD_DATA_MYSQL_PASSWORD``
* ``DOTCLOUD_DATA_MYSQL_PORT``
* ``DOTCLOUD_DATA_MYSQL_URL``

Note that DATA will be replaced by the (upper-cased) name of your service.

Master/Slave
------------

The MySQL service can be scaled to Master/Slave for high availability,
as explained in the :doc:`scaling guide </guides/scaling>`::

   dotcloud scale data:instances=2

Replication and fail-overs will happen automatically. No changes are
required to your code: the address and port returned by ``dotcloud info``
and recorded into :doc:`environment.json </guides/environment/>` will
not change over time (it always points to the current master). However
you have to make sure that your application aggressively tries to
reconnect to the database because connections will be killed during a
fail-over.

You can use ``dotcloud info`` to check the status of the replication::

   dotcloud info data

Notice the ``replication_lag`` value, this is the delay between a write
on the master, and the same write on the slave. It should usually be
very low. It gives a rough indication of the amount of data that would
be lost if the master was to fail unexpectedly. I.e., if the replication
lag is 10s, it means that losing the master would cause the data written
during the last 10 seconds to be lost.

While this replication lag might sound dangerous (since you can lose
transactions), one must keep in mind that a fully synchronous
replication system would incur a very high performance penalty. All
writes would have to go through an extra round-trip transaction, with
the master waiting for the slave to complete the write operation before
reporting it as completed to the application code.

Under the hood, the replication format used is MIXED. This format is
compatible with nondeterministic statements [#]_.

----

.. [#] http://dev.mysql.com/doc/refman/5.1/en/replication-rbr-safe-unsafe.html
