:title: Migrating to CLI 0.4x
:description: Migrate to the latest dotCloud CLI version.
:keywords: dotCloud documentation, CLI

Migrating to CLI 0.4.x
======================


Upgrading the CLI
-----------------

You can check your DotCloud Command-Line Interface client (CLI) version
by running the following command::

    $ dotcloud --version

You can upgrade your CLI to the latest version by running::

    $ sudo easy_install pip && sudo pip install -U dotcloud


Key Changes from 0.3.x to 0.4.x
-------------------------------

DotCloud CLI 0.4.x and the associated platform update has several major
improvements. Some of these have altered the behavior of how you can
interact with the platform. A few of the key changes include:

#. Applications are made of services. See :doc:`/firststeps/platform-overview`
   for details.
#. The DotCloud Build File (dotcloud.yml) is now required. See the
   :doc:`build-file` guide for details.
#. Setting up an application on DotCloud only requires the create
   command. The deploy command is deprecated. The Build File takes the
   place of the deploy command.
#. The push command requires you only to specify the application name,
   not the service name.
#. The URLs for accessing your services are now a generated string and
   do not use the pattern of service name and application name.


Migrating Your Application
--------------------------

The platform update requires you to explicitly move your application
from the old platform to the new platform. While it is currently
possible to use the old platform, you will eventually need to migrate
your application or you will lose the ability to push updated code and
manage your application until you do so.

The steps to migrate your application to the new platform are as
follows. Before you start, make sure you have the new CLI installed.

#. Create a dotcloud.yml file. See the :doc:`build-file` guide for details.
#. Run "dotcloud create <appname>" with the same app name.
   ::

       $ dotcloud create myapp

#. Push your code by running "dotcloud push <appname>".
   ::

       $ dotcloud push myapp

Repeat these steps for each app. Your existing application URLs will
continue to work. You can check that the migration completed
successfully for all of your apps by listing them using the "dotcloud
list" command.
