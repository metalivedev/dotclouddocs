:title: Git and Mercurial
:description: How dotCloud works with your Version Control System: Git, Mercurial or anything else (SVN, CVS, no source control).
:keywords: dotCloud documentation, quick start guide, 

Git and Mercurial
=================

.. sidebar:: Incremental transfer

   Whether you use git/mercurial or not, dotCloud will do its best to
   transfer only changed files when you push your code.


Automatic Mode Selection
------------------------

When you do "dotcloud push ramen my-code-directory", different things
can happen.

* If "my-code-directory" is a mercurial repository 
  (i.e. it contains a ".hg" subdirectory),
  it will be pushed to dotCloud just as if you did "hg push".
* If "my-code-directory" is a git repository 
  (i.e. it contains a ".git" subdirectory),
  it will be pushed to dotCloud just as if you did "git push".
* If "my-code-directory" is none of the above, 
  it will be uploaded to dotCloud in full.

What does that mean for you?

**If you use a git/mercurial repository**, by default, you have to commit your
changes. Any outstanding change will not make it to dotCloud (since under
the hood, it will use "git push"/"hg push".


Pushing Uncomitted Changes
--------------------------

If you are using git/mercurial but don't want to commit each time you want
to push to dotCloud, just add the "``--all``" option when you push::

  $ dotcloud push --all ramen

It will force the uploader to use the "rsync" method, and therefore push all
your files.


Excluding Files From the Push
-----------------------------

If you are using git or mercurial, you can use their respective "ignore"
mechanisms. You can use the following rule of thumb: "if it's not pushed with
git push (respectively hg push), it won't be pushed by dotcloud push".

If you are using plain rsync upload, or if you are using git or mercurial
together with "``--all``", you can create a file named "``.dotcloudignore``" at
the root of your code, and list patterns to ignore.

.. note::
   If this file exists, rsync will run with "``--exclude-from
   .dotcloudignore``"; so you can use all the funkiness of rsync if you need
   complex exclusion patterns!


Omitting Code Directory
-----------------------

If you omit the last argument of the push (i.e., if you just do
"dotcloud push ramen"), it will look for a dotcloud.yml file in the current
directory. If it cannot be found, it will look in its parent directory,
and recursively in all parent directories until a dotcloud.yml file is found.
The directory containing the dotcloud.yml will be used as a base for the push.
If no dotcloud.yml file can be found, the push will abort.

.. note::
   The "``--all``" flag and the "``.dotcloudignore``" file require to run
   version 0.4.2 of the CLI or higher.
