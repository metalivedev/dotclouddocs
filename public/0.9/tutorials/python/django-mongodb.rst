:title: Django and MongoDB
:description: How to build a minimal Django project from scratch on dotCloud, storing data in a MongoDB database.
:keywords: dotCloud, tutorial, documentation, python, Jango, MongoDB

Django and MongoDB
==================

.. include:: ../../dotcloud2note.inc

This tutorial will show how to build a minimal Django project from scratch
on dotCloud, storing data in a MongoDB database.

All the code presented here is also available on GitHub, at
http://github.com/jpetazzo/django-and-mongodb-on-dotcloud;
moreover, you can read the instructions in two original ways:

* using GitHub's awesome `compare view
  <https://github.com/jpetazzo/django-and-mongodb-on-dotcloud/compare/begin...end>`_ --
  click on each individual commit to see detailed explanations for each step;
* or, if you prefer text mode (or offline inspection), fallback on
  ``git log --patch --reverse begin..end``.


.. contents::
   :local:
   :depth: 1



dotCloud Build File
-------------------

The dotCloud Build File, ``dotcloud.yml``, describes our stack.

We will need a ``Python`` service to run our Django code.
This service allows us to expose a WSGI-compliant web application.
Django can do WSGI out of the box, so that's perfect for us.

Since our goal is to show how Django and MongoDB work together,
we will also add a ``MongoDB`` service.

`dotcloud.yml <https://github.com/jpetazzo/django-and-mongodb-on-dotcloud/blob/878c60d23102339104f6f495f1824af15b4b4cea/dotcloud.yml>`_:

   .. code-block:: yaml

      www:
        type: python
      db:
        type: mongodb


The role and syntax of the dotCloud Build File is explained in further
detail in the documentation, at http://docs.dotcloud.com/guides/build-file/.


Specifying Requirements
-----------------------

A lot of Python projects use a ``requirements.txt`` file to list
their dependencies. dotCloud detects this file, and if it exists,
``pip`` will be used to install the dependencies.

We need (at least) four things here:

* ``pymongo``, the MongoDB client for Python;
* ``django_mongodb_engine``, which contains the real interface between
  Django and MongoDB;
* ``django-nonrel``, a fork of Django which includes minor tweaks to
  allow operation on NoSQL databases;
* ``djangotoolbox``, which is not strictly mandatory for Django itself,
  but is required for the admin site to work.

To learn more about the specific differences between "regular" Django
and the NoSQL version, read `django-nonrel on All Buttons Pressed
<http://www.allbuttonspressed.com/projects/django-nonrel>`_.

`requirements.txt <https://github.com/jpetazzo/django-and-mongodb-on-dotcloud/blob/master/requirements.txt>`_::

   pymongo
   git+http://github.com/django-nonrel/mongodb-engine.git#egg=django_mongodb_engine
   hg+http://bitbucket.org/wkornewald/django-nonrel#egg=Django
   hg+http://bitbucket.org/wkornewald/djangotoolbox#egg=djangotoolbox


``pip`` is able to install code from PyPI (just like ``easy_install``);
but it can also install code from repositories like Git or Mercurial,
as long as they contain a ``setup.py`` file. This is very convenient
to install new versions of packages automatically without having to
publish them on PyPI at each release -- like in the present case.

See http://www.pip-installer.org/en/latest/requirement-format.html for
details about ``pip`` and the format of ``requirements.txt``.


Django Basic Files
------------------

Let's pretend that our Django project is called ``hellodjango``.
We will add the essential Django files to our project.
Actually, those files did not come out of nowhere: we just ran
``django-admin.py startproject hellodjango`` to generate them!

.. note::

   The rest of the tutorial assumes that your project is in the
   ``hellodjango`` directory. If you're following those instructions
   to run your existing Django project on dotCloud, just replace
   ``hellodjango`` with the real name of your project directory,
   of course.


The files:

