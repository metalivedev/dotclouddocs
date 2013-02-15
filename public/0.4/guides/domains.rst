:title: Custom Domains
:description: How to use custom domains with the dotCloud platform.
:keywords: dotCloud documentation, quick start guide, custom domain, DNS records, SSL

Custom Domains
==============

New custom domains require a `'live'`_ flavor. So the first steps in
adding a custom domain to your application are:

1. Deploy a `'live'`_ application on dotCloud.
2. Purchase your domain name.
3. ``"dotcloud alias add"``
4. Edit your DNS CNAME record via your registrar.
5. Wait up to 48 hours (but it could be as quick as 2 minutes).

Adding a Custom Domain
----------------------

1. Deploy
.........

Please see the `'live'`_ documentation for how to create a
'live'-flavored application.

2. Purchase your domain
.......................

There are many domain registration services. You can use any domain
registrar that allows you to edit your CNAME records. Some popular registrars are:

  * GoDaddy.com
  * ix web hosting
  * 1and1
  * EveryDNS.net
  * Yahoo!SmallBusiness

3. Alias add
............

Let's imagine you've purchased the domain "example.com". You cannot
map a naked domain like "example.com" to a dotCloud-hosted service,
but you can map a wildcard domain ("\*.example.com") or a specific
subdomain like "www.example.com".

To use www.example.com with your ramen.www service::

   $ dotcloud alias add ramen.www www.example.com
   Ok. Now please add the following DNS record:
   www.example.com. IN CNAME gateway.dotcloud.com

If everything goes well, you'll see the "Ok..." message and the
command will show you the DNS records to use to complete the
setup. The first part ("www.example.com.") is the name of the record
you should edit at your registrar. The second part,
"gateway.dotcloud.com" is the data you should map to your domain. Note
that "gateway.dotcloud.com" could change, so please pay attention to
the value returned by ``'dotcloud alias'`` after "IN CNAME".

You should follow your registrar's instructions for editing your DNS
record and CNAME. The specifics of how to edit vary from registrar to
registrar. In general you follow these steps:

   a. Log in to your registrar.
   b. Select the domain you want to edit.
   c. Open the DNS editor.
   d. Add a CNAME record

      i. *Name* is usually the new subdomain, e.g. "www" or "\*" (for all subdomains)
      ii. *Host Name* or *Data* is the value ``"dotcloud alias add"`` returned after "IN CNAME", e.g. "gateway.dotcloud.com".

dotCloud's alias command will be effective in about a minute after
completing, and your service will be available on your custom domain
as soon as your registrar propagates the change in your DNS CNAME. The
DNS name change could take as long as 48 hours, but often completes in
just a few minutes.

Removing a Custom Domain
------------------------

::

   $ dotcloud alias remove ramen.www www.example.com


Naked Domains
-------------

You cannot use a "naked domain" (i.e. an address without an host part, like
my-own-domain.com). See why on `our forum <http://answers.dotcloud.com/question/2/more-flexible-aliases-that-dont-require-a>`_.

.. note::

   You can use *wildcard domains*, by using a domain like \*.example.com.



SSL
---

dotCloud can host your own CA-signed certificates with your custom domains.
For more information, please see the SSL_ documentation.

.. _`'live'`: ../flavors
.. _ticket: http://dotcloud.zendesk.com
.. _SSL: ../ssl
