:title: PHP
:description: How to deploy PHP-Resque on dotCloud.
:keywords: dotCloud, tutorial, documentation, PHP library, PHP-Resque, 

PHP-Resque
==========

.. include:: ../../dotcloud2note.inc

As you write your application you will certainly need to execute some
asynchronous tasks. It could be anything that requires some form of (lengthy)
processing: image resizing, archiving, document analysis...

These jobs could be run from the same machine were your application server is,
but best practices advise you to do this on a different machine because:

1. You avoid impacts from the background jobs to your application;
2. Decoupling parts of the application eases maintenance and scaling.

dotCloud makes it very easy to architecture your application on *different*
"Services" each one with a different role and on a different machine.
Background jobs match perfectly this feature: we will be using a Redis service
to queue background jobs launched on one PHP service and executed on a second
PHP service:

..  +-----+         *-------*         +-----+
..  |     |         |       |         |     |
..  | PHP >=========> Redis >=========> PHP |
..  |     |         |       |         |     |
..  +-----+         *-------*         +-----+
..  Enqueue                           Perform

.. literalinclude:: resque-diagram.utf8
   :language: none
   :encoding: utf-8

The first PHP instance creates and enqueues jobs on Redis, which are dequeued
and performed on a second PHP instances. This architecture is very scalable: you
can have as much as PHP instances you want to perform or enqueue jobs and Redis
could be replicated in a master/slave setup.

All of this is done with the help of a PHP library called `PHP-Resque
<https://github.com/chrisboulton/php-resque>`_, which implements the job queues
on Redis and offers an easy way to create and perform jobs. PHP-Resque is
actually the PHP port of the `Resque <https://github.com/defunkt/resque>`_
Ruby library used at `GitHub <https://github.com/>`_ for all their asynchronous
jobs since 2009.

We are going to see how to use PHP-Resque through a `simple web application
<http://php-resque.dotcloudapp.com/>`_. As a matter of fact the application
also uses `CodeIgniter <http://codeigniter.com/>`_ a very straightforward web
framework.

.. contents::
   :local:
   :depth: 1

Application Architecture
------------------------

You can clone the application from https://bitbucket.org/lopter/ci-resque/.
Here is what the application directory looks like::

   .
   ├── application/ # Hold our application code
   ├── dotcloud.yml # The description of our stack
   ├── index.php    # Entry point of the framework
   ├── nginx.conf   # Some Nginx rules to run the framework
   ├── php.ini      # Optional PHP configuration for development
   ├── postinstall* # Run at the end of the dotCloud build to setup PHP-Resque
   ├── static/      # Hold some CSS files
   └── system/      # The framework “private” code

The relevant PHP code for PHP-Resque are all located in the "application"
directory, we have:

- a class to load PHP-Resque and connect it to the Redis Service;
- a class to easily launch workers;
- a class to display the content of the PHP-Resque queues as a web page;
- a class to enqueue jobs from a web page.

We will see how to connect PHP-Resque to Redis and launch some workers first,
then how to create jobs. There is also some dotCloud specific files that we are
going to cover last.

Connecting PHP-Resque To Redis
------------------------------

PHP-Resque comes as a class called "Resque" but only with static methods to
connect to Redis and enqueue jobs. We can wrap it up in a class called "Workers"
to seamlessly add some functionalities:

- Redis authentication support;
- Redis credentials parsing from dotCloud's environment.json;
- A method to see the list of pending jobs on a queue;
- A method to run the workers loop where PHP-Resque picks up and perform jobs.

The Redis connection and authentication is conveniently done from the
constructor of the Workers class:

.. Using literalinclude to include the code to show is not really convenient
   because it preserves indentation making it hard to readably quote only a
   small part of the file.

.. Also if somebody knows how I could remove the <?php opening tag while still
   get the syntax highlighting, that would be cool.

First, the constructor parses the Redis credentials from
:doc:`~/environment.json </guides/environment>`:

.. code-block:: php

   <?php /* application/models/workers.php: */

       $this->_redis_host = self::_redis_credentials();

   ?>

This calls a "parsing" function which returns an associative array with the
Redis host, port and password. It is just a sequence of `file_get_contents
<http://www.php.net/manual/en/function.file-get-contents.php>`_, `json_decode
<http://www.php.net/manual/en/function.json-decode.php>`_ and `array_combine
<http://www.php.net/manual/en/function.array-combine.php>`_ calls with some
error handling:

.. This error handling is so perfect that it almost looks scary, should we
   remove it? At least just here? (and not in the repo).