* `hellodjango/__init__.py <https://github.com/jpetazzo/django-and-mongodb-on-dotcloud/blob/d13108e8d0729f42356df32dc4150f8aac877b55/hellodjango/__init__.py>`_
* `hellodjango/manage.py <https://github.com/jpetazzo/django-and-mongodb-on-dotcloud/blob/d13108e8d0729f42356df32dc4150f8aac877b55/hellodjango/manage.py>`_
* `hellodjango/settings.py <https://github.com/jpetazzo/django-and-mongodb-on-dotcloud/blob/d13108e8d0729f42356df32dc4150f8aac877b55/hellodjango/settings.py>`_
* `hellodjango/urls.py <https://github.com/jpetazzo/django-and-mongodb-on-dotcloud/blob/d13108e8d0729f42356df32dc4150f8aac877b55/hellodjango/urls.py>`_



wsgi.py
-------

The ``wsgi.py`` file will bridge between the ``Python`` service and our
Django app.

We need two things here:

* inject the ``DJANGO_SETTINGS_MODULE`` variable into the environment,
  pointing to our project settings module;
* setup the ``application`` callable, since that is what the dotCloud
  service will be looking for.

`wsgi.py <https://github.com/jpetazzo/django-and-mongodb-on-dotcloud/blob/094a586677205526d5d4832083f306e0cc969594/wsgi.py>`_:

  .. code-block:: python

     import os
     os.environ['DJANGO_SETTINGS_MODULE'] = 'hellodjango.settings'
     import django.core.handlers.wsgi
     application = django.core.handlers.wsgi.WSGIHandler()


We can now push our application, by running ``dotcloud create
djangomongo`` and ``dotcloud push`` (you can of course use
any application name you like). A Python service and a MongoDB service
will be created, the code will be deployed, and the URL of the service
will be shown at the end of the build. If you go to this URL, you will
see the plain and boring Django page, typical of the "just started"
project.


Add Database Credentials to settings.py
---------------------------------------

Now, we need to edit ``settings.py`` to specify how to connect to our
database. When you deploy your application, these parameters are stored
in the dotCloud Environment File.
This allows you to repeat the deployment of your application
(e.g. for staging purposes) without having to manually copy-paste
the parameters into your settings each time.

If you don't want to use the Environment File, you can retrieve the
same information with ``dotcloud info db``.

The Environment File is a JSON file holding a lot of information about
our stack. It contains (among other things) our database connection
parameters. We will load this file, and use those parameters in Django's
settings.

See http://docs.dotcloud.com/guides/environment/ for more details about
the Environment File.

`hellodjango/settings.py step 1 <https://github.com/jpetazzo/django-and-mongodb-on-dotcloud/blob/e57297673f96b7e411ccc2801cb89b1bfb0de7ce/hellodjango/settings.py>`_:

   .. code-block:: python

      # Django settings for hellodjango project.

      import json
      import os
      with open(os.path.expanduser('~/environment.json')) as f:
        env = json.load(f)

      DEBUG = True
      TEMPLATE_DEBUG = DEBUG

      ADMINS = (
          # ('Your Name', 'your_email@example.com'),
      )

      MANAGERS = ADMINS

      DATABASES = {
          'default': {
              'ENGINE': 'django_mongodb_engine',
              'NAME': 'admin',
              'HOST': env['DOTCLOUD_DB_MONGODB_URL'],
              'SUPPORTS_TRANSACTIONS': False,
          }
      }

      # Local time zone for this installation. Choices can be found here:
      # …


.. note::

   We decided to use the ``admin`` database here. This was made
   to simplify the configuration process. While you can actually use
   any database name you like (MongoDB will create it automatically),
   MongoDB admin accounts have to authenticate against the ``admin``
   database, as explained in `MongoDB Security and Authentication docs
   <http://www.mongodb.org/display/DOCS/Security+and+Authentication>`_.
   If you want to use another database, you will have to create a
   separate user manually, or add some extra commands to the
   ``postinstall`` script shown in next sections.

