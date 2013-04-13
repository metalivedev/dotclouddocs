:title: Scaling
:description: Scaling your application means adding or removing computing resources to run it. Learn about how dotCloud scales your application vertically and horizontally.
:keywords: dotCloud documentation, quick start guide, scaling dotCloud, scale vertically, scale horizontally, HA scaling, High-availability

Scaling
=======

.. include:: ../dotcloud2note.inc

Scaling your application up or down means adding or removing computing resources to 
it. Because there are different ways to do this, *scaling* in our case has
different meanings:

- Scaling *Horizontally*: the goal here is to handle tasks in parallel, such as
  concurrently handling more requests by launching a new web server or storing
  your database in different servers at the same time ("sharding");
- Scaling for *High-Availability (HA)*: the goal for high-availability is to
  avoid downtime both in normal operations (e.g: rebooting a machine after a
  kernel upgrade) and when a failure occurs (e.g: a machine crashes);
- Scaling *Vertically*: vertical scaling improves the performance of a single
  service of your stack. This is often done by allocating more resources to this
  service (more Memory, CPU time, disk I/Os…) sometimes with some parameters
  tuned specifically to this component (e.g: adjusting some SQL buffers).

This guide explains what scaling features dotCloud has available and how to use
them. Here you will find advice on both taking advantage of these features and
on scaling in general. The rule of thumb is that dotCloud scales *stateless*
services horizontally and scales *stateful* services for high-availability.

.. _scaling_horizontally:

Scaling Horizontally
--------------------

dotCloud supports horizontal scaling for *stateless* services. Any service that
runs application code is considered stateless by dotCloud. For example,
:doc:`Python </services/python>`, :doc:`Ruby </services/ruby>`, :doc:`NodeJS
</services/nodejs>` as well as the :doc:`worker services </guides/daemons>` are
considered stateless.

Scaling Up and Down
~~~~~~~~~~~~~~~~~~~

To scale a service in your stack, you can use the ``dotcloud scale`` command
once your stack is running on dotCloud::

    dotcloud scale www:instances=3

Obviously, you can use it to scale back down to fewer services too::

    dotcloud scale www:instances=1

.. note::

   The old syntax used to be ``dotcloud scale www=1`` and is still
   perfectly valid; "``:instances``" is the default.

   If you need to scale multiple services, just run the ``dotcloud scale``
   command as many times as necessary.

When you scale a service exposing an HTTP port (e.g: Python), incoming HTTP
requests will automatically be load balanced to the instances using a
round-robin algorithm.

Keep in mind that dotCloud services can run an arbitrary number of processes:

- Worker services can be used to run different background processes and even
  have a way to run :ref:`multiple workers <guides-multiple-workers>` for the
  same background process;
- Python, PHP and Perl always have 4 workers running;
- The Ruby service launches workers dynamically with a maximum of 6 workers.

Shared File System
~~~~~~~~~~~~~~~~~~

While your code can store data in the file system, this data will not be
shared across your instances. As soon as you turn on the scaling,
you can *no longer create* a file in one request and access it again in
a different request. If you want shared folders, you can add FUSE to your
service; see :doc:`s3fs` for details.

This not only applies to files that users might upload to your site, but also
to states that might be stored in shared memory or on disk like PHP sessions.
Make sure you are storing these states in a database or a key-value store like
:doc:`Redis </services/redis>`.

.. _guides_scaling_vs_ha:

Horizontal Scaling vs HA
------------------------

Scaling for high-availability often involves a part of horizontal scaling. In
some cases, higher reliability is achieved through horizontal scaling only.

That's the case for stateless services, so when you launch multiple instances of
your front-end service, dotCloud puts them behind a load balancer. The load
balancer will distribute the traffic across all the instances and is clever
enough to send requests only to healthy instances.

To improve reliability, databases will also use several instances. But, in that
case, not any instance can serve any request. In other words, one cannot simply
use a "load balancer" to evenly distribute the requests across all the
instances. All database services have a notion of "master" or "primary"
instance. This instance will be the only one to handle requests. The other
instances ---called "slave" or "secondary"--- are "hot standby" instances;
ready to become a new master if the current one crashes.

