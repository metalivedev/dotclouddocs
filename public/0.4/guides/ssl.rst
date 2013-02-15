:title: SSL
:description: Piggyback SSL is automatically included with your application. Simply access your application at its regular URL over HTTPS.
:keywords: dotCloud documentation, quick start guide, Piggybacking SSL, force ssl, SSL

SSL
===


Piggyback SSL
-------------

Piggyback SSL is automatically included with your application. Simply access
your application at its regular URL over HTTPS.

Custom Domain SSL
-----------------

dotCloud can host your own CA-signed certificates with your custom domains.

With `Custom Domains`_ dotCloud lets you attach your own domain name
(e.g. ``example.com``) to any application.

If you haven't configured your custom domain yet, and you would like
to use SSL with a custom domain, please see the `Custom Domains`_
documentation first, and then return here.

Using your own SSL certificates requires dotCloud to set up a load
balancer with 160MB on it. You will be billed for that monthly fee in
addition to any other memory used by your services.

Here is an overview of the steps to configure SSL for your custom domain:

  1. Set up a live service with a custom domain, as documented above.
  2. Purchase your SSL certificate from your registrar or Certificate Authority.
  3. Use the ``dotcloud`` CLI to securely push your certificates to a temporary application.
  4. File a ticket_ with support to let dotCloud know you want us to set up SSL for you.
  5. Receive confirmation that the SSL certificates are in place from dotCloud.
  6. Your bill will increase by the price of 160MB.
  7. [Optional] Destroy the temporary service you used to send dotCloud the certificates.

1. Set up a `'live'`_ service with a custom domain
..................................................

You should follow the steps in `Custom Domains`_ to create your own
custom domain first. Once you have your custom domain mapped
successfully to your `'live'`_ dotCloud service, then you can purchase
your SSL certificate for your domain or subdomain from your DNS
registrar or a Certificate Authority.

2. Purchase your SSL certificate
................................

Your registrar may also sell SSL certificates, or you can purchase
from another Certificate Authority. The choice is up to you.

The SSL material you purchase will generally be made of two or three
parts:

  * the secret key (which should NOT be protected by a passphrase),
  * the certificate (which should be in PEM format, not DER or PKCS
    format),
  * an optional "certificate chain" or "intermediate certificates"
    (this is not always necessary; and when it is, it can be bundled
    with your certificate, or come as separate files; your certificate
    authority will supply that if it's needed).

3. Securely push your certificates
..................................

.. warning::

   Do not send the certificate materials by email or by attaching them
   to a ticket in support! Please follow the instructions below to
   ensure secure handling of your secrets.

Once you have the files listed in step 2, please deploy a
new, temporary app to transfer them to dotCloud in a secure manner.

Create a new application for the transfer. It is ok to use a 'sandbox'
app for the transfer. In this example, let's call the new application
"edge":

::

    $ dotcloud create edge

Create a directory named e.g. "edge", containing those certificate files::

   edge/
     ├── ssl.key (containing the private key)
     ├── ssl.crt (containing the certificate)
     ├── ssl.chain (optional; containing the chain of intermediary certificates)
     └── dotcloud.yml

dotcloud.yml contains

.. code-block:: yaml

    ssl:
      type: python-worker

Then, push this directory under the app named "edge" (so, from the
directory containing those files, just run ``"dotcloud push edge"``).

::

    $ dotcloud push edge

4. File a ticket_
.................

Once you have pushed this service, notify dotCloud with a support ticket_ and
we will go on with the setup of your SSL instance. Please use the
email address associated with your dotCloud account to file the
ticket_ .

5. Receive confirmation
.......................

dotCloud support will reply to you via the ticket_ to let you know when
your certificate has been deployed.

6. The price
............

The bill for your `'live'`_ application will increase by the cost of
160MB of RAM. For this money you automatically get a pair of edge
servers configured with your SSL certificate. The pair is for
load-balancing and high availability.

7. [Optional] Destroy the temporary application
...............................................

::

   $ dotcloud destroy edge

The ``"edge.ssl"`` service was temporary and can safely be removed
once we have notified you that your SSL setup is complete. This
temporary app ("edge", if that is what you named it) is just a
convenient and secure way to transfer the SSL material, which will be
only available to the dotCloud infrastructure.

Update or Deactivate Custom Domain SSL
--------------------------------------

Once dotCloud has set up your SSL certificate on your own edge
servers, your certificates will persist, even if you destroy your
application. To make changes to your certificates (e.g. to update them
before they expire), please follow steps 3, 4 and 5 above to securely
send dotCloud the new certificates.

To disable your SSL service, please file a ticket_ and let dotCloud know
which URL should have its SSL edge server destroyed. At that point
your application or service would still be accessible via HTTP but not
via HTTPS.

Forcing SSL
-----------

The simplest method is to create a "nginx.conf" file in your approot,
with the following line:

.. code-block:: nginx

   if ($http_x_forwarded_port != 443) { rewrite ^ https://$http_host/; }

However, that will work only with nginx-powered stacks (i.e., it will *not* work
with Java or Node.js). With those stacks, you can check the value of the HTTP
header "X-Forwarded-Port". If it's 443, you're over SSL! This HTTP header will
appear as a variable in your request environment: HTTP_X_FORWARDED_PORT.

.. _`Custom Domains`: ../domains/
.. _`'live'`: ../flavors
.. _ticket: http://dotcloud.zendesk.com
