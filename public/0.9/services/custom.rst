Custom Service
==============

.. include:: ../dotcloud2note.inc

The custom service does not include specific support for a fixed
language or framework. Instead, it comes pre-loaded with a whole host
of interpreters, compilers, and libraries, as well as a very flexible
build process; allowing you to run virtually any application on dotCloud.

Here is a non-exhaustive list of services that have already been written
using this custom service:

* new databases like 
  `Riak <https://github.com/dotcloud/riak-on-dotcloud-ALPHA>`_,
  `CouchDB <https://github.com/dotcloud/couchdb-on-dotcloud-ALPHA>`_,
  `Neo4j <https://github.com/jpetazzo/neo4j-on-dotcloud-ALPHA>`_;
* search engines like `ElasticSearch
  <https://github.com/dotcloud/elasticsearch-on-dotcloud-ALPHA>`_;
* languages like Haskell, 
  `Erlang <https://github.com/dotcloud/ucengine-on-dotcloud-ALPHA>`_;
* ready-to-run apps like
  `JIRA <https://github.com/dotcloud/jira-on-dotcloud>`_,
  `Jenkins <https://github.com/dotcloud/jenkins-on-dotcloud>`_,
  `Gitosis <https://github.com/dotcloud/gitosis-on-dotcloud>`_,
  `Hummingbird <https://github.com/jpetazzo/hummingbird>`_;
* `and many more! <https://github.com/search?q=on-dotcloud&type=Repositories>`_

Custom services can be used in your dotCloud stack just like any other
service. For instance, you could have a dotCloud application using
the stock :doc:`nodejs` service, with a CouchDB database running in
as a custom service.


.. contents::
   :local:
   :depth: 1


Authoring a Simple Custom Service
---------------------------------

You can deploy a custom service with this minimal ``dotcloud.yml`` file:

.. code-block:: yaml

   www:
     type: custom

However, this custom service would be (almost) totally useless,
because it would not start any process or run any code by itself.
To make our custom service slightly more useful, we need to add
some code (obviously!) and a *build script* (not to be confused
with the *build file*, which is the ``dotcloud.yml`` file).

For our example, we will author a service that runs an arbitrary
Python server supposed to listen on a given TCP port and answer
to HTTP requests.

We will use the following :download:`httphello.py` sample program:

.. literalinclude:: httphello.py
   :language: python

This Python script will start a HTTP server exposing a simple WSGI
application. Note that we can't make it listen on an arbitrary TCP port.
We use the port given by the environment variable ``PORT_WWW``
(this variable will be automatically set by the dotCloud runtime).
To make it easy to run the application in our local environment,
we added a fallback to port 8080. This is of course not mandatory --
just a mere convenience.

We now need the *build script*. You can write your build script to do
many things: fetch additional dependencies (e.g. using ``requirements.txt``
like the usual dotCloud Python builder does), compile and/or install
third party libraries...

But the most important thing is to *install* somehow the code that your
service will be running. Here is a sample *build script* to do just that --
let's call it ``builder``:

.. code-block:: sh

   #!/bin/bash
   cp -a . ~
   cd
   ln -s httphello.py run

The next section will give more format "do" and "don't" about what
the *build file* can and should do. For now, just note that we:

#. Copied all the code to ~ (the home directory), because stuff has
   to be in this directory to be available later when the service
   will be run.
#. Created a symlink called "run", because by default, the custom service
   will try to start the executable called "run" sitting in the home
   directory.

Now, change your ``dotcloud.yml`` (a.k.a. *build file*) to reflect
the location of the *build script*:

.. code-block:: yaml

  www:
    type: custom
    buildscript: builder

At this point, we should have those files in a directory::

  my-custom-service/
  ├── builder
  ├── dotcloud.yml
  └── httphello.py

We can now push this service to dotCloud::

  dotcloud create mycustomservice
  dotcloud push my-custom-service

At the end of the push, the URL of the service will be shown.

Congratulations, you just authored your first custom service!


Build Script Modus Operandi
---------------------------

There are a few simple principles that you should know about how
the *build script* works and what it should, and shouldn't, do.

