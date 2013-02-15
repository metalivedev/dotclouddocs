:title: Static Service
:description: dotCloud's Static Service is a simple web server that can be used to host static content.
:keywords: dotCloud documentation, dotCloud service, static, web server

Static
======

.. include:: ../dotcloud2note.inc

The static service is a simple web server that can be used to host static
content (images, packages...) efficiently.

Like all our Nginx-based services, it can receive a custom Nginx configuration
snippet, which allows it to be more than a dummy HTTP server.

This makes it ideal to serve:

* static "maintenance" pages or placeholders for future services,
* URL routers, to dispatch requests to other services,
* and of course, static assets that you want to keep on a separate domain,
  for hardcore optimization reasons (like cookie separation).


.. include:: service-boilerplate.inc

.. code-block:: yaml

   www:
     type: static
     approot: hellostatic

Our static content will be in the "hellostatic" directory::

  mkdir ramen-on-dotcloud/hellostatic

And we should create a little "index.html" file here:

.. code-block:: html

  <html>
  <head><title>Hello World!</title></head>
  <body>This is a static service running on dotCloud.</body>
  </html>

.. include:: service-push.inc


Maintenance Page
----------------

If you have to take down a service for extended maintenance, you might want
to replace it with a maintenance page on a static service for the duration
of the maintenance.

To achieve that, put up the files you need for your maintenance page.
This could be a single "index.html" page; maybe with some additional
assets (images and CSS files); maybe even a whole set of static pages
explaining to your users what's going on.

You will also need create an additional Nginx configuration snippet.
Just create the file "ramen-on-dotcloud/hellostatic/nginx.conf" with
this content:

.. code-block:: nginx

  location ^~ / { 
    try_files $uri $uri/ /index.html ; 
  }

.. note::
   The ^~ is here to override the default "location /" block shipped with
   the stock Nginx configuration.

This will instruct Nginx to fall back on the default "index.html" file
if the user requests a non-existent page (which will certainly happen
if the user is following an external link or bookmark).

After adding the new files and the nginx.conf snippet, run "dotcloud push"
again. At the end of the push, go to your service URL, and try to add some
extra path at the end: instead of showing a "404 not found" page, you will
see the "index.html" page.
