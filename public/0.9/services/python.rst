:title: Python Service
:description: dotCloud's Python service can host any Python web application compatible with the WSGI standard.
:keywords: dotCloud documentation, dotCloud service, Python, WSGI

Python
======

.. include:: ../dotcloud2note.inc

The *Python* service can host any Python web application compatible with the
WSGI standard.

That includes all modern Python web frameworks: Django, Pyramid, web.py,
web2py, etc.

.. note:: For the impatient...

   Your WSGI callable should be named "application", be located in a 
   "wsgi.py" file, itself located at the top directory of the service.

   Your dependencies should be specified in a PIP-style "requirements.txt" file.

   But read on for a guided tour of the service!


.. include:: service-boilerplate.inc

.. code-block:: yaml

   www:
     type: python
     approot: hellopython

This Build File instructs the platform to use the code in the directory
"hellopython" for our service. Let's create it::

  mkdir ramen-on-dotcloud/hellopython

As said above, the *python* service expects you to provide it with a WSGI app.
So let's write a very minimal one. Create the file 
"ramen-on-dotcloud/hellopython/wsgi.py" with the following content:

.. code-block:: python

   def application(environ, start_response):
       status = '200 OK'
       response_headers = [('Content-type', 'text/plain')]
       start_response(status, response_headers)
       return ['Hello world!\n'] + ['{0}={1}\n'.format(k,environ[k])
       	      	      	      	    for k in sorted(environ)]

This will not only display a "Hello world!" message, but also dump the whole
WSGI environment for informative purposes.

.. include:: service-push.inc


Internals
---------

dotCloud *python* runs with Nginx as the HTTP frontend.  All URLs
referring to directories you defined as location aliases in your
``nginx.conf`` file will be served as static files from subdirectories
in your application. Everything else will be handed to your WSGI app
through uWSGI. The uWSGI workers are managed by Supervisor.

Your `code dependencies` are automatically handled with *pip* if your app ships
an optional "requirements.txt" file.

.. warning::

   The default Nginx configuration for python changed on August 22,
   2012. We removed the default location definition of /static/ and so
   now you *must* define the location of static content for your
   application in your own ``nginx.conf`` file.


Adapting your application
-------------------------

dotCloud relies on well-established tools and conventions to build your app. It
should be trivial to adapt any application to run on dotCloud.


Expose a WSGI application
^^^^^^^^^^^^^^^^^^^^^^^^^

If you wrote a WSGI application from scratch, all you have to do is to
make sure that the callable is named "application" and in the "wsgi.py" file.

If you are using a framework like Django, Pylons, Web2py, and lots of others,
you will want to use the WSGI handler provided by the framework.

Here are a few examples of wsgi.py files for some common frameworks:

.. tabswitcher::

   .. tab:: Django

      .. code-block:: python

         import os
	 os.environ['DJANGO_SETTINGS_MODULE'] = '.'
 	 import django.core.handlers.wsgi
	 application = django.core.handlers.wsgi.WSGIHandler()

   .. tab:: Tornado

      .. code-block:: python

      	 import tornado.wsgi
	 import wsgiref.handlers
	 # Assuming that you defined a MainHandler class somewhere...
	 from mycode import MainHandler
	 application = tornado.wsgi.WSGIApplication([(r"/",MainHandler)])

   .. tab:: web2py

      For web2py, you should just copy or symlink wsgihandler.py to wsgi.py.

   .. tab:: Pyramid

      .. code-block:: python

      	 import os.path
	 from pyramid.scripts.pserve import cherrypy_server_runner
	 from pyramid.paster import get_app
	 application = get_app(os.path.join(os.path.dirname(__file__), 'production.ini'))

      A more detailed WSGI file is available in the `Pyramid Cookbook
      <http://docs.pylonsproject.org/projects/pyramid_cookbook/en/latest/deployment/dotcloud.html>`_.



Static files
^^^^^^^^^^^^

Nginx will serve your static files (e.g. images, css, javascript...) directly,
bypassing the Python interpreter. To achieve this, you must move all your
static files together and you must provide an ``nginx.conf`` file to tell Nginx
the location(s) of the static files.

If you are using a framework like Django_ then you should also update
your application settings so that links to static assets will point to
the directory you have defined.

For example, if your static assets are in the "media" subdirectory of your
application, and your code expects them to be found on the "/media" URI,
you can create the following "nginx.conf" file in your approot:

.. code-block:: nginx

   location /media { root /home/dotcloud/current; }

.. note::

   The "root" should be /home/dotcloud/current, *not* 
   /home/dotcloud/current/media, because the location (/media) will be
   appended when looking for files.


Code dependencies
^^^^^^^^^^^^^^^^^

You can specify external requirements in a file called *requirements.txt*. A 
typical requirements file looks like this::

  Django==1.4.3
  Pil
  http://jsonschema.googlecode.com/files/jsonschema-0.2a.tar.gz

This is not specific to dotCloud at all: more and more Python projects
are using this style of files for their dependencies. If you want to install
those dependencies in your local environment, just run
"pip install -r requirements.txt". You might also want to check "pip freeze",
which can output a requirements.txt file for you, containing the exact
version numbers of all Python packages currently installed.
This is very convenient to make sure that you will get the very same set of
packages on different servers (e.g. dotCloud and your local development
environment).

For more information on pip and the format of requirements.txt,
see pip's documentation (http://pip.openplans.org/).


.. include:: nginx-customization.inc


Python Versions
---------------

