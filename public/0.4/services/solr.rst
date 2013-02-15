:title: Solr Service
:description: dotCloud features the search platform Solr 3.4.0.
:keywords: dotCloud documentation, dotCloud service, Solr, full-text search, faceted search

Solr
====

DotCloud features Solr 3.4.0, a search platform which can do full-text search,
hit highlighting, faceted search, database integration, rich document (e.g.,
Word, PDF) handling, and geospatial search.

Solr uses the Lucene Java search library at its core for full-text indexing and
search, and has REST-like HTTP/XML and JSON APIs that make it easy to use from
virtually any programming language. Solr runs on top of `Jetty
<http://jetty.codehaus.org/jetty/>`_ which also powers our :doc:`Java service
<java>`.


Basic Setup
-----------

To deploy a Solr service, you will need a Solr configuration. In case you want
to get started without worrying about that, you can :download:`download this
default Solr configuration <solr.zip>`.

If you followed other DotCloud tutorials, you are already familiar with
the "ramen-on-dotcloud" directory and its Build File. Adding Solr to the
mix will result with the following::

  ramen-on-dotcloud/
  |_ dotcloud.yml
  |_ solr/
  |  |- solr.xml   (main configuration file)
  |  |_ conf/      (contains solrconfig.xml, schema.xml, and more config files)
  |  |_ data/      (will hold your index; Solr creates it when needed)
  |  |_ lib/       (optional; Solr will load JAR files located there)
  |  |_ bin/       (optional; default location for replication scripts)
  |_ www/          (some web service, part of your app)
     |_ ...

.. note::

   In your dotcloud.yml :doc:`../guides/build-file`, the approot directory for
   solr should be the "solr" directory containing the solr configuration.
   Example:

   .. code-block:: yaml

      frontend:
        type: ruby
        approot: www
      search:
        type: solr
        approot: solr


Authentication
~~~~~~~~~~~~~~

Basic HTTP authentication is enabled by default, the user is always "dotcloud"
and you can find its password using "dotcloud.info"::

   $ dotcloud info ramen.search

The password is also written in the :doc:`/guides/environment`.

If you want to disable the authentication you can set ``solr_authentication``
to ``false`` in your dotcloud.yml:

.. code-block:: yaml

   search:
     type: solr
     approot: solr
     config:
        solr_authentication: false

.. include:: /guides/config-change.inc


Run Queries
~~~~~~~~~~~

Once you have "dotcloud push" your app, you can start to use your Solr
deployment. Here is how to upload and index a simple XML document with curl::

   $ curl -udotcloud:password --data-binary @- -H 'Content-type:application/xml' http://d07c100d.dotcloud.com/solr/update?commit=true <<EOF
   <add>
   <doc>
     <field name="id">SOLR1000</field>
     <field name="name">Solr, the Enterprise Search Server</field>
     <field name="manu">Apache Software Foundation</field>
     <field name="cat">software</field>
     <field name="cat">search</field>
     <field name="features">Advanced Full-Text Search Capabilities using Lucene</field>
     <field name="features">Optimized for High Volume Web Traffic</field>
     <field name="features">Standards Based Open Interfaces - XML and HTTP</field>
     <field name="features">Comprehensive HTML Administration Interfaces</field>
     <field name="features">Scalability - Efficient Replication to other Solr Search Servers</field>
     <field name="features">Flexible and Adaptable with XML configuration and Schema</field>
     <field name="features">Good unicode support: h&#xE9;llo (hello with an accent over the e)</field>
     <field name="price">0</field>
     <field name="popularity">10</field>
     <field name="inStock">true</field>
     <field name="incubationdate_dt">2006-01-17T00:00:00.000Z</field>
   </doc>
   </add>
   EOF

And here is a simple query to retrieve this document::

  $ curl -udotcloud:password http://d07c100d.dotcloud.com/solr/select?q=*

You can also point your browser at http://d07c100d.dotcloud.com/solr/admin/
for a more friendly interface.

To learn more, have a look at the Solr wiki:
http://wiki.apache.org/solr/FrontPage!


Custom Configuration
--------------------

If you are using an application that comes with its own custom solr config
(like Sunspot for Ruby apps), remember that you have to push the provided
configuration to your solr service.

Also, some solr configurations use port 8983 by default. The DotCloud solr
setup uses port 80, like all web-based stacks. If you see an URL like
http://127.0.0.1:8983/solr in the configuration of the product, you will
probably have to change it to http://d07c100d.dotcloud.com/solr (where
"d07c100d" is actually whatever is shown for your solr service at the end of
the "dotcloud push").
