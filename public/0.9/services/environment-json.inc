Using ``environment.json``
^^^^^^^^^^^^^^^^^^^^^^^^^^

When your application is built by the dotCloud platform, a file named
``environment.json`` is created in the home directory of each of your services.

This file contains a JSON-formatted dictionary with most of the configuration
information of the services in your stack. You can use it to retrieve
the host, port, and password, of your databases, caches, and so
on.

You will find snippets of code to retrieve the connection information of your
databases in the :ref:`Environment Guide <guides-reading-environment>`.

.. vim: set ft=rst :
