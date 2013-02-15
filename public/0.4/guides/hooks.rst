:title: Build Hooks
:description: specifics hooks are available during the build and installation of an application.
:keywords: dotCloud documentation, quick start guide, build hooks

Build Hooks
===========

You can ask the dotCloud platform to run custom scripts at three
different moments when you push an application:

- before the build starts;
- at the end of the build;
- after the build is deployed (just before the services are actually started).

.. note:: 
   The dotCloud build system checks the return value of each
   script used as a build hook. If the script returns anything except
   0 then that is considered an error and the build fails.

.. warning::
   If you *want* the build to halt on an error, be sure you
   exit your script with an error code (non-zero) as soon as the error
   occurs, or else a following statement which succeeds could mask the
   error. One easy way to do this in a shell script is to add ``set
   -e``


Pre-Build
---------

The pre-build hook is an optional command defined in your ``dotcloud.yml`` that
will be executed *before* every build of your application. The command to
execute has to be defined under the key ``prebuild`` in your service.

.. note::

   The command will be executed at the root of your application, or in the
   :ref:`approot <guides_service_approot>` if defined.

Example
^^^^^^^

If you have some external requirements in a private code repository
(e.g: a github private repository), you can use a pre-build hook to setup
SSH keys to access this repository.

Add the ``prebuild`` section to your ``dotcloud.yml`` file:

.. code-block:: yaml

    www:
      type: python
      prebuild: ./prebuild.sh

And create the ``prebuild.sh`` script:

.. code-block:: sh

    #!/bin/sh
    set -e
    cat >~/.ssh/id_rsa <<EOF
    -----BEGIN RSA PRIVATE KEY-----
    MIIEowIBAAKCAQEArV8je2p2OUkRBEbPDSXvxWCXkEk74S0bMs7VmForvaM/9PNo
    [...] pPu6QS3EMG2Z4gJiyY
    -----END RSA PRIVATE KEY-----
    EOF


Post-Build
----------

The pre-install hook is an optional command defined in your
``dotcloud.yml`` that will be executed *after* every build of your
application. The command to execute has to be defined under the key
``"postbuild"`` for the concerned service.

.. note::

   If you are using an :ref:`approot <guides_service_approot>` your postinstall
   script should be in the directory pointed by the ``approot`` directive of your
   dotcloud.yml.

For example, if you want to remove the SSH key installed by the pre-build hook,
you can do the following:

.. code-block:: yaml

    www:
      type: python
      prebuild: ./prebuild.sh  # from the previous example
      postbuild: rm -f ~/.ssh/id_rsa

.. _postinstall_hook:

Post-Install
------------

The post-install hook is an optional special script placed at the root of your
application that will be executed by dotCloud after every push.

The script must be executable and must be named ``"postinstall"``. So, if you
are not on Windows don't forget to run ``"chmod +x postinstall"``.

.. note::

   If you are using an :ref:`approot <guides_service_approot>` your postinstall
   script should be in the directory pointed by the "approot" directive of your
   dotcloud.yml.

For example the post-install hook is the simplest way to setup your
:doc:`crontab <periodic-tasks>`:

.. code-block:: sh

   #!/bin/sh
   set -e
   crontab ~/current/jobs/crontab

You can write your post-install hook in any language, as long as you specify
the right language to use with a *shebang*::

   #!/usr/bin/env python

Obviously, replace "python" by your interpreter, e.g: "ruby", "node", "perl"…

.. note::

   The :doc:`~/data directory <persistent-data>` is moved from the
   previous version of your service after the post-install hook
   execution. This means that the post-install hook cannot modify the
   content of the ``~/data`` directory after the first push.

.. warning::

   If your post-install script returns an error (non-zero exit code),
   or if it runs for more than 10 minutes, the platform will consider
   that your build has failed, and the new version of your code will
   not be deployed.

