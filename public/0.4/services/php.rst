:title: PHP Service
:description: dotCloud's PHP service can host any kind of PHP5 based website.
:keywords: dotCloud documentation, dotCloud service, PHP service, PHP5, Drupal, Joomla, WordPress, PHBB

PHP & PHP Worker
================

The *php* service can host any kind of PHP5 based website: custom-tailored
applications, frameworks like Symfony or PHP, CMS like Drupal or Joomla,
and ready-to-use components such as WordPress or PHPBB.

.. include:: service-boilerplate.inc

.. code-block:: yaml

   www:
     type: php
     approot: hellophp

Our PHP code should be in the "hellophp" directory::

  $ mkdir ramen-on-dotcloud/hellophp

Let's create a very minimal (but informative!) "index.php" file here:

.. code-block:: php

   <?php phpinfo(); ?>

.. include:: service-push.inc


The PHP Stack
-------------

You may have already heard about the "LAMP stack". LAMP is an acronym that
stands for Linux, Apache, MySQL, PHP. This software stack is the "engine" that
runs the vast majority of PHP-based websites.

DotCloud uses a slightly different stack:

- we use Nginx instead of Apache;
- you can use MySQL, but also other SQL engines like PostgreSQL,
  and "NoSQL" databases like Redis or MongoDB.


Nginx
~~~~~

DotCloud relies on Nginx and Fast-CGI to forward PHP requests to PHP-FPM:

.. An ascii^Wunicode drawing is better that no drawing at all:

.. literalinclude:: php-fpm.utf8
   :language: none
   :encoding: utf-8

This setup has several advantages:

- the HTTP server and the PHP interpreter are decoupled: that means that
  the web server does not have to load the PHP interpreter to serve static
  assets (images, JS, CSS...), thus saving great amounts of memory;
- Nginx is well-known to be very fast at serving static files;
- instead of using ``.htaccess`` files, Nginx loads its whole configuration
  at startup time; this is much more efficient, because the ``.htaccess``
  mechanism incurs configuration file lookups and parsings for *each* HTTP
  request and for *each* directory leading to the requested file;
- if you are an Apache exert, you might be confused at first by Nginx
  configuration syntax; however, after testing both, a lot of people find
  that Nginx is more flexible, and that complex Apache ``RewriteRule``
  directives will often we expressed in a simpler way with Nginx syntax.

You will find ready to use Nginx rewrite and access rules for most common use
cases (https, basic authentication...) and frameworks in the :doc:`Nginx guide
</guides/nginx>` and in the :doc:`tutorials </tutorials/index>`.


Any Database
~~~~~~~~~~~~

DotCloud let you choose the kind of database you want to use: it can be
:doc:`mysql`, :doc:`mysql-masterslave`, :doc:`postgresql` or the document
oriented database :doc:`mongodb`. You can also use the advanced key-value
store :doc:`redis`.

Databases are *not* shared. If you use a MySQL service, you will have your
own MySQL server, with access to the MySQL root account and all its privileges.

Databases instances are always separated from the PHP instances. This
means that your database server will never be "localhost".
This separation is mandatory to allow application scaling, since it allows
us to place databases and PHP instances on different physical machines.

.. _service_php_layout:

Layout of a PHP Instance
------------------------

.. XXX This kinda conflicts with the Basic Use section

The following tree view will give you a clear overview of the file layout
of your code, after you push it to the DotCloud PHP service::

   home/
   └── dotcloud/
       ├── current/
       │   ├── dotcloud.yml     # Your code lives here: /home/dotcloud/current/
       │   ├── fastcgi.conf     # Optional FastCGI parameters
       │   ├── index.php        # Your application code
       │   ├── nginx.conf       # Optional Nginx configuration (e.g: rewrite rules)
       │   ├── php.ini          # Optional configuration directive for PHP
       │   ├── postinstall*     # Optional post installation script
       │   └── supervisord.conf # Optional background processes configuration
       ├── environment.json     # Hold informations on your stack and your environment variables
       └── environment.yaml     # Same thing but in YAML

If you use an :ref:`approot <guides_service_approot>`, then the ``~/current``
directory will actually point to a subdirectory of your application and the
whole application tree will be under ``~/code``. This way, relative includes in
your application (e.g: ``../libs/mylib.php``) will remain valid.

Running Background Processes and Periodic Tasks
-----------------------------------------------

Our PHP services can run background processes (using Supervisor) and periodic
tasks (using cron). This is covered extensively in two guides:

- :doc:`Running Background Processes </guides/daemons>`;
- :doc:`Running Periodic Tasks </guides/periodic-tasks>`.

You can run background processes on the same service as your web pages
(i.e. on the ``php`` service), but you are *strongly* encouraged to run them
on a dedicated ``php-worker`` service, as shown in the following Build File:

.. code-block:: yaml

   www:
     type: php

   daemons:
     type: php-worker

This will avoid any inconvenient interaction between your web processes and
your background processes. It will allow to scale both services separately,
and make sure that one service resource usage (CPU/RAM) will not impair the
other one.


Connect to the Services in your Stack
-------------------------------------

To connect to your database, mail server or any service in your stack you will
need to where it lives, on which port, you will also certainly the right user
name and password.

All this information can be found in the :doc:`Environment File </guides/environment/>`:
``environment.json`` (or ``environment.yaml``, if you prefer the YAML format).


.. _services_php_pear_pecl:

Installing Additional PHP Packages
----------------------------------

If the PHP library or extension you want is not installed by default, you can
install it using `PEAR <http://pear.php.net/>`_ or `PECL <http://pecl.php.net/>`_
with a requirements list in your dotcloud.yml:

