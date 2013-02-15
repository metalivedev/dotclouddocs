:title: Drupal PHP Tutorial
:description: How to deploy Drupal to dotCloud.
:keywords: dotCloud, tutorial, documentation, PHP, Drupal

Drupal
======

.. include:: ../../dotcloud2note.inc

In this tutorial, we will deploy Drupal to dotCloud.

.. contents::
   :local:
   :depth: 1

Create a dotcloud.yml file
--------------------------

To deploy to dotCloud, you need to create a file in the root directory of
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

Initialize the database by pushing to dotCloud
----------------------------------------------

Before running the installation wizard, we'll need to create a Drupal
database in our MySQL instance. First, create your application with the
:doc:`flavor </guides/flavors>` of your choice and push to dotCloud by running
this command in the root directory of your Drupal deployment (on your local
computer)::

   dotcloud create my_drupal_deployment
   dotcloud push

Then connect to mysql and create the database::

   dotcloud run mysql -- mysql

In the MySQL prompt that opened, type the following::

   CREATE DATABASE drupal;
   \q


Install Drupal using the installation wizard
--------------------------------------------

Now you can open the url you saw when you pushed (or run
"dotcloud url my_drupal_app.www") and complete the installation wizard. To
get the database credentials, run "dotcloud info my_drupal_app.mysql".


Optionally, you can also celebrate the launch of your Drupal app with the
beverage of your choice.
