Web2py
======

You can deploy `web2py <http://web2py.com/>`_ on DotCloud by using our
regular Python WSGI stack. web2py is very easy to setup on your local
computer, but deploying it to a cloud platform requires some extra
steps, which are detailed there.

Before getting to web2py, you should start with some DotCloud
basics; see :ref:`setupaccount`.


Deploy a Python service
-----------------------

The first step is to create a Python service to host web2py.
This is achieved by a single command::

  $ dotcloud deploy -t python mycrm.www
  Created "mycrm.www"

.. note:: Replace *mycrm.www* by any :ref:`deployment name <faq-deploymentname>`
          of your choice.

You can now see the new WWW service in action at http://www.mycrm.dotcloud.com/.
Don't forget to adapt the URL with your deployment name.


Download web2py
---------------

On your local computer, `download web2py 
<http://www.web2py.com/examples/static/web2py_src.zip>`_ 
(of course, you can skip that part if you already have web2py 
installed locally). 

Unzip the archive; it will create a web2py directory. We will now work
inside this directory. 

.. note::
   If you already have a local web2py install and are not sure
   which directory we are talking about, it's the one with web2py.py!


Prepare web2py for DotCloud
---------------------------

DotCloud offers a Python+WSGI environment. The service we deployed
a few minutes ago expects a file named "wsgi.py" containing the WSGI
application. Since web2py conveniently provides a WSGI handler, all
we have to do is a symlink::

  web2py$ ln -s wsgihandler.py wsgi.py


Push your web2py stack to DotCloud
----------------------------------

You can now run the following command::

  web2py$ dotcloud push mycrm.www .

The first time you run this command, the whole directory will be
uploaded to DotCloud. This might take some time, especially if you
have a slow Internet connection. However, the next time you run
"dotcloud push", it will only do a differential transfer, which
will be much faster.

You can now go to http://www.mycrm.dotcloud.com/ and see the default
web2py "hello world" page.


Install and configure applications
----------------------------------

Applications must be installed locally, and then pushed to DotCloud.
Just follow those steps:

#. Start the local web2py server::

     web2py$ ./web2py.py

   This will ask you to chose an admin password.

#. Point your browser to the local web2py server (this should be 
   http://127.0.0.1:8000).

#. Go to the administration interface (follow the link on the home page
   or go straight to http://127.0.0.1:8000/admin).

#. Use the "Upload & install packed application" panel on the right.
   In our example, we will name the application "mycrm" and use
   the `free CRM appliance <http://web2py.com/appliances/default/show/57>`_
   available on the web2py site. You can download the `package
   <http://web2py.com/appliances/default/download/app.source.9ee5fc505dd76ea2.7765623270792e6170702e63726d2e773270.w2p>`_
   or specify its URL.

#. Push your stack again::

     web2py$ dotcloud push mycrm.www .

#. Point your browser to http://www.mycrm.dotcloud.com/mycrm and see
   your new web2py application in its whole glory!

.. warning::
   By default, all data will be saved to SQLite databases. This has the
   advantage of being easy to setup (actually, it was automatically setup
   by web2py without any action on our part), but it has serious drawbacks:

   * it does not scale -- when DotCloud scales up your service, you
     will end up with multiple copies of the database, without any
     synchronization whatsoever;
   * each time you do a "dotcloud push", the database will be reset to
     the state of your local database (in your local web2py copy).

   In the next step, we will see how to plug in a PostgreSQL database and
   achieve true persistence.


Deploy a PostgreSQL database
----------------------------

This is as easy as::

  web2py$ dotcloud deploy -t postgresql mycrm.sql
  Created "mycrm.sql"

