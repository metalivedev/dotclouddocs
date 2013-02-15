:title: Symfony PHP Tutorial
:description: How to deploy Symfony Standard Edition on dotCloud.
:keywords: dotCloud, tutorial, documentation, PHP, Symfony, standard edition

Symfony
=======

.. include:: ../../dotcloud2note.inc

Symfony is a popular web framework written in PHP that makes it easy to build
robust web applications for enterprises. Find out more at http://symfony.com/.

In this tutorial, we will walk through the deployment of Symfony Standard
Edition (a sandbox for getting started with Symfony). But, you should be able to
deploy any Symfony application on dotCloud by using these simple steps.

Make sure you already have the dotCloud CLI installed, if not, check out the
:doc:`/tutorials/php/quickstart` first.

.. contents::
   :local:
   :depth: 1

Directory Structure
-------------------

Download Symfony standard edition, without vendors, at http://symfony.com/download,
and extract it::

   tar -xzf Symfony_Standard_2.0.3.tgz
   cd Symfony

Before we get any further, here is what the directory structure should look
like at the end of the tutorial::

   .
   ├── app/         # Mainly contains the configuration of the application
   ├── bin/         # Contains a script to perform actions from the command line
   ├── deps         # List of dependencies that are installed under vendor/
   ├── deps.lock    # List of the installed dependencies versions
   ├── dotcloud.yml # The description of your application on dotCloud
   ├── LICENSE
   ├── README.md
   ├── src/         # Where you will place the sources of your application
   ├── vendor/      # Third parties libraries
   └── web/         # Holds the front-controller and is the root of the webserver
       ├── app.php      # The production mode front-controller
       ├── app_dev.php  # The development mode front-controller
       ├── apple-touch-icon.png
       ├── bundles/     # Hold static files
       ├── config.php
       ├── favicon.ico
       ├── nginx.conf   # A specific Nginx configuration for Symfony
       ├── php.ini      # Additional PHP configuration
       ├── postinstall* # A post install hook to install the vendors libraries
       └── robots.txt

dotCloud Build File
-------------------

Open the file ``dotcloud.yml`` using your favorite text editor and copy paste
this description:

.. code-block:: yaml

   www:
     type: php
     approot: web

   db:
     type: mysql

.. note::

   We are using MySQL here but you can also use PostgreSQL (replace "mysql" by
   "postgresql").

Setup the Front Controller
--------------------------

Edit a customized Nginx configuration in ``web/nginx.conf``, we need this to
redirect all the requests to the front controller:

.. code-block:: nginx

   # Set this to "app.php" to go into production mode:
   set $front_controller app_dev.php;

   index $front_controller;

   try_files $uri $uri/ /$front_controller$uri;

   location ~ ".+\.php($|/.*)" {
      if ( -f /home/dotcloud/current/maintenance) {
          return 503;
      }

      fastcgi_pass                     unix:/var/dotcloud/php5-fpm.sock;
      include                          fastcgi_params;
      include                          /home/dotcloud/current/\*fastcgi.conf;
      fastcgi_split_path_info          ^(.+\.php)(/.+)$;
      fastcgi_param PATH_INFO          $fastcgi_path_info;
      fastcgi_param PATH_TRANSLATED    $document_root$fastcgi_path_info;
   }

By default, the demo access is restricted to localhost, we need to edit the
front controller to remove this restriction, you just have to remove the first
block of code in ``web/app_dev.php``:

.. code-block:: diff

   --- Symfony/web/app_dev.php     2011-07-28 01:52:29.000000000 -0700
   +++ Symfony/web/app_dev.php     2011-08-25 16:08:27.348433170 -0700
   @@ -1,15 +1,5 @@
    <?php

   -// this check prevents access to debug front controllers that are deployed by accident to production servers.
   -// feel free to remove this, extend it, or make something more sophisticated.
   -if (!in_array(@$_SERVER['REMOTE_ADDR'], array(
   -    '127.0.0.1',
   -    '::1',
   -))) {
   -    header('HTTP/1.0 403 Forbidden');
   -    die('You are not allowed to access this file. Check '.basename(__FILE__).' for more information.');
   -}
   -
    require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../app/AppKernel.php';

Configure your Database
-----------------------

Symfony gets the database settings from the file: ``app/config/config.yml``.
Actually, this file contains placeholders (surrounded by "%" characters) for the
database parameters. By default, Symfony expects that you define the values for
these placeholders in the file: ``app/config/parameters.ini``. But dotCloud
defines the database settings in another file called the :doc:`/guides/environment`.
The dotCloud environment file contains, among other variables, the credentials
to access your database.

The simplest way to automatically configure your Symfony database is to write
another "resource file" that will load the dotCloud environment and set the
right values for the placeholders, this resource file will be ``app/config/dotcloud.php``:

.. code-block:: php

   <?php /* app/config/dotcloud.php: */

   $dotcloud_env_file = '/home/dotcloud/environment.json';
   $dotcloud_env = json_decode(file_get_contents($dotcloud_env_file), true);

   /* Change it to 'pdo_pgsql' if you want to use PostgreSQL: */
   $container->setParameter('database_driver', 'pdo_mysql');
   $container->setParameter('database_host', $dotcloud_env['DOTCLOUD_DB_MYSQL_HOST']);
   $container->setParameter('database_port', $dotcloud_env['DOTCLOUD_DB_MYSQL_PORT']);
   $container->setParameter('database_user', $dotcloud_env['DOTCLOUD_DB_MYSQL_LOGIN']);
   $container->setParameter('database_password', $dotcloud_env['DOTCLOUD_DB_MYSQL_PASSWORD']);

