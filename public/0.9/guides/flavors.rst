:title: Application Flavors
:description: dotCloud flavors are different runtime for your applications, with different features and prices
:keywords: dotCloud documentation, dotCloud flavors, flavors

Application Flavors
====================

The primary "flavor" of dotCloud applications is called *Live*. This
gives simple pricing based entirely on the RAM you reserve with
``dotcloud scale``. You will need to enter your credit card
information into `your account
<https://dashboard.dotcloud.com/settings/billing>`_ before you can
create a *Live* application. There is no separate fee for network
or disk bandwidth, and 10G of disk storage is included for each 1G of
RAM you reserve.

For some customers we also enable the *Enterprise* flavor which
runs in a separate cluster and includes a higher level of
monitoring. This is available upon request but may require extra fees.

Flavors all share the same work-flow but have different features and
prices. You choose the flavor to use when you create an application
and you can have as many applications with as many different flavors
as you want, all on the same dotCloud account.

For example to create an application with the *Live* flavor, you can do::

   dotcloud create -f live  myapplication

Likewise, to create an application with the *Enterprise* flavor, you can do
do (if your account has been enable to run apps on this cluster)::

   dotcloud create -f enterprise mytestapplication

Once your application has been created you can push your code as usual
with ``"dotcloud push"``.

.. note::

  You can't change the flavor of an application once it has been pushed,
  so take the time to choose the right one!

Flavors: Current and Past
-------------------------

Live
....

The Live flavor is targeted at both development and production and is
the preferred dotCloud flavor. It comes with all the platform features:
Custom Domains (with SSL), Scaling and High Availability.

Applications with the Live flavor have guaranteed amounts of memory and
run on a performance monitored cluster. Each service will have
reasonable default memory limits, but since applications will be billed
at a `hourly rate <https://www.dotcloud.com/pricing.html>`_, make sure
you correctly set your :ref:`memory limits <scaling_vertically>`!

Enterprise
..........

The Enterprise flavor is similar to the Live flavor but comes with
premium support and custom application level monitoring.

Signing up for an Enterprise account requires a review with our
support and engineering team to make sure that your application will be
highly available. Enterprise applications live on a dedicated cluster
which is more conservative about new dotCloud features and which give us
more control about the underlying hardware resources. Finally, pricing
will be tailored to your needs and usage.

If you want more information about our Enterprise flavor, please
`contact us <mailto:support@dotcloud.com>`_.

Flavors Features Comparison
---------------------------

Here is a table to quickly know if the feature you need is in a specific
flavor:

+-----------------------------------------------+------+------------+
| Feature ↴, Flavor →                           | Live | Enterprise |
+===============================================+======+============+
| Websockets                                    | ✔    | ✔          |
+-----------------------------------------------+------+------------+
| Piggyback SSL                                 | ✔    | ✔          |
+-----------------------------------------------+------+------------+
| Scaling (``dotcloud scale``)                  | ✔    | ✔          |
+-----------------------------------------------+------+------------+
| Custom Domains & SSL (``dotcloud alias``)     | ✔    | ✔          |
+-----------------------------------------------+------+------------+
| Custom Monitoring                             | ✘    | ✔          |
+-----------------------------------------------+------+------------+

Checking the Cost of your Application
-------------------------------------

You can use the `dashboard <https://dashboard.dotcloud.com/>`_ to check how much
an application costs with the `Live` or `Enterprise` flavors.


