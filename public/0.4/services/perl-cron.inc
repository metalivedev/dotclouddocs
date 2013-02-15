Cron Jobs
---------
We use `PerlBrew <http://perlbrew.pl>`_ to manage the different Perl versions.
This causes problems when you want to run cron jobs because cron wants to use
the system version of Perl, instead of the one you selected.

In order to get your cron jobs to use the correct version of Perl, you will
need to use the following fully qualified path::

    /opt/perl5/perls/current/bin/perl

We realize this isn't ideal, and we are working on a better solution, but in
the meantime this should get you working.

Here is an example crontab file::

    MAILTO='me@example.com'

    # cron job that runs every minute.
    # m h dom mon dow command 
    * * * * * /opt/perl5/perls/current/bin/perl ~/current/cron_command.py

For more information about running cron jobs on dotCloud, checkout the
:doc:`periodic tasks guide <../guides/periodic-tasks>`.