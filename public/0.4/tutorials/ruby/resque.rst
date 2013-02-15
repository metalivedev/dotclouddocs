Resque
======

`Resque <https://github.com/defunkt/resque>`_ is a Ruby library for creating
background jobs, placing those jobs on multiple queues, and processing them
later. Resque uses `Redis <http://redis.io/>`_ to store its jobs queues.

This example uses the :doc:`/services/ruby-worker`
to run Resque and the :doc:`Redis service </services/redis>` as a
backend for Resque.

Our sample application will allow you to (metaphorically) order 
bowls of ramen on a web form, and have them delivered into your mailbox 
after a specified amount of time. 

Through this simple example, want to demonstrate how you can trigger
the execution of a chunk of code on a server (a "worker") from another
server (the web frontend in that case; but nothing prevents you from
having workers talking to other workers).


Deploy the instances
--------------------

We need a redis instance, a ruby (web) instance, and a ruby worker.
Getting them is as simple as::

  $ dotcloud deploy -t redis ramen.redis
  Created "ramen.redis".
  $ dotcloud deploy -t ruby ramen.resqueweb
  Created "ramen.resqueweb".
  $ dotcloud deploy -t ruby-worker ramen.resqueworker
  Created "ramen.resqueworker".


Project structure
-----------------

.. note::
   We will use *the same code* on the web server and on the worker.
   This will behave correctly because the web server is driven by the
   file "config.ru" (which is ignored by the worker service), and
   the worker service is driven by "supervisord.conf" (which is ignored
   by the web server). How convenient!

Let's create a ramen-resque directory::

  $ mkdir ramen-resque