.. note::

   You might wonder why we put the MongoDB connection URL in the
   ``HOST`` parameter! Couln't we just put the hostname, and then
   also set ``USER``, ``PASSWORD``, and ``PORT``? Well, we could.
   However, when we will want to switch to replica sets, we will
   have to specify multiple host/port combinations. And one convenient
   way to do that is to use the `Standard Connection String Format
   <http://www.mongodb.org/display/DOCS/Connections>`_.

   Quite conveniently, ``django_mongodb_engine`` will just pass
   the database parameters to ``pymongo``'s ``Connection``
   constructor. By using the ``mongodb://`` URL as our ``HOST``
   field, we're actually handing it to ``pymongo``, which will
   do The Right Thing.


Disable "sites" and Enable "djangotoolbox"
------------------------------------------

By default, the application ``django.contrib.sites`` won't behave
well with the Django MongoDB engine. Under the hood, it boils down
to differences in primary keys, which are strings with MongoDB,
and integers elsewhere. It would of course be more elegant to fix
``sites`` in the first place, but for the sake of simplicity, we
will just disable it since we don't need it for simple apps.

Also, we need ``djangotoolbox`` to make user editing in the
admin site work correctly. Long story short, ``djangotoolbox``
allows us to do some JOINs on non-relational databases.


`hellodjango/settings.py step 2 <https://github.com/jpetazzo/django-and-mongodb-on-dotcloud/blob/c39455eef8eb9e4a95620a8d6db2ae3e6c9d8e16/hellodjango/settings.py>`_:

   .. code-block:: python

      # …

      INSTALLED_APPS = (
          'django.contrib.auth',
          'django.contrib.contenttypes',
          'django.contrib.sessions',
          #'django.contrib.sites',
          'django.contrib.messages',
          'django.contrib.staticfiles',
          # Uncomment the next line to enable the admin:
          # 'django.contrib.admin',
          # Uncomment the next line to enable admin documentation:
          # 'django.contrib.admindocs',
          'djangotoolbox',
      )

      # …

Django Admin Site
-----------------

We will now activate the Django administration application.
Nothing is specific to dotCloud here: we just uncomment the relevant
lines of code in ``settings.py`` and ``urls.py``.

`hellodjango/settings.py step 3 <https://github.com/jpetazzo/django-and-mongodb-on-dotcloud/blob/12430eb61eb4b46e5c45401b45b59d9688b11426/hellodjango/settings.py>`_:

   .. code-block:: python

      # …

      INSTALLED_APPS = (
          'django.contrib.auth',
          'django.contrib.contenttypes',
          'django.contrib.sessions',
          #'django.contrib.sites',
          'django.contrib.messages',
          'django.contrib.staticfiles',
          # Uncomment the next line to enable the admin:
          'django.contrib.admin',
          # Uncomment the next line to enable admin documentation:
          # 'django.contrib.admindocs',
          'djangotoolbox',
      )

      # …

`hellodjango/urls.py (updated) <https://github.com/jpetazzo/django-and-mongodb-on-dotcloud/blob/12430eb61eb4b46e5c45401b45b59d9688b11426/hellodjango/urls.py>`_:

   .. code-block:: python

      from django.conf.urls.defaults import patterns, include, url

      # Uncomment the next two lines to enable the admin:
      from django.contrib import admin
      admin.autodiscover()

      urlpatterns = patterns('',
          # Examples:
          # url(r'^$', 'hellodjango.views.home', name='home'),
          # url(r'^hellodjango/', include('hellodjango.foo.urls')),

          # Uncomment the admin/doc line below to enable admin documentation:
          # url(r'^admin/doc/', include('django.contrib.admindocs.urls')),

          # Uncomment the next line to enable the admin:
          url(r'^admin/', include(admin.site.urls)),
      )


If we push our application now, we can go to the ``/admin`` URL,
but since we didn't call ``syncdb`` yet, the database structure
doesn't exist, and Django will refuse to do anything useful for us.


Automatically Call syncdb
-------------------------

To make sure that the database structure is properly created, we
want to call ``manage.py syncdb`` automatically each time we push
our code. On the first push, this will create the Django tables;
later, it will create new tables that might be required by new
models you will define.

To make that happen, we create a ``postinstall`` script. It is
called automatically at the end of each push operation.

`postinstall <https://github.com/jpetazzo/django-and-mongodb-on-dotcloud/blob/60226083ebcf0b453a048bc39ed69b973251a560/postinstall>`_:

   .. code-block:: sh

      #!/bin/sh
      python hellodjango/manage.py syncdb --noinput


