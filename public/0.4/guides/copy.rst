:title: Copy Files To and From Services
:description: How to transfer files to and from your dotCloud services.
:keywords: dotCloud documentation, quick start guide, copy files, transfer files, dotCloud server, text file, Binary files, multiple files, SSH

Copy Files To and From Services
===============================

There are a few cases where you would need to transfer files to and from
your servers:

* you want to upload manually a bundle of confidential data that you don't
  want to put in your code repository;
* you want to regularly update some data on your service, without having to
  do a complete push each time (e.g., some JSON files containing stuff that
  you want to update on a daily basis);
* you want to transfer some user-uploaded data from your service to somewhere
  else (another service, or a 3rd party server);
* something else.

Here are a few recipes to do that!

.. note::

   ``scp`` and ``rsync`` are available too. Hop down to :ref:`the last
   sections of this guide <generic-ssh>`.


Download a Single Text File
---------------------------

If you want to download a single text file (as opposed to "multiple files" or
to "some binary file"), you can use the following command::

  $ dotcloud run myapp.myservice cat remotefilename > localfilename


Upload a Single Text File
-------------------------

This will be slightly more complex than the download case, since we need
to do a remote redirection. Quoting is of the essence here::

  $ dotcloud run myapp.myservice "cat > remotefilename" < localfilename


Download Binary or Multiple Files
---------------------------------

The SSH connection provided by ``dotcloud run`` is (unfortunately!) not
binary-safe. Therefore, we will protect the transfer with ``base64``
encoding::

  $ dotcloud run myapp.myservice -- \
             "tar -chf- remotefileordirectory 2>/dev/null | base64 -w0" \
             | base64 --decode > localarchivename.tar

This will:

* run ``tar`` inside your service,
* create an archive containing ``remotefileordirectory``,
* dereference symlinks thanks to the ``-h`` flag (not always necessary,
  but since DotCloud uses some symlinks here and there, it's probably a
  good idea!),
* use ``base64`` to make sure that the archive won't get mangled by the
  unsafe SSH session,
* use a single long line thanks to ``-w0``, to avoid end-of-line encoding
  issues,
* decode the base64 stream on your local machine,
* store the decoded tar archive into ``localarchivename.tar``.


Upload Binary or Multiple Files
-------------------------------

This will be quite similar to the previous example.

To upload a single binary file from a Linux machine::

  $ base64 -w0 localbinaryfile | dotcloud run myapp.myserver -- \
           "base64 --decode > remotebinaryfile"

To upload a local directory from a Linux machine::

  $ tar -cf- localdirectory | base64 -w0 | dotcloud run myapp.myserver -- \
        "base64 --decode | tar -xf-"

If your machine is not a Linux machine, please adjust the parameters
to the first ``base64`` to avoid adding any line endings. On a Mac
this is the default. On a Linux machine (like your dotCloud container)
this requires ``-w0``.

Note that this will create "localdirectory" in "/home/dotcloud" in the
remote server. If you want it with another name or at another place,
you can fix it manually with "dotcloud ssh", or use tar options to
specify the output directory.

.. _generic-ssh:

Generic SSH (scp, rsync...)
---------------------------

If you are used to ``scp`` or ``rsync``, you can also setup a SSH configuration
file to use direct SSH connection to your service. Here is how.

#. Retrieve the SSH parameters (user, host, and port) of your service, using
   ``dotcloud info myapp.myservice``.
#. Append the following section to your ``~/.ssh/config`` file (create this
   file if it does not exist), replacing HOST, PORT and USER with the values
   shown by ``dotcloud info``::

     Host myapp.myservice
       HostName HOST
       Port PORT
       User USER
       IdentityFile ~/.dotcloud/dotcloud.key

#. You can now do things like:

   * ``ssh myapp.myservice``
   * ``scp somefile myapp.myservice:``
   * ``rsync -av somedirectory/ myapp.myservice:remotedirectory/``

.. warning::

   If you are using multiple DotCloud accounts, and switch between them
   with the ``DOTCLOUD_CONFIG_FILE`` environment variable, you have to
   change the ``IdentityFile`` statement accordingly.


Connecting From a Service to Another
------------------------------------

The procedure is the same, but you have to copy the ``dotcloud.key`` file
to the remote service first.
