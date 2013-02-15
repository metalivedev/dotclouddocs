:title: Applications Flavors
:description: dotCloud flavors are different runtime for your applications, with different features and prices
:keywords: dotCloud documentation, dotCloud flavors, flavors

Applications Flavors
====================

dotCloud comes in different "flavors" which are different runtime for
your applications. Flavors all share the same work-flow but have
different features and prices. You choose the flavor to use when you
create an application and you can have as many applications with as many
different flavors as you want, all on the same dotCloud account.

For example to create an application on the "Live" flavor, you can do::

   $ dotcloud create -f live myapplication

Likely, to create an application on the "Sandbox" flavor, you can do
do::

   $ dotcloud create -f sandbox mytestapplication

Once your application has been created you can push your code as usual
with ``"dotcloud push myapplication"``.

.. note::

  You can't change the flavor of an application once it has been pushed,
  so take the time to choose the right one!

dotCloud has three different flavors and a "legacy" flavor for
application created before the introduction of flavors. Let's review
them:

Sandbox
-------

The Sandbox flavor allows you to test the platform and develop simple
applications for free. You can have as much services as you want in a
sandbox application, you have access to the full service :doc:`catalog
</services/index>` and :doc:`websockets <websockets>`.

However sandbox applications:

- cannot be :doc:`scaled <scaling>`;
- cannot be attached to :doc:`Custom Domains <domains>`;
- will be put in hibernation after a period of inactivity [#]_;
- run on a best effort, slower cluster.

Live
----

The Live flavor is targeted at both development and production and is
the preferred dotCloud flavor. It comes with all the platform features:
custom domains (with SSL), scaling and High Availability.

Applications on the Live flavor have guaranteed amounts of memory and
run on a performance monitored cluster. Each services will have
reasonable default memory limits, but since applications will be billed
at a `hourly rate <https://www.dotcloud.com/pricing.html>`_, make sure
you correctly set your :ref:`memory limits <scaling_vertically>`!

Enterprise
----------

The Enterprise flavor is similar to the Live flavor but comes with
premium support and custom, application level, monitoring.

Signing up for an enterprise account involve a procedure with our
support and engineering teams to make sure that your application will be
highly available. Enterprise applications live on a dedicated cluster
which is more conservative about new dotCloud features and which give us
more control about the underlying hardware resources. Finally, pricing
will be tailored to your needs and usage.

If you want more informations about our Enterprise flavor, please
`contact us <mailto:support@dotcloud.com>`_.

Flavors Features Comparison
---------------------------

Here is a table to quickly know if the feature you need is in a specific
flavor:

+---------------------------------------------+---------+------+------------+
| Feature ↴, Flavor →                         | Sandbox | Live | Enterprise |
+=============================================+=========+======+============+
| Websockets                                  | ✔       | ✔    | ✔          |
+---------------------------------------------+---------+------+------------+
| Piggyback SSL                               | ✔       | ✔    | ✔          |
+---------------------------------------------+---------+------+------------+
| Scaling (``"dotcloud scale"``)              | ✘       | ✔    | ✔          |
+---------------------------------------------+---------+------+------------+
| Custom Domains & SSL (``"dotcloud alias"``) | ✘       | ✔    | ✔          |
+---------------------------------------------+---------+------+------------+
| Custom Monitoring                           | ✘       | ✘    | ✔          |
+---------------------------------------------+---------+------+------------+

Checking the Cost of your Application
-------------------------------------

You can use ``"dotcloud info"`` to check how much an application costs you
on the `Live` or `Enterprise` flavors::

   $ dotcloud info myapplication
   -   flavor: live
   -   memory:
       -   total reserved: 128MB
       -   total used: 39MB
   -   pricing:
       -   hours in billing cycle: 0
       -   cost per hour: $0.024
       -   cost to date: $0.0
       -   estimated monthly cost: $0.0
   …

You can even do that on a specific service::

   $ dotcloud info myapplication.www

Or, if your scaled your service :ref:`horizontally <scaling_horizontally>`,
on a specific service instance::

   $ dotcloud info myapplication.www.0

To get detailed costs informations.

.. note::

   “total reserved” will not be displayed for sandbox applications
   because memory is not guaranteed to be available on the Sandbox
   flavor.

Legacy
------

The Legacy flavor is for applications deployed before the introduction
of the flavors system in June 2012.

You can't create new applications on the Legacy flavor, however, you can
continue to use them (and you will be billed at the same rate), with no
restrictions until the 14 of December 2012. After this date,
applications on the Legacy flavor will be moved to the Sandbox flavor.

If you wish to move an application from Legacy to Live, please contact us, with
the name of your application, on `support <mailto:support@dotcloud.com>`_!

----

.. [#] When that happens, the application is simply woke up on the next HTTP request.
