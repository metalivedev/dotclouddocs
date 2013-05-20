S3FS Shared Filesystem
======================

.. note::

  We've seen issues with the fuse libraries.  We no longer recommend this type
  of configuration for production systems and suggest using a package like
  https://github.com/boto/boto


If you need to share files between multiple services (or between multiple
instances of a scaled services), you can mount FUSE-based filesystems like
S3FS or GlusterFS (just to name a few).

We will show here how you can use 
`S3FS <http://code.google.com/p/s3fs/wiki/FuseOverAmazon>`_
to access a S3 bucket from your dotCloud service.


Requirements
------------

You will need your own S3 bucket in your own AWS account.
Note that S3 is pretty cheap, and if you create a bucket in the default region,
it will be in the same region as the main dotCloud cluster; meaning that you
won't have to pay for data transfer. You will mostly pay for storage,
which amounts to pennies if your data is small. 

The recipe described here only works for *code* services. It won't work
with MySQL, PostgreSQL, and Redis services.

Before starting, make sure that you have:

* your S3 bucket name,
* an AWS access key and secret key with full access to the bucket.

Also, your app should be using the Granite runtime. At the end of the build,
if you see ``Waiting for the service to become responsive...``, it means
that you are using it. If you don't see that line, it means that your
app is still on the old builder and that you have to create a new app
(since all new apps use Granite by default).


System Packages
---------------

We will need the FUSE userspace tools and libraries. While we could
install them manually, it will be faster & easier to ask the dotCloud
platform to install them for you!

Edit your ``dotcloud.yml`` file, and add a ``systempackages`` section
similar to the following one:

.. code-block:: yaml

   www:
     type: php
     approot: src
     systempackages:
       - libfuse2
       - fuse-utils

The next time you will push your code, the platform will detect this
section and install those packages for you.


S3FS binary and helper script
-----------------------------

You will need to install the S3FS binary. You can install your own,
or download our precompiled 
`S3FS binary <http://dotcloud-plugins.s3.amazonaws.com/s3fs>`_.
Our version has only one slight difference: it skips some initial
checks on the S3 bucket, allowing you to mount the bucket anyway when
you don't have the permission to list all the buckets. That way.
you can create sets of credentials that have only read/write access
to a bucket, and no other access, and use them with S3FS.

You will also need the :download:`S3FS script <run.s3fs>`, which takes
care of extracting the required parameters from the dotCloud environment.

Download both files (or use your own versions if you prefer), make sure
that they are both executable (with ``chmod +x``), and put them in your
approot.

Put both files the S3FS binary in your approot.


Setup Supervisor
----------------

To make sure that the S3 bucket is mounted automatically, we will use
a small Supervisor configuration file snippet. Create a ``supervisord.conf``
file in your approot, and insert the following section (if you already
have a ``supervisord.conf`` file, no problem, just append the section):

.. code-block:: ini

   [program:s3fs]
   command=/home/dotcloud/current/run.s3fs
   stdout_logfile=/var/log/supervisor/s3fs.log
   stderr_logfile=/var/log/supervisor/s3fs.log


Configure S3FS
--------------

You will have to set the bucket name, AWS credentials, and desired mountpoint.
To avoid having to store secure credentials along with your code (which might
be in a shared or public repository), we will use the dotCloud environment
variables. The following 4 variables should be set:

* ``S3FS_BUCKET``: name of the S3 bucket; you don't need to include a ``s3://``
  prefix or anything like that.
* ``S3FS_MOUNTPOINT``: absolute path to which the bucket should be mounted
  this directory will be created automatically if it doesn't exist. 
  We suggest to use ``/home/dotcloud/store``.
* ``S3FS_ACCESSKEY``: the AWS access key (it should be about 20 characters
  long).
* ``S3FS_SECRETKEY``: the AWS secret key (it should be about 40 characters
  long).

To set them, you will run a command like the following one::

  dotcloud env set \
             S3FS_BUCKET=yourbucketname \
             S3FS_MOUNTPOINT=/home/dotcloud/store \
             S3FS_ACCESSKEY=AKI1234567ABCDEFGHIJ \
             S3FS_SECRETKEY=abcdefghij1234567890ABCDEFGHIJ+987654321

After pushing your app and setting the variables, your S3 bucket should
be available. You can check that by logging in with ``dotcloud ssh``
and going to the mountpoint directory.


Custom Service
--------------

.. note::
   The *custom service* is a beta feature. If you are not part of the beta
   group, you can safely ignore this section. Otherwise, if you started
   to use the custom service and want to add S3FS, read on -- the procedure
   is slightly different.

If you are using a custom service (i.e., one that has ``type: custom`` in
the ``dotcloud.yml``), you should adapt two things.

First, you need to make sure that the *builder script* will copy your
files with the rest of the code.

Next, since the custom service does not use ``supervisord.conf`` snippets,
you will have to use a ``processes`` directory instead.

If your current custom service looks like this:

.. code-block:: yaml

   www:
     type: custom
     buildscript: foo/builder
     ports:
       www: http
     process: ~/run

You will have to make sure that ``foo/builder`` copies ``s3fs`` and
``run.s3fs`` in the target build directory, and update the ``dotcloud.yml``
to add ``systempackages`` and upgrade ``process`` to ``processes``, as
shown below:

.. code-block:: yaml

   www:
     type: custom
     buildscript: foo/builder
     ports:
       www: http
     process:
       foo: ~/run
       s3fs: ~/run.s3fs


Troubleshooting
---------------

The S3FS process logs are visible with ``dotcloud logs``. If anything
goes wrong, your best bet is to double-check your AWS credentials.
Remember that all dotCloud services include ``s3cmd``, a small and
convenient S3 command-line client. Try ``s3cmd --configure`` and then
``s3cmd ls s3://nameofthebucket`` to see what happens. ``s3cmd`` is usually
more informative than S3FS.

.. note::

    There may be issues when connecting to s3 buckets with existing content. It
    may require re-creating the directories on the newly mounted directory for
    the existing data to appear.  Before trying this suggested workaround, it is
    recommended that you backup your data.

    http://code.google.com/p/s3fs/issues/detail?id=73
