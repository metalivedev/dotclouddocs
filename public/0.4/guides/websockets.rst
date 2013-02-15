WebSockets
==========

As of mid-March 2012, all applications deployed on dotCloud can receive connections
over `WebSockets <http://en.wikipedia.org/wiki/WebSocket>`_ as well as standard HTTP.

There are a few details that you might want to know about it!


Enable WebSockets Support
-------------------------

If you are accessing your application through *something*.dotcloud.com, there is
nothing to do. HTTP requests to your application are already handled by our
WebSocket-aware HTTP load balancers.

If you are using a :doc:`custom domain <domains>`, the ``dotcloud alias`` command
instructed you to setup a CNAME to *gateway.dotcloud.com*. You need to update
that CNAME to *ws.dotcloud.com* instead.

.. note::
   The WebSocket-aware HTTP loadbalancers will soon serve traffic for *gateway.dotcloud.com*
   as well. But during a transition period, we want to make sure that everyone has the
   opportunity to test the behavior of his app with those new load-balancers, while
   having the option to switch back to the old load balancer if any incompatibility
   arises.


WebSocket over SSL
------------------

What if you want to use the ``wss://`` protocol? Again, if you are using your vanity
alias under the *.dotcloud.com* domain, you have nothing to do. It works out of the
box.

If you are using a custom SSL certificate, you need to `contact our support
<http://support.dotcloud.com>`_, and we will upgrade your SSL load balancer to
the newer version.


Technical Notes
---------------

As of end of March, 2012, there are a few differences between the "old" load balancers
and the "new" ones:

* the old load balancers are based on Nginx, while the new ones are based on Node.js;
* the old load balancers don't support the WebSocket protocol, while the new ones do;
* the old load balancers will automatically negociate and enable gzip compression
  when possible, while the new ones won't do that for now;
* both old and new load balancers detect dead back-ends passively (i.e., if there is
  an error while processing a request, they will tag the relevant back-end as being dead);
* when detecting a dead back-end, the old load balancers repeat the request to the next
  live back-end (even if it's a POST request), while the new load balancers serve an
  error page (but subsequent requests won't hit the dead back-end since it has been
  flagged dead).
