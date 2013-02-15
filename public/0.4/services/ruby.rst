:title: Ruby Service
:description: dotCloud'sRuby service can host any Ruby web application compatible with the Rack standard including Rails and Sinatra.
:keywords: dotCloud documentation, dotCloud service, Ruby

Ruby
====

The *ruby* service can host any Ruby web application compatible with the Rack
standard. 

That includes modern Ruby web frameworks such as Rails and Sinatra.

.. include:: service-boilerplate.inc

.. code-block:: yaml

   www:
     type: ruby
     approot: helloruby

This Build File instructs the platform to use the code in the directory
"helloruby" and do whatever is necessary to expose the Rails or Rack app
contained therein.

Let's create a very minimal Rack application for demonstration purposes::

  $ mkdir ramen-on-dotcloud/helloruby

We also need the following directories; otherwise, the web server won't
recognize your app as being valid::

  $ mkdir ramen-on-dotcloud/helloruby/public
  $ mkdir ramen-on-dotcloud/helloruby/tmp

Now, we just need to add a file called ``config.ru`` into the ``helloruby``
directory, containing the following code:


.. code-block:: ruby

   app = Proc.new do |env|
       [ 200, {'Content-Type' => 'text/plain'},
         env.each.map {|k,v| "#{k}=#{v}\n"}.collect ]
   end
   run app

This will dump the whole environment of the request in the simplest possible
format.

.. include:: service-push.inc


Internals
---------

DotCloud *ruby* uses Nginx as the HTTP frontend. The *passenger* module
is used to mount Rack-compliant applications.

To support switching between different versions of Ruby, as well as custom
gems installation, we use `RVM <http://rvm.beginrescueend.com/>`_ 
and `bundler <http://gembundler.com/>`_.


Adapting Your Application
-------------------------

The *ruby* service relies on standard tools and best practices to build and
deploy your app. That means that adapting your code to run on dotCloud
will require minimal effort (if any!); moreover, the few changes that
you might have to do will ensure that your app will run more easily
on other platforms. 


Dependencies
^^^^^^^^^^^^

If you don't already have done it, you should describe your dependencies
in a file called *Gemfile*, located at the top of your application code.
Those dependencies will be installed automatically when you push your code.


Rails Applications
^^^^^^^^^^^^^^^^^^

Those applications should work out of the box. If you want to learn more
about the way passenger handles Rails apps, you can read 
`the passenger RoR doc 
<http://www.modrails.com/documentation/Users%20guide%20Nginx.html#deploying_a_ror_app>`_ for details.


Other Rack Applications
^^^^^^^^^^^^^^^^^^^^^^^

Other frameworks will work as easily, as long as they implement the `Rack 
<http://rack.rubyforge.org/>`_ interface). You will need to create a
*config.ru* file (if one is not already supplied with your application).

If you need to create such a file from scratch, check the `passenger Rack doc 
<http://www.modrails.com/documentation/Users%20guide%20Nginx.html#deploying_a_rack_app>`_ for details, or get some inspiration from the example above.

.. note::
   The Rack specification requires a *config.ru* file, but also a couple
   of directories named *public* and *tmp* (even if they are empty).
   Don't forget to create them if you are trying to deploy, e.g., a simple
   Sinatra app!


.. include:: ruby-unsupported-gems.inc


Using Specific Settings on your DotCloud Deployment
---------------------------------------------------

To support switching from development to production without changing
a lot of things in your code (database servers, directories, etc.),
Rack applications use the RACK_ENV environment variable.

Rail applications will have multiple environments defined in the
"config/environments" directory: typically, "production.rb", "test.rb", etc.

The default value for RACK_ENV is "production"; if you want to set it
to something else (e.g. if you have multiple production environments,
and need to use different database settings on DotCloud), you can
set the RACK_ENV variable when you deploy your service with the following
Build File directive:

.. code-block:: yaml

   www:
     type: ruby
     approot: helloruby
     config:
       rack-env: noodles

.. include:: ruby-custom-version.inc

.. include:: ruby-cron.inc

.. include:: nginx-customization.inc

