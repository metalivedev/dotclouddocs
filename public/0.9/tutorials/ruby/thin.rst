:title: Thin Ruby Tutorial
:description: How to use Thin instead of the default nginx + Passenger stack offered by dotCloud.
:keywords: dotCloud, tutorial, documentation, ruby, thin

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
http://github.com/jpetazzo/thin-on-dotcloud.


dotCloud Build File
-------------------

The dotCloud Build File, ``dotcloud.yml``, describes our stack.

We declare a single "Ruby-Worker" service. We use a "Ruby-Worker" because it
features two useful things for us:

* a Ruby interpreter (obviously!)
* a mechanism to install gems

The role and syntax of the dotCloud Build File is explained in further
detail in the documentation, at http://docs.dotcloud.com/guides/build-file/.

.. literalinclude:: ./thin/dotcloud.yml
   :lines: 1-4

Gemfile & Dependencies
----------------------

To tell dotCloud to automatically install the gem for Thin, we create a
standard Gemfile. dotCloud will detect this, and automatically use ``bundler``
to install dependencies defined in the Gemfile.

See http://gembundler.com/gemfile.html for details about bundler and the
Gemfile format.


.. literalinclude:: ./thin/Gemfile


Ruby Code
---------

We now need an application that will respond to the http requests.  We create a
ruby script that defines three endpoints `/`, `/publish`, and `/subscribe`.

.. literalinclude:: ./thin/stream_demo.rb
    :language: ruby


Route Requests to Thin
----------------------

By default, "Ruby-Workers" don't have an http route. We'll need to expand the
dotcloud.yml file to add a http endpoint and to connect that endpoint to the
running process.

.. literalinclude:: ./thin/dotcloud.yml

Now we can push our application with `dotcloud push` and browse our service URL.
