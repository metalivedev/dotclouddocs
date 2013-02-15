PHP Hello World
===============

Installing the CLI
------------------

To manage your dotCloud applications, you need the Command-Line
Interface client (CLI) installed on your computer.

.. tabswitcher::

   .. tab:: Linux

      Open Terminal and run the following command::

        sudo easy_install pip && sudo pip install https://github.com/dotcloud/dotcloud-cli/tarball/master

      If you don't have ``easy_install``, you need to install it;
      preferably with your distribution specific packaging tools.
      On Fedora, RHEL, and other systems providing ``yum``, you
      should run ``sudo yum install python-setuptools``. On Debian,
      Ubuntu, and other systems providing APT, you should run
      ``sudo apt-get install python-setuptools``. Then install ``pip``
      and ``dotcloud`` as shown above.

   .. tab:: Windows

      Windows is not officially supported at this time. However, if you
      feel brave, here is how to get dotCloud running on Windows in less than
      one minute (not counting download times).

      #. Start the `Cygwin Setup <http://cygwin.com/setup.exe>`_.
      #. Select default choices until you reach the package selection dialog.
      #. Enable the following packages:

         * net/openssh
         * net/rsync
         * devel/git
         * devel/mercurial
         * python/python (make sure it's at least 2.6!)
         * web/wget

      #. After the installation, you should have a Cygwin icon on your desktop.
         Start it: you will get a command-line shell.
      #. Download easy_install::

          wget http://peak.telecommunity.com/dist/ez_setup.py

      #. Install it::

          python ez_setup.py

      #. You now have easy_install; let's use it to install pip::

          easy_install pip

      #. Now install dotcloud::

          pip install https://github.com/dotcloud/dotcloud-cli/tarball/master

      That's it! Remember to always go through the Cygwin shell when running
      "dotcloud".

      .. note::

         If you already have Cygwin installed and are actually upgrading it
         when setting up dotCloud, you might have to do a "rebaseall".
         If you see weird error messages in Cygwin after upgrading, read
         `Cygwin Upgrades and rebaseall
         <http://www.heikkitoivonen.net/blog/2008/11/26/cygwin-upgrades-and-rebaseall/>`_
         for a fix! (Thanks to Kevin Li for this tip!)

   .. tab:: Mac OS X

      To install, open a terminal and run the following commands::

        sudo easy_install pip && sudo pip install https://github.com/dotcloud/dotcloud-cli/tarball/master

When the installation is finished, run ``dotcloud setup`` for the first time and
enter your dotCloud credentials:

.. FIXME Your API key is ****

::

   dotcloud setup



.. note::
  Are you used to our old CLI? You will be able to find the major differences between the two
  conveniently compiled in `this document <https://github.com/dotcloud/dotcloud-cli/blob/master/README.md>`_.

Hello World
-----------

Now that you have the dotCloud CLI installed, let's go ahead and deploy
our first application. In this example, we will be deploying a very
simple application that serves a "Hello World" php script.

Create a new folder and change to that folder. Fire up your favorite
text editor, and write a message to yourself and save it as index.php:

.. code-block:: php

   <html>
     <head>
       <title>Hello World!</title>
     </head>
     <body>
       <?php echo "Hello World!\n"; ?>
     </body>
   </html>


Then create your application with the :doc:`flavor </guides/flavors>` of
your choice. In this example, we'll name the application "helloworldapp" and
use the default flavor, Sandbox, which is free::

   dotcloud create helloworldapp



Next, we'll create a dotCloud Build File that describes an application with a
single *PHP* service. Create a file named *dotcloud.yml* with the following
text:

.. code-block:: yaml

   www:
     type: php

Your application is ready with a *PHP* service. Now you can push your current
directory with your index.php file and your dotCloud Build File::

   dotcloud push

Congratulations!
----------------

You have just deployed your first dotCloud application.

We chose to deploy a very simple site in this example, but you'll find that it's
just as easy to deploy any kind of application. See the full list of services
available under the Services section in the navigation bar on the left. You can
mix and match various services, such as a :doc:`PHP service </services/php>` for
your PHP application, and a :doc:`MySQL service </services/mysql>` for your
database.
