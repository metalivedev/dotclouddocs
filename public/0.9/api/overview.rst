Overview
========

Temporary Link to API Top_

This page describes the overview of dotCloud REST API version 1.

URI
---

All API access should be made over HTTPS on ``rest.dotcloud.com``. All
request and response data should be encoded in JSON unless otherwise
specified in the corresponding HTTP headers.

API resources are versioned, and the current implementation is located
under the URI ``https://rest.dotcloud.com/v1/``.

Versioning and Deprecation
--------------------------

The dotCloud REST API is versioned, and when we make big
backward-incompatible changes to the whole API scheme, we'll introduce
a new version of the API and namespace (``/v2/`` for example).

We continously introduce API revisions without a notice, as long as
they are backward-compatible. All experimental new resources and
fields are marked as such in the documentation, or not documented at
all. If you see some unknown resources or fields that have no docs,
consider it as experimental. Experimental means we may remove or
change the behavior without a notice.

The current minor version (revision) of the REST API is available via
the ``X-dotCloud-API-Revision`` header.

.. note::

   dotCloud REST API is in a beta state. During the beta period,
   incompatible changes may be made regardless whether it was
   documented or experimental. In most cases, you will be notified in
   the beta developers list first, before we make such changes.

Limitations
-----------

Current version of the dotCloud API `does not` support pushing code to
the platform, nor does it provide remote access (SSH) to your
instances. We will add support to these capabilities to the platform
and the REST API in the upcoming releases.

Old application instances created with dotCloud CLI 0.3 or older are
not supported in the dotCloud REST API. These applications and
instances might appear in the listing APIs, but actual data requests
might fail in 500 errors. This is a known issue, and you are
encouraged to upgrade to the newer instance versions by destroying the
instances and recreating them using dotCloud CLI 0.4 or later.

Authentication
--------------

dotCloud REST API supports two authentication methods: Basic
Authentication and OAuth2.

Basic Authentication
~~~~~~~~~~~~~~~~~~~~

Basic Authentication is only for the simple use case where you want to
write an application or quick script to run against *your own*
dotCloud applications and services. ::

    $ curl -u "<key>:<secret>" https://rest.dotcloud.com/v1/me

In this curl example, ``<key>:<secret>`` means you use your *key* as
username and *secret* as password for the Basic authentication. Your
API key and secret are availble in your dotcloud.com/settings_
dashboard.

Your API key should contain ``:`` (the semicolon character) in itself,
and it can be treated as a separator of the key and secret field. For
instance, if your API key looks like: ::

    2W9O7TlDnDFxbOBAMbaM:e5f490a87b04e9587d90cdc69060078d413a2f68

Your REST API access key is ``2W9O7TlDnDFxbOBAMbaM`` and secret is
``e5f490a87b04e9587d90cdc69060078d413a2f68`` and could be used like: ::

    $ curl -u 2W9O7TlDnDFxbOBAMbaM:e5f490a87b04e9587d90cdc69060078d413a2f68 \
        https://rest.dotcloud.com/v1/me

Basic Authentication should be only used for accessing your own
application, and must not be used to obtain access to other users, in
which case OAuth2 should be used instead.

OAuth2
~~~~~~

dotCloud REST API also supports, and recommends the use of `OAuth 2.0 draft 16`_.

.. note::

   As of this writing, OAuth 2 support is considered alpha. The full
   support is coming in upcoming releases and the documentation will
   be updated accordingly.

OAuth2 tokens can be sent either using a query parameter or in a header. ::

     $ curl -H "Authorization: Bearer OAUTH2_TOKEN" https://rest.dotcloud.com/v1/me
     $ curl https://rest.dotcloud.com/v1/me\?access_token=OAUTH2_TOKEN

For more about OAuth2, see :ref:`api-oauth2`.

Rate Limiting
-------------

.. note::

   Rate Limiting is not enforced at this moment, but will be supported
   in the upcoming release before the API goes out of beta.

Usage of the dotCloud REST API is subject to rate limits. The limit is
keyed off either your request identity, your OAuth2 token, or request
IP. You can check the returned HTTP headers of any API request to see
the current rate limit status, by looking for
``X-RateLimit-Remaining`` and ``X-RateLimit-Limit`` headers. ::

    $ curl https://rest.dotcloud.com/v1/me
    HTTP/1.1 200 OK
    X-RateLimit-Limit: 1000
    X-RateLimit-Remaining: 922

You can contact `our support <https://www.dotcloud.com/about/contact/>`_ to
request unlimited access to dotCloud REST API for your application.
    
.. note::

   As of this writing, API version 1 *does not* implement Rate
   Limiting. It will be implemented in the following weeks, and until
   then you can use the Basic Authentication to test your API client
   applications without any rate limit.

