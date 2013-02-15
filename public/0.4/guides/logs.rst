:title: Consulting Logs
:description: With dotCloud's "logs" command, you can watch, in real time, any of your application logs.
:keywords: dotCloud documentation, quick start guide, streaming logs, read archives, dotCloud consulting logs

Consulting Logs
===============

Streaming The Logs
------------------

You can watch, in real time, the logs of an instance using the "dotcloud logs"
command::

   $ dotcloud logs myapp.www

The relevant logs for your service will be displayed. For example, for a
Python service, the logs command will show the web server access logs,
the web server error logs, and the WSGI error logs.

If you have scaled the service to multiple instances you can watch the logs for
a specific instance of the formation by appending the instance number (starting
from 0) to the service name::

   $ dotcloud logs myapp.www.1

By default, the logs of the first instance (i.e: ".0") are opened.

Press *Ctrl+C* to stop the streaming.

Read The Archives
-----------------

You can also open a remote shell and directly read the log you are interested
into, for example::

   $ dotcloud ssh myapp.www
   myapp-www$ less /var/log/nginx/error.log

If you have scaled the service to multiple instances you can open a remote shell
to a specific instance of the formation by appending the instance number
(starting from 0) to the service name::

   $ dotcloud ssh myapp.www.1
   myapp-www$ less /var/log/nginx/error.log

.. note::

   If you have a lot of logs, you may want to list them by date of last time of
   modification, to do so, use the command "ls -t".
