:title: Twilio Tutorial
:description: How to run a Sinatra or Twilio application on dotCloud.
:keywords: dotCloud, tutorial, documentation, twilio

Twilio
======

Today we're going to go through a demo application that will show you a very
simple example of running a Sinatra and Twilio application on dotCloud! Twilio
provides a REST API for adding telephony to your apps.  Using Twilio you can
easily add voice and SMS capabilities to your application! While this tutorial
uses Ruby, Twilio is simple to use in every language they support (PHP,
ColdFusion, Ruby, Python, C#, and more). You can read all about Twilio on the
`Twilio homepage <http://twilio.com>`_.

The sources of this tutorial are available on GitHub:
https://github.com/caleywoods/cloudsinger.

.. contents::
   :local:
   :depth: 1


dotCloud Build File
-------------------

The dotCloud Build File, ``dotcloud.yml`` describes our stack.

We declare a single Ruby service. We use Ruby because it features two
useful things for us:

* a Ruby interpreter and a mechanism to install gems;
* a public-facing HTTP server (Nginx).

The role and syntax of the :doc:`dotCloud Build File </guides/build-file>` is
explained in further detail in the documentation,

``dotcloud.yml``:

.. code-block:: yaml

   www:
     type: ruby


YAML Configuration File
-----------------------

Next we're adding a YAML file that contains our Twilio information that will be
used later in ``app.rb``. If you don't have an account already you can sign up
for one on the `Twilio homepage <http://twilio.com>`_.

.. note::

   If you don't have any "Twilio number" yet, you can use the one of e.g. your
   cellphone or landline. On your Twilio dashboard, go to "Numbers", then
   "Verified Numbers", and enter your phone number.  Twilio will call this
   number to make sure that it's *your* number.

   *However*, if you are using a free account, the "Twilio number" must be the
   one of the sandbox, i.e. 4155992671 as we write this document.

Change the settings in this file to match your Twilio number, SID, and token.
``app.rb`` is already setup to use the settings you place in ``config.yml``.

``config/config.yml``:

.. code-block:: yaml

   caller_id: "your_twilio_number"
   twilio_sid: "your_twilio_sid"
   twilio_tkn: "your_twilio_token"


Demo Sinatra Application and Rack Config
----------------------------------------

This is a simple Sinatra application and rack configuration file. The Sinatra
application is contained in ``app.rb`` and is fairly straightforward. This
application will display a simple form that asks for a phone number and gives a
textarea for the message you wish to send via SMS.

``config.ru`` is also telling rack to serve static files from ``public/css``
and ``public/images``. Sinatra, by default, looks in public (and we've told it
again by saying ``:root => 'public'``) so you won't need to use it when
referring to static file locations within your Sinatra application. In this
example, you would link the CSS by writing ``/css/style.css``; see how we
didn't use ``public/`` at the beginning of the path? You can find more
information on the ``config.ru`` file at `this site
<http://www.modrails.com/documentation/Users%20guide%20Nginx.html#deploying_a_rack_app>`_.

You should have an account by now and you should've configured your phone
number, SID, and Twilio token in ``config/config.yml``. As a bonus, twilio
gives you $30 when you sign up!!

Our application isn't quite ready yet, but almost. I've chosen to use Haml for
templates, so we'll add those views shortly.

``app.rb``:

.. code-block:: ruby

   require 'sinatra'
   require 'yaml'
   require 'twilio'

   config = YAML.load_file('./config/config.yml')
   CALLER_ID = config['caller_id']

   get '/' do
     haml :index
   end

   post '/sms' do
     Twilio.connect(config['twilio_sid'], config['twilio_tkn'])
     number  = params[:number]
     message = params[:msg]

     Twilio::Sms.message(CALLER_ID, number, message)

     redirect '/'
   end

``config.ru``:

.. code-block:: ruby

   require './app'
   use Rack::Static, :urls => ["/css", "/images"], :root => "public"
   run Sinatra::Application


Gemfile & Dependencies
----------------------

To tell dotCloud to automatically install the gems we need (Haml and Twilio),
we create a standard ``Gemfile``. dotCloud will detect this and automatically
use ``bundler`` to install dependencies defined in Gemfile.

This also gets ran when you scale your application on dotCloud. See
http://gembundler.com/gemfile.html for details about Bundler and the Gemfile
format.

``Gemfile``:

.. code-block:: ruby

   source :rubygems

   gem 'sinatra'
   gem 'haml'
   gem 'twilio'


Views & CSS/Images
------------------

Now that we've gotten the Gemfile taken care of, dotCloud can install
our dependencies we add in this last part of our application. We're adding
all the views used, as well as the images and CSS of the page.

This tutorial is using Haml as the markup language but you could
substitute your preferred language (erb, slim, etc). Let's talk just a
bit about some of the views.

First, ``layout.haml`` serves to generate the "layout" of our site or
"what goes where" if you will. Thanks to Haml we save quite a bit of
typing. Sinatra by default looks for a file named layout in the
``views/`` directory to serve as the applications layout, we don't need
to do anything special to get this to work.

The ``index.haml`` gives us the header text, image, and 'view source'
link and tells Sinatra to render our two remaining templates, text and
footer.

``text.haml`` is the form that contains the input fields for "Number"
and "Message" as well as the "Text Me!" button. Finally ``footer.haml``
is made up of just the Twitter logo. It might be overkill in this simple
example to give this its own template but if the application were to
grow it makes it a little easier to add things and keeps our templates small.

With these files added you can now ``dotcloud push`` and
then visit the URL of your deployed app. When you visit the URL you
should be able to supply a phone number and a message (140 characters or
less of course!) that you would like to send. Click on "Text Me!" and if
you've setup ``config.yml`` correctly and all systems are go, you should
get a text shortly at the number you provided. Please note that if
you're using a Twilio demo account you may need to edit ``app.rb`` line
33. In this file you will need to add your Twilio PIN as the first
argument to ``Twilio::Sms.message()``, this is also documented directly
in ``app.rb``.

This is just the beginning of what's possible with Twilio. You can make
and receive phone calls, record them, create conferences, and receive
SMS messages. In a non-trivial application you would login to the Twilio
dashboard and setup Twilio to HTTP POST to a URL of your app when
someone calls or SMS messages your application and you would then write
the code to handle those POST events.

I hope you've enjoyed this dotCloud + Twilio demo, feel free to drop us
a line if you have problems.
