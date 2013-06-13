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

   .. tab:: Mac OS X

      To install, open your terminal and run the following command::

        sudo easy_install pip && sudo pip install dotcloud

   .. tab:: Windows

      dotCloud developer support cannot help you install or use the
      ``dotcloud`` CLI on Windows. You may find some help on
      `StackOverflow <http://stackoverflow.com/search?q=%5Bdotcloud%5D+windows>`_ (in particular, the old installation steps are `here <http://stackoverflow.com/questions/16969119/dotcloud-push-on-cygwin-fails-with-rsync-error-unexplained-error-code-255>`_ )

When the installation is finished, run “dotcloud setup” for the first time and
enter your dotCloud credentials:

.. FIXME Your API key is ****

.. code-block:: none

   dotcloud setup


.. note::
  Are you used to our old CLI? Head over to our :doc:`../guides/migration` guide.

.. rst-class:: next

:doc:`quickstart`