Some database services, may also let you offload (split) reads (i.e: operations
that don't modify the data) to these standby instances. For example, MongoDB,
lets you do this seamlessly from your code. You can scale up MongoDB both for
reliability and/or performance like you can do for your front-end and worker
services.

Scaling Databases for HA
------------------------

dotCloud uses a specific method to scale each *stateful* service. Any service
that holds persistent data is considered stateful. For example databases
(:doc:`MySQL </services/mysql>`, :doc:`PostgeSQL </services/postgresql>`) are
considered stateful. :doc:`Redis </services/redis>` and :doc:`Solr </services/solr>`
are considered stateful too.

Scaling Up and Down
~~~~~~~~~~~~~~~~~~~

The following stateful services support scaling:

- :doc:`MySQL </services/mysql>`: using Master/Slave;
- :doc:`Redis </services/redis>`: using Master/Slave;
- :doc:`MongoDB </services/mongodb>`: using Replica Sets (with 3 or 5 members).

You scale them exactly like stateless services, with the ``dotcloud scale``
command::

    dotcloud scale db:instances=2

In the background, slave or replica set members will be launched and synchronized.
You can watch the status of the synchronization with the ``dotcloud info``
command::

    dotcloud info db

You can scale down as easily::

    dotcloud scale db:instances=1

.. note::

   The old syntax used to be ``dotcloud scale www=1`` and is still
   perfectly valid; "``:instances``" is the default.

Fail-over and Recovery
~~~~~~~~~~~~~~~~~~~~~~

If one of the services goes down, the platform will automatically fail-over to
one of the backup service. Depending on the type of the service, you may have to
ensure things in your code. For Redis or MySQL your code must gracefully handle
database disconnections and timeouts. For MongoDB you have to use "connection
strings". Those details are explained on the respective services pages.

The recovery is also automatic, services will be reconfigured and resynchronized
as soon as they are back up.

Split Reads
~~~~~~~~~~~

For now, our focus is to scale databases for reliability. You cannot offload
reads on your Redis or MySQL slave.

Because split reads are completely part of the MongoDB replication protocol, it
is possible (and supported by dotCloud) to read on "secondary" replica set
members. This is explained on the :doc:`MongDB service page
</services/mongodb>`.

.. _scaling_vertically:

Scaling Vertically
------------------

.. sidebar:: Scaling up CPU & Disks

  The ability to scale the CPU and disk limits is being worked on! If you have
  specific CPU or disk needs, please contact the `support team
  <support@dotcloud.com>`_ for more information.

dotCloud allows you to change, up and down, the quantity of memory allocated for
any service at any moment, by using the ``dotcloud scale`` command::

   dotcloud scale www:memory=512M

The change is applied immediately and transparently for your service. If your
service is already scaled horizontally this will change the quantity of memory
available for each instance of the service.

You can consult the memory usage of your service with ``dotcloud info``::

   dotcloud info www

.. note::

   You can currently scale anywhere between 32MB and 64GB. However scaling
   above 4GB must be explicitly enabled for your account, `contact us
   <support@dotcloud.com>`_ if you need it!

Important Considerations
------------------------

Other Bottlenecks
~~~~~~~~~~~~~~~~~

Scaling horizontally will *not* make your application faster. If your
application takes 1 second to send the result scaling it up to 4 instances won't
make it quicker. Instead, you will just be able to handle more requests
simultaneously.

The bottleneck might not be on your web front-ends: if you scale your front-ends
and do not see any improvements then the bottleneck is somewhere else.

Growing traffic will add pressure on your database. Profiling and/or caching
your database queries might be helpful.

Concurrency
~~~~~~~~~~~

Even worse, if you didn't design your application with concurrency in mind you
might reveal nasty side effects when a segment of your code gets executed at
the same time in multiple processes.

Consider, these two trivial PHP snippets that increment a counter in
`Redis <http://www.redis.io>`_:

.. tabswitcher::

    .. tab:: Naïve incrementation

        This snippet retrieves the value first, increment it locally and then
        store back the result to Redis:

        .. code-block:: php

           <?php

               $value = $redis->get('counter');
               $value += 1;
               $redis->set('counter', $value);

           ?>

        This will *never* work on dotCloud, because two concurrents requests may
        overwrite each other's result.

    .. tab:: Atomic incrementation

        Here the atomic increment *(INCR)* command of Redis is used:

        .. code-block:: php

           <?php

               $value = $redis->incr('counter');

           ?>

        There is no race condition in that case.

While this example looks trivial, it is not hard to imagine a bigger application,
that does a lot more between the ``GET`` and the ``SET`` of the counter, making
the issue hard to spot.

When to Scale Up or Down
------------------------

A single request requires multiple steps - making it difficult to know when to 
scale up or down.

An approach is to look at the throughput of your application: you can divide the
number of workers you have by the time a request takes to be completed, to get
how much requests per second you can handle (e.g: 2 workers / 0.2 seconds = 10
requests/s).

You could profile the request to identify bottlenecks in your code. Maybe most
of the request time is spent waiting on a database result. Do not forget to use
memory efficiently, that CPU time is limited and that disk operations take a
very long time.

:ref:`Scaling is also about availability <guides_scaling_vs_ha>`, even if your
application is doing well with one front-end instance, you may want to scale to
a second instance to be resilient against a machine that crashes.

Your `dashboard <https://www.dotcloud.com/dashboard>`_ may teach you a lot about
your application and is here to help you make decisions on scaling.
