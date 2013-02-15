:title: New Relic PHP Tutorial
:description: How to deploy New Relic on dotCloud  for web app performance monitoring.
:keywords: dotCloud, tutorial, documentation, PHP, New Relic

New Relic
=========

.. include:: ../../dotcloud2note.inc

`New Relic <http://newrelic.com>`_ allows "web app performance
monitoring and real user monitoring". It supports PHP (and
others). These instructions are for installing the PHP agent and
daemon.

Here is how to use the New Relic client version 3.x on dotCloud. 
You will need:
  * a PHP application (let's say it is in directory ``myapp``)
  * a New Relic `license key <https://newrelic.com/docs/php/new-relic-for-php#license_key>`_

.. contents::
   :local:
   :depth: 1


Download New Relic for PHP
--------------------------

Go to the `New Relic download site
<http://download.newrelic.com/php_agent/release/>`_, and download the
Linux tarball. The example URL used below will need a specific
VERSION. These instructions were tested with VERSION=3.1.5.141::

  cd /tmp
  wget http://download.newrelic.com/php_agent/release/newrelic-php5-VERSION-linux.tar.gz

Decompress the New Relic Tarball, and Pick a Few Files
------------------------------------------------------

We just need the PHP extension and the New Relic daemon. Uncompress the tarball,
and copy those two files to your dotCloud stack (remember to change the file names, don't just copy and paste)::

  tar -zxf newrelic-php5-VERSION-linux.tar.gz
  cp newrelic-php5-VERSION-linux/agent/x64/newrelic-20100525.so  myapp/
  cp newrelic-php5-VERSION-linux/daemon/newrelic-daemon.x64 myapp/

Agent ``newrelic-20100525.so`` is for PHP 5.4. If you are using PHP
version 5.3 in your container, you'll need ``newrelic-20090626.so``
instead.

Configure New Relic
-------------------

With the version 3 New Relic agent, you can do all of your
configuration and initialization in ``php.ini``.  You just have to
load the New Relic PHP extension agent and let the agent know where to
find the daemon and where to log. It is important to provide the
``extension`` value *without* quotes and all the ``newrelic.`` values
*with* quotes::

  ; Edit or create php.ini
  ; Give full path to the newrelic agent
  extension=/home/dotcloud/current/newrelic-20100525.so
  ; Give full path to the new relic daemon. The agent will start this as needed.
  newrelic.daemon.location="/home/dotcloud/current/newrelic-daemon.x64"

  ; Add your license key
  newrelic.license="your_license_key"

  ; The location of the logs can be any writable directory.
  newrelic.logfile="/home/dotcloud/current/php_agent.log"
  newrelic.daemon.logfile="/home/dotcloud/current/newrelic-daemon.log"

.. note:: 
   You can also specify extra New Relic options in ``php.ini``; see
   the `New Relic PHP docs
   <https://newrelic.com/docs/php/php-agent-phpini-settings>`_ for
   more information about those.


Start the New Relic Daemon
--------------------------

The newer versions of the New Relic agent (version 3 and up) will
automatically start the daemon for you if necessary. You just need to
give the full path to the daemon in your ``php.ini``

Push Your App
-------------

Everything is now ready for New Relic. You know the last part::

  dotcloud create myapp
  dotcloud push

Problems?
---------

New Relic offers good `troubleshooting tips <https://newrelic.com/docs/php/php-agent-troubleshooting>`_.

.. note::
   Remember that metrics do not appear in real-time into your New Relic
   dashboard; there will probably be a few minutes delay between your
   tests and the moment they are visible in your dashboard.
