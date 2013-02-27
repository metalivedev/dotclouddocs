:title: FAQ
:description: 
:keywords: 

F.A.Q.
======

.. include:: dotcloud2note.inc

Can I build my business on dotCloud?
------------------------------------

Yes. We want you to feel safe running your product on our
servers. We're building a world-class ops and support organization to
offer you the guarantees you need. If there's something missing that
would make you feel safer, please tell us!


What is your Support SLA?
-------------------------

+---------------+-----------------------------------+---------------------------------------+
| Support Level | Operations Support                | Development Support                   |
+===============+===================================+=======================================+
| Expedited     | * 24/7 application monitor        | * ticket response same business day   |
|               | * 24/7 paging                     | * live chat via tickets and IRC       |
|               | * <30 minute response             |                                       |
+---------------+-----------------------------------+---------------------------------------+
| Live          | * 24/7 container monitoring       | * ticket response < 2 business days   |
|               | * <1 hour response, business days | * live chat via tickets and IRC       |
+---------------+-----------------------------------+---------------------------------------+
| Sandbox       | * machines monitored for problems | * tickets responded to as time allows |
|               |                                   | * live chat via IRC as time allows    |
|               |                                   | * stackoverflow monitored by engineers|
|               |                                   | * comprehensive documentation         |
+---------------+-----------------------------------+---------------------------------------+


Where is dotCloud hosted?
-------------------------

dotCloud runs on Amazon EC2. More specifically, we run on the
us-east-1 region, across multiple availability zones.


Can I use multiple databases together?
--------------------------------------

Absolutely, you can combine as many components as you want in your
stack. dotCloud is designed specifically to support heterogenous
stacks.


Can I add or remove components later?
-------------------------------------

Yes! For example, you can add redis when you're ready for it. Or
you can stop using your static website when you find that 
you like to generate pages dynamically instead.


You don't support my favorite database/language!
------------------------------------------------

Are you sure? There are so many `recipes for custom services! <https://github.com/search?q=on+dotcloud>`_


Do I need to use git to use dotCloud?
-------------------------------------

No. We want to help all developers - and not all developers use git.


Can I "git push" my code to dotCloud?
-------------------------------------

Yes you can! Not with the "git push" command per se, but our CLI
is able to leverage git or hg if you use them.

See how dotCloud handles :doc:`guides/git-hg` for an in-depth explanation
of the push mechanism.


How can you be experts in all these software components?
--------------------------------------------------------

We are experts at operating and scaling heterogenous web
applications. There is a learning curve to properly configure,
fine-tune and scale each of these components. Our job is to tackle
that learning curve so you don't have to.

It's hard work for us. But 90% of the work is the same across all
deployments: there are only so many ways to compute, store and move bits
around. We take advantage of that fact to deliver massive savings in
engineering and sysadmin time.


Can I run dotCloud on a different cloud than EC2?
-------------------------------------------------

Not right now. However, this is on our roadmap. If you feel strongly
about it, let us know and it will happen sooner!


How do you handle scaling?
--------------------------

You may scale up any component in your stack by allocating more
service containers to it. Service containers are lightweight units
of parallel computation built on `Linux containers. <https://wiki.archlinux.org/index.php/Linux_Containers#Synopsis>`_

dotCloud configures new instances automatically, by applying the best
scaling strategy specific to each component. For example, an
application server will be automatically attached to our http load
balancers.

Sometimes different scaling strategies are necessary for different
usage profiles: for example MongoDB will benefit from additional
slaves in a read-intensive environment, but will need a more
sophisticated setup in write-intensive scenarios. We will offer a
choice of scaling strategy whenever it's relevant.


Can you magically scale a component not designed to scale?
----------------------------------------------------------

No. Different software components scale differently. Some can't be
scaled at all. Our job is to find the very best scaling strategy for
each component, and apply it automatically, so you don't have to.

If a part of your application is hard to scale, we recommend isolating
it from the rest of your stack. For example, a legacy library can be
wrapped in a background process, and communicate with the rest of your
application through a message queue.


How do you handle upgrades?
---------------------------

Each component has its own release cycle, managed by our in-house
expert. Upgrades are prudent, thoroughly tested, and favor stability.

If there is demand for it, we will also publish a bleeding edge version
as a separate component.


Can I use my own domain name with dotCloud?
----------------------------------------------

Sure! See the :doc:`custom domains <guides/domains>` documentation.


How can I use SSL with dotCloud?
--------------------------------

Each web application running on dotCloud is available over HTTPS as well.
If your application is available over http://<myapp-myuser>.dotcloud.com/,
it will also be available over https://<myapp-myuser>.dotcloud.com/.

Note, however, that if you want to have SSL on your own domain name,
you will need to purchase your own SSL certificate, and upgrade to
one of our paying offers. Why? Because we will need to dedicate a load
balancer instance to your application (since SSL requires at least one
IP address per domain, or more accurately, per certificate).


How can I setup a crontab?
--------------------------

You can do it manually (using ``dotcloud run myservice`` then ``crontab``), or automatically
through a postinstall script. This is documented in the :doc:`Periodic Tasks
guide <guides/periodic-tasks>`.
