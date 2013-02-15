:title: GeoDjango Python Tutorial
:description: How to build a geographic Django application using GeoDjango on dotCloud.
:keywords: dotCloud, tutorial, documentation, python, GeoDjango, geographic web framework

GeoDjango
=========

According to the `GeoDjango project page <http://geodjango.org/>`_,
GeoDjango is "a world-class geographic web framework".

If you are a Django developer willing to build geographic applications,
read ahead! We will see here how easy it is to deploy GeoDjango on dotCloud.

All the code presented here is also available on GitHub, at
http://github.com/jpetazzo/geodjango-on-dotcloud; moreover, you can
read the instructions in two original ways:

* using GitHub's awesome `compare view
  <https://github.com/jpetazzo/geodjango-on-dotcloud/compare/begin...end>`_ --
  click on each individual commit to see detailed explanations for each step;
* or, if you prefer text mode (or offline inspection), fallback on
  ``git log --patch --reverse begin..end``.

This tutorial assumes that you already went through the :doc:`Django
on dotCloud <django>` tutorial, or at least, that you already know
how to deploy a Django app on dotCloud.


.. contents::
   :local:
   :depth: 1



Switch to PostGIS instead of plain PostgreSQL
---------------------------------------------

The first step is to edit ``dotcloud.yml``, and replace our
plain PostgreSQL with PostGIS. Technically, PostGIS is just
PostgreSQL with additional geographic extensions. It provides
new SQL types, functions, and also a bunch of tables that
are used by PostGIS to store projection information (allowing
you to add your custom projections without recompiling or
even editing local files), as well as metadata about your
own geographic tables.

``dotcloud.yml``::

  www:
    type: python
  db:
    type: postgis
  


If you're new to PostGIS, we recommend checking out
http://postgis.refractions.net/ to get a primer about
how it works. It's not strictly necessary to work with
GeoDjango, but it can help -- just like knowing SQL is
not mandatory to work with Django, but will make you
a better Django developer.


Enable PostGIS ORM driver and GeoDjango app
-------------------------------------------

To turn Django into GeoDjango, you only need two things:

* use a geographic-capable ORM engine;
* enable the ``django.contrib.gis`` application.

Since our database is PostGIS, we will use the relevant ORM engine.

``hellodjango/settings.py``::

  [...]  
  DATABASES = {
      'default': {
          'ENGINE': 'django.contrib.gis.db.backends.postgis',
          'NAME': 'template1',
          'USER': env['DOTCLOUD_DB_SQL_LOGIN'],
          'PASSWORD': env['DOTCLOUD_DB_SQL_PASSWORD'],
          'HOST': env['DOTCLOUD_DB_SQL_HOST'],
          'PORT': int(env['DOTCLOUD_DB_SQL_PORT']),
      }
  }
  [...]
  INSTALLED_APPS = (
      'django.contrib.auth',
      'django.contrib.contenttypes',
      'django.contrib.sessions',
      'django.contrib.sites',
      'django.contrib.messages',
      'django.contrib.staticfiles',
      # Uncomment the next line to enable the admin:
      'django.contrib.admin',
      # Uncomment the next line to enable admin documentation:
      # 'django.contrib.admindocs',
      'django.contrib.gis',
  )
  [...]

As you can see, these two steps are as easy as pie!


Add a Geographic App
--------------------

We will now write the (possibly) simplest GeoDjango app ever.
It defines a model with a ``PointField`` and a name. Think of it
as a way to record famous landmarks. Of course, GeoDjango also has
other types, allowing to use lines, polygons, as well as collections
of those basic types. Available fields and their options are
described in the documentation at
https://docs.djangoproject.com/en/dev/ref/contrib/gis/model-api/.

``hellodjango/places/__init__.py``::

  # This is just an empty file :-)  

``hellodjango/places/models.py``::

  from django.contrib.gis.db import models
  
  class Place(models.Model):
      def __unicode__(self):
          return self.name
      name = models.CharField(max_length=100)
      geom = models.PointField()
      objects = models.GeoManager()
  
``hellodjango/settings.py``::

  INSTALLED_APPS = (
      'django.contrib.auth',
      'django.contrib.contenttypes',
      'django.contrib.sessions',
      'django.contrib.sites',
      'django.contrib.messages',
      'django.contrib.staticfiles',
      # Uncomment the next line to enable the admin:
      'django.contrib.admin',
      # Uncomment the next line to enable admin documentation:
      # 'django.contrib.admindocs',
      'django.contrib.gis',
      'hellodjango.places',
  )


Also, we will allow this app to show up in the Django admin site.
GeoDjango provides a special geographic admin, that displays
geographic fields on a map, and allows you to edit them, just like
you would edit a regular text or numeric field.

``hellodjango/places/admin.py``::

  from django.contrib.gis import admin
  from models import Place
  
  admin.site.register(Place, admin.OSMGeoAdmin)
  

At that point, your first GeoDjango app is ready: push it to
dotCloud! At the end of the push, you will see the application
URL. Add "/admin" at the end of the URL, login with "admin"
and "password", and try to add something in the "Places" table!