.. code-block:: php

   <?php /* application/models/workers.php: */

       private static function _redis_credentials()
       {
           $json = file_get_contents('/home/dotcloud/environment.json');
           if ($json === false) {
               throw new WorkersException("Can't open environment.json");
           }

           $env = json_decode($json, true);
           if ($env === NULL) {
               throw new WorkersException("Can't decode the environment.json");
           }

           if (!array_key_exists('DOTCLOUD_SHM_REDIS_URL', $env)) {
               throw new WorkersException("Can't find redis in the environment.json, try to push again.");
           }

           return array_combine(
               array('scheme', 'user', 'pass', 'host', 'port'),
               preg_split('/[\/:@]+/', $env['DOTCLOUD_SHM_REDIS_URL'])
           );
       }

   ?>

Then, the constructor forwards those credentials to the Resque class and
authenticates against Redis:

.. code-block:: php

   <?php /* application/models/workers.php: */

       Resque::setBackend("{$this->_redis_host['host']}:{$this->_redis_host['port']}", $this->_db);
       Resque::redis()->auth($this->_redis_host['pass']);

   ?>

Here is the complete constructor, $db is an optional parameter to use a
particular Redis database number:

.. code-block:: php

   <?php /* application/models/workers.php: */

       public function __construct($db = 0)
       {
           parent::__construct();

           $this->_db = $db;
           $this->_redis_host = self::_redis_credentials();

           Resque::setBackend("{$this->_redis_host['host']}:{$this->_redis_host['port']}", $this->_db);
           Resque::redis()->auth($this->_redis_host['pass']);
       }

   ?>

Running The Workers
-------------------

PHP-Resque provides a PHP script to run workers that will perform the jobs
queued on Redis. Once again, we wrap that in our "Workers" class:

.. code-block:: php

  <?php /* application/models/workers.php: */

      public function run_workers()
      {
          require(APPPATH.'/third_party/resque/resque.php');
      }

  ?>

The workers are permanently running and polling the Redis queues. The workers
are launched from another class that can be called through the CodeIgniter
framework:

.. code-block:: php

   <?php /* application/controllers/jobs.php: */

       class Jobs extends CI_Controller {

           /**
            * Instanciate our Workers class.
            */
           public function __construct()
           {
               parent::__construct();

               $this->load->model('workers');
           }

           /**
            * Proxy method to run our workers.
            */
           public function run_workers()
           {
               $this->workers->run_workers();
           }
       }

   ?>

With this class in our hands we can run this command to launch our workers run
from the root of the application::

   $ QUEUES=* php index.php jobs run_workers

We will see how to do this automatically when the code is pushed to dotCloud, but
let's create some jobs to perform first.

Create Some Jobs
----------------

Jobs in the PHP-Resque sense are just classes that define a "perform" method:

.. code-block:: php

   <?php

       class My_Job
       {
           public function perform()
           {
               /*
                * My_Job will be instantiated by a worker and its perform method
                * called.
                */
           }
       }

   ?>

The job class can also provide two optional methods "setUp" and "tearDown" that
will be called before and after the "perform" method.

In our sample application jobs are created from the "Editor" class. It receives
a filled HTML form and then use the enqueue method on our workers object to
create a new job:

.. code-block:: php

   <?php /* application/controllers/editor.php: */

       $this->workers->enqueue(
               $this->input->post('queue_name'),
               'FakeJob',
               array('name' => $this->input->post('job_name'))
       );

   ?>

The enqueue method, in its turn, calls the Resque::enqueue static method that
takes three parameters:

#. The *queue name* that is extracted from the $_POST variable using one of the
   CodeIgniter function;
#. The *class name* that should be used to actually instantiate a new job
   object on the worker process, here the class is *"FakeJob"*;
#. An associative array of parameters that will be passed back to the job when
   it is performed, here our job will receive a "name" parameter.

dotCloud Specific Details
-------------------------

We have already seen how the "Workers" class uses the *environment.json* file to
automatically connect to Redis. It is not the only dotCloud specific detail,
there is also:

- A dotCloud Build File: *dotcloud.yml*;
- A Nginx configuration include file: *nginx.conf*;
- A Supervisor configuration include file: *supervisord.conf* that is installed from
  the postinstall hook.

The dotCloud Build File
~~~~~~~~~~~~~~~~~~~~~~~

The dotCloud Build File is straightforward and just describes the architecture
of our application on dotCloud, with its three services (a web server, a
service to launch workers and a Redis service):

