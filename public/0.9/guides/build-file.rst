:title: Build File
:description: Use dotCloud's Build File to  define the structure of your application.
:keywords: dotCloud documentation, quick start guide, dotcloud.yml

Build File (dotcloud.yml)
=========================


Background
----------

The dotCloud Build File defines the structure of your code and the
architecture of the services within your application. This information
enables dotCloud to automatically create a custom stack designed for
your application.


Example dotcloud.yml
--------------------

The best way to explain the contents of a Build File is to walk through
an example. Let's look at a Build File with all the options. Don't worry,
your typical Build File will be *much* shorter than this example,
which contains almost all the bells and whistles that you could ever use.

.. code-block:: yaml

    servicename1:
      type: ruby

    # Comments start with #
    # This is a second service being defined in the same dotcloud.yml
    # You should have one dotcloud.yml file per application.

    # Required parameters for a service: service name and type
    servicename2:        # Any name up to 16 characters using a-z, 0-9 and _
      type: python       # Must be valid service type.

      # ---------------------------------------------------------------
      # Optional parameters: All the following parameters are optional.

      # Define the location of this service's code
      approot: directory/relative/to/dotcloud_yml/  # Defaults to '.'

      # Build Hooks. Paths are relative to approot.
      prebuild: executable_name    # Defaults to undefined.
      postbuild: executable_name   # Defaults to undefined.
      postinstall: executable_name # Defaults to './postinstall'.

      # Ubuntu packages installed via apt-get install.
      systempackages:
        - packagename
	- another-packagename

      # Configuration for your service. See docs for each dotCloud Service.
      config:
        service_specific_parameter1: valueA
        service_specific_parameter2: valueB

      # Custom ports. HTTP ports are proxied. 
      # Most services do not need custom ports.
      ports:
        portname1: http            # Name is arbitrary, type is (http|tcp|udp)
	portname2: tcp

      # Environment variables. Shared by all services.
      environment:
        EXAMPLEVAR1: EXAMPLE_VALUE
	EXAMPLEVAR2: EXAMPLE_VALUE_TOO

      # Supervisor.conf shortcuts
      # You can use one or the other of (process|processess), but not both.
      process: executable_name  # Defaults to './run'
      processes: # For when you have more than one process to run.
        process_name1: path/to/executable1
	process_name2: path/to/executable2

      # List of dependencies, best for PERL/PHP but also Python and Ruby
      requirements: # Defaults to empty list.
        - dependency_package_name_1
	- dependency_package_2

The Build File is YAML formatted, so it is easily read by humans and
computers alike. Most developers create the Build File using a text editor
because the format is so simple. 

In YAML, indentation is important. You should use **spaces, not tabs**!!!

In this example ``dotcloud.yml`` we define an application with two
services: a Ruby service named *servicename1* and a Python service
named *servicename2*.

In line 1, we create a service named *servicename1*. Any indented line
below this one are attributes of this service. In this case, line 2
defines the type attribute of the servicename1 service. The type
attribute describes the technology that the service requires. It is
very important, and every service must have a type attribute. Most
services will be this simple, just two lines.

But in *servicename2* we show all the configuration parameters you
might set for more control over your service. We'll discuss each of
the parameters below in more detail.

*servicename*: Naming Your Service
----------------------------------

The name for your service can be up to 16 characters long, from the
set _a-z0-9 (that is, all lowercase, digits and underscores allowed
but no spaces). In the example above, we chose "servicename", but that
could have been "www", "a_cool_name_1234", etc. You will use this name
a lot! This is how you tell the dotCloud CLI which service you want to
scale, destroy, get logs from, or shell into. You'll be able to see
the amount of RAM used by each copy of this service by name in the
`dashboard <https://dashboard.dotcloud.com>`_

When you add a new service you your ``dotcloud.yml`` file, the builder
will create a new service with that name. **However**, when you remove
a service from your ``dotcloud.yml`` and push again, the builder
**does not** destroy the missing service. It will continue to run and
you will continue to have access too it via SSH. You will also
continue to get billed!

**To destroy a service** you must use ``dotcloud destroy`` +
*servicename*. If you do not want the service to reappear in the next
push, then you must also remove it from your ``dotcloud.yml``.

