:title: Quick Start Guide
:description: dotCloud's Quick Start Guide to running your first hello world command.
:keywords: dotCloud, dotCloud documentation, quick start guide, prerequisites, simple example, hello world, static service, deploy your first dotCloud application

.. rst-class:: next

:doc:`platform-overview`


Quick Start Guide
=================


Prerequisites
-------------

This guide assumes you have already `signed
up <http://www.dotcloud.com/accounts/register>`_ and :doc:`installed the
CLI <install>`. This example will require approximately 10
minutes to complete.


A Simple Example
----------------

Now that you have the dotCloud CLI installed, let's go ahead and deploy
your first application. In this example, we will be deploying a very
simple application that serves a static "Hello World" file.

Create a new folder and change to that folder

.. code-block:: none

  mkdir helloworldapp 
  cd helloworldapp

Fire up your favorite text editor, write a message to yourself and save it as
``index.html``:

.. code-block:: html

  <html>
    <head>
      <title>Hello World!</title>
    </head>
    <body>
      Hello World!
    </body>
  </html>


Then ``create`` your application.  In this example, we'll name the
application, "helloworldapp", and use the default flavor, "sandbox",
which is free.

.. code-block:: none

  dotcloud create helloworldapp


.. note::

  By default, your application will be created using the free
  "sandbox" flavor. Application flavors correspond to different
  runtime environments (clusters), features, and pricing.
  See :doc:`Flavors <../guides/flavors>` for more information.


Next, we'll create a dotCloud Build File that describes an application
with a single *static* service. The static service is a simple web
server that can be used to host static files such as HTML and images.
Create a file named *dotcloud.yml* with the following text:

.. code-block:: yaml

  www:
    type: static


Your application is ready with a *static* service. Now you can ``push`` your
current directory with your index.html file and your dotCloud Build File.

.. code-block:: none

  dotcloud push


Congratulations!
----------------

You have just deployed your first dotCloud application!

We chose to deploy a very simple static site in this example, but you'll
find that it's just as easy to deploy any kind of application. See the
full list of services available under the **Services** section in the
navigation bar on the left. You can mix and match various services, such
as a PHP service for your PHP application, and a MySQL service for your
database.

Continue with the tutorials to learn how to deploy a more advanced,
database-backed application.

.. rst-class:: next

:doc:`platform-overview`
