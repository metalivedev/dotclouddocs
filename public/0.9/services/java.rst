:title: Java Service
:description: dotCloud's Java service can host any application based on java.servlet.
:keywords: dotCloud documentation, quick start guide, java, java.servlet, basic use, Jetty, WAR file

Java
====

.. include:: ../dotcloud2note.inc

The *java* service can host any kind of application based on *java.servlet*.

This makes it suitable not only for "barebones" servlets, but also many 
frameworks and languages, including (but not limited to):

* Clojure
* Grails
* JRuby on Rails
* Lift
* Play!
* Scala
* and a lot more

Most ready-to-use applications will also run fine. You can generally assume
that if it ships as a WAR [#]_ file, it will run on dotCloud.


.. include:: service-boilerplate.inc

.. code-block:: yaml

   www:
     type: java
     approot: hellojava

This Build File tells the dotCloud platform that the code (or the WAR file) 
for this service will be located in the "hellojava" directory.

If you are *not* deploying multiple services, you can omit the approot
directive, and put your WAR files in the same directory as dotcloud.yml.

After copying your WAR file into the ramen-on-dotcloud directory, your
app is ready.

.. include:: service-push.inc


Advanced Configuration
----------------------

You can modify some advanced parameters of the Java configuration from your
"dotcloud.yml":

.. code-block:: yaml

   www:
     type: java
     config:
       java_initial_heap_size: 64
       java_maximum_heap_size: 128
       java_maximum_permgen_size: 64
       jetty_version: jetty-6
       java_version: java-6

Let's see what each setting does:

- ``java_initial_heap_size``: this option controls the initial size (in
  megabytes) of the memory allocation pool of the JVM;
- ``java_maximum_heap_size``: this option controls the maximum size of the
  memory allocation pool of the JVM (keep in mind that this is not changed when
  you :doc:`vertically scale </guides/scaling>`);
- ``java_maximum_permgen_size``: this option controls the maximum size of the
  memory pool allocated by the JVM to store class files, you might need to
  change this for large applications;
- ``jetty_version``: this option lets you specify which jetty version you want
  to use. Options are ``jetty-6``, ``jetty-7``, and ``jetty-8``. Default is
  ``jetty-6``;
- ``java_version``: this option controls which java version you will be using
  with this service. Options are ``java-6``, and ``java-7``. Default is
  ``java-6``. ``java-7`` only works with the following ``jetty_version``'s
  ``jetty-7`` and ``jetty-8``.


New settings are applied on the next push.


Java and Jetty Versions
-----------------------

The java service lets you decide which java and jetty versions you want
to use with your application. It currently supports Java 1.6, and Java
1.7. It also supports Jetty 6, 7 and 8.

.. note::

   Java 1.7 only works with Jetty 7 and 8. The default settings are Java
   1.6 and Jetty 6. To learn how to configure these options, see the the
   `Advanced Configuration`_ section.


Internals
---------

The java servlet is served by `Jetty <http://jetty.codehaus.org/jetty/>`_.
For most purposes, it will be 100% compatible with other application
servers like Tomcat.

If you upload a WAR file named ``thing.war``, it will come up as
http://...dotcloud.com/thing/. To host your application directly
at the top of the URL hierarchy, name it ``ROOT.war``.

If you push an unpacked WAR, and want to host it at the root of your
service, you should place your code (i.e. the ``WEB-INF`` directory
and everything) in a directory called ``ROOT``.

Note that while you can push your apps as unpacked WAR files,
it is not recommended to do so, since some class loading mechanisms
work better when your app ships as a WAR file.

.. [#] http://en.wikipedia.org/wiki/Web_application_archive
