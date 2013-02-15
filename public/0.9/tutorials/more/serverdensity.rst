:title: Server Density Tutorial
:description: How to install server density monitoring platform in your dotCloud stack.
:keywords: dotCloud, tutorial, documentation, server density

Server Density
==============

`Server Density <http://www.serverdensity.com/>`_ is a monitoring-as-a-service
platform. If you have an account with them, and want to use their service
to monitor your dotCloud stack, you can! (If you don't have an account with
them, but still want to have a look, they have a 15 days trial.)

The installation is actually pretty easy; it is based on the manual install.

.. note:: Important!

   The basic Server Density setup requires only Python, without any special
   extra dependency. Therefore, it can run on any dotCloud service.

   However, if you want to monitor a database, you will need additional
   Python packages. The easiest way to install those packages will be to
   run an extra Server Density agent from a python-worker service.

.. contents::
   :local:
   :depth: 1


Basic Installation
------------------

Go to your service application root. It is the directory pointed by the
"approot" parameter in your "dotcloud.yml" file. If you did not specify
an approot, it defaults to the directory containing "dotcloud.yml".

For the sake of clarity, we will assume that it's named "approot"::

  cd approot


Download the Server Density agent::

  wget http://www.serverdensity.com/downloads/sd-agent.tar.gz

Uncompress it::

  tar -zxf sd-agent.tar.gz

You can now remove the tarball::

  rm sd-agent.tar.gz

Configure the agent with your favourite editor (*vi* or anything else),
and enter the *sd_url* and *agent_key* shown in your Server Density dashboard::

  vi sd-agent/config.cfg


You will now need a little supervisord.conf file to start the agent 
automatically (if you already have a supervisord.conf file, the following 
will add a new section to it, without breaking anything)::

  cat >>supervisord.conf <<EOF
  [program:serverdensity]
  command=/home/dotcloud/current/sd-agent/agent.py foreground
  EOF

Now, just push your code -- the Server Density agent will be started
automatically, and will start sending data right away.


Using a Database Plugin
-----------------------

If you want to enable monitoring for your databases (mysql, mongodb...)
the best way to do it is to add a dedicated python-worker service in your
dotcloud.yml build file. Here is a sample dotCloud Build File:

.. code-block:: yaml

   www:
     type: python
     approot: frontendcode
   sql:
     type: mysql
   mongo:
     type: mongodb
   serverdensity:
     type: python-worker
     approot: serverdensity

Create a "serverdensity" directory and repeat the steps shown in the basic
setup to download and uncompress the agent::

  wget http://www.serverdensity.com/downloads/sd-agent.tar.gz
  tar -zxf sd-agent.tar.gz
  rm sd-agent.tar.gz

Configure the agent with your favourite editor (*vi* or anything else),
and enter the *sd_url* and *agent_key* shown in your Server Density dashboard::

  vi sd-agent/config.cfg

Enter the following configuration::
  [Main]
  sd_url: http://...myapp...serverdensity.com
  agent_key: 12345678...12345678

Add the configuration specific to your database plugin:

.. tabswitcher::

   .. tab:: MySQL

        Use "dotcloud info" to see the MySQL server, port, user, and password.
        Then, add the following lines to your "config.cfg" file::

          mysql_server: xxx.dotcloud.com:12345
          mysql_user: root
          mysql_password: xxx

   .. tab:: MongoDB

      Use "dotcloud info" to retrieve the MongoDB URL for your MongoDB
      database. Then, add the following line to your "config.cfg" file::

        mongodb_server: mongodb://root:xxx@xxx.dotcloud.com:12345

      Quite conveniently, the URL is the same as the one shown by
      "dotcloud info".

You will also need to specify the dependencies used by the Server Density
agent to connect to your databases. Create a file named "requirements.txt"
in the "serverdensity" directory, containing the following line:

.. tabswitcher::

   .. tab:: MySQL

      ::

         MySQL-python

   .. tab:: MongoDB

      ::

         pymongo

Last step: the supervisord.conf, which will be slightly different, because
it will need to use the customized Python environment (loaded with the
database connection libraries) instead of the system default Python::

  cat >>supervisord.conf <<EOF
  [program:serverdensity]
  command=/home/dotcloud/env/bin/python /home/dotcloud/current/sd-agent/agent.py foreground
  EOF

Now push your stack, and watch as your servers and database appear on your
Server Density dashboard!


Scaling
-------

If you intend to use Server Density on multiple dotCloud services,
or if you intend to be able to scale your dotCloud service and still
get relevant information on your Server Density dashboard, you should
`enable auto-copy <http://support.serverdensity.com/customer/portal/articles/72261-auto-copy>`_.
Else only the first service will be monitored (others will be blocked).


Updating the Agent
------------------

You can run "python agent.py update" in your local repository. This will
download the latest agent version and overwrite the current version. Then
push your application again to push the new agent to your services.