We also need to create the following directories (we won't put anything
in them; but if we don't create them, the web server is going to be very sad)::

  $ mkdir ramen-resque/public
  $ mkdir ramen-resque/tmp


Ruby code dependencies
----------------------

Our sample app uses two gems: resque (obviously!) and pony (to send 
e-mails super easily). We will list them in "ramen-resque/Gemfile":

.. code-block:: ruby

   source :rubygems
   gem "resque"
   gem "pony"


"Business logic" code
---------------------

We will now write the class modeling a "job" (i.e., a task to be
done by a worker, as requested by the web frontend). All you need
is a class with a "perform" method (or, technically, anything answering
to the :perform message). 
So let's put the following code in "ramen-resque/job.rb":

.. code-block:: ruby

   require "resque"
   require "pony"

   module Kitchen
     module Bowl
       @queue = :kitchen
       def self.perform(params)
         puts params
         puts "Cooking some ramen for #{params['time']} seconds"
         sleep Integer params["time"]
         puts "Sending ramen to #{params['to']}"
         Pony.mail \
           :to => params["mailto"], 
           :subject => params["subject"], 
           :body => params["body"]
         puts "Done!"
       end
     end
   end

   Resque.redis = Redis.new({:host => "redis.ramen.dotcloud.com",
                             :port => "12345",
                             :password => "&W.piPAi>7",
                             :thread_safe => true})

.. note::
   Of course, you will need to put the real parameters of your Redis service
   here. To see the actual values of those parameters, run 
   "dotcloud info ramen.redis".


Web application
---------------

The next big chunk of code is the web app (well, let's be honest: it's
big just because we did put a lousy HTML form in it!); so don your biggest
clipboard, and copy-paste the following to "ramen-resque/web.py":

.. code-block:: ruby

   require "sinatra/base"
   require "resque"

   require_relative "job"

   module Kitchen
     class App < Sinatra::Base
       get '/' do
         info = Resque.info
         out = "<html><head><title>dotCloud: resque tutorial</title></head><body>"
         out << "<p>"
         out << "There are #{info[:pending]} pending and "
         out << "#{info[:processed]} processed jobs across #{info[:queues]} queues."
         out << "</p>"
         out << '<form method="POST">'
         out << '<fieldset><legend>Recipient</legend>'
         out << '<input type="text" name="mailto" value="ramen@yopmail.com"/>'
         out << '</fieldset>'
         out << '<fieldset><legend>Cook time (seconds)</legend>'
         out << '<input type="text" name="time" value="10"/>'
         out << '</fieldset>'
         out << '<fieldset><legend>Subject</legend>'
         out << '<input type="text" name="subject" value="Lunch is ready!"/>'
         out << '</fieldset>'
         out << '<fieldset><legend>Message</legend>'
         out << '<textarea name="message">Hello, sir. Your ramen are hot and ready to eat!</textarea>'
         out << '</fieldset>'
         out << '<input type="submit" value="Cook a bowl of ramen (submit a new job in the queue)"/>'
         out << '&nbsp;&nbsp;<a href="/resque/">View Resque</a>'
         out << '</form>'
         out << "</body></html>"
         out
       end

       post '/' do
         Resque.enqueue Kitchen::Bowl params
         redirect "/"
       end
    
     end
   end

This is a simple Sinatra app. It displays a web form, and handles POST
requests. See how easy it is to put a job in the queue: you call Resque.enqueue
and specify the class of the message, and its value. Under the hood,
the worker will create an instance of the given class and send it a 
:perform message with the given parameters.

We will now plug this web app as a Rack application. Moreover: we will
expose Resque's administration page (a Rack app itself), using a
Rack::URLMap to mix our Sinatra app and Resque's admin page. Copy the following
code into "ramen-resque/config.ru":

.. code-block:: ruby

   require "resque/server"
   require "./web"

   use Rack::ShowExceptions

   class URLMap
     def initialize app
       @app = app
     end
     def call env
       env["SERVER_NAME"] = env["HTTP_HOST"]
       @app.call env
     end
   end

   run URLMap.new \
     Rack::URLMap.new \
       "/" => Kitchen::App.new,
       "/resque" => Resque::Server.new

.. note::
   More than half of this file (the custom URLMap class) is actually
   a workaround fixing a little bug in Rack::URLMap. If you don't use
   URLMap, you won't need it at all, of course. Also, we will soon
   deploy a global workaround, so URLMap users won't have to patch anything.

The code of the web stack is now complete; let's push it::

  $ dotcloud push ramen.resqueweb ramen-resque

You can now visit:

* http://resqueweb.ramen.dotcloud.com/ to order a few bowls of ramen:
  each time you submit the form, a new task is created in the queue;
  since there is no worker (yet!) the tasks will just pile up here;
* http://resqueweb.ramen.dotcloud.com/resque/ to see the state of the queue
  (you should see the requests you did in the previous step), the number
  of workers (none for now), etc.


Worker
------

The last part is the worker, which will dequeue the job requests,
sleep for a while and send a confirmation e-mail. 

The worker will be started using "rake"; therefore, we must create
"ramen-resque/Rakefile":

.. code-block:: ruby

   require 'resque/tasks'
   require './job'

We need to make sure that the rake process is started automatically
by the ruby-worker service. We will rely on Supervisor for that,
and create the following file, "ramen-resque/supervisord.conf"::

  [program:kitchen]
  command = rake resque:work
  environment = QUEUE=kitchen
  directory = /home/dotcloud/current

We can finally push this code to dotCloud::

  $ dotcloud psuh ramen.resqueworker ramen-reque

The code will be uploaded, the dependencies will be installed, and the
worker process will start. If you go back to 
http://resqueweb.ramen.dotcloud.com/resque/ you will notice that
there is now a worker connected to Resque, and the number of tasks in
the queue should decrease. If you left the default destination address
(ramen@yopmail.com), you can go to http://yopmail.com/ramen and see
the notification emails. They are probably tagged as "spam", but nevermind.

.. note::
   By default, a lot of e-mail providers will tag mails coming from EC2
   as SPAM. Therefore, if you use your own e-mail address (instead of
   ramen@yopmail.com), chances are that you won't receive the notifications.


Last words
----------

You can deploy+push multiple workers if you need to. Resque will take
care of the concurrency. You can also change the worker configuration
to start multiple processes in a single service.


.. FIXME add something about logs
