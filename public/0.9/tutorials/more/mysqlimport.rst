:title: MySQL Import Tutorial
:description: How to import an existing database into dotCloud.
:keywords: dotCloud, tutorial, documentation, mysql, import

Import Existing MySQL Databases
==================================

1. Backup your existing database:

::

  mysqldump -u [uname] -p[pass] dbname > backupfile.sql

.. note::

    It is important that you only capture the database which contains your
    table structures and data. Importing the "mysql" database could cause
    issues if it overwrites the existing root user's password!

2. Create a dotCloud.yml file that provisions a new :doc:`mysql database </services/mysql/>`:

::

  db:
    type: mysql

3. Login to mysql and create your database:

::

  dotcloud run -A <application> db -- mysql

  mysql> create database testdatabase;
  mysql> exit;

4) Load the data. You have a couple options:

*Option 1*: Load the data directly over the CLI (not recommended for large datasets)

::

  $ dotcloud run -A importtest db -- mysql testdatabase < /path/to/data/backupfile.sql


*Option 2*: Scp a tarred version of the data to your db instance (better for
large data sets).  For more information on moving files around with the command
line, see the :doc:`copy guide </guides/copy/>`.

::

    # tar & compress your backup file
    $ tar cvzf backupfile.sql.tar backupfile.sql

    # transfer the file to your database application
    $ base64 backupfile.sql.tar | dotcloud run -A <appname> db -- "base64 --decode > backupfile.sql.tar"

    # ssh into your database service and untar the file
    $ dotcloud run -A importtest db $ tar xvf backupfile.sql.tar

    # load the database
    $ mysql testdatabase < backupfile.sql
