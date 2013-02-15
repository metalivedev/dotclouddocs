:title: Handling Errors
:description: How to catch errors (404, 500, 502, 504, etc.) and display them nicely.
:keywords: dotCloud documentation, quick start guide, handling errors, inform user, application errors, intercept errors, tricks, 502 error, 504 error, user experience

Handling Errors
===============

When something goes wrong in your code (or worse: in the platform itself!),
you generally want to display a page that is both:

* informative for your user,
* coherent with the graphical design of your site.

If some database timeout occurred, you might want to tell the user to try
again later. If some unexpected exception happened, you might prefer to
tell the user to contact your support; or maybe to log it somewhere,
or send you an email with as much information as possible.

It's generally better if you can handle the error from your application
(using exceptions or a similar mechanism, depending of your language or
framework). However, in some case, it won't be possible -- or maybe
the code supposed to handle the error will fail.

Let's see how we can address that!


What Can Possibly Go Wrong?
---------------------------

The following things can happen to your application.

#. Timeout. 
   If your application takes more than 60 seconds to serve the
   request, our load balancers will think it died, and serve a nice
   ``504 Gateway Timeout`` error page. Your user will see a warning sign,
   informing them that the application took too long to answer.

#. Failure to reply properly. 
   If your app crashes, or fails to catch an exception,
   it will result into a ``502 Bad Gateway`` error code (unless it
   already sent an HTTP response code, in which case the user will
   generally see a partial page).

   * If that happens in a stack using Nginx (e.g. Python, Ruby, Perl, PHP),
     Nginx will detect the error and serve a default 502 error page.
     Our balancers will, in turn, detect this error code, and serve a nice
     error page, similar to the 504 error page.
   * If that happens in a stack serving requests directly, without Nginx
     (e.g. Node.js), the load balancer will also detect the failure
     (but directly, without the intervention of the "local" Nginx)
     and serve the 502 page in a similar way.

#. Unscheduled application downtime. 
   If the processes running your app are stopped
   (or, more proabably, if they failed to start), it will also translate
   to a ``502 Bad Gateway`` error.

.. note::

   Ruby applications run by Passenger will behave differently; if
   there is an uncaught exception, Passenger will generate a 
   ``500 Internal Server Error`` instead (showing a traceback).


How Can I Intercept Errors?
---------------------------

If you want to display your own page instead of the default error page
provided by DotCloud, you have to use a few tricks.

First, note that this will only work for stacks that embed a Nginx
server. For other stacks, DotCloud load balancers will be the only
layer between your user and your app, and for now, it can only
serve a default error page.

You need to tell Nginx to do all those things:

* use a custom static page for 502 and 504 errors;
* remap the error code to e.g. 500 (else, the DotCloud load balancers
  will serve the default 502 and 504 pages);
* intercept the errors sent by the uwsgi/fastcgi (else, our custom static
  page won't be used);
* reduce the default timeout, so *your* timeout handler will kick in
  before the platform-wide timeout handler.

Assuming your error pages are in ``/static/502.html`` and ``/static/504.html``,
you can use the following ``nginx.conf`` snippets:

.. tabswitcher::

   .. tab:: PHP

      .. code-block:: nginx

	 fastcgi_read_timeout 10;
	 fastcgi_intercept_errors on;
	 error_page 502 =500 /static/502.html;
	 error_page 504 =500 /static/504.html;

   .. tab:: Perl

      .. code-block:: nginx

         uwsgi_read_timeout 10;
	 uwsgi_intercept_errors on;
	 error_page 502 =500 /static/502.html;
	 error_page 504 =500 /static/504.html;

   .. tab:: Python

      .. code-block:: nginx

      	 # Define this to point to your static files
         location /static/ {
	     alias /home/dotcloud/current/static/;
	 }
	 uwsgi_read_timeout 10;
	 uwsgi_intercept_errors on;
	 error_page 502 =500 /static/502.html;
	 error_page 504 =500 /static/504.html;

   .. tab:: Ruby

      For Ruby applications, since Passenger will use error code 500,
      no rewriting is needed.
      The default Nginx configuration already provides a handler for that
      (``errorpage 500 /static/500.html``). Also, since Passenger does not
      expose a configuration variable to change the timeout, you cannot
      provide a custom 504 page.

.. note::

   Once you enable ``intercept_errors`` in Nginx, you can no longer
   generate your own error pages for e.g. HTTP codes 500, 403, etc.
   You have to define static pages for those errors in Nginx as well.
   This limitation will be lifted in a future version of the services.