Removing a service from your ``dotcloud.yml`` file can be a way to
prevent it from getting updates or otherwise changing the container in
the next push. But you will not be able to scale your services until
you add the missing service(s) back in to ``dotcloud.yml``.

type: Defining Your Service
---------------------------

While there is a lot of freedom in naming your service, the service
*type* must come from this list:

 =============  =============
 Code Services  Data Services
 =============  =============
 custom         mongodb
 java           mysql
 nodejs         postgis
 perl           postgresql
 perl-worker    redis
 php            smtp
 php-worker     solr
 python          
 python-worker   
 ruby            
 ruby-worker     
 static          
 =============  =============

The details of each service are found in the :doc:`../services/index`
docs, but in general there are two types: Code and Data.

Code services get recreated as part of each push. Data services
are considered "stateful" and, after the first push creates them,
further pushes do not alter their containers or running services.

Furthermore, "-worker" services are just like their non-worker
language services, except that "-worker" services have no HTTP front end.

.. _guides_service_approot:

approot: Specifying the Root Directory of a Service
---------------------------------------------------

If your stack uses multiple web services, you will probably want to put
the source of each web service in a different directory. You can use the
optional ``approot`` attribute to define a root directory for each service.

For instance, if your code is structured like this::

   myapp/
   ├── admin/
   │   ├── djangoproj/
   │   │   ├── settings.py
   │   │   └── …
   │   ├── wsgi.py
   │   └── …
   └── frontend/
       ├── index.php
       ├── logo.png
       ├── style.css
       └── …

You will put the following dotcloud.yml file in "myapp":

.. code-block:: yaml

    www:
      approot: frontend
      type: php
    backoffice:
      approot: admin
      type: python


In this case, the service "www" would be a typical PHP application in
the "frontend" directory; and the service "backoffice" would be a
Django application in the "admin" directory.

prebuild, postbuild, postinstall: Build Hooks
---------------------------------------------

These parameters let you specify scripts to run at various points in the build
process. The general steps in the build process are:

#. You ``dotcloud push`` to upload your code.
#. for each Code service defined in ``dotcloud.yml``, the dotCloud builder will: 

   #. Create a new build container.
   #. Fetch the results of the previous build and unpack them, unless ``--clean`` was specified as part of the push or deploy.
   #. Fetch or update any ``systempackages``
   #. Run the **prebuild** script, if defined.
   #. Run the dotCloud-defined build script for your *type* of service.
   #. Run the **postbuild** script, if defined.
   #. Take a snapshot of the build container.
   #. Destroy the build container for the service.
#. for each Code service, the dotCloud deployer will:

   #. Create as many new runtime continers as specified previously by ``dotcloud scale service=#`` (default is 1)
   #. Unpack the build snapshot to each runtime container.
   #. Update the ``environment.json`` & ``.yml`` files in the containers as well as the environment variables.
   #. Run the **postinstall** script, if defined.
   #. Update routing tables to send traffic to the new containers instead of the old containers.
   #. Destroy the old containers.

The build process runs in a container with a lot of memory, probably
more than your runtime container, so it is a good place to do
memory-intensive preprocessing or moving data. A build may run for as
long as 15 minutes. After that, we terminate the build and consider it
failed.

Note that if you have a very complicated build that takes more than 15
minutes, you could break it into pieces and let the build results
accumulate over serveral pushes. Once your dependencies have been
fetched or built (in most Code types), they will not need get built
again until you ``push --clean``.

systempackages: Install Additional System Packages
--------------------------------------------------

The ``systempackages`` parameter was originally only available in the
*custom* type service, but now it is available in all Code (not Data)
services. This allows you to install almost any additional software
quickly & easily -- as long as the said software is part of the
official Ubuntu 10.04 LTS repositories. All you have to do is to list
the packages you want to install in the *build file*, using the
following syntax:

.. code-block:: yaml

   www:
     type: custom
     systempackages:
       - openoffice.org
       - mysql-client-5.1

.. note::
   The packages and their dependencies will be installed, but no
   daemon of background process will be started automatically.
   For instance, if you list Apache in system packages, it will be
   installed, but it won't be started. You will have to execute it
   from e.g. a ``run`` script or ``process`` configuration directive.
   If you are looking for a specific package, check `Ubuntu's package
   directory <http://packages.ubuntu.com/>`_ (keeping in mind that
   you can only install packages from the 10.04 LTS repository,
   codenamed "lucid").

