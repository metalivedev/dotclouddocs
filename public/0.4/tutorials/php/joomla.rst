Joomla
======

Joomla is a popular CMS written in PHP that makes it easy to build content-rich websites.

Setup a MySQL database
----------------------

Make sure you already have the DotCloud CLI installed, then run::

    $ dotcloud create YOUR_APP_NAME
    $ dotcloud deploy -t mysql YOUR_APP_NAME.mysql

Deploy Joomla
---------------

Download the latest stable release of Joomla and extract it to a directory
on your local computer. Next, navigate to that directory and add a new file
called "nginx.conf". This file must contain the following line in order for
Joomla to work::

    try_files $uri $uri/ /index.php;

Now (in the same directory) you can run::

    $ dotcloud deploy -t php YOUR_APP_NAME.www
    $ dotcloud push YOUR_APP_NAME.www .

Setup Joomla
-------------

You should now see the Joomla setup wizard when you navigate to 
http://www.YOUR_APP_NAME.dotcloud.com. When the setup wizard asks you for
your MySQL credentials, simply run::

    $ dotcloud info YOUR_APP_NAME.mysql

The hostname will look something like "mysql.YOUR_APP_NAME.dotcloud.com:PORT_NUMBER",
the user will be "root", and the database name can be anything you like. The password
and port number can both be found in the output of the `dotcloud info` command.

That's it!
