:title: Failover and reconnect
:description: How failover works and why you app need to support automatic reconnection
:keywords: dotCloud documentation, quick start guide, failover, dotCloud failover, reconnect, database, auto reconnect


Automatic reconnection
======================

Why?
----
Most apps today keep a persistent connection to their database for better performances, but when you do such if you do not handle a disconnection properly then you expose you app to failure.
To increase the availability or our app you should not presume your database connection is never going to be closed. Because it will eventually.

How?
----
There is no magic solution it both depends from the data store and the language/library combo you use.
Some make it very easy, mongodb-native in nodejs for example auto reconnect by default.
Other stacks may require a bit more effort.

.. note::

    For example, if you are developing a Flask application with a MongoDB using pymongo_, you will have to catch the `pymongo.errors.AutoReconnect` exception for every query and either pass or retry.


The easiest way to test if your application supports reconnecting is to restart the master of your data store cluster (if your service is call db, this could be db.0, go check on the dashboard overview tab)::
    
    dotcloud restart db.0

.. note::
    As restarting a container is not considered like a failure of the data store (it is a manual action) it will not trigger a failover on MySQL and Redis data stores.


As the master reboot you applications is likely going to through an error or hanged if the application retry to connect to the data store.
If your application is capable of handling automatic reconnection as the master up it should work normally. If it does not come back then your application is not handling disconnections properly.



Failover
========

.. warning::
    An application that does not support automatic reconnection will not failover properly.
    Make sure that your application handle data store connection lose.


MySQL & Redis
-------------
When a misbehaving master is detected we perform a failover of the data store and then route the traffic to the new master. As this process go through all active connection will be lost.

MongoDB
-------
MongoDB's failover_ is happens differently. The Replica Set will it self detect the failure of the master and elect a new one. As this goes through clients will get disconnected and will need to reconnect.

.. note::
    
    For a proper MongoDB failover make sure to use ``*_MONGODB_URL`` rather than ``*_MONGODB_HOST``, ``*_MONGODB_PORT``, ``*_MONGODB_LOGIN`` and ``*_MONGODB_PASSWORD`` environment variables.


.. _pymongo: pymongo http://api.mongodb.org/python/current/api/pymongo/errors.html?highlight=reconnect#pymongo.errors.AutoReconnect
.. _failover: http://docs.mongodb.org/manual/core/replication/#failover