A few remarks:

* this is a shell script (hence the ``#!/bin/sh`` shebang at the
  beginning), but you can also use a Python script if you like;
* the script should be made executable, by running ``chmod +x
  postinstall`` before the push;
* by default, ``syncdb`` will interactively prompt you to create
  a Django superuser in the database, but we cannot interact with
  the terminal during the push process, so we disable this thanks
  to ``--noinput``.

If you push the code at this point, hitting the ``/admin`` URL
will display the login form, but we don't have a valid user yet,
and the login form won't have the usual Django CSS since we didn't
take care about the static assets yet.


Create Django Superuser
-----------------------

Since the ``syncdb`` command was run non-interactively, it did not
prompt us to create a superuser, and therefore, we don't have a
user to login.

To create an admin user automatically, we will write a simple Python
script that will use Django's environment, load the authentication
models, create a ``User`` object, set a password, and give him
superuser privileges.

The user login will be ``admin``, and its password will be ``password``.
Note that if the user already exists, it won't be touched. However,
if it does not exist, it will be re-created. If you don't like this
``admin`` user, you should not delete it (it would be re-added each
time you push your code) but just remove its privileges and reset its
password, for instance.

`mkadmin.py <https://github.com/jpetazzo/django-and-mongodb-on-dotcloud/blob/2f0975b1c91d3bf4d46b369b5615a26284510064/mkadmin.py>`_:

   .. code-block:: python

      #!/usr/bin/env python
      from wsgi import *
      from django.contrib.auth.models import User
      u, created = User.objects.get_or_create(username='admin')
      if created:
          u.set_password('password')
          u.is_superuser = True
          u.is_staff = True
          u.save()

`postinstall (updated) <https://github.com/jpetazzo/django-and-mongodb-on-dotcloud/blob/2f0975b1c91d3bf4d46b369b5615a26284510064/postinstall>`_:

   .. code-block:: sh

      #!/bin/sh
      python hellodjango/manage.py syncdb --noinput
      python mkadmin.py

At this point, if we push the code, we will be able to login, but
we still lack the CSS that will make the admin site look nicer.


Handle Static and Media Assets
------------------------------

We still lack the CSS required to make our admin interface look nice.
We need to do three things here.

First, we will edit ``settings.py`` to specify ``STATIC_ROOT``,
``STATIC_URL``, ``MEDIA_ROOT``, and ``MEDIA_URL``.

``MEDIA_ROOT`` will point to ``/home/dotcloud/data``. By convention, the
``data`` directory will persist across pushes. This is important: You
don't want to store media (user uploaded files...) in ``current`` or
``code``, because those directories are wiped out at each push.

We decided to point ``STATIC_ROOT`` to ``/home/dotcloud/volatile``,
since the static files are “generated” at each push. We could have put
them in ``current`` but to avoid conflicts and confusions we chose a
separate directory.

The next step is to instruct Nginx to map ``/static`` and ``/media``
to those directories in ``/home/dotcloud/data`` and ``/home/dotcloud/volatile``.
This is done through a Nginx configuration snippet. You can do many
interesting things with custom Nginx configuration files;
http://docs.dotcloud.com/guides/nginx/ gives some details about that.

The last step is to add the ``collectstatic`` management command to
our ``postinstall`` script. Before calling it, we create the required
directories, just in case.