.. code-block:: yaml

   ui:
       type: php

   workers:
       type: php-worker
       requirements:
           - proctitle

   shm:
       type: redis

Notice the "proctitle" requirement on the "workers" service. This is an optional
dependency of PHP-Resque to rewrite unix process names so you can quickly see
what the workers are doing when you run something equivalent to "ps aux". This
:ref:`requirement <services_php_pear_pecl>` correspond to a `PECL package
<http://pecl.php.net/package/proctitle>`_ that will be installed automatically
when you push the code on dotCloud.

The Nginx Configuration Include
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

We need to adjust how Nginx handle the HTTP requests and forward them to PHP,
you can think of this file as a global *.htaccess*:

.. code-block:: nginx

   location ~ /system/.* {
       deny all;
   }

   location ~ /application/.* {
       deny all;
   }

   try_files $uri $uri/ /index.php?$args;

These three statements:

- Deny access to the CodeIgniter framework and the application code;
- Correctly rewrite the HTTP requests to CodeIgniter with "try_files" (this is
  actually needed for any PHP MVC framework).

The Supervisor Configuration Include
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The Supervisor configuration include file is used to launch and monitor a pair of
PHP-Resque workers:

.. code-block:: ini

   [program:experiment]
   directory = /home/dotcloud/current/
   command = php index.php jobs run_workers
   environment = QUEUE=*
   stopsignal = QUIT
   stderr_logfile = /var/log/supervisor/%(program_name)s_error-%(process_num)s.log
   stdout_logfile = /var/log/supervisor/%(program_name)s-%(process_num)s.log
   numprocs = 2
   process_name = "%(program_name)s-%(process_num)s"

You may have recognized a piece of a .ini file, let's break it down:

.. code-block:: ini

   [program:experiment]

Defines a new background process configuration block. The process is called
"experiment" here.

.. code-block:: ini

   directory = /home/dotcloud/current/
   command = php index.php jobs run_workers
   environment = QUEUE=*

These three lines tell Supervisor how the background process should be
launched:

- In the directory *"/home/dotcloud/current/"* (this is where the :ref:`dotCloud
  builder <firststeps_builder>` will install your application);
- Using the command: "php index.php jobs run_workers";
- With the environment variable QUEUE set to \* (you can use multiple queues
  with PHP-Resque and only launch workers on a specific queue), with \* workers
  will process jobs on any queue.

.. code-block:: ini

   stopsignal = QUIT

This is a hint to tell Supervisor to use the SIGQUIT Unix signal when it tries
to gracefully stop the workers (instead of using the default SIGTERM
convention). Since the workers will be stopped and started each time you push
your code on dotCloud, but it is interesting to be able to gracefully interrupt the
current jobs.

.. This is currently flawed in PHP-Resque as Unix signals are not forwarded to
   the workers, but we are working on a patch that will be merged in PHP-Resque
   to address this issue. (Should we talk about that?)

.. code-block:: ini

   stderr_logfile = /var/log/supervisor/%(program_name)s_error-%(process_num)s.log
   stdout_logfile = /var/log/supervisor/%(program_name)s-%(process_num)s.log

Redirect the output of the workers into these two log files, "%(program_name)s"
will be replaced by "experiment" and "%(process_num)s" by the number of the
worker.

.. code-block:: ini

   numprocs = 2
   process_name = "%(program_name)s-%(process_num)s"

Finally, this tells Supervisor to launch two concurrent workers, so we will be
able to process two jobs at the same time.

Conclusion
----------

PHP-Resque is very easy to get working on dotCloud. Let's review what we did
here:

#. Connect PHP-Resque to Redis by using :doc:`environment.json
   </guides/environment>` and ``Resque::setBackend("host:port")``;
#. Authenticate to Redis by using: ``Resque::redis()->auth("password")``;
#. Launched some :doc:`workers </guides/daemons>` by using the PHP-Resque worker
   script *resque.php*;
#. Created a job class with a ``perform()`` method;
#. Enqueued some jobs with: ``Resque::enqueue(QueueName, ClassName, Args)``.

If you want to dig into the short PHP files used here, you can clone the
application and start to look into the *application/controllers* and
*application/models* directories where the interesting parts are.

The example application lives on: http://php-resque.dotcloudapp.com/.

You can clone the code from https://bitbucket.org/lopter/ci-resque/, "cd" into
it, create your application with the :doc:`flavor </guides/flavors>` of
your choice, and deploy it on dotCloud with::

  dotcloud create phpresque
  dotcloud push

While you are fiddling with the web interface you can see the jobs being
performed in the logs::

   dotcloud logs workers
