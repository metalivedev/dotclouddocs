Redmine
=======

Redmine is a famous ticket tracking system, using Ruby on Rails.

Its installation is not always easy; you have to use specific versions
and customize some bits -- so here is how to do it on dotCloud.


Setup a database
----------------

Before getting to Redmine itself, we will setup a PostgreSQL database.
First, deploy a PostgreSQL service::

  $ dotcloud deploy -t postgresql ramen.pgsql

Wait a bit until it has booted (you can check its state with 
"dotcloud info ramen.pgsql"). Then, login to the service, and create
a user and database for Redmine ::

  $ dotcloud ssh ramen.pgsql
  postgres@ramen-pgsql:~$ psql
  postgres=# create user "redmine" password 'abracadabra';
  CREATE ROLE
  postgres=# CREATE DATABASE "redmine" OWNER "redmine";
  CREATE DATABASE
  postgres=# \q
  postgres@ramen-pgsql:~$ exit

You might want to use something safer than "abracadabra" for your database
password.

.. note::
   If you plan to dedicate this database service to Redmine, you can 
   skip this step. You will then use the database "template1", the 
   user "root" and the password seen when doing "dotcloud info ramen.pgsql".


Deploy a ruby service
---------------------

This is almost as easy as deploying the PostgreSQL database.
There is a little cacth however: Redmine won't work with Ruby 1.9, so
we will explicitly request Ruby 1.8::

  $ dotcloud deploy -t ruby -c '{"ruby-version":"ree"}' ramen.redmine

.. note::
   REE stands for Ruby Enterprise Edition, and is the thing to use
   if you want Ruby 1.8.


Prepare Redmine code
--------------------

First, retrieve the code; the following line will retrive Redmine version 1.1
from the Subversion repository, and put it in a directory named 
"redmine-1.1"::

  $ svn co http://redmine.rubyforge.org/svn/branches/1.1-stable redmine-1.1

The first thing to is to create the file "redmine-1.1/Gemfile", to specify
the Ruby dependencies::

  source :rubygems
  gem "i18n", "0.4.2"
  gem "rails", "2.3.5"
  gem "rack", "1.0.1"
  gem "pg"
  gem "coderay", "0.9.7"
  gem "rubytree", "0.5.2"

.. note::
   Redmine seems to be very picky about versions; that's why we specified
   manually almost all of them!

You will also need to edit two files. The first one is 
"redmine-1.1/config/environment.rb", to redefine an old method used by
Redmine but removed from the recent Rubygems versions. 
Find the following lines::

  # Bootstrap the Rails environment, frameworks, and default configuration
  require File.join(File.dirname(__FILE__), 'boot')

Just after those lines, add the following code::

  if Gem::VERSION >= "1.3.6"
      module Rails
          class GemDependency
              def requirement
                  r = super
                  (r == Gem::Requirement.default) ? nil : r
              end
          end
      end
  end

The second file is "redmine-1.1/config/boot.rb". The modification is
simpler, you just need to insert one line in the beginning of the file::

  require 'thread'

.. note::
   Those modifications might or might not be needed with the trunk version
   of Redmine, or with version 1.2 (when it gets released). We will do our
   best to update this information accordingly!

Next step is to configure your database. 
Create the file "redmine-1.1/config/database.yml" and add the following lines::

  production:
    adapter: postgresql
    database: redmine
    host: pgsql.ramen.dotcloud.com
    port: 12345
    username: redmine
    password: abracadabra
    encoding: utf8

.. note::
   To see the host and port that you need to use, check 
   "dotcloud info ramen.pgsql". Remember to use the same password
   as set when creating the database (or the one listed
   in "dotcloud info ramen.pgsql" if you decide to skip
   the user and database creation and use the default "root"
   user with the default "template1" database).

The last step is to take care of uploaded files.
Each time you do a "dotcloud push", your Redmine files will be wiped out.
If you plan to store attachments to your tickets (and you probably do!),
some specific setup must be carried out.

Redmine will live in ~/current; everything outside this directory
will be left untouched. We will replace the "files" directory by a symlink
pointing outside the ~/current directory; and to make it happen automatically
each time we do a "dotcloud push", we will setup an script which will be
called after each push.

Moreover, after each push, we need to initialize the "session store".
This can be done automatically in the same script.

Create the file "redmine-1.1/postinstall"::

  #!/bin/sh
  [ -d ~/data/files ] || mkdir -p ~/data/files
  [ -L ~/current/files ] || {
      rm -rf ~/current/files
      ln -s ~/data/files ~/current/files
  }
  rake generate_session_store

Make it executable::

  $ chmod +x redmine-1.1/postinstall

That's all!


Push your code
--------------

This is achieved by a simple command::

  $ dotcloud push ramen.redmine redmine-1.1

It will take a while since it will upload all the code to dotCloud.
Don't worry: if you do some modifications and push again, only the
differences will be transferred.


Finalize the setup
------------------

A few last steps need to be performed, to initialize the Redmine database.
Here is how::

  $ dotcloud ssh ramen.redmine
  dotcloud@ramen-redmine:~$ cd current
  dotcloud@ramen-redmine:~/current$ rake db:migrate
  dotcloud@ramen-redmine:~/current$ rake redmine:load_default_data

This last step will ask you for the default language that you want to use.
Once it has completed, you can get back to your local shell with "exit".

Your Redmine is ready to use, at http://redmine.ramen.dotcloud.com/; the
default login and password are "admin" and "admin".

.. FIXME explain how to do backups :-)
