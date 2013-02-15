:title: Unicorn Ruby Tutorial
:description: How to switch from Passenger to Unicorn on dotCloud.
:keywords: dotCloud, tutorial, documentation, ruby, unicorn, 

Unicorn
=======

.. figure:: unicorn.png

`Unicorn <http://unicorn.bogomips.org/>`_ is a preforking HTTP server
written in Ruby. It's designed to only serve fast clients on low-latency,
high-bandwidth connection and take advantage of features in Unix/Unix-like
kernels.

One of the big users of Unicorn is `Github <http://github.com>`_. They're
using a combination of Nginx and Unicorn to do code deployment with little
downtime. You can read about their setup `in this blog post
<https://github.com/blog/517-unicorn>`_.

This tutorial shows you how to switch from Passenger on dotCloud to
using Unicorn. This tutorial does not cover configuring Unicorn in such a
way that supports hot code deployment.


dotCloud Build File
-------------------

The dotCloud Build File, ``dotcloud.yml``, describes our stack.

We declare a single "Ruby" service. We use Ruby because it features two
useful things for us:

* a Ruby interpreter and a mechanism to install gems
* a public-facing HTTP server (Nginx)

The role and syntax of the dotCloud Build File is explained in further
detail in the documentation, at
http://docs.dotcloud.com/guides/build-file/

``dotcloud.yml``::

  www:
    type: ruby
  


Sample Rack Application
-----------------------

A very simple Rack application is placed into ``config.ru``.

For testing purposes, our app will just display the HTTP request
environment. You can actually use any other Rack or Rails app.

At this point, you can ``dotcloud push`` the application, and go to its
URL (the URL is shown at the end of the push).

The app doesn't run on Unicorn yet but it is running instead on
Passenger.

``config.ru``::

  app = Proc.new do |env|
    [ 200, {'Content-Type' => 'text/plain'},
      env.each.map {|k,v| "#{k}=#{v}\n"}.collect ]
  end
  run app
  


Gemfile & Dependencies
----------------------

To tell dotCloud to automatically install the gem for Unicorn, we create
a standard Gemfile. dotCloud will detect this, and automatically use
``bundler`` to install dependencies defined in the Gemfile.

See http://gembundler.com/gemfile.html for details about bundler and the
Gemfile format.

``Gemfile``::

  source "http://rubygems.org"
  
  gem 'unicorn'
  


Route Requests to Unicorn
-------------------------

By default, dotCloud sends HTTP requests to Nginx, on port 80. Unicorn
by default is going to run on port 8080. We will now connect the two
together by adding an Nginx configuration snippet instructing Nginx to
proxy all the requests to Unicorn.

We can't quite test out our app on unicorn yet, we still need to create
a unicorn config file.

Without the configuration file for unicorn browsing to the service URL
will still show that we're using Passenger.

``nginx.conf``::

  location ^~ / {
    proxy_pass http://localhost:8080;
  }
  


Install God Process Manager
---------------------------

God is a process monitoring framework. For in depth examples and
documentation please see http://god.rubyforge.org/

We want something that can start, stop, and monitor the Unicorn process
better than what we could do with scripts alone. For this, we're going
to use God which is the ruby equivalent of Supervisor or Monit.

To make dotCloud install it for us automatically each time you
``dotcloud push`` we simply add it to our Gemfile.

``Gemfile``::

  source "http://rubygems.org"
  
  gem 'unicorn'
  gem 'god'
  


God Configuration File
----------------------

God requires a configuration file. The one included here is a working
example, but leaving out a lot of interesting features (e.g. restarting
processes automatically if CPU or memory usage limits are exceeded).

The import part is the ``w.start`` line which tells god to make that
Unicorn will be running.

We also redirect Unicorn's output messages to
``/home/dotcloud/log/unicorn.log`` so in the event something goes wrong
we can inspect that file to see what's wrong. You can either ``dotcloud
run bash`` and examine the log yourself or ``dotcloud logs`` to get a tail
output of all the logs.

You can see more God configuration options at the official web page
(http://god.rubyforge.org/).

``unicorn.god``::

  # Start with "god -c unicorn.god"
  
    God.watch do |w|
      w.name = "unicorn"
      w.dir = "/home/dotcloud/current"
      w.interval = 30.seconds # default
      w.start = "unicorn"
      w.log = "/home/dotcloud/log/unicorn.log"
  
      w.start_if do |start|
        start.condition(:process_running) do |c|
          c.interval = 5.seconds
          c.running = false
        end
      end
  
  end
  


Postinstall Script
------------------

To start God (and Unicorn) automatically we will use a ``postinstall``
script.

We could use ``dotcloud run`` and start God manually, each time we push
the service, but that wouldn't be convenient. To make sure that our
processes are started automatically when the service is pushed or
scaled, we will write a small postinstall script. ``postinstall`` is
executed after each push, and after each deployment of a new scaled
instance.

In this script we just have to invoke ``God`` and give it the
configuration file we created earlier, ``unicorn.god``.

Remember to make sure that the ``postinstall`` script is executable, with
``chmod +x postinstall``.

``postinstall``::

  #!/bin/sh
  mkdir -p ~/log
  god -c unicorn.god
  


Better Postinstall Script
-------------------------

To make sure that Unicorn gets restarted properly when we push new versions
of our code, we augment our postinstall script to restart God each time
the postinstall script is run (i.e. after each push).

The last ``god status`` line is for information purposes only: it will
just add a line in the build log, telling us if Unicorn runs correctly.

``postinstall``::

  #!/bin/sh
  mkdir -p ~/log
  god terminate || echo "(ignore the error on the previous line; these aren't the droids you're looking for)"
  god -c unicorn.god
  god status
  
