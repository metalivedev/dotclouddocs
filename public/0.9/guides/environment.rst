:title: Environment File
:description: dotCloud automatically ties together the services that make up your application with a file called environment.json.
:keywords: dotCloud documentation, quick start guide, 

Environment File
================

.. include:: ../dotcloud2note.inc

What is the Environment File?
-----------------------------

When your application is built by the dotCloud platform, a file named
``environment.json`` is created in the home directory of each of your services.

This file contains a JSON-formatted dictionary with most of the configuration
information of the services in your stack. You can use it to retrieve
automatically the host, port, and password, of your databases, caches, and so
on.

Here is an example of an environment file:

.. code-block:: javascript

   {
      "DOTCLOUD_ENVIRONMENT": "default", 
      "DOTCLOUD_DB_MYSQL_LOGIN": "root", 
      "DOTCLOUD_DB_MYSQL_URL": "mysql://root:pass@7a96f954.dotcloud.com:7780", 
      "DOTCLOUD_DB_MYSQL_PASSWORD": "B61J14)]U4^L}.najnyE", 
      "DOTCLOUD_PROJECT": "demodcapp", 
      "DOTCLOUD_SERVICE_NAME": "www", 
      "DOTCLOUD_DB_MYSQL_PORT": "7780", 
      "DOTCLOUD_DB_MYSQL_HOST": "7a96f954.dotcloud.com", 
      "DOTCLOUD_SERVICE_ID": "0"
   }

The environment file will contain the access information of database services in
the same application. In the example, the MySQL database service named "db" is
present.

.. note::

   The environment variables defined in ``environment.json`` are not related to
   the "Unix" environment. In the future, dotCloud will mirror them in the
   "Unix" environment.

.. _guides-reading-environment:

Reading the Environment
-----------------------

To read an environment variable you need to load the environment file first.
Since it is a JSON file, this is very easy from any language:

.. tabswitcher::

   .. tab:: PHP

      .. code-block:: php

         <?php

              // Read the file and convert the underlying JSON dictionary into a PHP array
              $env = json_decode(file_get_contents("/home/dotcloud/environment.json"), true);

              echo "Application Name: {$env['DOTCLOUD_PROJECT']}\n";

         ?>

   .. tab:: Ruby

      Add the "json" requirement to your Gemfile:

      .. code-block:: ruby

         source :rubygems
	 gem 'json'

      Then use the following snippet to parse environment.json:

      .. code-block:: ruby

         env = JSON.parse(File.read('/home/dotcloud/environment.json'))

         puts "Application Name: #{env['DOTCLOUD_SERVICE_NAME']}"

   .. tab:: Perl

      Add the "JSON" requirement in your Makefile.PL, e.g:

      .. code-block:: perl

         use ExtUtils::MakeMaker;

         WriteMakefile(
                 NAME => "dummy",
                 PREREQ_PM => {
                    'JSON' => 0,
                },
         );

      .. note::

         Adding "JSON" in the "requirements" list in your :doc:`dotcloud.yml
         <build-file>` works too (but that's dotCloud specific).

      Then use the following snippet to parse environment.json:

      .. code-block:: perl

         use JSON;

         open my $fh, "<", "/home/dotcloud/environment.json" or die $!;
         my $data = JSON::decode_json(join '', <$fh>);

         print  "Application name: $data->{DOTCLOUD_SERVICE_NAME}\n";

   .. tab:: Python

      .. code-block:: python

         import json

         with open('/home/dotcloud/environment.json') as f:
           env = json.load(f)

         print 'Application Name: {0}'.format(env['DOTCLOUD_SERVICE_NAME'])

   .. tab:: NodeJS

      .. code-block:: javascript

         var fs = require('fs');

         var env = JSON.parse(fs.readFileSync('/home/dotcloud/environment.json', 'utf-8'));

         console.log('Application Name: ' + env['DOTCLOUD_SERVICE_NAME']);

Adding Environment Variables
----------------------------

You can set your own environment variables either from :doc:`dotcloud.yml
<build-file>` or directly from the command line with the ``dotcloud env``
command.

The first approach is quite simple, just list your variables in the environment
dictionary of the concerned service in ``dotcloud.yml``:

.. code-block:: yaml

   www:
     type: python
     environment:
       MODE: production
       API: http://www.externalapi.com/v1/

This approach is good for environment variables that never change across
deployments of your application and that can be stored in your code repository.
But —most of the time— the environment holds variables that always change across
deployments of your application and that can't be stored in your repository. A
perfect example is third parties service credentials. Also, you must push your
application again to change environment variables defined in ``dotcloud.yml``.

The ``dotcloud env`` command solves these issues by allowing you to set
environment variable from the command line, once the application is running on
dotCloud::

    dotcloud env set MYVAR=MYVALUE

Whereas ``dotcloud.yml`` allows you to define different environment variables
for each service, ``dotcloud env`` set environment variables for the whole
application. Moreover, environment variables set with ``dotcloud env`` supersede
environment variables defined in ``dotcloud.yml``.

You can set multiple variables at once::

    dotcloud env set \
        'AWS_ACCESS_KEY=IA4F0njNcmKKg3YndpOe' \
        'AWS_SECRET_KEY=Ideeghu0Ohghe7oi?Y6ogh7qui%jeiph7yai[coo'

You can list the variables you set with the ``list`` subcommand::

    dotcloud env list

.. note::

   Your environment variables keys must be alphanumeric characters only and
   cannot start with "``DOTCLOUD_``".


Removing Environment Variables
------------------------------

You can remove environment variables set in ``dotcloud.yml`` by editing the file
and pushing your code again.

You can remove environment variables set "at runtime" with ``dotcloud env`` with
the ``unset`` subcommand::

    dotcloud env unset AWS_ACCESS_KEY

YAML Format
-----------

This document shows how to use ``environment.json`` but the environment is also
available in the YAML format in the file ``environment.yaml``.

If you think that another environment format is relevant, let us know at
support@dotcloud.com.
