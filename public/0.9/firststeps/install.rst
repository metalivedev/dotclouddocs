:title: Installing the CLI
:description: dotCloud's guide to install the command-line interface tool.
:keywords: dotCloud, dotCloud documentation, quick start guide, prerequisites, CLI, command line

.. rst-class:: next

:doc:`quickstart`

Installing the CLI
==================

To manage your dotCloud applications, you need the Command-Line
Interface client (CLI) installed on your computer. If you have previously installed an older version of the CLI, you may prefer to read :doc:`../guides/migration`

.. tabswitcher::

   .. tab:: Linux

      Open Terminal and run the following command::

        sudo easy_install pip && sudo pip install dotcloud

      If you don't have ``easy_install``, you need to install it;
      preferably with your distribution specific packaging tools.
      On Fedora, RHEL, and other systems providing ``yum``, you
      should run ``sudo yum install python-setuptools``. 
      On Debian, Ubuntu, and other systems providing APT, you should 
      run ``sudo apt-get install python-setuptools``. Then install 
      ``pip`` and ``dotcloud`` as shown above.

   .. tab:: Windows

      Windows is not officially supported at this time. However, if you
      feel brave, here is how to get dotCloud running on Windows in less than
      one minute (not counting time to download).

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
      #. Download easy_install
         
          .. code-block:: none

            wget http://peak.telecommunity.com/dist/ez_setup.py

      #. Install it
         
          .. code-block:: none

            python ez_setup.py

      #. You now have easy_install; let's use it to install pip
         
          .. code-block:: none

            easy_install pip

      #. Now install dotcloud
         
          .. code-block:: none
         
            pip install dotcloud

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

      To install, open your terminal and run the following command::

        sudo easy_install pip && sudo pip install dotcloud

When the installation is finished, run “dotcloud setup” for the first time and
enter your dotCloud credentials:

.. FIXME Your API key is ****

.. code-block:: none

   dotcloud setup


.. note::
  Are you used to our old CLI? Head over to our :doc:`../guides/migration` guide.

.. rst-class:: next

:doc:`quickstart`