config: Service-specific Configuration
--------------------------------------

The ``config`` parameters vary depending on the service you're
running. They can allow you to specify a version (e.g. Python 2.6
versus 2.7) or set other values that determine either how the service
starts up or how to configure the container. For that reason,
``config`` values can only change when you have a new container. That
means for Code type services you can make changes and they will have
an effect in your next push, but for Data type services you must
destroy your container explicitly first to get the new config
parameters. **Destroying a Data type service will result in losing all
your data!** So you should back up first if your data is valuable.

For more information about specific configuration parameters, please
see the individual service documentation.

ports: Custom Ports
-------------------

Like ``systempackages``, this feature was first offered in *custom* type
services, but now all Code services can request custom ports. **Most
services do not need custom ports.**

By default, dotCloud services are allocated HTTP or TCP ports,
depending of their type. Most database services like MySQL, MongoDB,
PostgreSQL... will expose a TCP port allowing to contact them using
their native protocol. All web-oriented services will expose a HTTP port,
which can in turn be used with your :doc:`/guides/domains`.
Some services may expose *both* a TCP port (for their data
protocol) and a HTTP port (for administration). All services also expose
at least a SSH endpoint over a SSH port.

You can request additional UDP and TCP ports for your custom service,
as shown in the following ``dotcloud.yml`` file (other parameters
have been omitted for clarity):

.. code-block:: yaml

   service:
     type: custom
     ports:
       www: http
       logs: tcp
       control: tcp
       peek: udp

Each port entry will create several variables in the environment file:

For TCP/UDP ports:

- ``PORT_LOGS``: The port where your should bind your server to;
- ``DOTCLOUD_SERVICE_LOGS_HOST``: The host where your server is running;
- ``DOTCLOUD_SERVICE_LOGS_PORT``: The port where your server is
  reachable (used on the client side);
- ``DOTCLOUD_SERVICE_LOGS_URL``: both of the above.

``LOGS`` is the upper case name of the port entry.

For HTTP ports:

- ``PORT_WWW``: The port where you should bind your server to;
- ``DOTCLOUD_SERVICE_HTTP_HOST``: The host where your server is running;
- ``DOTCLOUD_SERVICE_HTTP_URL``: Like above but as an URL.

Likewise, ``WWW`` is the upper case name of the port entry.

If you vertically scale a service with "custom ports", then the
environment will contain additional variables suffixed with ``_#``, "#"
being the instance number of the service. Each additional variable
contain the port informations for the service instance it is attached
to. Finally, the unsuffixed variables are identical to the variables
suffixed with ``_0``.

.. note::

   Note how the port you listen to will not be the same as the port you
   will connect to. For instance, in the above example, ``$PORT_LOGS``
   might be 42801 (indicating that the program using it will have to
   ``bind()`` to local port 42801), but it will be accessible from the
   outside using a totally different port like 17455.

environment: Defining Environment Variable
------------------------------------------

The recommended way to set environment variables is to use the
``dotcloud env`` command. You can, however, also define them in
your Build File, using the optional ``environment`` section:

.. code-block:: yaml

   www:
     type: python
     environment:
       MODE: production
       API: http://www.externalapi.com/v1/

Check out the :doc:`environment guide <environment>` to know more
about ``dotcloud env``, as well as the special files ``environment.json``
and ``.yml``.

process(es): ``supervisor.conf`` Shortcuts
------------------------------------------

The ``process`` and ``processes`` parameters are not needed for most
services, but they can come in handy for custom, workers and for Node
JS type services. They provide a shortcut way to automatically
generate a ``supervisor.conf`` file. This file will configure
``supervisord`` which acts as a watchdog on your service processes.

.. note::
   The ``processes`` variable is not a list, it's a dictionary.
   The name you give to each process will be used as a base for
   log files, and will allow you to stop/start/restart them
   independently by name.

requirements: Listing Code Service Dependencies
-----------------------------------------------

The ``requirements`` parameter lets you list your Code service
dependencies. Not every Code service uses this parameter. In
particular, you can use it with PERL, PHP, Python and Ruby. The
dependencies will be installed according to the rules of each
service. For Python and Ruby we recommend using the
``requirements.txt`` and ``Gemfile`` dependency lists instead, keeping
more in-line with how those languages typically define dependencies.
