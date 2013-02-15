:title: SMTP Service
:description: dotCloud's SMTP service is based on Postfix. It can receive e-mails from your instances, and forward them in a number of ways.
:keywords: dotCloud documentation, dotCloud service, SMTP, postfix, email

SMTP
====


Introduction
------------

.. sidebar:: Different words for the same thing

    Before an e-mail arrives in your Inbox it goes through several servers with
    the `SMTP <http://en.wikipedia.org/wiki/SMTP>`_ protocol.

    These servers are also called *SMTP servers*, *e-mails relays* or *MTA*
    (Mail Transfer Agent) -- but it's all the same thing.

Our SMTP service is based on `Postfix <http://www.postfix.org/start.html>`_.
It can receive e-mails from your instances, and forward them:

* either to their final destination (e.g., if you send an e-mail to
  someone@gmail.com, send it to GMail servers);
* or to a relay or *smarthost* like `SendGrid <http://sendgrid.com/>`_,
  `CritSend <http://www.critsend.com/>`_, or others. DotCloud uses (and
  recommends!) `MailGun <http://mailgun.net/>`_ for that purpose.

If the destination server is down, instead of bouncing an error message to
the sender immediately, the MTA will keep it into a queue, and retry to
deliver it at regular intervals. If the delivery still fails after a while
(it can be hours or even days, depending of the setup), it will give up
and send you back an error message. This mechanism allows for mail servers
to be down for short period of time, or to do throttling (stop accepting
new messages) when they are overloaded, for instance.

.. note:: Why should I use a SMTP service?

   If SMTP services were bundled with web application servers, you could
   experience the following issue:

   #. At normal time, you have 4 web frontends.
   #. Due to a peak of activity, you scale to 10 frontends.
   #. Instance #7 sends a notification e-mail to some user.
   #. The e-mail server of this user is temporarily down, causing instance #7
      to spool the message.
   #. The activity goes back to normal, and you want to scale down.
   #. Ah, wait, you can't scale down as long as you have pending messages
      in instances: they would be lost!

   That's why scalable web applications need to decouple things as much
   as possible -- and SMTP servers are just one of the many places where
   it is required.

.. note:: Why should I use a service like MailGun instead of, e.g., SES?

   SES is a basic *outgoing* email service. Mailgun is a complete email
   platform which one can easily build Gmail-like service on top of.

   Even when looking only from outgoing email perspective, Mailgun gives
   developers full reputation isolation and control over their mailing queue.


Deploying
---------

To include a SMTP service in your stack, just add a section like the
following one in your Build File:

.. code-block:: yaml

   mailer:
     type: smtp

.. warning::
   The default configuration will try to send e-mails directly to their
   destination. A lot of e-mail providers will systematically flag messages
   coming from the cloud as "spam". You can still use this default
   configuration for testing purposes, but when going into production,
   you should rely on a third-party mail routing service.

.. sidebar:: Can I reliably send e-mails without a 3rd party service?

   A lot of free email services do provide access to a SMTP server.
   For instance, if you have a GMail account, you can use it to send
   e-mail from your dotCloud deployments.
   Use "smtp.gmail.com" as your smtp_relay_server; port is 587; your username
   will be your GMail e-mail address and your password will be your
   regular GMail password. Note, however, that mails will look like they
   are sent by your GMail address.

You can use a third-party relay (also called a "smarthost") if it supports
standard SMTP routing (most services do). The parameters must be specified
in the Build File.

Here is an example with MailGun:

.. code-block:: yaml

   mailer:
     type: smtp
     config:
       smtp_relay_server: smtp.mailgun.org
       smtp_relay_port: 587
       smtp_relay_username: postmaster@yourmailgundomain.com
       smtp_relay_password: YourMailgunPassword

.. note::
   The *smtp_relay_port*, *smtp_relay_username* and *smtp_relay_password*
   are optional arguments here.

.. include:: ../guides/config-change.inc

Using Your New SMTP Service
---------------------------

There are two ways to use your new SMTP service:

* by entering its address, port, etc. in the local SMTP configuration
  for each other service (PHP, Ruby, Python, Java, and so on);
  to relay them to the SMTP service.
* by sending your messages directly to it, from your code.

.. note:: Why can't I send my messages directly to, e.g., SendGrid?

   Well, you can. As explained above, the whole point of using your
   own dotCloud SMTP relay is to prevent messages from lingering in
   the local SMTP queue, where they could be lost if the service scales down.
   If you are 100% sure that SendGrid will accept your messages 100% of
   the time, you can configure the local SMTP relays to use SendGrid
   servers (or send to SendGrid directly from your code).


Get Your SMTP Service Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You can retrieve all the parameters with "dotcloud info", like this::

  $ dotcloud info ramen.mailer
  cluster: wolverine
  config:
      smtp_password: _|KZ&CKWa[
  name: ramen.mailer
  namespace: ramen
  ports:
  -   name: smtp
      url: smtp://dotcloud:_|KZ&CKWa[@mailer.ramen.dotcloud.com:1587
  -   name: ssh
      url: ssh://dotcloud@mailer.ramen.dotcloud.com:1586
  type: smtp

In that case, the parameters are:

* login: dotcloud
* password: _|KZ&CKWa[
* host: mailer.ramen.dotcloud.com
* port: 1587


Configure the Local MTA to Use Your SMTP Service
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

*All* our services come with a built-in MTA listening on localhost.
You can specify a *smarthost* (SMTP relay) for this MTA when you deploy
the service.

.. note::
   You cannot yet reconfigure a running service. You have to destroy it
   and re-deploy a new one for now. Sorry about that.

The configuration format is the same for all images. A short example is
worth one thousand words:

.. code-block:: yaml

   www:
     type: php
     config:
       smtp_server: mailer.ramen.dotcloud.com
       smtp_port: 1587
       smtp_username: dotcloud
       smtp_password: _|KZ&CKWa[

.. note::
   If you want to use your SendGrid/CritSend/MailGun/etc. account, you can do it
   there. Just put the right parameters at the right place.

.. include:: ../guides/config-change.inc

You can then use default mail functions in your code. If a programming
language or framework asks you a specific SMTP server address, use
"localhost".


Send Your Messages Directly Through Your SMTP Service
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If you don't want to go through the local MTA, you can use the parameters
retrieved with "dotcloud info". You will need to pass them to the mail()
function (or its equivalent) directly in your code.

It is probably easier to configure the local MTA to relay to your SMTP
service, since it means that you won't have to change your code between
different environments.

.. note::
   For your reference, the SMTP service only supports the CRAM-MD5
   authentication method. Any decent SMTP library will automatically detect and
   use this authentication method.


Troubleshooting
---------------

You can check if Postfix is running and display the status of its different
queues with::

  $ dotcloud status ramen.mailer

You can read Postfix's e-mails logs with::

  $ dotcloud logs ramen.mailer

Press Ctrl+C to stop watching the logs.

You can always restart Postfix with::

  $ dotcloud restart ramen.mailer


Receiving Mails
---------------

The SMTP service cannot receive messages coming from the outside. 
However, if you need to handle those, we suggest you have a look at 
excellent third party services like MailGun. Incoming mails can be:

* routed to a URL in your app;
* stored in a mailbox, which can be accessed using an API.

Don't hesitate to have a look at `MailGun documentation for receiving and 
parsing mail <http://documentation.mailgun.net/user_manual.html#receiving-messages>`_
to see how easy it is to handle incoming mails!
