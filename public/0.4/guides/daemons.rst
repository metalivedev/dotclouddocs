:title: Background Processes
:description: How dotCloud defines worker services and background processes.
:keywords: dotCloud documentation, quick start guide, 

Background Processes
====================

.. include:: ../services/worker.inc

.. Ok, this is going to be difficult to merge with the new features in
   dotcloud.yml. The documentation between supervisord.conf and dotcloud.yml
   will overlap in same places but not everywhere. For example the section about
   catching signal should be common to supervisord.conf and dotcloud.yml but
   only supervisord.conf allows you to configure the signal to send.

.. _guides-define-daemons:

Defining Daemons
----------------

To run a background process you need to write a *supervisord.conf* file in the
.ini format, for example:

.. code-block:: ini

   [program:daemonname]
   command = php /home/dotcloud/current/my_daemon.php

This is the most simple configuration that you can write, it defines a daemon
called *daemonname* that is launched using the *"php /home/dotcloud/current/my_daemon.php"*
command.

You can use multiple *[program:x]* sections to run different daemons.

Once, you have the code of your daemon, your supervisord.conf file and your
dotcloud.yml build file can push your application on DotCloud using "dotcloud
push" like for any service.

.. note::

   When you run "dotcloud push ...", your code will be installed in
   /home/dotcloud/current by the DotCloud builder process. That's why we
   specified this path.

   If your script is installed into the $PATH, you don't need to include its
   full path. (i.e: you can just write "command = my_daemon.php" instead).

.. note::

   If you are not on Windows, do not forget to set the executable bit on your
   daemon by using "chmod +x".


Configuring The Environment
---------------------------

You can easily modify the environment of execution of your daemon with the
"directory" and "environment" directives to change the directory where the
command is executed and to define additional environment variable. For example:

.. code-block:: ini

   [program:daemonname]
   command = php my_daemon.php
   directory = /home/dotcloud/current/
   environment = QUEUE="*" , VERBOSE="TRUE"

.. note::
   Don't forget the quotes around the environment variable values!
   Supervisor is quite picky about this. If you specify something like
   ``PYTHONPATH=/foo/bar`` instead of ``PYTHONPATH="/foo/bar"``,
   Supervisor will truncate it to ``/``. So beware!


Exit Cleanly With Signals
-------------------------

You are advised to catch the SIGTERM signal sent by Supervisor when it tries to
gracefully stop or restart your daemon. This will happen each time you push a
new revision of your application. This is important if your daemon cannot be
interrupted in the middle of a job.

Here is some complete daemon examples that exit cleanly on SIGTERM:

.. tabswitcher::

   .. tab:: PHP

      To catch signals in PHP you need to add a *"declare()"* at the top of your PHP
      files and then register a callback:

      .. code-block:: php

         #!/usr/bin/env php
         <?php

             // This is mandatory to use the UNIX signal functions:
             // http://php.net/manual/en/function.pcntl-signal.php
             declare(ticks = 1);

             // A function to write on the error output
             function    warn($msg)
             {
                 $stderr = fopen("php://stderr", "w+");
                 fputs($stderr, $msg);
             }

             // Callback called when you run `supervisorctl stop'
             function    sigterm_handler($signo)
             {
                 warn("Kaboom Baby!\n");
                 exit(0);
             }

             function    main()
             {
                 while (true) {
                     warn("Tick\n");
                     sleep(1);
                 }
             }

             // Bind our callback on the SIGTERM signal and run the daemon
             pcntl_signal(SIGTERM, "sigterm_handler");
             main();

         ?>

      You can find the full reference on Unix signal functions here:
      http://www.php.net/manual/en/intro.pcntl.php

   .. tab:: Ruby

      .. code-block:: ruby

         #!/usr/bin/env ruby

         # Callback called when you run `supervisorctl stop'
         def sigterm_handler
             warn "Kaboom Baby!"
             exit
         end

         def main
             while true do
                 warn "Tick"
                 sleep 1
             end
         end

         # Bind our callback to the SIGTERM signal and run the daemon:
         Signal.trap("TERM") { sigterm_handler }
         main

   .. tab:: Perl

      .. code-block:: perl

         #!/usr/bin/env perl

         use strict;
         use warnings;

         sub sigterm_handler()
         {
             print STDERR "Kaboom Baby!\n";
             exit 0;
         }

         sub main()
         {
             while (1) {
                 print STDERR "Tick\n";
                 sleep 1;
             }
         }

         $SIG{TERM} = \&sigterm_handler;
         main;

   .. tab:: Python

      .. code-block:: python

         #!/usr/bin/env python

         import sys
         import time
         import signal

         # Callback called when you run `supervisorctl stop'
         def sigterm_handler(signum, frame):
             print >> sys.stderr, "Kaboom Baby!"
             sys.exit(0)

         def main():
             while True:
                 print >> sys.stderr, "Tick"
                 time.sleep(1)

         # Bind our callback to the SIGTERM signal and run the daemon:
         signal.signal(signal.SIGTERM, sigterm_handler)
         main()

   .. tab:: NodeJS

      .. code-block:: javascript

         #!/usr/bin/env node

         function sigterm_handler() {
             console.warn('Kaboom Baby!');
             process.exit(0);
         }

         function main() {
             setInterval(function() {
                         console.warn('Tick');
                     },
                     1000
             );
         }

         process.on('SIGTERM', sigterm_handler);
         main();