Then, you need to add this file to the imports list in ``app/config/config.yml``:

.. code-block:: yaml

   imports:
       - { resource: parameters.ini }
       - { resource: security.yml }
       - { resource: dotcloud.php }

   framework:
       # …

.. note::

  The keys in the dotCloud environment file reflect the content of your
  ``dotcloud.yml`` file. If you modify the name or the type of a service you
  will certainly need to edit the ``app/config/dotcloud.php`` file again.

Configure the Timezone
----------------------

Symfony needs a properly defined timezone, you can configure it from a
``web/php.ini`` file:

.. code-block:: ini

   [Date]
   date.timezone = America/Los_Angeles

Install the Vendors
-------------------

Symfony depends on several separate libraries (an ORM, a SMTP library...). All
these libraries take a significant amount of place and cannot be conveniently
tracked in a version control system. These dependencies are listed in the file
called ``deps`` at the root of the application.

Symfony provide a little script named ``vendors`` in the ``bin`` directory that
read the ``deps`` and ``deps.lock`` files to install all the needed libraries.

We can execute this script with :doc:`/guides/hooks`,
``web/postinstall``:

.. code-block:: sh

   #!/bin/sh

   # Install the vendor libraries
   #
   # The ~/code directory, where your application lives on dotCloud, is erased by
   # the newer version of your code, each time you push.
   # So, we would need to reinstall all the vendor libraries if we left them
   # inside ~/code.
   # That's why we install the vendor libraries outside of ~/code and create a
   # symlink to it.
   symfony_install_vendor() {
       local vendor_directory=$HOME/vendor/

       [ -d $vendor_directory ] || mkdir -p $vendor_directory

       rm -rf ~/code/vendor
       ln -s $vendor_directory ~/code/vendor

       php ~/code/bin/vendors install
   }

   symfony_install_vendor

.. note::

   The :ref:`~/code <service_php_layout>` directory, where your application
   lives on dotCloud, is erased by the newer version of your code, each time
   you push. So, we would need to reinstall all the vendor libraries if we left
   them inside ~/code. That's why we install the vendor libraries outside of
   ~/code and create a symlink to it.

Deploy your application
-----------------------

Create your application::

    dotcloud create symfony

Push it to dotCloud::

    dotcloud push

Initialize the database::

    dotcloud run www -- php code/app/console doctrine:database:create
    dotcloud run www -- php code/app/console doctrine:schema:create

Navigate to http://symfony-username.dotcloud.com and see your application
running!

If you run into any problems, please contact us at support@dotcloud.com or visit
us in #dotcloud, our IRC chatroom on Freenode.

.. Blocked by https://dotcloud.fogbugz.com/default.asp?2916

.. ----

.. Bonus - Add a SMTP Server
.. --------------------------

.. Configuring a SMTP server is quite similar to configuring a database. First add
.. an SMTP service to your build file, ``dotcloud.yml``:

.. .. code-block:: yaml

..    www:
..      type: php
..      approot: web

..    db:
..      type: mysql

..    mta:
..      type: smtp

.. Load the SMTP server credentials from the Environment File in
.. ``app/config/dotcloud.php``:

.. .. code-block:: php

..    <?php /* app/config/dotcloud.php: */

..    /* … */

..    $container->setParameter('mailer_host', $dotcloud_env['DOTCLOUD_DB_MYSQL_HOST']);
..    $container->setParameter('mailer_port', $dotcloud_env['DOTCLOUD_DB_MYSQL_PORT']);
..    $container->setParameter('mailer_user', $dotcloud_env['DOTCLOUD_DB_MYSQL_LOGIN']);
..    $container->setParameter('mailer_password', $dotcloud_env['DOTCLOUD_DB_MYSQL_PASSWORD']);

.. The main configuration file, ``app/config/config.yml``, doesn't configure the
.. SMTP server port, so let's add the right placeholder:

.. .. code-block:: yaml

..    # …

..    # Swiftmailer Configuration
..    swiftmailer:
..        transport: %mailer_transport%
..        host:      %mailer_host%
..        username:  %mailer_user%
..        password:  %mailer_password%
..        port:      %mailer_port% # Add this line

..    # …

.. Finally, push your changes::

..     $ dotcloud push

.. If you are using the demo, as deployed in the first part of this tutorial, you
.. can make sure that E-mails work by adding this block of code in the beginning of
.. the function ``indexAction`` in ``src/Acme/DemoBundle/Controller/WelcomeController.php``:

.. .. code-block:: php

..    <?php /* src/Acme/DemoBundle/Controller/WelcomeController.php: */

..    $message = \Swift_Message::newInstance()
..         ->setSubject('Hello Email')
..         ->setFrom('symfony@localhost')
..         ->setTo('symfony.dotcloud@yopmail.com')
..         ->setBody(<<<EOF
..    Hello,

..    This E-mail has been sent from Symfony running on dotCloud!

..    Best regards

..    -- 
..    The Symfony
..    EOF
..                  );
..    $this->get('mailer')->send($message);

.. Push again, visit the website and you can check that the E-mail has been sent
.. here: http://www.yopmail.com/en/?login=symfony.dotcloud.

.. .. note::

..    Anti-spam policies about E-mails coming from public clouds are very strict.
..    If you want to make sure that E-mails from your application are relayed
..    successfully to their recipients checkout the :doc:`SMTP service
..    documentation </services/smtp>`.
