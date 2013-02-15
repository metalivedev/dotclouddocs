:title: Installing the CLI
:description: Install the dotCloud Command Line Interface.
:keywords: dotCloud CLI, dotCloud documentation, installing the CLI, Installation instructions

.. rst-class:: next

:doc:`quickstart`


Installing the CLI
==================

To manage your DotCloud applications, you need the Command-Line
Interface client (CLI) installed on your computer. Before proceeding,
make sure you have already `signed up <http://www.dotcloud.com/>`_ for
an account.


Installation Instructions
-------------------------


.. tabswitcher::

   .. tab:: Linux

      Open Terminal and run the following command::

        $ sudo easy_install pip && sudo pip install dotcloud

      If you don't have ``easy_install``, you need to install it;
      preferably with your distribution specific packaging tools.
      On Fedora, RHEL, and other systems providing ``yum``, you
      should run ``sudo yum install python-setuptools``. On Debian,
      Ubuntu, and other systems providing APT, you should run
      ``sudo apt-get install python-setuptools``. Then install ``pip``
      and ``dotcloud`` as shown above.
          
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

   .. tab:: Mac OS X

      To install, open a terminal and run the following commands::

        $ sudo easy_install pip && sudo pip install dotcloud
          
When the installation is finished, run “dotcloud” for the first time and
enter your API key.

.. FIXME Your API key is ****

::

   $ dotcloud
   Enter your api key (You can find it at http://www.dotcloud.com/settings): 
                      

.. rst-class:: next

:doc:`quickstart`
