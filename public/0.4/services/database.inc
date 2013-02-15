.. note::
   You can give your database or cache service any name you like instead of
   "data".
   If for some reason your application needs multiple databases, you can
   either use multiple independant services (by repeating the database section
   multiple times in dotcloud.yml, each with a different name instead of just
   "db"), or create multiple users and databases inside one single service.
   The first option (multiple services) will bring more resources, but keep
   in mind that when you go to production, this will generally incur higher
   costs as well.

Your data service will be globally available. This means that you
can use it with applications running on other platforms.

If for some reason, you want to use DotCloud just to deploy a standalone
data service, create a two-lines "dotcloud.yml" file in an empty
directory, and from this directory, just run::

  $ dotcloud create mynicedb
  $ dotcloud push mynicedb
  $ dotcloud info mynicedb.data

In less than one minute, your data service will be up and running,
and the "dotcloud info" command will show its connection credentials.


Configure Your Application
--------------------------

There are two ways to configure your app to use your new data service:

* manually, by hard-coding the host, port, user and password;
* automatically, by retrieving those parameters from the :doc:`/guides/environment`.

Manual Configuration
^^^^^^^^^^^^^^^^^^^^

Check your service parameters using "dotcloud info":

.. vim: set ft=rst :
