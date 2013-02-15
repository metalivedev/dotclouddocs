:title: Drupal PHP Tutorial
:description: How to deploy Drupal to dotCloud.
:keywords: dotCloud, tutorial, documentation, PHP, Drupal

Drupal
======

In this tutorial, we will deploy Drupal to DotCloud.

.. contents::
   :local:
   :depth: 1

Create a dotcloud.yml file
--------------------------

To deploy to DotCloud, you need to create a file in the root directory of
your Drupal deployment called "dotcloud.yml" which describes the structure
of your application. For this application, the dotcloud.yml will look like
this:

.. code-block:: yaml

   www:
       type: php
   mysql:
       type: mysql

Turn off APC during installation
--------------------------------

Because APC interferes with the Drupal installation wizard, we'll need to
temporarily disable it. To do this, create a file in the root directory of
your Drupal deployment called "php.ini" with the following contents:

.. code-block:: ini

   apc.enabled=0

Route all requests to the router in index.php
---------------------------------------------

All requests which don't map to static files should be sent to index.php,
which will route them appropriately. To do this, create a file called
"nginx.conf" in the root directory of your Drupal deployment with the
following contents:

.. code-block:: nginx

   try_files $uri $uri/ /index.php;

Initialize the database by pushing to DotCloud
----------------------------------------------

Before running the installation wizard, we'll need to create a Drupal
database in our MySQL instance. First, create your application on the
:doc:`flavor </guides/flavors>` of your choice and push to DotCloud by running
this command in the root directory of your Drupal deployment (on your local
computer)::

   $ dotcloud create my_drupal_deployment
   $ dotcloud push my_drupal_deployment

Then connect to mysql and create the database::

   $ dotcloud run my_drupal_deployment.mysql -- mysql
   mysql > CREATE DATABASE drupal;
   mysql > \q


Install Drupal using the installation wizard
--------------------------------------------

Now you can open the url you saw when you pushed (or run
"dotcloud url my_drupal_app.www") and complete the installation wizard. To
get the database credentials, run "dotcloud info my_drupal_app.mysql".


Re-enable APC and celebrate
---------------------------

Once you've completed the installation wizard, you can re-enable APC by
removing the php.ini file that you created earlier and pushing again.
Optionally, you can also celebrate the launch of your Drupal app with the
beverage of your choice.
