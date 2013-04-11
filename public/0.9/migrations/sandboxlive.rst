Sandbox to Live
===============
On April 25th, 2013, we will sunset our free *Sandbox* flavor and
focus more on open-sourcing more of the dotCloud code to make
containers and PaaS something that everyone can do. You can read more
about it on our `blog <http://blog.dotcloud.com/new-sandbox>`_.

This document will help gather frequently asked questions and
techniques for preserving your application.

You must first choose between:

1. **Go Live**. You can either create a new *Live* application immediately and push your code there, or you can file a request to have the support team migrate your application to *Live* with no further work required by you.
2. **Download**. Back up your application and data so that you can host elsewhere.
3. **Do nothing**. Your *Sandbox* applications will be deleted on April 25th.

Go Live
-------
You must have a credit card on file to push a *Live* application. You
can add your credit card information in the `billing
<https://dashboard.dotcloud.com/settings/billing>`_ section of the
dotCloud dashboard.

Unlike *Sandbox* applications, you can (and usually must) ``dotcloud
scale`` to reserve extra RAM for your services. If your *Sandbox* service
has been running for more than 24 hours, you can ask the dotCloud
Developer Support team to give you an estimate of the proper scaling.

If you don't give your services enough RAM then they may not run at
all, or they may be restarted repeatedly by kernel when they try to
allocate too much memory.

Upgrade
"""""""
If you choose to have the dotCloud Developer Team support upgrade your
*Sandbox* application to a *Live* application, then you should:

1. `Create a ticket <http://support.dotcloud.com>`_ using the same email address as your dotCloud account. If you can't do that you may be asked some additional questions to confirm your right to control the application and billing.
2. Give the full URL for each application you want to upgrade.
3. That's it. We'll do the rest, usually within 1-2 business days.

Self-Push
"""""""""
If your application doesn't contain any databases, then the fastest
way to go *Live* is to ``dotcloud create -f live myliveapp`` and push
your code to the new application. Done.

**In both cases, there is no real work for you to do, so upgrading to Live should be effortless.**


Download
--------
If you don't want to go *Live* and you'd like to preserve your
application, you can download your code and data. You may not need to
do this if you were already backing up your database and pushing your
code -- that would mean you already have a copy of everything.

But if you need to back up your database(s), we have some a :doc:`handy
script </guides/backups>` that will help. Once you've created your
backup, you can transfer the file several different ways, including
via ``scp``. The technique is outlined in our :doc:`Platform Guides
</guides/copy/>` 
and there is `a nice little utility to help create the SSH configuration files
<https://github.com/metalivedev/dcdumper>`_.

**Downloading code and data can be a lot of work.** 
If you need more time, you should have the support team upgrade you to *Live* until you have all of your data.

Your *Sandbox* applications will become unreachable by HTTP on April
22, but the code and data will be accessible via ``ssh`` and ``scp``
until April 25th.

or Do Nothing
-------------
If your *Sandbox* application has no value for you, there is nothing
for you to do. We'll delete it on April 25th.
