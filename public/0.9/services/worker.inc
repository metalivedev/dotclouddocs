.. sidebar:: Different words for the same thing

   Such processes are also known as "background", "long-running", "workers" or
   "daemons" depending on the context --- but it's all the same thing.

Sometimes, you need to run a program for a longer time than a single HTTP
request. Use cases include:

* CPU-intensive jobs (e.g. video transcoding);
* running some code at a specified time, or on regular intervals;
* background activity (e.g. crawling 3rd party service to update your database);
* run a specific web server, like Node.js or Tornado;
* and much more!

dotCloud provides "worker services" dedicated to those tasks. There is a
different service for each language: :doc:`/services/ruby-worker`,
:doc:`/services/php`, :doc:`/services/perl-worker`,
:doc:`/services/python-worker`, :doc:`/services/nodejs`... The only difference
between them is the set of pre-installed packages, and dependencies handling:
python-worker supports requirements.txt, while ---for instance--- ruby-worker
supports Gemfile.

All worker services rely on `Supervisor <http://supervisord.org/>`_ to start
and monitor your processes. Supervisor will start defined programs
automatically, and will restart them automatically if they crash or exit. If
you just need to run a program at a specified interval, you can also use a
crontab and ignore Supervisor.

.. note::

   You can also use a "non-worker" service to run some background jobs.
   More specifically, all services feature crontab, allowing you to run
   :doc:`/guides/periodic-tasks`. So if you want to run a daily Python
   script, e.g., a stock ticker in your database, you don't have to 
   dedicate a ``python-worker`` to this task: you cansetup the crontab 
   in the same ``python`` service that you use for your web application. 
   However, you should be aware that when you scale your application, 
   the cron tasks will be scheduled in *all* scaled instances -- 
   which is probably not what you need! So in many cases, it will still 
   be better to use a separate service.

   Similarly, a lot of (non-worker) services already run Supervisor,
   so you can run additional background jobs in those services.
   Then again, remember that those background jobs will run in multiple
   instances if you scale your application. Moreover, if you add
   background jobs to your web service, it will get fewer resources
   to serve pages, and your performance will take a significant hit.

   Technically, if you really want to know -- there is almost no
   difference between worker and non-worker services. For instance,
   the ``python-worker`` service is basically the ``python`` service
   without Nginx (HTTP server) and uWSGI (Python web workers) running.
   Both can optionally run background processes using Supervisor.

.. vim: ft=rst:
