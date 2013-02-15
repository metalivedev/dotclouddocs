:title: Redis Service
:description: dotCloud's Redis service features Redis 2.4.11.
:keywords: dotCloud documentation, dotCloud service, Redis

Redis
=====

.. include:: ../dotcloud2note.inc

dotCloud features Redis 2.4.11. If you don't know Redis yet, the great docs and 
awesome tutorials are available at http://redis.io/.


Basics
------

If your application requires a Redis data store for caching or other purposes,
all you need to do is add the following lines to your dotcloud.yml 
:doc:`../guides/build-file`:

.. code-block:: yaml

   data:
     type: redis

.. include:: database.inc

::

  dotcloud info data

.. include:: environment-json.inc

Your ramen.data service will expose the following variables:

* ``DOTCLOUD_DATA_REDIS_HOST``
* ``DOTCLOUD_DATA_REDIS_LOGIN``
* ``DOTCLOUD_DATA_REDIS_PASSWORD``
* ``DOTCLOUD_DATA_REDIS_PORT``
* ``DOTCLOUD_DATA_REDIS_URL``

Note that DATA will be replaced by the (upper-cased) name of your service.


Redis CLI
---------

To start playing with Redis (or for debugging purposes),
you can get a Redis shell. Using the password shown by ``dotcloud info``,
with the following command::

   dotcloud run data redis-cli

In the newly opened redis shell, type the following::

   AUTH <password>

You can then use standard Redis commands.