The Python service supports four different branches of Python (2.6, 2.7, 3.1,
3.2), it will default to Python 2.6 unless you specify otherwise. The current
versions of each Python branch are listed in the table below. Pick the branch
that works best for you.

+--------+---------+----------+
| branch | version | variable |
+========+=========+==========+
| 2.6*   | 2.6.5   |  v2.6    |
+--------+---------+----------+
| 2.7    | 2.7.2   |  v2.7    |
+--------+---------+----------+
| 3.1    | 3.1.2   |  v3.1    |
+--------+---------+----------+
| 3.2    | 3.2.2   |  v3.2    |
+--------+---------+----------+

\* Python 2.6 is the default

To configure your service to use a version of Python other than 2.6, you will
set the ``python_version`` config option in your ``dotcloud.yml``.

For example, this is what you would have if you wanted Python 2.7.x:

.. code-block:: yaml

   www:
     type: python
     config:
       python_version: v2.7


Custom uWSGI Configuration
--------------------------

You can define additional uWSGI parameters in a *uwsgi.conf* file at the root
of your application. For example:

.. code-block:: nginx

   uwsgi_param   ENVIRONMENT  prod;

You can also modify the following uWSGI options in the config section of
``"dotcloud.yml"``:

- ``uwsgi_processes``: integer, the number of workers to spawn (the default is 4);
- ``uwsgi_memory_report``: if set to true, this will log each request with
  information about the memory usage (the default is false);
- ``uwsgi_buffer_size``: set (in bytes) the size of the buffer used to parse uwsgi
  packets. The default value is 4096 but you will need to increase it if you
  have requests with large headers;
- ``uwsgi_harakiri``: every request that takes longer than the seconds
  specified in the harakiri timeout will be dropped and the corresponding
  worker recycled;
- ``uwsgi_enable_threads``: (default false) Enable threads, this will allow you
  to spawn threads in your app;
- ``uwsgi_single_interpreter``: (default false) Python has the concept of
  "multiple interpreters". This allows it to isolate apps living in the same
  process. If you do not want this kind of feature use this option.

Here is an example:

.. code-block:: yaml

   www:
     type: python
     config:
       uwsgi_memory_report: true
       uwsgi_buffer_size: 8192
       uwsgi_harakiri: 75
       uwsgi_enable_threads: true
       uwsgi_single_interpreter: true

.. include:: /guides/config-change.inc

New Relic
---------

If you want to use `New Relic <http://www.newrelic.com>`_ to monitor your Python
service, then you need to complete the following steps.

Config
^^^^^^

To enable New Relic support in your Python application, set ``enable_newrelic``
to true in your ``dotcloud.yml`` config. Setting ``enable_newrelic`` to
``True`` will automatically configure uWSGI to work properly with New Relic.

.. note::

    The New Relic client doesn't work with Python 3 yet. If you try to use it
    with Python 3, your deploy will fail.


Environment Variables
^^^^^^^^^^^^^^^^^^^^^

The New Relic client requires some configuration information in order to work
correctly. You will do this by adding some environmental variables.

- ``NEW_RELIC_LICENSE_KEY``: (required) Your license key as found in 'Help
  Center' of your account when you log into New Relic;
- ``NEW_RELIC_APP_NAME``: The name of the application you wish to report data
  against in the New Relic UI. If not defined the default is "Python Application”;
- ``NEW_RELIC_LOG``: The path to a file to be used for the agent log. If not
  defined then no logging will occur. Can also be set to "stdout" or "stderr" to
  have logging go to standard output or standard error;
- ``NEW_RELIC_LOG_LEVEL``: The level at which logging will be output by the
  agent. If not defined then defaults to "info". Possible values, in increasing
  order of detail, are: "critical", "error", "warning", "info" and "debug";
- ``NEW_RELIC_CONFIG_FILE``: if they want a full config file, use this variable
  with the full path to the ``newrelic.ini`` file.

See :doc:`dotCloud environment variables guide </guides/environment/>` for more
information about environmental variables.

Here is an example ``dotcloud.yml`` for using New Relic with all of the environment
variables. It is important to note that you don't need all of these environment
variables. They are all listed here just to show you an example of them being
used:

.. code-block:: yaml

   www:
     type: python
     environment:
        NEW_RELIC_LICENSE_KEY: zbc1234sjnfnsdnFAKE0KEYk3232kjnknfksdnfkw23
        NEW_RELIC_APP_NAME: dotCloud Python App
        NEW_RELIC_LOG: /var/log/supervisor/newrelic.log
        NEW_RELIC_LOG_LEVEL: info
        NEW_RELIC_CONFIG_FILE: /home/dotcloud/current/newrelic.ini
     config:
        enable_newrelic: true

.. include:: /guides/config-change.inc

Other documentations
--------------------

You might be interested by the following dotCloud tutorials:

.. toctree::
   :maxdepth: 1

   /tutorials/python/django
   /tutorials/python/django-celery
   /tutorials/python/django-mongodb
   /tutorials/python/mobwrite

Or third-party documentation:

* `Deploying a Pylons app on dotCloud <http://rgarcia.github.com/2011/04/28/deploying-a-pylons-app-on-dotcloud.html>`_
* `Tornado in WSGI mode on dotCloud <http://pith.org/notes/2011/06/13/tornado-in-wsgi-mode-on-dotcloud/>`_
* `Pyramid on dotCloud <http://docs.pylonsproject.org/projects/pyramid_cookbook/en/latest/deployment/dotcloud.html>`_

.. _Django: http://docs.djangoproject.com/en/1.4/howto/static-files/