If your daemon doesn't follow the SIGTERM convention you can tell Supervisor to
use the signal of your choice with the "stopsignal" directive:

.. code-block:: ini

   [program:daemonname]
   command = php /home/dotcloud/current/my_daemon.php
   stopsignal = QUIT

This example will use SIGQUIT to try to gracefully stop the daemon instead of
SIGTERM.


Configure Logging
-----------------

By default, Supervisor will create log files for you in the */var/log/supervisor/*
directory.

You can change this by using the "stderr_logfile" ---for the error output---
and the "stdout_logile" ---for the standard output--- directives:

.. code-block:: ini

   [program:daemonname]
   command = php /home/dotcloud/current/my_daemon.php
   stderr_logfile = /var/log/supervisor/daemonname_error.log
   stdout_logfile = /var/log/supervisor/daemonname.log

You can also choose to redirect the error output to the standard output, to get
everything in one log file, with ``redirect_stderr = true``.


.. _guides-multiple-workers:

Launching Multiple "Workers"
----------------------------

You can use Supervisor to launch multiple instance of one daemon. To do this you
need three things in your supervisord.conf.

First, a "numprocs" entry, for the number of identical processes to launch:

.. code-block:: ini

   numprocs = 2

Second, a "process_name" entry, to give each process a different name:

.. code-block:: ini

   process_name = "%(program_name)s-%(process_num)s"

Finally, you also need to give each log file a different name:

.. code-block:: ini

   stderr_logfile = /var/log/supervisor/daemonname_error-%(process_num)s.log
   stdout_logfile = /var/log/supervisor/daemonname-%(process_num)s.log

Here is a complete supervisord.conf example:

.. code-block:: ini

   [program:daemonname]
   command = php /home/dotcloud/current/my_daemon.php
   numprocs = 2
   process_name = "%(program_name)s-%(process_num)s"
   stderr_logfile = /var/log/supervisor/daemonname_error-%(process_num)s.log
   stdout_logfile = /var/log/supervisor/daemonname-%(process_num)s.log


Troubleshooting
---------------

You can check that your daemon has been started properly with the following
command::

  $ dotcloud run ramen.workers supervisorctl status
  daemonname                          RUNNING    pid 975, uptime 0:03:20

Supervisor provides some useful commands to start, stop, and restart programs::

  $ dotcloud run ramen.tick supervisorctl stop daemonname
  daemonname: stopped
  $ dotcloud run ramen.tick supervisorctl start daemonname
  daemonname: started
  $ dotcloud run ramen.tick supervisorctl restart daemonname
  daemonname: stopped
  daemonname: started

You can also run the Supervisor shell interactively if you like::

  $ dotcloud run ramen.tick supervisorctl
  daemonname                          RUNNING    pid 1003, uptime 0:01:48
  supervisor>


Caveats
-------

For performance reasons, Supervisor buffers the standard output, so the "echoes"
you do in your daemon will not show immediately in the log file. However, the
error output is written right away in the corresponding log file. Here is how to
write on the error output for different languages:

.. tabswitcher::

   .. tab:: PHP

      .. code-block:: php

         #!/usr/bin/env php
         <?php

             function    warn($msg)
             {
                 $stderr = fopen("php://stderr", "w+");
                 fputs($stderr, $msg);
             }

             warn("This message will be written on the error output\n");

         ?>

   .. tab:: Ruby

      .. code-block:: ruby

         #!/usr/bin/env ruby

         warn "This message will be written on the error output"

   .. tab:: Perl

      .. code-block:: perl

         #!/usr/bin/env perl

         print STDERR "This message will be written on the error output\n";

   .. tab:: Python

      .. code-block:: python

         #!/usr/bin/env python

         print >> sys.stderr, "This message will be written on the error output"

   .. tab:: NodeJS

      .. code-block:: javascript

         #!/usr/bin/env node

         console.warn("This message will be written on the error output\n");

Supervisor stops to read any output as soon as it begins to stop daemons [#]_.
For example that means that the output of our *sigterm_handler* function above
will never show up in the logs, however the function is perfectly executed.

You can write daemons that listen on TCP or UDP ports but you will not be able
to reach them over the Internet or from another DotCloud instance. However this
is one of our next features.

You can find the supervisord.conf reference on the `Supervisor documentation
<http://www.supervisord.org/configuration.html#program-x-section-settings>`_.

----

.. [#] http://www.plope.com/software/collector/271
