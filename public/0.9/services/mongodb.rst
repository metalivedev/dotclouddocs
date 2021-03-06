:title: MongoDB Service
:description: How to launch your MongoDB service on the dotCloud platform.
:keywords: dotCloud documentation, dotCloud service, MongoDB, MongoDB 2.2.2, MongoDB basics, manual configuration, environment.json, mange MongoDB users, scale up, scale down, replicate MongoDB servers, replica set, offload reads

MongoDB
=======

.. include:: ../dotcloud2note.inc

The dotCloud's MongoDB service uses `MongoDB 2.2.2 <http://www.mongodb.org/>`_.

In this documentation, you will see how to launch your MongoDB service, deal
with the MongoDB authentication model, use `Replica Sets
<http://www.mongodb.org/display/DOCS/Replica+Sets>`_ to get automatic failover
and split reads. We will also see some basic troubleshooting commands.

Basics
------

You can add a MongoDB service to your app, or deploy a standalone MongoDB
server, by adding the following lines to your :doc:`dotcloud.yml
<../guides/build-file>`:

.. code-block:: yaml

   data:
     type: mongodb

.. include:: database.inc

::

   dotcloud info data

You can find the database credentials under ``ports``.

.. include:: environment-json.inc

The MongoDB service exposes the following variables:

- ``DOTCLOUD_DATA_MONGODB_PORT``: port where the MongoDB server is listening for
  incoming requests;
- ``DOTCLOUD_DATA_MONGODB_HOST``: host where the MongoDB server is running;
- ``DOTCLOUD_DATA_MONGODB_LOGIN``: login for the "admin" database (always "root"
  on dotCloud);
- ``DOTCLOUD_DATA_MONGODB_PASSWORD``: password for the "root" user;
- ``DOTCLOUD_DATA_MONGODB_URL``: a ``mongodb://`` URL with all of the above.
  This is certainly the easiest way to configure your MongoDB driver.

.. warning::

  The exact names of the environment variables depend on the name of the service
  in your ``dotcloud.yml``. Here we have DATA as in "mynicedb.data". If your
  service is named "db" in your ``dotcloud.yml``, then the environment variables
  will be named ``DOTCLOUD_DB_MONGODB_PORT`` and so on. This schema allows you
  to have several MongoDB services in your application.

Manage MongoDB Users and Databases
----------------------------------

Before you can do anything on your MongoDB server you will need to authenticate
yourself. By default we create a user "root" on the database "admin". In
MongoDB, users are different for each database. However successfully authenticating against the "admin"
database grants privileges on all databases.

The password for the "root" user of the "admin" database is shown when you do
``dotcloud info``.

Use it as follows to get a MongoDB root shell::

   dotcloud run data mongo

In the MongoDB prompt that opens, type the following commands::

   use admin
   db.auth("root", "GDpPRN2auFvBVzfjbca2");

Create a New Database
^^^^^^^^^^^^^^^^^^^^^

Database creation is implicit: there is no command to create a new database, the
first object saved in a database automatically creates it.

In the following example, we create a database named "metrics" by authenticating
against the "admin" database, and then creating an object into "metrics"::

   dotcloud run data mongo

And::

   use metrics
   switched to db metrics
   db.getSisterDB("admin").auth("root", "GDpPRN2auFvBVzfjbca2");
   db.my_collection.save({"object": 1});
   db.my_collection.count();

.. note::

   Obviously, using the default "root" account to access your databases is
   actually a bad practice. In the next sections, we will see how to create new
   users.

Add a User to a Database
^^^^^^^^^^^^^^^^^^^^^^^^^

Adding a user to a database is quite simple from a MongoDB root shell. The
following example creates a user named "jack" with the password "OpenSesame" in
our previously created "metrics" database::

   dotcloud run data mongo

Then::

   use metrics
   db.getSisterDB("admin").auth("root", "GDpPRN2auFvBVzfjbca2");
   mynicedb.data:PRIMARY> db.addUser("jack", "OpenSesame");

.. note::

   You should of course use better passwords than "OpenSesame". We generally
   use *pwgen* to generate passwords.

Now, if our new "jack" user wants to add read-only access to a new "jane" user,
he can do the following::

   dotcloud run data mongo

Then::

   use metrics
   db.auth("jack", "OpenSesame");
   db.addUser("jane", "JaneSecretPassword", true);

Replica Sets
------------

.. sidebar:: Different words for the same thing

   In MongoDB, the master server is called PRIMARY and the slave (or standby)
   server is called SECONDARY.

MongoDB's Replica Sets are a form of asynchronous replication between different
MongoDB servers. They are the equivalent of master/slave in other kind of
databases. Replica Sets make your database highly available thanks to automatic
failover and recovery. They can also help you to scale up by offloading reads
to the secondary servers.

Scaling Up and Down
^^^^^^^^^^^^^^^^^^^

At any time, you can scale up from a single server to a Replica Set with up to 5
nodes. This is done with the ``dotcloud scale`` command, for example, to scale the
MongoDB service called "data" in the application "mynicedb" the syntax is::

   dotcloud scale data:instances=3

.. warning::

   You can only scale to an *odd* number of nodes to make sure that a majority
   is always reached when a new primary must be elected. Even numbers of nodes
   will be ignored.