The *build script* will be executed inside a directory containing
all your code. Remember this if your *build script* is located in
a subdirectory! (The next section will explain how to cleanly
run the *build script* in a subdirectory).

The *build script* can (and should!) copy all the files that you will
need to ~ (yes, the ``$HOME`` directory). If you don't copy something,
it won't be available when the service will run. The easiest solution
is generally do to something like ``cp -a . ~``, but you might want
to be more selective. For instance, Python code making use of
setuptools can generally be installed in a cleaner way using
``python setup.py install``.

You should not assume anything about the location of your code while
the *build script* runs. Currently it is under ``/tmp/code``. That
might change in the future, so never ever hard-code the path where
your *build script* will run. As said previously, you can however
rely on the fact that the *build script* will run at the root of your
code (that is the "absolute" root of your code; not the *approot* that
you might have used previously in classical services).

The place where your service is *built* will not always be the same
as the place where your service is *run*: your code can be placed
on a server, where the *build script* will be run, and then,
the result of the build (i.e. the content of the ``$HOME`` directory)
will be moved to another server, which will actually run your service.
That explains why you should make sure that anything you need will
be in ``$HOME``.

The *build environment* is different from the *run environment*:
notably, the ``PORT_WWW`` environment variable used in the example above will
be only available when the service is *run*, which means that the
builder cannot use it. As a consequence, if you need to e.g. setup the
HTTP port of a server in a configuration file, you have to do it at run time
instead. Same thing if you are making use of the dotCloud environment file:
it will be available only when the service is run.


Put the Build Script in a Separate Directory
--------------------------------------------

At some point, you will probably want to have multiple custom services
in your stack. Even if you don't, it would be a good idea to put
all the files related to a given custom service in a separate subdirectory,
like this::

  my-custom-service/
  ├── dotcloud.yml
  └── simplecustom
      ├── builder
      └── httphello.py

Let's update our ``dotcloud.yml`` file like this:

.. code-block:: yaml

  www:
    type: custom
    buildscript: simplecustom/builder

Remember that the *build script* will be run from the top-level directory,
regardless of where it's located. We could change the above *build script*
to look like the following one:

.. code-block:: sh

   #!/bin/bash
   cp -a simplecustom ~
   cd
   ln -s httphello.py run

But we don't want to hard-code the path of the *build script*, so we
propose that you do this instead:

.. code-block:: sh

   #!/bin/bash
   cd "$(dirname "$0")"
   cp -a ~
   cd
   ln -s httphello.py run

The first line of the script will change the current directory to the
directory where the *build script* is sitting, wherever that is.

If you need to go back to the root of the code at a later point,
you can either save it in the beginning with e.g. ``CODEROOT="$(pwd)"``
or run sections of the *build script* into subshells.


Use ``approot`` Build Parameter
-------------------------------

If you are used to the classical (i.e. non-custom) services of the
dotCloud platform, you have probably already used the ``approot`` parameter
in your *build file*. Let's see how we can improve our sample
*build script* to use this parameter. First, change ``dotcloud.yml``
as follows:

.. code-block:: yaml

   www:
     type: custom
     buildscript: simplecustom/builder
     approot: httphello

All the parameters set for our service will be available as
upper-cased environment variables prefixed with ``SERVICE_``. The
``approot`` will therefore be ``$SERVICE_APPROOT``, and our build
script will look like the following one:

.. code-block:: sh

   #!/bin/bash
   cd "$SERVICE_APPROOT"
   cp -a ~
   cd
   ln -s httphello.py run


Specify Which Program(s) To Run
-------------------------------

Now, what if you want to run a program other than ``httphello.py``?
How can we specify the name of the program to run, without changing
the *build script*? What if we want to run multiple programs at the
same time?

There is a *build file* parameter for that!

By default, the custom service tries to execute the program named ``run``.
But you can change that easily, using the ``process`` parameter.
Here is a ``dotcloud.yml`` file that uses this parameter:

.. code-block:: yaml

   www:
     type: custom
     buildscript: simplecustom/builder
     approot: httphello
     process: ~/httphello.py

