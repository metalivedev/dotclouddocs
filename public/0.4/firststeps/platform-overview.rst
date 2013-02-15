:title: Platform Overview
:description: dotCloud platform overview and documentation to get started developing with dotCloud.
:keywords: dotCloud, platform overview, dotCloud documentation, dotCloud first steps, applications and services, dotCloud build file, dotCloud naming, dotCloud workflow

.. rst-class:: next

:doc:`install`


Platform Overview
=================

Applications & Services
-----------------------

On DotCloud, an *application* is a collection of one or more *services*
working together. Each *service* is a component in your application,
such as a front-end application server, a worker, or a database.

.. figure:: docs-mediawiki-ex.png
   :alt: PHP + MySQL = MediaWiki

   For example, you could run MediaWiki (the software that powers
   Wikipedia) by creating a DotCloud application with a PHP service and a
   MySQL service.

Simply define the services for your application by creating a :doc:`DotCloud
Build File <../guides/build-file>`, and DotCloud will intelligently assemble
your application’s stack in the cloud and provide a comprehensive
automation architecture around it.

DotCloud provides services for the most widely used web languages and
databases. You have the flexibility to choose exactly the services you
need for your application, all on one platform. You will be able to
deploy to your stack with ease, benefit from cloud scalability, and
manage it with complete visibility.

If a component of your preferred stack isn’t on our list of supported
services, `contact us <http://support.dotcloud.com/>`_. We’re always
adding more.


Naming
------

All of the services belonging to the same application will live in a
*namespace*, which is the name you choose for your application at the
time of creation.

You will also assign a name for each service, which will be shown as
namespace.service\_name. For example, you may create a Python service
named helloworld.www. In this case, you named the Python service "www"
and it belongs in the "helloworld" namespace.


General Workflow
----------------

When using DotCloud to push a new application, your general workflow
will follow this pattern:

#. Work on your code
#. Create a DotCloud Build File
#. Push your code
#. Initialize databases, if required

To push code to an existing application, your general workflow will be
like this:

#. Work on your code
#. Push your code

.. rst-class:: next

:doc:`install`