.. note::
   I used "mycrm.sql" so the database service uses the same *deployment*
   name (that's the "mycrm" part), but this is not mandatory.

   Also, I used "sql" as the second part because it is more explicit
   than "foobar"; but you are free to use whatever you like: "db", 
   "pgsql", "storage", "bananas"...

We must now retrieve the parameters of the newly created database::

  web2py$ dotcloud info mycrm.sql
  cluster: wolverine
  config:
      postgresql_password: /.d/EfXzi|esfFz?+-c-
  deployment: mycrm
  name: mycrm.sql
  ports:
  -   name: ssh
      url: ssh://postgres@sql.mycrm.dotcloud.com:1233
  -   name: sql
      url: pgsql://root:/.d/EfXzi|esfFz?+-c-@sql.mycrm.dotcloud.com:1234
  type: postgresql

The next step is to hook our application to the PostgreSQL database.
If you are deploying the CRM application, edit *application/mycrm/models/db.py*
(you can edit it with your favorite editor, or through your local web2py
administration interface). In the beginning of the file, find the line
configuring the SQLite DB::

  db=DAL("sqlite://storage.sqlite")

You need to replace that with a PostgreSQL connection string::

  db=DAL("postgres://root:/.d/EfXzi|esfFz?+-c-@sql.mycrm.dotcloud.com:1234/template1")

.. note::
   To build the connection string, take the URL starting with pgsql://
   and replace pgsql:// with postgres://. You should also add /template1
   at the end of the connection string.
   
.. note::
   We are taking two shortcuts there:

   * we are using the default "root" user which pre-exists in our 
     PostgreSQL service;
   * we are using the database "template1" which was also created
     automatically.

   That's not very clean -- unless you are going to use only one
   database. 
   Otherwise, you should use a PostgreSQL client like "psql" or "pgadmin"
   to create a new user and a new database, and specify them in
   your connection string. These steps are explained in the :doc:`PostgreSQL
   service documentation </services/postgresql>`.

Save the file and get back to http://127.0.0.1:8000/mycrm.
You might think that nothing has changed, but actually, behind the
scenes, you're now storing data in a PostgreSQL database.

The last step is to push the new web2py stack along with its CRM 
application and SQL settings::

   web2py$ dotcloud push mycrm.www .

Your application should now be visible at http://www.mycrm.dotcloud.com/mycrm.


Using web2py admin
------------------

Although web2py allows remote connections to the admin interface
through SSL, we recommend that you perform all admin actions on your
local web2py server and then push to DotCloud. That is the best way to
ensure that your deployment will remain scalable.

.. note::
   At this time, DotCloud does not support SSL connections, so you will
   not be able to connect to the web2py admin remotely. As noted above,
   though, to ensure scalability, we don’t recommend remote connections
   to admin anyway.

If you really need to
access the admin interface for your web2py application on DotCloud,
it can be enabled by the following two steps.

.. warning::
   You should not do this for production sites since it is inherently
   insecure and will not scale!

To allow remote connections through plain HTTP, we will change one
line in web2py. This will allow the session cookie to be sent even
through plain HTTP connections, instead of being "secure" (i.e.
sent only through HTTPS).

::

  web2py$ sed -i 's/session.secure()/pass # Do not setup secure cookie/' \
          applications/admin/models/access.py

(This is a single command-line, the \\ is there for continuation purposes.)

We also need to generate the admin password and put it in the default
parameters file. Using a Python interpreter in the web2py directory,
here is one way to do it::

  from hashlib import md5
  hash = md5('yoursecretpassword').hexdigest()
  with open('parameters_80.py','w') as f:
      f.write('password="%s"\n'%hash)

You can now do "dotcloud push" one more time and access the admin
site directly on the DotCloud service.


Last word about web2py and databases
------------------------------------

When deploying third-party web2py appliances, you should switch
from the default SQLite backend to :doc:`PostgreSQL </services/postgresql>` or
:doc:`MySQL </services/mysql>`.

This is generally done by editing "application/<appname>/models/db.py",
as shown above.

However, you may run into the following problems:

* The DAL (Data Abstraction Layer) does not quote table names when
  creating SQL schemas -- therefore, if some application defines a table
  named "user" (which is fairly common), the schema
  creation will fail.
  To avoid this problem, you can check for names that clash with
  backend-specific reserved words by including the check_reserved
  argument in your call to DAL (see `this doc
  <http://web2py.com/book/default/chapter/06#Reserved-Keywords>`_ or `this post
  <http://thadeusb.com/weblog/2010/2/1/check_reserved_sql_keywords_on_web2py_dal>`_
  for more details).
* some applications ship with a SQLite database, containing
  the database schema and some preloaded data -- but when you switch
  to another database, the schema is not always fully re-created, and the
  data is not migrated (leaving the application in a semi-usable state).

This is not related to DotCloud: you will experience the same issues
when running web2py on your local computer with some third-party web2py
appliances when you switch the default SQLite backend to PostreSQL.

*Thanks to Anthony Bastardi for his invaluable feedback about this tutorial.*