.. note::
   Since our *build script* copies everything under ``$HOME``, our
   ``process`` directive specifies ``~/httphello.py`` instead of just
   ``httphello.py``. If you want to run a program that is installed
   in one of the directories listed by the ``$PATH`` environment
   variable, you can (and should!) remove the ``~/`` part, of course.

You can then simplify a little bit the *build script*, since we
don't need to create the ``run`` symlink anymore:

.. code-block:: sh

   #!/bin/bash
   cd "$SERVICE_APPROOT"
   cp -a ~

If you need to run multiple processes, the syntax of ``dotcloud.yml``
will be slightly different:

.. code-block:: yaml

   www:
     type: custom
     buildscript: simplecustom/builder
     approot: httphello
     processes:
       hello: ~/httphello.py
       goodbye: ~/some-other-program

.. note::
   The ``processes`` variable is not a list, it's a dictionary.
   The name you give to each process will be used as a base for
   log files, and will allow you to stop/start/restart them
   independently by name.


Available Resources
-------------------

Keep in mind that a custom service has the same amount of resources
as other services. Therefore, while you could technically run a whole
LAMP stack inside a custom service, it is probably not advised to do so,
unless you have tightened very carefully the resources alloted to
Apache, PHP, and MySQL.

Also, scaling up a compound service will be much more difficult
than scaling up a service running only one basic building block.


Scaling
-------

Scaling is only partially supported with custom services. If you
scale a custom service handling HTTP traffic, the HTTP requests
will be balanced between the different service instances in a round-robin
fashion. TCP and UDP traffic will, however, only hit the first
instance.

Our roadmap towards fully scalable and high-available custom
services includes the following items:

* expose in ``environment.json`` the individual TCP and UDP ports
  of each service (instead of the first service only);
* give an option, for each port, to choose between "load balancing"
  and "master/slave": the first one dispatches the connections (or
  packets, for UDP traffic) to all instances, while the second one
  will send all the traffic to a single instance, swapping it transparently
  with another instance if it goes down (just like our MySQL
  and Redis master/slave services already do).

Meanwhile, it is technically possible to emulate the "load balancing"
behavior by deploying e.g. a custom service holding a HAProxy load
balancer, and sending traffic to multiple single-instance services,
as show in the fictious build file below:

.. code-block:: yaml

   www:
     type: python
     instances: 4
   zmqlb:
     type: custom
     buildscript: haproxy/builder
     haproxy_mode: tcp
     haproxy_backends: zmqback*
   zmqback1:
     type: custom
     buildscript: zmqback/builder
     ports:
       zmq: tcp
   zmqback2:
     type: custom
     buildscript: zmqback/builder
     ports:
       zmq: tcp
   zmqback3:
     type: custom
     buildscript: zmqback/builder
     ports:
       zmq: tcp

We don't recommend using that kind of recipe for anything else than
testing, but hey, if that's the only piece missing to deploy your app
on dotCloud, here you have it!


Profile
-------

Your builder can create the file ``~/profile``. It will be sourced
by the shell spawning other processes at runtime. It would be a good
place to set environment variables like ``$PATH``.


Postinstall
-----------

If you need to perform additional tasks after deploying your code,
but in the *run* environment instead of the *build* environment,
you can define a ``postinstall`` entry in your ``dotcloud.yml``.

Example:

.. code-block:: yaml

   www:
     postinstall: ./postinstall.sh

.. note::

   **What's the difference between ``buildscript`` and ``postinstall``?**

   The Build Script does not have access to the whole environment,
   but its output can potentially be saved for later use (e.g. deploying
   it on multiple scaled instances, to avoid repeating lengthy builder
   steps at each push/scale order). The ``postinstall``, on the other
   hand, will have access to the full environment (since it's running
   on the "live thing") but it will run *each time*, so any time-consuming
   operation done here will make your pushes longer.

.. note::

   **Is there any difference between ``postinstall`` and ``run`` scripts?**

   Not much! You could equally use a run script to "do stuff" and then
   ``exec`` your process; or "do stuff" in ``postinstall`` and start
   your process with the ``process`` statement in ``dotcloud.yml``.

   When authoring a re-usable custom service, it is recommenced to *not*
   use ``postinstall`` (since you can do everything from the run script
   anyway), so if the users of your service need some extra customization,
   they can start with a blank ``postinstall`` instead of editing an
   existing file.


