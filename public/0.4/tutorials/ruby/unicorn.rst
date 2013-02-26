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

All the code presented here is also available on GitHub, at:
https://github.com/johncosta/unicorn-on-dotcloud

dotCloud Build File
-------------------

The dotCloud Build File, ``dotcloud.yml``, describes our stack.

We declare a single "Ruby-Worker" service. We use a "Ruby-Worker" because it
features two useful things for us:

* a Ruby interpreter (obviously!)
* a mechanism to install gems

The role and syntax of the dotCloud Build File is explained in further
detail in the documentation, at http://docs.dotcloud.com/guides/build-file/.

.. literalinclude:: ./unicorn/dotcloud.yml
    :lines: 1-4


Gemfile & Dependencies
----------------------

To tell dotCloud to automatically install the gem for Unicorn, we create
a standard Gemfile. dotCloud will detect this, and automatically use
``bundler`` to install dependencies defined in the Gemfile.

See http://gembundler.com/gemfile.html for details about bundler and the
Gemfile format.

.. literalinclude:: ./unicorn/Gemfile

Rack config file
----------------

Unicorn can load a Rack config file. We'll use this start our Sinatra application.

.. literalinclude:: ./unicorn/config.ru


Ruby Code
---------

We now need an application that will respond to the http requests.  We create a
ruby script that defines one trivial endpoint `/`.

.. literalinclude:: ./unicorn/helloworld.rb
:language: ruby


Route Requests to Unicorn
-------------------------

By default, "Ruby-Workers" don't have an http route. We'll need to expand the
dotcloud.yml file to add a http endpoint and to connect that endpoint to the
running process.

.. literalinclude:: ./unicorn/dotcloud.yml

Now we can push our application with `dotcloud push` and browse our service URL.
  
