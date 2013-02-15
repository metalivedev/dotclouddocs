:title: Build File
:description: Use dotCloud's Build File to  define the structure of your application.
:keywords: dotCloud documentation, quick start guide, 

Build File
===========


Background
----------

The dotCloud Build File defines the structure of your code and the
architecture of the services within your application. This information
enables dotCloud to automatically create a custom stack designed for
your application.


What's in a Build File?
-----------------------

The best way to explain the contents of a Build File is to walk through
an example. Let's look at a Build File.

.. code-block:: yaml

    www:                 # Could actually be anything: front, joe...
      type: ruby         # But this must be a valid service type.
    db:                  # There again, the name can be anything you like.
      type: postgresql   # ...but the type has to be a valid one.

The Build File is YAML formatted, so it is easily read by humans and
computers alike. Most developers create the Build File using a text editor
because the format is so simple. In YAML, indentation is important. Each
indentation is two spaces!!!

In line 1, we create a service named "www". Any indented line below
this one are attributes of this service. In this case, line 2 defines
the type attribute of the www service. The type attribute describes the
technology that the service requires. It is very important, and every
service must have a type attribute.

In lines 3 and 4, we create a :doc:`PostgreSQL service </services/postgresql>`
named db.


.. _guides_service_approot:

Specifying the Root Directory of a Service
------------------------------------------

If your stack uses multiple web services, you will probably want to put
the source of each web service in a different directory. You can use the
optional ``approot`` attribute to define a root directory for each service.

For instance, if your code is structured like this::

   myapp/
   ├── admin/
   │   ├── djangoproj/
   │   │   ├── settings.py
   │   │   └── …
   │   ├── wsgi.py
   │   └── …
   └── frontend/
       ├── index.php
       ├── logo.png
       ├── style.css
       └── …

You will put the following dotcloud.yml file in "myapp":

.. code-block:: yaml

    www:
      approot: frontend
      type: php
    backoffice:
      approot: admin
      type: python


In this case, the service "www" would be a typical PHP application in the
"frontend" directory; and the service "backoffice" would be a Django application
in the "admin" directory.


Scaling a Service
-----------------

Scaling services was previously done from the Build File using an optional ``instances``
field in a given service, but this is being deprecated in favor of the
``dotcloud scale`` command explained in our :doc:`scaling guide <scaling>`.

We believe that a command to scale your services at "runtime" is better because
tying an amount of computing resources to an application code doesn't really
make sense. For example you might want to use 2 web front-ends for your
production deployment but only 1 for your staging deployment.


Defining Environment Variable
-----------------------------

The recommended way to set environment variables is to use the
``dotcloud env`` command. You can, however, also define them in
your Build File, using the optional ``environment`` section:

.. code-block:: yaml

   www:
     type: python
     environment:
       MODE: production
       API: http://www.externalapi.com/v1/

Check out the :doc:`environment guide <environment>` to know more
about ``dotcloud env``, as well as the special files ``environment.json``
and ``.yml``.