Logging
-------

Each process started by the service instance (either the single one
defined by ``process``, or the multiple ones defined by ``processes``)
will log its standard output and error to a file in ``/var/log/supervisor``.
You can check those files with a simple ``dotcloud logs`` (just like
any regular dotCloud service-, or by logging into your service instance
and doing your analysis there.


Best Practices Summary
----------------------

If you are about to author a custom service, and would like to do it
in such a way that it is easy for others to use it in their apps,
but also to customize it, we recommend to use the following "best practices".
None of them is mandatory; consider them as "tips&tricks" that we
gathered ourselves at dotCloud when authoring our first custom
services ourselves.

**Put everything related to your custom service in a single directory**
(except ``dotcloud.yml``, of course). That way, if someone wants to
integrate your service in his stack, he will just have one directory +
one ``dotcloud.yml`` snippet to copy over. No cherry-picking of multiple
files accross different directories!

**Your buildscript should be called ``builder``.** And it should be
in the single directory mentioned above. Again, you can name it anything
you like, but using a single coherent name makes it easier for everyone.

**The builder script should not assume the name of its enclosing directory.**
Remember that the builder will be started from the root of the repository.
To make sure that you end up in the same directory as the builder,
rather do ``cd $(dirname "$0")`` than ``cd servicename``. That way,
an user can rename your directory without much adverse affect.

Likewise, if you need to come back to the root directory later, you can
save the content of the ``$PWD`` environment variable before going
into your service directory.

**When starting a daemon from a run script, remember to "exec".**
At the end of your script. if you just do ``somethingd --foreground``,
it will not restart properly at the next push. Why? Because when signals are
sent to terminate and restart a process, they will merely hit your
shell script, not the daemon itself. However, ``exec somethingd --foreground``
will do the trick, since your process will replace the shell script.

.. note::

   Some programs (like ``safe_mysqld``) are themselves script
   shells around another daemon; in those cases, you have at least
   two solutions:

   * extract the actual background process start command from the
     script (or using ``ps fauxwwww`` once it's running), and use
     that in your run script;
   * use ``pidproxy``, which is designed exactly for that purpose,
     and will "relay" any signal it receives to the process whose
     PID is stored in a given file. More information about ``pidproxy``
     can be found in the `Supervisor documentation
     <http://supervisord.org/subprocess.html#pidproxy-program>`_.

**Configuration files should be generated at run-time, not build-time.**
Informations like port numbers (``$PORT_...``) won't be available
while the Build Script is running. Therefore, configuration files
(if they must include the port number, that is) should be generated
just before starting the servers.

**Feel free to use ``bash``, ``zsh``, ``python``... instead of ``sh``.**
For the Build Script, but also for your other scripts, you don't have
to restrict yourself to "pure" ``#!/bin/sh``. There are a lot of
very useful built-ins and variable substitutions patterns in ``zsh``
and ``bash``, that do not exist in plain Posix shell. Also, if your
builder (or run script) needs to do more complex stuff, you are
totally free to use your language of choice. Keep in mind that the
builder will have the same binaries and libraries than the service
runtime, so you can use the full toolset!

**Be verbose**, both in the builder scripts and in the run scripts.
The output of the builder will be shown when people do a ``dotcloud push``
of your custom service. The output of the run script will appear
in the logs of the custom service.

**Implement support for ``approot``,** when relevant, of course.
If you are implementing support for a database, there is probably
no reason to use an approot (unless, maybe, you are cramming in
a clever script that can load a dump if it has the right name).

**When possible, fetch settings from the service parameters or environment.**
Remember that all parameters defined in ``dotcloud.yml`` can be retrieved
as ``$SERVICE_...`` (see the approot example above), while using
``dotcloud env set`` allows to expose variables without having to
change ``dotcloud.yml`` or the application source code. You can check
the `ZNC bouncer service <https://github.com/dotcloud/znc-on-dotcloud>`_
to see how a service can use variables from ``dotcloud.yml``, but let them
be overridden by ``dotcloud env set`` if necessary.
