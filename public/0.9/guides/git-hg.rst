:title: Git and Mercurial
:description: How dotCloud works with your Version Control System: Git, Mercurial or anything else (SVN, CVS, no source control).
:keywords: dotCloud documentation, quick start guide, 

Git and Mercurial
=================

.. include:: ../dotcloud2note.inc
.. sidebar:: Incremental transfer

   Whether you use git/mercurial or not, dotCloud will do its best to
   transfer only changed files when you push your code.


Mode Selection
--------------

CLI 0.9 no longer does automatic mode selection for pushes. Instead,
CLI 0.9 defaults to rsync and if you want to use git or mercurial, you
must state that explicitly when you ``dotcloud create`` or ``dotcloud
connect``

**If you use a git/mercurial repository** you must commit your
changes. Any outstanding change will not make it to dotCloud since
under the hood, it will use ``git push/hg push``.


Pushing Uncomitted Changes
--------------------------

If you are using git/mercurial but don't want to commit each time you want
to push to dotCloud, just add the "``--rsync``" option when you push::

  dotcloud push --rsync

It will force the uploader to use the "rsync" method, and therefore push all
your files.


Excluding Files From the Push
-----------------------------

If you are using git or mercurial, you can use their respective "ignore"
mechanisms. You can use the following rule of thumb: "if it's not pushed with
git push (respectively hg push), it won't be pushed by dotcloud push".

If you are using plain rsync upload, or if you are using git or mercurial
together with "``--rsync``", you can create a file named "``.dotcloudignore``" at
the root of your code, and list patterns to ignore.

.. note::
   If this file exists, rsync will run with "``--exclude-from
   .dotcloudignore``"; so you can use all the funkiness of rsync if you need
   complex exclusion patterns!


Omitting Code Directory
-----------------------

If you omit the last argument of the push (i.e., if you just do
``dotcloud push``), it will look for a dotcloud.yml file in the current
directory. If it cannot be found, it will look in its parent directory,
and recursively in all parent directories until a dotcloud.yml file is found.
The directory containing the dotcloud.yml will be used as a base for the push.
If no dotcloud.yml file can be found, the push will abort.

.. note::
   The ``--rsync`` flag and the ``.dotcloudignore`` file require to run
   version 0.9 of the CLI or higher.
