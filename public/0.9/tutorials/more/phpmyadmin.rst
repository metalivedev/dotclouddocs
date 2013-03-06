:title: phpMyAdmin Tutorial
:description: How to augment an existing project with phpMyAdmin.
:keywords: dotCloud, tutorial, documentation, phpMyAdmin

phpMyAdmin
==========

Adding phpMyAdmin to your existing project is easy!  It can accomplished with
the following steps:

0. Start in the directory containing your application.
1. Clone into the repo into your project structure, followed by removing the .git repository directory.::

    $ git clone https://github.com/dotcloud/phpmyadmin-on-dotcloud phpmyadmin
    $ rm -rf phpmyadmin/.git

2. Edit the dotcloud.yml file to add the http interface.::

    db:                    # Your existing database
      type: mysql 

    # Add This...........................................
    # New service, can be any name, but type must be php.
    # approot should be phpmyadmin since that is where
    # you put the github code in the clone step.
    pma:                   # <-- new service
      type: php            # <-- new type
      approot: phpmyadmin  # <-- new subdir of code


3. dotcloud push!::

    $ dotcloud push -A <application>

4. Get the url for your phpmyadmin interface::

    $ dotcloud domain list
    pma: myapp-myusername.dotcloud.com

5. Get the username and password you'll need for phpmyadmin::

    $ dotcloud env list | grep MYSQL_LOGIN
    ==> Environment variables for application myapp
    DOTCLOUD_DB_MYSQL_LOGIN=root

    $ dotcloud env list | grep MYSQL_PASSWORD
    ==> Environment variables for application myapp
    DOTCLOUD_DB_MYSQL_PASSWORD=something secret

6. Open the ``pma`` URL in your browser and enter the LOGIN and PASSWORD.

Now you're ready to manage your MySQL databases with PHPMyAdmin.

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
