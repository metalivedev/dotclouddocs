:title: Sending Emails
:description: dotCloud services are able to send E-mails out of the box, but we recommend using a third-party mail relay to avoid being flagged as spam.
:keywords: dotCloud documentation, quick start guide, send emails

Sending E-Mails
===============

DotCloud services are able to send E-mails out of the box. However, there
is a high chance that the mail will never be delivered correctly due to
anti-spam policies for mail coming from the "cloud".

The best way to send mail is to use a third party service that will relay messages
for you and ensure they are not viewed as spam. DotCloud uses (and recommends!)
`MailGun <http://mailgun.net/>`_ for this purpose, but other options exist: the
simplest is to use your GMail account, but the messages will look like they are
sent from your GMail address. Other providers include
`SendGrid <http://sendgrid.com/>`_ or `CritSend <http://www.critsend.com/>`_.

The relay is configured from your dotcloud.yml, here is an example with MailGun:

.. code-block:: yaml

   www:
     type: php
     config:
       smtp_server: smtp.mailgun.org
       smtp_port: 587
       smtp_username: postmaster@company.com
       smtp_password: YourMailGunPassword

.. include:: config-change.inc

You are also advised to use an internal DotCloud mail relay that will spool
mails from your instances and forward them to the third party of your choice as
explained in the :doc:`SMTP service documentation </services/smtp>`.
