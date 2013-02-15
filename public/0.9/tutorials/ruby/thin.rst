:title: Thin Ruby Tutorial
:description: How to use Thin instead of the default nginx + Passenger stack offered by dotCloud.
:keywords: dotCloud, tutorial, documentation, ruby, thin runy

Thin
====

Thin is a Ruby web server that glues together the Mongrel parser,
the Event Machine I/O library, and the Rack interface. According to
`Thin webpage <http://code.macournoyer.com/thin/>`_, this makes it
"with all humility, the most secure, stable, fast and extensible Ruby
web server bundled in an easy to use gem for your own pleasure".

This tutorial will show how to use Thin instead of the default nginx +
Passenger stack offered by dotCloud.

All the code presented here is also available on GitHub, at
http://github.com/jpetazzo/thin; moreover, you can read the
instructions in two original ways:

* using GitHub's awesome `compare view
  <https://github.com/jpetazzo/thin/compare/begin...end>`_ --
  click on each individual commit to see detailed explanations for each step;
* or, if you prefer text mode (or offline inspection), fallback on
  ``git log --patch --reverse begin..end``.


dotCloud Build File
-------------------

The dotCloud Build File, ``dotcloud.yml``, describes our stack.

We declare a single "Ruby" service. We use "Ruby" because it features
two useful things for us:

* a Ruby interpreter (obviously!) and a mechanism to install gems;
* a public-facing HTTP server (Nginx).

The role and syntax of the dotCloud Build File is explained in further
detail in the documentation, at http://docs.dotcloud.com/guides/build-file/.

``dotcloud.yml``::

  www:
    type: ruby
  
  


Sample Rack Application
-----------------------

A very simple Rack application is placed into ``config.ru``.

For testing purposes, our app will just display the HTTP request environment.
You can actually use any other Rack or Rails app.

At this point, you can ``dotcloud push`` the application, and go
to its URL (the URL is shown at the end of the push): the app does
not run on Thin (yet!) but it runs on Passenger.

``config.ru``::

  app = Proc.new do |env|
      [ 200, {'Content-Type' => 'text/plain'},
        env.each.map {|k,v| "#{k}=#{v}\n"}.collect ]
  end
  run app
  


Gemfile & Dependencies
----------------------

To tell dotCloud to automatically install the gem for Thin, we create a
standard Gemfile. dotCloud will detect this, and automatically use ``bundler``
to install dependencies defined in the Gemfile.

See http://gembundler.com/gemfile.html for details about bundler and the
Gemfile format.

``Gemfile``::

  source :rubygems
  gem 'thin'
  


Route Requests to Thin
----------------------

By default, dotCloud sends HTTP requests to Nginx, on port 80.
Thin will be running on port 8080. We will connect the two together,
by adding a Nginx configuration snippet instructing Nginx to proxy
all the requests to Thin.

At this point, we can test our setup manually, by:

* pushing to dotCloud with ``dotcloud push``;
* logging into our service with ``dotcloud run www``;
* starting Thin manually with ``cd current ; thin start -p 8080``.

If we browse our service URL, we should see our simplistic Rack
app. The headers will display the Thin version, showing that we
are not using Passenger anymore.

``nginx.conf``::

  location ^~ / {
  	proxy_pass http://localhost:8080;
  }
  
  


Install God Process Manager
---------------------------

We want something to start, stop, and monitor the "Thin" process
better than what we could do with crude scripts. We will use the
"God" process manager. It is the Ruby equivalent of e.g. Supervisor
or Monit.

To tell dotCloud to install it automatically for us, we simply
add it to our Gemfile.

``Gemfile``::

  source :rubygems
  gem 'thin'
  gem 'god'
  

God Configuration File
----------------------

God requires a configuration file. The one included here is
a working example, but leaving out a lot of interesting features
(e.g., restarting the process automatically if its CPU or memory
usages exceed given limits).

The important part is the ``w.start`` line, which basically
tells God to make sure that Thin will be running, and sets
the appropriate command-line flag to make it listen on port
8080.

We also redirect Thin's output messages to ``/home/dotcloud/thin.log``,
so if anything goes wrong, we can inspect that file and see what's
happening.

You can see more God configuration options on its official
web page (http://god.rubyforge.org/).

``thin.god``::

  # Start with "god -c thin.god"
  
    God.watch do |w|
      w.name = "thin"
      w.dir = "/home/dotcloud/current"
      w.interval = 30.seconds # default      
      w.start = "thin start -p 8080"
      w.log = "/home/dotcloud/thin.log"
  
      w.start_if do |start|
        start.condition(:process_running) do |c|
          c.interval = 5.seconds
          c.running = false
        end
      end
  
  end
  

Postinstall Script
------------------

To start God (and, therefore, Thin) automatically, we will use a
``postinstall`` script.

We could use SSH and start God manually each time we push the service,
but that would not be very convenient. To make sure that our processes
are started automatically when the service is pushed or scaled, we will
write a short postinstall script. The postinstall script is executed
automatically after each push, and after each deployment of a new
scaled instance.

In this script, we just have to invoke ``God`` and give him the
configuration file name.

Remember to make sure that the ``postinstall`` script is executable,
with e.g. ``chmod +x postinstall``.

``postinstall``::

  #!/bin/sh
  god -c thin.god
  


Better Postinstall Script
-------------------------

To make sure that Thin gets restarted properly when we push new versions
of our code, we augment our postinstall script to restart God each time
the postinstall script is run (i.e. after each push).

The last ``god status`` line is for information purposes only: it
just adds a line in the build log, telling us if Thin runs correctly.

``postinstall``::

  #!/bin/sh
  god terminate || echo "(you should ignore the error on the previous line)"
  god -c thin.god
  god status
  



