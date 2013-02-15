:title: New Relic Tutorial
:description: How to add the New Relic Server Monitoring agent to any of your dotCloud services.
:keywords: dotCloud, tutorial, documentation, new relic

New Relic Server Monitoring
===========================

`New Relic Server Monitoring
<http://newrelic.com/docs/server/new-relic-for-server-monitoring>`_ 
enables you to "watch critical system metrics such as CPU and memory usage,
network activity, processes and disk utilization/capacity."

You can add the New Relic Server Monitoring agent to any of your
dotCloud services. Here is how.

.. note::
   You will need a New Relic license key.

.. contents::
   :local:
   :depth: 1


Download
--------

Go to the `New Relic download site 
<http://download.newrelic.com/server_monitor/release/>`_, and download
the most recent tarball.


Pick a Few Files
----------------

Uncompress the tarball in a temporary directory, and pick the following
files:

* ``nrsysmond.cfg`` (configuration file)
* ``nrsysmond.x64`` (executable)

Put them both in the ``approot`` of the service you want to monitor.


Configure
---------

Edit the ``nrsysmond.cfg`` file to enter your license key.


Start the Daemon
----------------

To start the daemon automatically from a non-custom service (e.g. php,
python, etc.), we will use a "supervisord.conf" file, placed in the
``approot`` of the service::

  [program:newrelic]
  command=/home/dotcloud/current/nrsysmond.x64 -f

.. FIXME explain for custom service


Push
----

You can now push your app. The New Relic daemon file will be pushed
with the configuration file, and automatically started.
