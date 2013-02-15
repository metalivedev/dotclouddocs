:title: Node.Js Service
:description: dotCloud's Node.js service can host any Node.js application. 
:keywords: dotCloud documentation, dotCloud service, Node.js, HTTP server, JavaScript, Node.js workers, signal handlers

Node.Js
=======

.. sidebar:: Node.js vs nodejs

   `Node.js <http://nodejs.org/>`_ is the awesome evented I/O platform
   to run JavaScript.

   nodejs is the name of the DotCloud service offering Node.js capabilities.


The nodejs service can host any Node.js application. The application can
be an HTTP server or just a background worker: the deployment method will
be the same. Background workers will be allocated a HTTP virtual host as
well, but it won't be used, and nothing bad will happen.

.. note:: For the impatient...

   * create a :ref:`three-lines supervisord.conf file <nodejs-supervisord>`;
   * if you need to install external dependencies, use a standard NPM
     :ref:`package.json <nodejs-packagejson>` file (but this step is optional);
   * if you your app should be reachable through HTTP, make sure your code
     is setup to listen on port 8080 (don't worry, it will be exposed on
     port 80 anyway, but you *need* to listen on port 8080 in your app).


.. include:: service-boilerplate.inc

.. code-block:: yaml

   www:
     type: nodejs
     approot: hellonode

This Build File tells the DotCloud platform that the code for your Node.js
app will be under "hellonode". Our code will have the following structure::

  ramen-on-dotcloud/
  |_ dotcloud.yml         (DotCloud Build File shown above)
  |_ hellonode/           (your application root or "approot")
     |_ supervisord.conf  (mandatory boilerplate file; see next paragraph)
     |_ package.json      (optional file to specify NPM dependencies)
     |_ server.js         (your app; can be named differently)
     |_ ...               (other files: code, static assets...)

.. note::
   Your main Node.js program does not have to be called server.js,
   but it is a pretty common "good practice".

.. _nodejs-supervisord:

Assuming that you want to run the code inside the file named "server.js"
and located at the root of your application, you need to create the following
"ramen-on-dotcloud/hellonode/supervisord.conf" file:

.. code-block:: ini

  [program:node]
  command = node server.js
  directory = /home/dotcloud/current

.. note::
   When you will push your code to DotCloud, it will be groomed by the
   builder process, and your approot will be mapped to /home/dotcloud/current. 
   That's why you need to specify this path.

You can configure many options in the *[program:x]* section (like environment
variables). See the `Supervisor reference
<http://supervisord.org/configuration.html#program-x-section-settings>`_
for more information.

.. note::
   We called the Supervisor configuration section "program:node", but 
   "program:anything" will do as well. Just in case you were wondering.

We don't need to specify any extra dependency for our small "Hello, World!"
program. That will be covered later, in `nodejs-packagejson`.

Let's write a simple Node.js app now. It will answer to every request with
"Hello World!" as well as the list of the HTTP headers found in the request.
Paste the following code into "ramen-on-dotcloud/hellonode/server.js":

.. code-block:: javascript

   require('http').createServer(function (request, response) {
     response.writeHead(200, {"Content-Type": "text/plain"});
     output = "Hello World!\n";
     for (k in request.headers) { 
       output += k + '=' + request.headers[k] + '\n'; 
     }
     response.end(output);
   }).listen(8080);

.. note::
   The "node" process will run on port 8080 for security reasons
   (port 80 requires root), but don't worry: you will be able to access it
   through http://something.dotcloud.com/ without specifying the port number.


.. include:: service-push.inc

Node.js Versions
----------------

The node.js service supports three different branches of Node.js (v0.4.x,
v0.6.x, v0.8.x), it will default to node.js ``v0.4.x`` unless you specify
otherwise. The current versions of each node.js branch are listed in the table
below. Pick the branch that works best for you.


+---------+---------+----------+
| branch  | version | variable |
+=========+=========+==========+
| v0.4.x* | v0.4.12 |  v0.4.x  |
+---------+---------+----------+
| v0.6.x  | v0.6.20 |  v0.6.x  |
+---------+---------+----------+
| v0.8.x  | v0.8.3  |  v0.8.x  |
+---------+---------+----------+


\* Node.js v0.4.x is the default

To configure your service to use a version of Node.js other than ``v0.4.x``, you
will set the ``node_version`` config option in your ``dotcloud.yml``.

For example, this is what you would have if you wanted to use Node.js version
``v0.6.x``:

.. code-block:: yaml

   www:
     type: nodejs
     approot: hellonode
     config:
       node_version: v0.6.x


WebSockets
----------

A lot of people use Node.js for real-time web applications, and want to leverage
on WebSockets for that purpose. WebSockets is fully supported on dotCloud.
See our :doc:`WebSockets guide </guides/websockets>` for more information.


Node.js Workers
---------------

.. include:: worker.inc


.. _nodejs-packagejson:

NPM Dependencies (with package.json)
------------------------------------

If your application already ships a package.json file, it will be automatically
used: just after your code is deployed, "npm install" will be run from
your code directory.

If you want to specify your dependencies that way, you can adapt the
following package.json file:

.. code-block:: javascript

   {
     "name": "helloworld",
     "version": "1.0.0",
     "dependencies": {
       "express": "2.0.0",
       "underscore": ""
     }
   }

See the `NPM documentation <https://npmsjs.org>`_ to see the full range of
options available in package.json.


.. _nodejs-port8080:

Listen on Port 8080
-------------------

Your main program (it should be "server.js", right?) must listen
on port 8080. If this confuses you: don't worry, it is usually very
easy to fix. Just look for the ".listen(XXXX);" part in your code and
replace XXXX with 8080.

.. note::
   Of course, you can ignore this part if your Node.js program does 
   not need to listen for HTTP requests. That might be the case if
   it will only do background processing, crawling, etc.


Troubleshooting
---------------

You can check that your daemon has been started properly with the following
command::

  $ dotcloud run ramen.www supervisorctl status
  node                           RUNNING    pid 975, uptime 0:03:20


Supervisor will capture the output (stdout and stderr) of your programs,
and write it into log files. You can check those logs with the *dotcloud logs*
command::

  $ dotcloud logs ramen.www

.. note::

   Anything printed to stderr is written right away in the log file.
   However, stdout will be buffered. That means that messages might not
   show up immediately. With Node you can use the "util" module to write on
   stderr: "require('util').debug".

Press Ctrl+C to stop watching the logs.


Signal Handlers
---------------

If your application needs to do some housekeeping when it exits
(which will happen each time you push a new version of the code,
since the old version will be stopped, and the new one will be started),
you can setup a signal handler.

Just include a snippet similar to the following one:

.. code-block:: javascript

   process.on('SIGTERM', function () {
       console.log('Got SIGTERM exiting...');
       // do some cleanup here
       process.exit(0);
   });


.. note::
   The "Got SIGTERM exiting..." message will never show up in the logs, because
   Supervisor stops capturing output as soon as you issue *supervisorctl stop*
   But don't worry, the callback is correctly executed.

*Thanks to Daniel Gasienica for his awesome feedback and ideas.*
