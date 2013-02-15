:title: New Relic PHP Tutorial
:description: How to deploy New Relic on dotCloud  for web app performance monitoring.
:keywords: dotCloud, tutorial, documentation, PHP, New Relic

New Relic
=========

`New Relic <http://newrelic.com>`_ allows "web app performance monitoring
and real user monitoring". It supports PHP (and others).

Here is how to use New Relic on DotCloud, assuming that already have
a DotCloud application (ramen.newrelic) stored in ~/newrelic-on-dotcloud,
and a New Relic license key.

.. contents::
   :local:
   :depth: 1


Download New Relic for PHP
--------------------------

Go to the `New Relic download site 
<http://download.newrelic.com/php_agent/release/>`_, and download the Linux
tarball (the actual URL used below might be slightly different)::

  $ cd /tmp
  # Check http://download.newrelic.com/php_agent/release/ for the latest newrelic-php5-*
  # and download it like this (where <LATEST> is the newest version):
  tmp$ wget http://download.newrelic.com/php_agent/release/newrelic-php5-<LATEST>-linux.tar.gz


Decompress the New Relic Tarball, and Pick a Few Files
------------------------------------------------------

We just need the PHP extension and New Relic daemon. Uncompress the tarball,
and copy those two files to your DotCloud stack::

  tmp$ tar -zxf newrelic-php5-<LATEST>-linux.tar.gz

Inside that tarball there are several versions of the files we
need. Choosing the right one depends on the version of PHP you're
using.

<table>
<tr><th>PHP</th><th>Version</th></tr>
<tr><td>5.3</td><td>agent/x64/newrelic-2009*.so</td></tr>
<tr><td>5.4</td><td>agent/x64/newrelic-2010*.so</td></tr>
</table>

  tmp$ cp newrelic-php5-<LATEST>-linux/agent/x64/newrelic-<Version>.so ~/newrelic-on-dotcloud
  tmp$ cp newrelic-php5-<LATEST>-linux/daemon/newrelic-daemon.x64 ~/newrelic-on-dotcloud/


Configure the New Relic Daemon
------------------------------

Create a "newrelic.cfg" file in your DotCloud app, containing your license 
key. We will also set the path to the New Relic Daemon log file, to allow
easier diagnostic::

  $ cd ~/newrelic-on-dotcloud
  newrelic-on-dotcloud$ cat > newrelic.cfg <<EOF
  license_key=insertyourownnewreliclicensekeyhere
  logfile=/home/dotcloud/data/newrelic-daemon.log
  EOF


Enable the New Relic PHP Agent
------------------------------

You just have to load the New Relic PHP extension, by adding one line into
your "php.ini" file. It's also the good place to set the path to the New Relic
PHP Agent log file (this is different from the Daemon log file set in the
previous section). The following command will do just that, creating the file
if it does not exist::

  newrelic-on-dotcloud$ cat >> php.ini <<EOF
  extension=/home/dotcloud/current/newrelic-<Version>.so
  newrelic.logfile = /home/dotcloud/data/php_agent.log
  EOF

.. note::
   You can also specify extra New Relic options here; see the `New Relic
   PHP docs <http://support.newrelic.com/kb/php/phpini-settings>`_ 
   for more information about those.


Start the New Relic Daemon
--------------------------

This is maybe the most difficult part! We need to make sure that the New Relic
daemon is started automatically. For that, we will use a "supervisord.conf"
file::

  newrelic-on-dotcloud$ cat >>supervisord.conf <<EOF
  [program:newrelic]
  command=/home/dotcloud/current/newrelic-daemon.x64 -f -c /home/dotcloud/current/newrelic.cfg
  EOF


Push Your App
-------------

Everything is now hopefully ready. You know the last part::

  newrelic-on-dotcloud$ dotcloud create ramen
  newrelic-on-dotcloud$ dotcloud push ramen

Enjoy!

.. note::
   Remember that metrics do not appear in real-time into your New Relic
   dashboard; there will probably be a few minutes delay between your
   tests and the moment they are visible in your dashboard.
