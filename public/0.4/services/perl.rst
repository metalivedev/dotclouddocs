:title: Perl Service
:description: dotCloud's Perl service can host any Perl web application compatible with the PSGI standard.
:keywords: dotCloud documentation, dotCloud service, Perl, Mojolicious, Dancer, PSGI hook, basic use, dependencies, Nginx

Perl
====

The *perl* service can host any Perl web application compatible with
the PSGI standard.

That includes Perl web frameworks like Mojolicious or Dancer.

Other applications can use the awesome Plack library to implement PSGI.
There are also modules to add PSGI hooks to legacy CGI or FastCGI applications.

You can also check those external blog posts:

* `Dancer tutorial (by Marco Fontani) <http://blogs.perl.org/users/marco_fontani/2011/04/dancing-on-a-cloud-made-of-pearls.html>`_
* `Catalyst tutorial (by garu) <http://onionstand.blogspot.com/2011/04/catalyst-in-cloud.html>`_


.. include:: service-boilerplate.inc

.. code-block:: yaml

   www:
     type: perl
     approot: helloperl

This Build File instructs the platform to use the code in the subdirectory
"helloperl" for our service. Our "ramen-on-dotcloud" directory will have
a structure similar to the following one::

  ramen-on-dotcloud/
  |_ dotcloud.yml   (also known as the "Build File")
  |_ helloperl/     (also known as your "approot")
     |_ app.psgi    (mandatory)
     |_ Makefile.PL (optional; can be used to define CPAN dependencies)
     |_ static/     (optional; put your static assets here)
     |_ ...         (all other files: Perl code, etc.)

Let's generate a sample PSGI application. Run the following on your
local machine::

  $ cpanm Dancer
  [...cpanm downloads and builds Dancer for you...]
  $ cd ramen-on-dotcloud
  $ dancer -a helloperl
  [...dancer creates dancecloud directory with a nice template in it...]
  $ echo "require 'bin/app.pl';" > helloperl/app.psgi

We need to add support for PSGI in the application. Just edit Makefile.PL
and add Plack in the dependencies, as shown below:

.. code-block:: perl

   PREREQ_PM => {
       'Test::More' => 0,
       'YAML'       => 0,
       'Dancer'     => 1.3030,
       'Plack'      => 0.9974,
   },

.. include:: service-push.inc


Internals
---------

Our Perl service runs with Nginx as the HTTP frontend. All URLs starting with
/static will be served as static files from the (optional) "static" directory as
told above. Requests to anything else will be handed to your app through uWSGI
and its PSGI module. Supervisor is used to keep the uWSGI daemon up and running.


Dependencies
------------

The preferred way to handle dependencies is to write a Makefile.PL, as shown
above. When your app is deployed on DotCloud, our custom builder will go
through your Makefile.PL and install the required dependencies.

There is also another way to specify dependencies: in the Build File.
You can add a ``requirements`` section, listing names of packages,
and even URLs of packages tarballs -- the latter being convenient
if you want to install something that is not on CPAN yet.

Example:

.. code-block:: yaml

   www:
     type: perl
     requirements:
       - Some::Thing::On::CPAN
       - http://example.com/something/not/on/cpan/yet.tar.gz

All those dependencies will be handed to ``cpanm``; so "if it can install
with ``cpanm``, you can list it in ``requirements``"!

Alternatively, you can call ``cpanm`` from the ``postinstall`` script.
Feel free to use whatever method feels the more convenient for you for those
external packages that can't fit in ``Makefile.PL``.


Perl Versions
-------------

The Perl service supports three different branches of Perl (5.12.x, 5.14.x,
5.16.x), it will default to Perl 5.12.x unless you specify otherwise. The
current versions of each Perl branch are listed in the table below. Pick the
branch that works best for you.

+---------+---------+-----------+
| branch  | version | variable  |
+=========+=========+===========+
| 5.12.x* | 5.12.4  |  v5.12.x  |
+---------+---------+-----------+
| 5.14.x  | 5.14.2  |  v5.14.x  |
+---------+---------+-----------+
| 5.16.x  | 5.16.0  |  v5.16.x  |
+---------+---------+-----------+

\* Perl 5.12.x is the default

To configure your service to use a version of Perl other than 5.12.x, you will
set the ``perl_version`` config option in your ``dotcloud.yml``

For example, this is what you would have if you wanted Perl 5.14.x:

.. code-block:: yaml

   www:
     type: perl
     approot: helloperl
     config:
       perl_version: v5.14.x

.. include:: perl-cron.inc

Custom uWSGI Configuration
--------------------------

You can modify the following uWSGI options in the config section of
``"dotcloud.yml"``:

- ``uwsgi_memory_report``: if set to true this will log each request with
  informations about the memory usage (the default is false);
- ``uwsgi_buffer_size``: set (in bytes) the size of the buffer used to parse
  uwsgi packets. The default value is 4096 but you will need to increase it if
  you have requests with large headers;
- ``uwsgi_harakiri``: every request that will take longer than the seconds
  specified in the harakiri timeout will be dropped and the corresponding worker
  recycled;
- ``uwsgi_processes``: (default 4) Set the number of workers for preforking
  mode. This is the base for easy and safe concurrency in your app. More workers
  you add, more concurrent requests you can manage. Each worker correspond to a
  system process, so it consumes memory, choose carefully the right number. You
  can easily crash your system if you set a value too high value. It is
  recommended that you do not change this value unless you have vertically
  scaled your service so that it has more RAM.

Here is an example:

.. code-block:: yaml

   www:
     type: perl
     approot: helloperl
     config:
       perl_version: v5.14.x
       uwsgi_memory_report: true
       uwsgi_buffer_size: 8192
       uwsgi_harakiri: 75
       uwsgi_processes: 4

New settings are applied on the next push.

.. include:: nginx-customization.inc
