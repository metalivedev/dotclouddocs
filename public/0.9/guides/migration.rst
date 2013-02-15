:title: Migrating to CLI version 0.9
:description: Migrate to the latest dotCloud CLI version.
:keywords: dotCloud documentation, CLI

Migrating to the CLI 0.9
========================

If this is your first time to install any version of the dotCloud CLI,
then you should probably read :doc:`../firststeps/install`. If you are
upgrading from the 0.4 CLI, please read on!

Upgrading the CLI
-----------------

You can check your dotCloud Command-Line Interface client (CLI) version by running the following command::

    dotcloud --version

If the output is ``0.4``, you're using an older flavor of the CLI, and
should upgrade to the newest version!

You can upgrade your CLI to the latest version by running::

    sudo easy_install pip
    sudo pip uninstall dotcloud dotcloud.cli
    sudo pip install -U dotcloud

Key changes from 0.4.x to 0.9.x
-------------------------------

This version of the CLI is designed to work with the application directory
"connected" to the remote dotCloud application.

This allows you to avoid typing the same application name multiple
times, reduces the possiblity of making typos or overwriting the wrong
applications by repeating the command line history.

+----------------------+----------------------------+
| CLI 0.9 (new)        | CLI 0.4 (old)              |
+======================+============================+
| ``dotcloud push``    | ``dotcloud push myapp .``  |
+----------------------+----------------------------+
| ``dotcloud info``    | ``dotcloud info myapp``    |
+----------------------+----------------------------+
| ``dotcloud run www`` | ``dotcloud ssh myapp.www`` |
+----------------------+----------------------------+

Other changes include:

* ``dotcloud push`` now defaults to rsync. If you want to use git or hg then you must explicitly flag this during creation or connection or pushes::

    # any one of these three will use git for pushes instead of rsync
    dotcloud create --git MY_APP
    dotcloud connect --git
    dotcloud push --git
* ``dotcloud ssh`` no longer exists. The behavior of the command can be reproduced using the following command::

    dotcloud run MY_SERVICE

* ``dotcloud setup`` no longer requires you to enter your API key. See **"Setup"** section below.
* ``dotcloud var`` has been renamed to ``dotcloud env``
*  New commands have been added:
    - ``dotcloud check`` checks your installation and credentials
    - ``dotcloud activity`` displays your recent activity on dotCloud.
    - ``dotcloud deploy`` deploys a specific revision of your application
    - ``dotcloud dlist`` lists your recent deployments.
    - ``dotcloud dlogs`` allows you to review past and running deployment logs
    - ``dotcloud revisions`` displays all the known revisions of the
      application.
    - ``dotcloud connect/disconnect``: Links or unlinks the current
        directory to a dotCloud application. See below **"Working with
        your application"**.
* Colored output!

Setup
-----

First, you have to configure your CLI by providing your dotCloud credentials::

    dotcloud setup

You're asked to provide your username and password for dotCloud, to
register the new CLI client as a dotCloud REST API consumer. You can
also use your email, instead of your username.

The CLI won't save this credentials locally - instead, it will save
the OAuth2 access token in the local disk. Once the setup is complete,
you can run the check command to see if everything is configured
correctly::

    dotcloud check

If this fails, try removing the directory ``~/.dotcloud_cli`` and start
over from the setup.

Working with your application
-----------------------------

::

    cd ~/dev
    mkdir myapp
    (write some code)

Once you've done writing your awesome application, run the `create` command::

    dotcloud create myapp

You will notice that the CLI asks you if you want to connect the current
working directory to the remote application. This allows you to omit
the application name when typing commands from now on.

Running commands
^^^^^^^^^^^^^^^^

To push the code to the dotCloud platform, simply type::

    dotcloud push

and it will upload the code from the current directory to the
application. You can see the currently connected application by typing::

    dotcloud app

You can see the list of commands by running ``dotcloud -h``.

If you typed ``n`` when asked to connect the current directory, then the CLI
will not find the application name for the commands. You can specify the
application name in such case, using the ``--application`` (or ``-A`` for
short) option::

    dotcloud -A myapp info

You can also use this option when you want to run commands against the
application that you don't have the working directory for.

Connecting your app
^^^^^^^^^^^^^^^^^^^

Similarly, if you already have a working directory *and* a dotcloud
remote application and want to connect them together, instead of
creating a new application, run the connect command::

    cd ~/dev/myapp
    dotcloud connect myapp

It will link your current working directory with the (existing)
dotcloud application ``myapp``.  You can revert this operation using
``dotcloud disconnect`` in your application directory.