When you scale up, the new nodes will automatically synchronize against the
primary, you can follow the progress of the synchronization with "dotcloud
info"::

   dotcloud info data


You can use ``dotcloud scale`` to scale down too. Check out the :doc:`Scaling
guide </guides/scaling>` for detailed information about ``dotcloud scale``.

Using a Replica Set from your Application
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The best way to connect to a Replica Set is to use the MongoDB URL, called a
*connection string*, printed by ``dotcloud info`` and written in the
:doc:`/guides/environment`.

This URL contains the addresses of each MongoDB node in the Replica Set, this
guarantees that your MongoDB driver (client) will always be able to connect to
the database, even if some nodes are down.

If your MongoDB driver doesn't support connection strings, you can use the
regular host, port, username, password list found in the environment file.
Upon connection, MongoDB will tell the driver where the other nodes are in the
Replica Set. So the driver might be able to reconnect to another node after a
fail-over. But, this is not ideal because the node pointed to by the environment
file could be the one down.

Offload Reads to the Secondaries
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Thanks to the MongoDB replication implementation, it is very easy to issue the
reads to the secondary servers. It has the advantage of lowering the pressure on
the primary server, which is then only used for writes. This is a very
important concept that allows you to scale any application with a high read/write ratio.

You can read from the secondary servers using the `slave_okay <http://www.mongodb.org/display/DOCS/Querying#Querying-slaveOk%28QueryingSecondaries%29>`_
boolean argument in your MongoDB driver's functions. This is also supported in
the MongoDB shell::

    mynicedb.data:SECONDARY> rs.slaveOk();
    mynicedb.data:SECONDARY> db.test.findOne();
    { "_id" : ObjectId("4ec3f34429e5c4390a3aaf2d"), "test" : 42 }
    mynicedb.data:SECONDARY> 

Caveats
^^^^^^^

The replication to the secondary servers happens concurrently with the writes
made to the primary master (i.e: the replication is *asynchronous*). This means
that the secondary servers always lag a bit behind the primary: if you write a
continuous stream of data to your MongoDB Replica Set and if the primary dies;
the secondaries will likely miss the last transactions made on the primary.

While this replication lag might sound dangerous (since you can lose
transactions), one must keep in mind that a fully synchronous replication
system would incur a very high performance penalty. All writes would have to go
through extra round-trip transactions, with the primary waiting for the
secondaries to complete the write operation before reporting it as completed to
the application code.

This replication lag also means that reads made to the secondaries with
``slave_okay`` might not be consistent with the data on the primary. You need to
keep this in mind when you use it.

You can wait for and verify the propagation of writes to the secondaries with
the `getLastError <http://www.mongodb.org/display/DOCS/getLastError+Command>`_
command. Consult your MongoDB driver documentation to see how to use it.

Advanced Configuration
----------------------

You can modify some advanced parameters of the MongoDB configuration from your
"dotcloud.yml":

.. code-block:: yaml

   data:
     type: mongodb
     config:
       mongodb_oplog_size: 256
       mongodb_noprealloc: true
       mongodb_nopreallocj: true
       mongodb_smallfiles: true

Let's see what each setting does:

- ``mongodb_oplog_size``: this option controls the size (in megabytes) of the
  operations log used to save changes that need to be send to the secondary servers, it only
  make sense when you are using a Replica Set;
- ``mongodb_noprealloc``: this boolean setting disables file pre-allocation of
  databases files, we recommend to leave it to true for production on our
  cluster;
- ``mongodb_nopreallocj``: this boolean setting is the equivalent of
  ``mongodb_noprealloc`` but for journal files, we recommend to leave it to true;
- ``mongodb_smallfiles``: MongoDB allocates files in increments (64M, 128M…).
  This option forces the use of smaller increments, we recommend to leave it to
  true.

.. include:: ../guides/config-change.inc

Troubleshooting
---------------

Manage the MongoDB Process
^^^^^^^^^^^^^^^^^^^^^^^^^^

You can check if MongoDB is running with::

   dotcloud status data

You can read MongoDB's logs with::

   dotcloud logs data

Press Ctrl+C to stop watching the logs.

You can always restart MongoDB with::

   dotcloud restart data

You can also get useful information from a MongoDB root shell with:

.. code-block:: javascript

   use admin
   db.stats();
   db.serverStatus();

Compact or Repair the Database
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

MongoDB uses large preallocated memory mapped files to store your database's
objects. As you add and remove objects, fragmentation will occur in the memory
mapped files that wastes disk space [#]_. Running a repair on the database will
compact those files.

.. FIXME Adapt for Replica Set + include online repair techniques.

To run a repair you can ``dotcloud run myservice`` to your service and run::

   sudo stop mongodb
   rm -f /var/lib/mongodb/mongod.lock
   mongod --repair --config /etc/mongodb.conf
   sudo start mongodb

.. note::

   If your database consists of several gigabytes, the repair process may take several
   minutes. If your database is tens of gigabytes then the repair process may take 
   several hours.

See Also
--------

You can find the official MongoDB documentation here:
http://www.mongodb.org/display/DOCS/Home.

We also have a tutorial which explain how to use MongoDB with the Django Python
web framework:

.. toctree::
   :maxdepth: 1

   /tutorials/python/django-mongodb

----

.. [#] http://www.mongodb.org/display/DOCS/Excessive+Disk+Space
