:title: phpMyAdmin Tutorial
:description: How to augment an existing project with phpMyAdmin.
:keywords: dotCloud, tutorial, documentation, phpMyAdmin

phpMyAdmin
==========

Adding phpMyAdmin to your existing project is easy!  It can accomplished with
the following:

1. Clone into the repo into our project structure, followed by removing the
.git repository directory.

::

    $ git clone https://github.com/dotcloud/phpmyadmin-on-dotcloud phpmyadmin
    $ rm -rf phpmyadmin/.git

2. Edit the dotcloud.yml file to add the http interface.

::

    db:           # Your existing database
      type: mysql #
    pma:          # <-- new
      type: php   # <-- new
    approot:      # <-- new
      phpmyadmin  # <-- new


3. dotcloud push!

::

    `dotcloud push -A <application>`


.. note::

  The phpmyadmin recipe finds the mysql databases that are located within the
  `environment.json`_ file. No additional web configuration is required.
  Basically, each mysql database in your dotcloud.yml file will be picked up by
  the phpMyAdmin recipe dynamically.

.. note::

  Using the phpMyAdmin is out of scope for this example as it assumes you
  already know how to use it. There's a few tutorials which can be found with a
  quick google search, one such can be found `here`_.


  .. _environment.json: http://docs.dotcloud.com/guides/environment/
  .. _here: http://www.reg.ca/faq/PhpMyAdminTutorial.html
