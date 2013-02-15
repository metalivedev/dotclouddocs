:title: Redis Service
:description: dotCloud's Redis service features Redis 2.4.11.
:keywords: dotCloud documentation, dotCloud service, Redis

Redis
=====

DotCloud features Redis 2.4.11. if you don't know Redis yet, the full docs and
some awesome tutorials are available at http://redis.io/.


Basics
------

If your application requires a Redis data store for caching or other purposes,
all you need to do is to add the following lines to your dotcloud.yml 
:doc:`../guides/build-file`:

.. code-block:: yaml

   data:
     type: redis

.. include:: database.inc

::

  $ dotcloud info ramen.data
  cluster: wolverine
  config:
      redis_password: lshYSDfQDe
  created_at: 1310511988.2404289
  ports:
  -   name: ssh
      url: ssh://redis@bd0715e0.dotcloud.com:7473
  -   name: redis
      url: redis://redis:lshYSDfQDe@bd0715e0.dotcloud.com:7474
  state: running
  type: redis

.. include:: environment-json.inc

Your ramen.data service will expose the following variables:

* DOTCLOUD_DATA_REDIS_HOST
* DOTCLOUD_DATA_REDIS_LOGIN
* DOTCLOUD_DATA_REDIS_PASSWORD
* DOTCLOUD_DATA_REDIS_PORT
* DOTCLOUD_DATA_REDIS_URL

Note that DATA will be replaced by the (upper-cased) name of your service.


Redis CLI
---------

To start playing with Redis (or for debugging purposes),
you can get a Redis shell. Using the password shown by "dotcloud info",
just use the following command::

   $ dotcloud run ramen.data redis-cli
   redis> AUTH <password>
   OK
   redis>

You can then issue standard Redis commands.