HTTP Verbs
----------

Where possible, dotCloud REST API uses appropriate HTTP verbs for each
action to be truly RESTful.

HEAD
  Used for retrieving only HTTP header info from any resources.
GET
  Used for retrieving full object from resources.
POST
  Used for creating resources or performing custom actions.
PUT
  Used for replacing resources entirely.
PATCH
  Used for updating resources with partial data.
DELETE
  Used for deleting resources.

For user agents or HTTP client libraries that do not support issuing
custom HTTP verbs other than GET or POST, or has no ability to set
custom headers, we support:

* Custom HTTP header to denote the verb ``X-Method-Override: PUT``
* Special query parameter in the URI: `_method=PUT`

Client Errors
-------------

dotCloud REST API returns most errors using appropriate HTTP Status code whenever possible.

* Sending invalid JSON will result in a ``400 Bad Request`` response.
* Invalid authentication headers and tokens will result in a ``401 Unauthorized`` response.
* When the request resource cannot be viewed by the authenticated user because of permission errors, it will result in a ``403 Forbidden`` response.
* When the request resource or endpoint does not exist, ``404 Not Found`` response will be returned.
* Attempting to use POST to a GET-only resource or vice-verca will result in a ``405 Method Not Allowed`` response.
* Trying to create a new resource using POST that conflicts with existing resources will result in a ``409 Conflict`` response.
* Sending or requesting unsupported MIME types (i.e. other than ``application/json``) will result in a ``415 Unsupported Media Type`` response.
* Sending invalid data (in a valid JSON encoding) will result in a ``422 Unprocessable Entity`` response.

Schema
------

All responses are encoded in JSON unless otherwise requested with
``Accept`` headers, and will have a common structure that looks like
this:

.. code-block:: javascript

     {
       "request": {
         "remote_addr": "255.255.255.xxx",
         "auth_user": {
           "type": "User",
           ...
         },
         "uri": "https://rest.dotcloud.com/v1/me"
       },
       "links": [
         {
           "rel": "self",
           "href": "https://rest.dotcloud.com/v1/me",
         },
         ...
       ],
       "object": { ... }
     }

The ``request`` attribute always contains the meta information about the
request that is made, such as originating IP address and authenticated
user information if available.

The ``links`` attribute contains a list of URI related to the resource you
requested. It will always contain ``self`` link, which is the
canonical form of the request you just made. These link URIs are also
available in the HTTP response header as well. ::

    Link: <https://rest.dotcloud.com/v1/me>; rel="self", ...

If the request is made against a specific resource, it will contain
the ``object`` attribute that contains the information about the requested
resource. If the request is made against a list endpoint that contains
one or more resources, the response will contain ``objects`` attribute,
which is an array containing multiple objects in it.

Object usually contains common attributes, such as ``type``, ``id`` and
``uri`` if they are resources accessible from the REST API. For
example, a user resource you receive from the ``/v1/me`` URI would
look like:

.. code-block:: javascript

     {
       "type": "User",
       "id": "tag:dotcloud.com,2011,User/john123",
       "uri": "https://rest.dotcloud.com/v1/users/john123",
       ...
     }

where ``id`` represents the `Tag URI`_ of the object, and ``uri``
gives the full URI to retrieve more information about, or perform some
actions on the particular user.

The combination of ``uri`` field in such objects and links (either in
the ``Links`` HTTP header or ``links`` attribute) would allow API
clients to fetch and perform actions on sub resources without
hard-coding the specific URI path patterns in the client library code.

Cross Origin Resource Sharing
-----------------------------

dotCloud REST API supports Cross Origin Resource Sharing (`CORS`_) for
full AJAX websites. Any domain that is registered as an OAuth
application is accepted as a ``Origin`` header value, which is sent
automatically from web browser that supports CORS. ::

     $ curl -i https://rest.dotcloud.com/v1/me\?access_token=... -H "Origin: http://example.com"
     HTTP/1.1 200 OK
     Access-Control-Allow-Origin: *
     Access-Control-Expose-Headers: Link, X-RateLimit-Limit, X-RateLimit-Remaining ...
     Access-Control-Allow-Credentials: true
     Access-Control-Max-Age: 86400

The API also support CORS preflight request using ``OPTIONS`` verb.

Changelog
---------
2012.08.21
  Updated URLs to /v1/. Minor grammatical changes and link additions.
2011.10.05
  Initial revision


.. _OAuth 2.0 draft 16: http://tools.ietf.org/html/draft-ietf-oauth-v2-16
.. _CORS: http://www.w3.org/TR/cors
.. _Tag URI: http://taguri.org/
.. _settings: http://www.dotcloud.com/settings
.. _Top: /0.9/api
