WebSockets
==========

As of mid-March 2012, all applications deployed on dotCloud can receive connections
over `WebSockets <http://en.wikipedia.org/wiki/WebSocket>`_ as well as standard HTTP.

Examples
--------
We have these examples of websocket usage on the dotCloud PaaS:

* **Node.js** https://github.com/dotcloud/socket.io-on-dotcloud
* **Python** https://github.com/dotcloud/geventwebsocket-on-dotcloud

You're Already Enabled for WebSockets Support!
----------------------------------------------

All of our main gateways can proxy websockets because they are running `Hipache`_, and if you request it, 
your custom SSL edges can run Hipache as well.

If you are accessing your application through *something*.dotcloud.com, 
then you are going through our main gateways and so you can use websockets.

If you are using a :doc:`custom domain <domains>`, and you do **not** have custom SSL edges, 
then your CNAME points to *gateway.dotcloud.com*, our main gateways, and so you can use websockets.

**However**, if your CNAME points to custom edges used for your SSL certificates, 
then you must let the `support team <http://support.dotcloud.com>`_ know so that we can make sure
your edges are running `Hipache`_. By default we create custom edges using a version of Nginx which does not
support websockets, so we may need to upgrade your edges.

WebSocket over SSL
------------------

What if you want to use the ``wss://`` protocol? Again, if you are using your service URL
under the *.dotcloud.com* domain, you have nothing to do. It works out of the box.

If you are using a custom SSL certificate, you need to `contact our support
<http://support.dotcloud.com>`_, and we will upgrade your SSL load balancer to
the newer version using `Hipache`_.


Historical Notes
----------------

As of end of March, 2012, there are a few differences between the "old" load balancers 
(now only running on dedicated edges) and the "new" `Hipache`_ ones:

* the old load balancers are based on Nginx, while the new ones are based on Node.js;
* the old load balancers don't support the WebSocket protocol, while the new ones do;
* the old load balancers will automatically negotiate and enable gzip compression
  when possible, while the new ones won't do that for now;
* both old and new load balancers detect dead back-ends passively (i.e., if there is
  an error while processing a request, they will tag the relevant back-end as being dead);
* when detecting a dead back-end, the old load balancers repeat the request to the next
  live back-end (even if it's a POST request), while the new load balancers serve an
  error page (but subsequent requests won't hit the dead back-end since it has been
  flagged dead).

.. _Hipache: https://github.com/dotcloud/hipache