.. code-block:: yaml

   www:
     type: php
     requirements:
       - mongo-1.2.2
       - oauth
       - channel://pear.amazonwebservices.com/sdk

You can also use this mechanism to get newer versions of the pre-installed
extensions (e.g: apc).

If for some reason, you want to check installed (or installable) packages
with ``pear`` or ``pecl``, feel free to do it using the SSH access to
your service::

   $ dotcloud ssh myapp.www
   dotcloud@myapp-www$ pear list -a
   INSTALLED PACKAGES, CHANNEL __URI:
   ==================================
   (no packages installed)

   INSTALLED PACKAGES, CHANNEL DOC.PHP.NET:
   ========================================
   (no packages installed)

   INSTALLED PACKAGES, CHANNEL PEAR.AMAZONWEBSERVICES.COM:
   =======================================================
   PACKAGE VERSION STATE
   sdk     1.4.1   stable

   INSTALLED PACKAGES, CHANNEL PEAR.PHP.NET:
   =========================================
   (no packages installed)

   INSTALLED PACKAGES, CHANNEL PECL.PHP.NET:
   =========================================
   PACKAGE VERSION STATE
   mongo   1.2.2   stable
   oauth   1.2.2   stable

   dotcloud@myapp-www$ 


Configuring PHP
---------------

If you need to set PHP options, you can create a file named ``php.ini`` in your
application top-level directory. This file is included after all other PHP
directives, so you can override anything you need.

For example, if you want to display all errors (very convenient during
development to be sure that you will spot even the slightest warnings and
notices):

.. code-block:: ini

   [PHP]
   error_reporting = E_ALL
   display_errors = On

.. _disable-apc:

By default, DotCloud enables APC (an extension which caches PHP opcodes
for your pages; it reduces CPU and execution time, at the expense of some RAM).
Some applications (notably, installation wizards like the one of Drupal,
which generate PHP files containing the application configuration)
can be incompatible with APC.
If you want to install such an application, or if you want to disable
APC for any reason, you can do it with the following ``php.ini`` file:

.. code-block:: ini

   [apc]
   apc.enabled = 0

Note that you can generally re-enable APC once you have gone through the
installation wizard.


Configuring Nginx
-----------------

You can include your own Nginx configuration (e.g rewrite rules, access,
control, authentication...) in a file called ``nginx.conf`` at the root of your
application. For those of you used to Apache, you can think of ``nginx.conf``
as a global ``.htaccess``.

Nginx is covered in details on the :doc:`Nginx guide </guides/nginx>`.
Below are small sections that covers some PHP specifics details.

.. note::

   Your Nginx configuration is included almost on the top of the server block.
   That means that you can override most of our Nginx configuration including
   the location block that handle PHP requests.


Denying Access to Includes Files
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

It is a best practice to deny direct access to your PHP libraries or framework.
That will prevent any side effect triggered by their unexpected execution.
Here is a sample ``nginx.conf`` file to achieve this:

.. code-block:: nginx

   location ~ /system/.* {
       deny all;
   }

   location ~ /application/.* {
       deny all;
   }

This will completely deny access to the ``/system`` and ``/application``
directories in your application.


MVC Frameworks
~~~~~~~~~~~~~~

A lot of PHP frameworks and applications require a custom Apache ``.htaccess``
configuration file to enable "pretty URLs" (that's the name used by WordPress
to designate this feature).

Without "pretty URLs", you will access your content with URLs like
http://myapp/index.php/blogpost/2011/06/23/what-pretty-urls or
http://myapp/index.php?node=451 instead of http://myapp/what-pretty-url.

To enable them, it is generally sufficient to create a ``nginx.conf`` file
at the root of your PHP code, containing the following line:

.. code-block:: nginx

   try_files $uri $uri/ /index.php?$args;

As surprising as it might seem, this single line will replace multiple
``RewriteCond`` and ``RewriteRule`` directives. It has been verified to work
with WordPress, Drupal, CakePHP, and others. If your favorite application or
framework does not work with it, contact our support about it!


Configuring FastCGI
-------------------

You can define additional FastCGI parameters in a file called ``fastcgi.conf``
at the root of your application.

One common use-case is to define an extra variable that will be available
in ``$_SERVER`` from your PHP code; e.g.:

.. code-block:: nginx

   fastcgi_param ENVIRONMENT production;

Check out the `FastCGI Nginx wiki page <http://wiki.nginx.org/HttpFcgiModule>`_
for a list of more advanced configuration options.


Caveats
-------

For improved performance, the PHP service uses APC, which caches pre-compiled
version of your PHP files. However, that means that if you modify the PHP
files while the service is running, or if you do things like PHP code
generation, you will see strange results. This is notably the case with
a few "setup wizards", which will ask you a few questions about your setup
(database setup, login, and so on) and generate a config.php file for you
on the server. If at the end of such a wizard, you are taken back to the
beginning and it asks you all the questions over again, you probably need
to `disable-apc` at least during the setup.


See Also
--------

You might want to read our tutorials related to PHP:

.. toctree::
   :maxdepth: 1

   /tutorials/php/cakephp
   /tutorials/php/drupal
   /tutorials/php/newrelic
   /tutorials/php/resque
   /tutorials/php/symfony

And those third-party documentations:

- `Setting up Twilio's OpenVBX on DotCloud
  <http://blog.arfrank.com/setting-up-openvbx-by-twilio-in-the-cloud-dot>`_
- `Install SugarCRM on DotCloud
  <http://developers.sugarcrm.com/wordpress/2011/08/15/howto-install-sugarcrm-on-cloud-paas-dotcloud/>`_
- `Wordpress on DotCloud <http://qpleple.com/wordpress-on-dotcloud/>`_