`hellodjango/settings.py step 4 <https://github.com/jpetazzo/django-and-mongodb-on-dotcloud/blob/41919d72aa35486eacc1bf890ca519aaadfde75d/hellodjango/settings.py>`_:

   .. code-block:: python

      # …

      # Absolute filesystem path to the directory that will hold user-uploaded files.
      # Example: "/home/media/media.lawrence.com/media/"
      MEDIA_ROOT = '/home/dotcloud/data/media/'

      # URL that handles the media served from MEDIA_ROOT. Make sure to use a
      # trailing slash.
      # Examples: "http://media.lawrence.com/media/", "http://example.com/media/"
      MEDIA_URL = '/media/'

      # Absolute path to the directory static files should be collected to.
      # Don't put anything in this directory yourself; store your static files
      # in apps' "static/" subdirectories and in STATICFILES_DIRS.
      # Example: "/home/media/media.lawrence.com/static/"
      STATIC_ROOT = '/home/dotcloud/volatile/static/'

      # URL prefix for static files.
      # Example: "http://media.lawrence.com/static/"
      STATIC_URL = '/static/'

      # URL prefix for admin static files -- CSS, JavaScript and images.
      # Make sure to use a trailing slash.
      # Examples: "http://foo.com/static/admin/", "/static/admin/".
      ADMIN_MEDIA_PREFIX = '/static/admin/'

      # …

`nginx.conf <https://github.com/jpetazzo/django-and-mongodb-on-dotcloud/blob/41919d72aa35486eacc1bf890ca519aaadfde75d/nginx.conf>`_:

   .. code-block:: nginx

      location /media/ { root /home/dotcloud/data ; }
      location /static/ { root /home/dotcloud/volatile ; }

`postinstall (updated again) <https://github.com/jpetazzo/django-and-mongodb-on-dotcloud/blob/41919d72aa35486eacc1bf890ca519aaadfde75d/postinstall>`_:

   .. code-block:: sh

      #!/bin/sh
      python hellodjango/manage.py syncdb --noinput
      python mkadmin.py
      mkdir -p /home/dotcloud/data/media /home/dotcloud/volatile/static
      python hellodjango/manage.py collectstatic --noinput


After pushing this last round of modifications, the CSS for the admin
site (and other static assets) will be found correctly, and we have a
very basic (but functional) Django project to build on!


Wait for MongoDB to Start
-------------------------

At this point, if you try to push the app with a different name
(e.g. ``dotcloud push``), you will see database connection
errors. Let's see why, and how to avoid that!

On dotCloud, you get your own MongoDB instance. This is not just
a database inside an existing MongoDB server: it is your own MongoDB
server. This eliminates access contention and side effects caused
by other users. However, it also means that when you deploy a MongoDB
service, you will have to wait a little bit while MongoDB pre-allocates
some disk storage for you. This takes about one minute.

If you did the tutorial step by step, you probably did not notice
that, since there was probably more than one minute between your
first push, and your first attempt to use the database. But if you
try to push all the code again, it will try to connect to the
database straight away, and fail.

To avoid connection errors (which could happen if we try to connect
to the server before it's done with space pre-allocation), we add
a small helper script, ``waitfordb.py``, which will just try to connect
every 10 seconds. It exists as soon as the connection is successful.
If the connection fails after 10 minutes, it aborts (as a failsafe
feature).

`postinstall (final update) <https://github.com/jpetazzo/django-and-mongodb-on-dotcloud/blob/5a0c613c0ba636586df70605b2d96192aae2dcf3/postinstall>`_:

   .. code-block:: sh

      #!/bin/sh
      python waitfordb.py
      python hellodjango/manage.py syncdb --noinput
      python mkadmin.py
      mkdir -p /home/dotcloud/data/media /home/dotcloud/data/static
      python hellodjango/manage.py collectstatic --noinput

`waitfordb.py <https://github.com/jpetazzo/django-and-mongodb-on-dotcloud/blob/5a0c613c0ba636586df70605b2d96192aae2dcf3/waitfordb.py>`_:

   .. code-block:: python

      #!/usr/bin/env python
      from wsgi import *
      from django.contrib.auth.models import User
      from pymongo.errors import AutoReconnect
      import time
      deadline = time.time() + 600
      while time.time() < deadline:
          try:
              User.objects.count()
              print 'Successfully connected to database.'
              exit(0)
          except AutoReconnect:
              print 'Could not connect to database. Waiting a little bit.'
              time.sleep(10)
          except ConfigurationError:
              print 'Could not connect to database. Waiting a little bit.'
              time.sleep(10)
      print 'Could not connect to database after 10 minutes. Something is wrong.'
      exit(1)


With this last step, our Django deployment can be reproduced as many
times as required (for staging, development, production, etc.) without
requiring special manual timing!
