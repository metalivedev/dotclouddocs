PHP Hello World
===============

Prerequisites
-------------

To manage your DotCloud applications, you need the Command-Line Interface client
(CLI) installed on your computer. Before proceeding, make sure you have already
`signed up <http://www.dotcloud.com/>`_ for an account. Here are the
installation instructions:

.. tabswitcher::

   .. tab:: Linux

      Open Terminal and run the following command::

        $ sudo easy_install pip && sudo pip install dotcloud
          
   .. tab:: Windows

      Windows is not officially supported at this time. However, if you
      feel brave, here is how to get DotCloud running on Windows in less than
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

         $ wget http://peak.telecommunity.com/dist/ez_setup.py
      
      #. Install it::

      	 $ python ez_setup.py

      #. You now have easy_install; let's use it to install pip::

      	 $ easy_install pip

      #. Now install dotcloud::

      	 $ pip install dotcloud

      That's it! Remember to always go through the Cygwin shell when running
      "dotcloud".

      .. note::

         If you already have Cygwin installed and are actually upgrading it
         when setting up DotCloud, you might have to do a "rebaseall".
	 If you see weird error messages in Cygwin after upgrading, read
         `Cygwin Upgrades and rebaseall
         <http://www.heikkitoivonen.net/blog/2008/11/26/cygwin-upgrades-and-rebaseall/>`_
         for a fix! (Thanks to Kevin Li for this tip!)

   .. tab:: Max OS X

      To install, open a terminal and run the following commands::

        $ sudo easy_install pip && sudo pip install dotcloud

When the installation is finished, run “dotcloud” for the first time and
enter your API key.

::

   $ dotcloud
   Enter your api key (You can find it at http://www.dotcloud.com/account/settings): 

Hello World
-----------

Now that you have the DotCloud CLI installed, let's go ahead and deploy
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


Then create your application on the :doc:`flavor </guides/flavors>` of
your choice. In this example, we'll name the application "helloworldapp" and
use the default flavor, Sandbox, which is free::

   $ dotcloud create helloworldapp
   Created application "helloworldapp" using the flavor "sandbox" (Use for development, free and unlimited apps. DO NOT use for production.)

Next, we'll create a DotCloud build file that describes an application with a
single *php* service. Create a file named *dotcloud.yml* with the following
text:

.. code-block:: yaml

   www:
     type: php

Your application is ready with a *php* service. Now you can push your current
directory with your index.php file and your DotCloud Build File::

   $ dotcloud push helloworldapp
   ...

Congratulations!
----------------

You have just deployed your first DotCloud application.

We chose to deploy a very simple site in this example, but you'll find that it's
just as easy to deploy any kind of application. See the full list of services
available under the Services section in the navigation bar on the left. You can
mix and match various services, such as a :doc:`PHP service </services/php>` for
your PHP application, and a :doc:`MySQL service </services/mysql>` for the
database.
