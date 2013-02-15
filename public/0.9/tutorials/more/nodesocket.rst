:title: nodeSocket Porting Guide
:description: How to port a Node.js application from nodeSocket to dotCloud
:keywords: dotCloud, tutorial, documentation, nodeSocket

nodeSocket Welcome
==================

Hello! And welcome to the dotCloud Platform. We hope you’ll like
running your Node.js code here. To help you get started, we’ve put
together a few notes on how some nodeSocket ideas map to dotCloud. 

If you’d rather get started with *doing* instead of reading, you can
skip ahead to the :ref:`handson` section.

.. contents::
   :local:
   :depth: 1

Translating Terms
-----------------

+-----------------+---------------+----------------------------------------+
| nodeSocket Term | dotCloud Term | Notes                                  |
+=================+===============+========================================+
| Machines        | Service       | You won't have root access on the      |
|                 | Containers /  | service container, but *you can still  |
|                 | Instances     | install packages*. You're not running  |
|                 |               | in a single VM. Instead, each service  |
|                 |               | runs in its own Linux Container,       |
|                 |               | `lxc <http://lxc.sourceforge.net/>`_,  |
|                 |               | each probably on a different host      |
|                 |               | machine. The dotCloud platform         |
|                 |               | automatically configures containers    |
|                 |               | to run your services. The containers   |
|                 |               | are running Ubuntu 10.04LTS            |
+-----------------+---------------+----------------------------------------+
| Gears           | approots      | Each approot directory holds all the   |
|                 |               | code for one service. It is a          |
|                 |               | subdirectory of your current           |
|                 |               | application directory. In each service |
|		  |		  | container, ``~/current/`` points to the|
|                 |               | approot you defined for that service in|
|                 |               | ``dotcloud.yml``. You can `scale`_ each|
|                 |               | service "horizontally" to run multiple |
|                 |               | copies.                                |
+-----------------+---------------+----------------------------------------+
| Lever           | dotCloud      | The dotCloud Builder is separate from  |
|                 | Builder +     | your containers. Builds happen on a    |
|                 | Supervisor    | different host machine. But, similar to|
|                 |               | Lever, the dotCloud Builder reads a    |
|                 |               | configuration file and sets up all the |
|                 |               | networking and containers. Within the  |
|                 |               | container, services like Node.js       |
|                 |               | applications may use Supervisor to     |
|                 |               | start.  `Supervisor`_ is not           |
|                 |               | proprietary. It will start a process   |
|                 |               | and restart it if it exits             |
|                 |               | unexpectedly.                          |
|                 |               |                                        |
|                 |               |                                        |
+-----------------+---------------+----------------------------------------+
| .torque         | dotcloud.yml  | There is only one `dotcloud.yml`_ file |
|                 |               | in the root of your application        |
|                 |               | directory tree, and it defines all of  |
|                 |               | your services.  Together this defines  |
|                 |               | your stack. dotCloud runs more than    |
|                 |               | Node.js! Your services could include a |
|                 |               | cache, multiple databases, and code in |
|                 |               | other languages. Each service runs in a|
|                 |               | separate service container, probably on|
|                 |               | a different host machine.  Never assume|
|                 |               | 'localhost' will connect you to a      |
|                 |               | service.                               |
|                 |               |                                        |
|                 |               |                                        |
|                 |               |                                        |
+-----------------+---------------+----------------------------------------+

.. _scale: http://docs.dotcloud.com/guides/scaling
.. _Supervisor: http://supervisord.org
.. _dotcloud.yml: http://docs.dotcloud.com/guides/build-file

Some Differences
----------------
We don't have a public REST API like nodeSocket. Instead we offer a
command line interface (CLI) called ``dotcloud`` written in
Python. You'll use the dotCloud CLI to create applications and push
your code up to the dotCloud platform for building and deployment.

We don't run ``git`` in your service containers, but our CLI knows how
to work with ``git`` on your local system, and we track the revisions
we push. We do not recommend using SFTP for deployments because of how
our platform builds and deploys. Like nodeSocket, we recognize and use
``package.json`` files to install dependent packages via ``npm``
during a push.

.. warning:: 
   On nodeSocket your Machine and Gears behaved like a
   virtual machine, persisting between pushes. On dotCloud, every time
   you push, you get new Service Containers!

That difference means that even though you can ssh into your
containers and make changes manually, these changes will disappear
upon your next push. Your push must include everything you need to
run, including any changes you’d like to make to directory structures,
cron jobs, etc. Anything you can do via the command line you should be
able to automate as part of a build hook, one of three scripts we run
as part of the build process.

FAQs
....

*What’s the benefit?* **New containers mean you can push new code with
zero downtime.** We bring up your new containers, install your code, get
it running, and then switch over traffic to the new containers,
finally destroying the old containers when the traffic has switched to
your newest code. **New containers also mean we can replicate your
service on demand to give you more running copies to handle more
traffic or do more work.**

*What about uploaded files?* **We recommend you store your persistent
data in a database or on S3**, accessed directly via a library and not
s3fs. If you just want a quick and dirty way to store files
persistently on your server (without losing them between pushes), then
you can store them in ``~/data``. If you create that directory, then
we will treat it in a special way: we will look for it and move it
from the old container to the new container during deployment.

