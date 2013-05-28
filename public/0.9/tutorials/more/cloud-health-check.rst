:title: CloudHealthCheck
:description: How to handle the /CloudHealthCheck
:keywords: dotCloud, tutorial, documentation, CloudHealthCheck, Health, Check

/CloudHealthCheck
=================

So...You're seeing the dotCloud active health check looking to make sure that
your service is up. There is no way for you to disable it, but you can prevent
it and you can handle it!

You can prevent this probe by making sure your app doesn't return any 5xx
errors unless there really is a problem. As soon as our router software,
`hipache <https://github.com/dotcloud/hipache>`_ sees a 5xx error, it will ask
the `active health check
<https://github.com/dotcloud/hipache#active-health-check>`_ to double-check
the health of the application. That causes us to do the `HEAD` on
`/CloudHealthCheck`. If we get anything back within 3 seconds AND it is not a
5xx error, then we consider the service to be up and we keep it in the
load-balanced rotation. If there is no response or we get a 5xx then we take
that service out of rotation. In either case, we start checking
`/CloudHealthCheck` once every 3 seconds to see if there is a problem.
Since there are multiple dotCloud gateways each doing the check, you can see
this check more than once per second. We'll stop the probes after 30 minutes
of no 5xx's and no delayed responses.

There are several ways to handle this probe:

    1. Do nothing. A 404 is not a problem and will not remove your service from
       the load-balanced rotation.

    2. Modify your nginx to immediately return a 200.

    3. Add a small file or handler to your app at /CloudHealthCheck

    4. Actually do more tests, like check your database connectivity, and
       return an appropriate response within 3 seconds. Remember, we only start
       checking /CloudHealthCheck if we already saw a 5xx error, so maybe there
       really is a problem
