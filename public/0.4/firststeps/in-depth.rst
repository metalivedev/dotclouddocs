:title: In-Depth Example
:description: Deploy a more advanced, database-backed application with just a few steps on dotCloud.
:keywords: dotCloud, Getting started, dotCloud documentation, dynamic app, Php and MySQL, helloworldapp2, dotCloud.yml, connect database, push code, set up database, restart app, try your app, deleting services

.. rst-class:: next

:doc:`how-it-works`


In-Depth Example
================


Dynamic App with Database
-------------------------

Similar to deploying a :doc:`simple static HTML app <quickstart>`,
deploying a dynamic app with a database takes just a few steps. Instead
of using the static service, we will be choosing a service based on the
language of the app we will deploy. In addition, we will add a database
that will back the app. For this example, we are using PHP and MySQL.

To get ready, create your application on the flavor of your choice. In this
example, we'll name the application "helloworldapp2" and use the default
flavor::

  $ dotcloud create helloworldapp2
  Created application "helloworldapp2" using the flavor "sandbox" (Use for development, free and unlimited apps. DO NOT use for production.)


Build File with Two Services
----------------------------

The build file defines the services that exist within your application.
In our previous example, we had one service named "www" that hosted
static HTML files. In this example, we will change the "www" service to
a different technology, and add a database service.

Create your dotcloud.yml file at the root of your application:

.. code-block:: yaml

   www:
     type: php
   db:
     type: mysql


Connecting the Database
-----------------------

To connect your front-end web service to your database, you will need to
add some code that detects the presence of the DotCloud environment and
imports the database URL.

For example, to access a MySQL database using the PHP mysqli module,
create a file named index.php with the following. This sample app prints
some information about the database connection:

.. code-block:: php

   <?php

   # Import environment settings from DotCloud
   $envjson = json_decode(file_get_contents("/home/dotcloud/environment.json"),true);

   # Create MySQL Connection
   $mysqli = new mysqli($envjson['DOTCLOUD_DB_MYSQL_HOST'],
                        'helloappuser',         # username
                        'helloworldpassword',   # password
                        'helloworldapp2',       # db name
                        $envjson['DOTCLOUD_DB_MYSQL_PORT']);

   print_r($mysqli->query('SELECT now();')->fetch_row());

   ?>


Push Your Code
--------------

Now that you have prepared your code by creating a Build File, you can
push the code to DotCloud with the following command::

  $ dotcloud push helloworldapp2 .


Set Up Your Database
--------------------

Create your database and database access credentials as follows::

  $ dotcloud run helloworldapp2.db -- mysql
  mysql> CREATE USER 'helloappuser' IDENTIFIED BY 'helloworldpassword';
  Query OK, 0 rows affected (0.02 sec)
  mysql> CREATE DATABASE helloworldapp2;
  Query OK, 1 row affected (0.00 sec)
  mysql> GRANT ALL ON helloworldapp2.* TO 'helloappuser'@'%';
  Query OK, 0 rows affected (0.00 sec)
  mysql> FLUSH PRIVILEGES;
  Query OK, 0 rows affected (0.00 sec)


Restart Your App
----------------

::

    $ dotcloud restart helloworldapp2.www


Try Your App
------------

Get the URL to access it with the url command. Open it in a browser!

::

    $ dotcloud url helloworldapp2


Deleting Services
-----------------

If you don't need a service anymore, you can delete it with the ``destroy``
command::

  $ dotcloud destroy helloworldapp2.www
  Please confirm destruction [yn]: y
  Destroy for "helloworldapp2.www" triggered.

Note, however, that it will be recreated automatically on the next ``push``,
unless you remove it from ``dotcloud.yml``.

You can also destroy a whole application::

  $ dotcloud destroy helloworldapp2
  Please confirm destruction [yn]: y
  Destroy for "helloworldapp2" triggered.

.. note::

   The destruction is asynchronous; so don't be surprised if your service
   or application still shows up in ``dotcloud list`` for a while. It should
   go away after a few minutes.

.. warning::

   Service or application destruction is permanent, and cannot be undone.
   A few seconds after the ``destroy`` command is sent, there is no way to
   recover your data.


.. rst-class:: next

:doc:`how-it-works`