.. warning::
   There are two significant flaws with using ``~/data`` to store
   files between pushes:

   1. Your pushes will take down your application. Pushes will not be
   seamless. Since the files must be moved from the old container to
   the new container, we must first halt the old container (and any
   internet traffic it was receiving) and move to the new container
   (which isn't running yet and cannot receive internet traffic).

   2. Each container has its own ``~/data``, so if you scale your
   front end to multiple containers, and someone uploads to container
   0, that file will not be available on container 1.

*Do database services get destroyed during a push?* **No**, we don’t
destroy these every push -- they’re special. Once they are created, we
don’t change these containers unless you destroy them yourself. We can
scale MySQL and MongoDB for you to provide redundancy and automatic
fail-over.

*Doesn’t starting with a new container mean my builds will take
forever?* It would if we built from scratch each time, but **we
preserve the results of your previous build to use as a starting
state**. The build happens on a separate build machine which has a lot
of memory (probably more than your service container). Once we’ve
processed your code and all its dependent packages, we take a snapshot
of the file system in that container and destroy the build
container. We then create as many new containers as you’ve requested
for that service and unpack the build snapshot into each one, starting
up each one. When you push again, we again create a new container on
the build machine, but this time we initialize the file system with
the snapshot we took at the end of the previous build, so all the
dependencies and build products are already there. Then we apply the
changes you’ve made in your newly uploaded code, build, snapshot, and
deploy again. If you ever want to build without your
previously-fetched dependencies and code, use ``dotcloud push
--clean``.

.. _handson:

Hands On
--------

.. note::
   **tl;dr (for the truly impatient):**

   - Web tutorial: http://deploy.dotcloud.com/

   - Sample app: https://github.com/dotcloud/stack-node-mongo

   - Reference docs: http://docs.dotcloud.com/services/nodejs/

Prepare Your Code
.................

You should make sure that all of your code is under one directory
tree, and if you are using ``git``, that everything is checked-in
locally. The dotCloud CLI knows how to work with ``git``, including
honoring ``.gitignore``, but it will not pull code from other
repositories. You’ll have to pull that code locally.

Let’s presume your nodesocket application looks like this:

::

    nodesocketapp/
    ├── Gear1
    │   ├── file1.js
    │   ├── file2.js
    │   ├── package.json
    │   ├── server.js
    │   └── .torque
    └── Gear2
        ├── file1.js
	├── file2.js
	├── package.json
    	├── server.js
    	└── .torque

Then your dotCloud application will look like this:

::

    dotcloudapp/
    ├── dotcloud.yml <--- NEW
    ├── Gear1
    │   ├── file1.js
    │   ├── file2.js
    │   ├── package.json
    │   ├── server.js
    │   └── supervisord.conf <-- NEW
    └── Gear2
        ├── file1.js
    	├── file2.js
    	├── package.json
    	├── server.js
    	└── supervisord.conf <-- NEW

Move Configuration From .torque to dotcloud.yml and supervisord.conf Files
,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,

Given this .torque file for Gear1:

::

   {
        "start": "server.js",
    	"route": "127.0.0.1:3535",
    	"env": {
            "port": 3535,
            "mongo-host": "pearl.mongohq.com",
            "mongo-port": 28936
    	},
    	"parameters": [
            "--development",
            "--verbose"
    	 ]
    }

Some of this configuration will go into the dotcloud.yml file at the
root of your application source tree, and some will go into a
supervisord.conf file in the Gear1 directory.

Example ``dotcloudapp/dotcloud.yml``:

::

    gear1:
      type: nodejs
        approot: Gear1
      config:
        node_version: v0.8.x
      environment:
        MONGO_HOST: pearl.mongohq.com
        MONGO_PORT: 28936
    gear2:
      type: nodejs
      approot: Gear2
      config:
        node_version: v0.8.x

Note that it is critical to use space, not tabs in the dotcloud.yml file.

Example ``dotcloudapp/Gear1/supervisord.conf``:

::

    [program:node]
    command = node server.js --development --verbose
    directory = /home/dotcloud/current

The dotCloud Builder will create a symbolic link between
``/home/dotcloud/current`` and the directory you specified as
*approot* in ``dotcloud.yml``, so “current” is always the approot of
the current application.

Update Your Code to Listen on Port 8080
,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,

If your Node code provides a service via HTTP, then it must listen to
port 8080.

If your Node code provides a service via TCP then you must request a
port in your dotcloud.yml file and one will be assigned. You cannot
specify the port number. You can read the assigned port number from a
file you will find generated on your service:
``/home/dotcloud/environment.json``

Create a dotCloud Account
,,,,,,,,,,,,,,,,,,,,,,,,,

https://www.dotcloud.com/register.html

Remember the username and password and password you create. You’re
going to need it with the CLI.  

Install the dotCloud CLI
,,,,,,,,,,,,,,,,,,,,,,,,

We strongly encourage you to use virtualenv when working with Python
tools like the dotCloud CLI, but it is not a requirement. Once you
have a Python environment working on your system:

::

  $ pip install dotcloud
  $ dotcloud setup
  $ dotcloud check


Create a New Application and Push
,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,

::

  # Change to the local directory to where we have our code
  $ cd dotcloudapp
  $ dotcloud create --git portedapp
  # Choose “Y” to connect this directory with the “portedapp” application.
  $ dotcloud push

And you’re done! If the push got disconnected from the push logs
(unfortunately that’s been happening a lot recently), you can
reconnect with dotcloud dlogs latest and get the status.  If there is
a problem, or you have any questions, please email us at
support@dotcloud.com using your dotcloud account email address, or let
us know the URL to your application.

We’re happy you’re here!

/dotCloud Developer Support
