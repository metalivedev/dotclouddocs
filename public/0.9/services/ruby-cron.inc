Running cron jobs
-----------------

We use `RVM <https://rvm.io/>`_ to manage the different versions of Ruby that
you can use on the ruby service. This causes some problems when you want to run
cron jobs. In order to use the correct version of Ruby, you need to first
tell cron what Ruby version to use. There are a few different ways of doing
this, but we will recommend the most straightforward way here.

To use an RVM Ruby from cron, we need to invoke our own bash session, like so::

    MAILTO='me@example.com'

    # cron job that runs every minute.
    # m h dom mon dow command 
    * * * * * /bin/bash -l -c '~/current/cron_command.rb'

/bin/bash -l -c Explained
~~~~~~~~~~~~~~~~~~~~~~~~~

We are wrapping our command in a bash session. The -c option tells bash a
command follows. The -l option tells bash to load the entire environment.
This option is crucial. Leave out the -l and you won't load the correct
version of Ruby.

The wrapper is necessary because cron runs a very limited environment.
RVM relies on environment variables to find Ruby, gems and gem executables.

bash -l brings all this into cron’s environment, so RVM just works. You don’t
have to worry about PATH, gem executables, etc. They all work just like they
do in your interactive bash sessions.

**Important note**: in most cases, you’ll want to use single quotes for the
command you pass to bash.

For more information about running cron jobs on dotCloud, checkout the
:doc:`periodic tasks guide <../guides/periodic-tasks>`.