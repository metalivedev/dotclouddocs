:title: Mobwrite Python Tutorial
:description: How to deploy MobWrite on dotCloud.
:keywords: dotCloud, tutorial, documentation, python, mobwrite

MobWrite
========

Google MobWrite is a "Real-time Synchronization and Collaboration Service".
The original project can be found at http://code.google.com/p/google-mobwrite.
This tutorial is based on http://github.com/plasticine/google-mobwrite,
which did the really important updates, like enabling MobWrite to run over
WSGI instead of mod_python.

All the code of the tutorial is available on GitHub, at
http://github.com/jpetazzo/google-mobwrite. To test it on DotCloud,
all you have to do is::

  git clone git://github.com/jpetazzo/google-mobwrite.git
  cd google-mobwrite
  dotcloud create mobwrite
  dotcloud push mobwrite

Moreover, you can read the whole instructions in two original ways:

* using GitHub's awesome `compare view
  <https://github.com/jpetazzo/google-mobwrite/compare/begin...end>`_ --
  click on each individual commit to see detailed explanations for each step;
* or, if you prefer text mode (or offline inspection), fallback on
  ``git log --patch --reverse begin..end``.

.. contents::
   :local:
   :depth: 1


DotCloud Build File
-------------------

The DotCloud Build File, ``dotcloud.yml``, describes our stack.

Since MobWrite is a Python web app, we will use a single "python" service.

The role and syntax of the DotCloud Build File is explained in further
detail in the documentation, at http://docs.dotcloud.com/guides/build-file/.

``dotcloud.yml``::

  www:
    type: python
  


wsgi.py File
------------

The "python" service dispatches HTTP requests to a WSGI-compatible callable
which must be found at ``wsgi.application``. In other words, we need to
have a ``application`` function in the ``wsgi.py`` file.

The MobWrite gateway has already been "WSGI-fied", and contains a suitable
``application`` callable. However, the gateway code is located in the
``daemon`` directory, and expects to import other modules from this
directory. Therefore, we will add the ``daemon`` directory to the Python Path
(instead of doing e.g. ``from daemon.gateway import application``).

``wsgi.py``::

  import sys
  sys.path.append('/home/dotcloud/current/daemon')
  from gateway import application
  


supervisord.conf File
---------------------

The MobWrite "gateway" (the WSGI application that we enabled in the previous
step) actually talks to the MobWrite "daemon". We need to start this "daemon".
While we could use ``dotcloud ssh`` to log into the service and start it
manually, we will rather write a ``supervisord.conf`` file, as explained in
the `Background Processes Guide <http://docs.dotcloud.com/guides/daemons/>`_.

``supervisord.conf``::

  [program:mobwrite]
  directory=/home/dotcloud/current/daemon
  command=/home/dotcloud/current/daemon/mobwrite_daemon.py
  


Update Shebangs
---------------

All the Python programs in the original code start with ``#!/usr/bin/python``.
That generally works, unless you are using a non-default Python install, or
a wrapper like `virtualenv <http://www.virtualenv.org>`_. Guess what: DotCloud
uses ``virtualenv``, so we have to replace all those ``#!/usr/bin/python``
with the more standard ``#!/usr/bin/env python``.

We modify 
``daemon/mobwrite_daemon.py``,
``lib/diff_match_patch.py``,
``lib/json_validator_test.py``,
``lib/mobwrite_core_test.py``,
``tools/download.py``,
``tools/loadtest.py``,
``tools/nullify.py``, and
``tools/upload.py``.


Final Touch
-----------

At that step, you can already ``dotcloud push`` and get a working app;
but you still need to display a custom HTML form to start it.
To make testing more convenient, we will do a minor change to the code,
so that going to the MobWrite app will show the editor if no valid POST
parameter is supplied.

After this change, you can (re-)push the code, and this time, when
you go to the app URL, you should see the edit form. Opening the app
from multiple tabs or browsers will allow you to edit the form concurrently
(like Etherpad or recent versions of Google Docs Text Editor).

``daemon/gateway.py``::
  
               out_string = form['q'].value # Client sending a sync.  Requesting text return.
           elif form.has_key('p'):
               out_string = form['p'].value # Client sending a sync.  Requesting JS return.
  +        else:
  +            # Nothing. Redirect to the editor.
  +            response_headers = [ ('Location', environ['SCRIPT_NAME']+'?editor') ]
  +            start_response('303 See Other', response_headers)
  +            return []
           
           in_string = ""
           s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
