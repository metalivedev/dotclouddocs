.. _api-oauth2:

OAuth2
======

Temporary Link to API Top_

.. note::

   As of this writing, OAuth 2 support in the REST API is considered
   alpha.  The full support is coming in a few weeks and the
   documentation will be updated accordingly.

dotCloud API implements `OAuth 2.0 draft 16`_ to authenticate and
authorize third party applications.

OAuth 2 is a preferred authentication protocol over HTTP Basic
authentication because users do not need to supply username and
password to third party applications, and authorized tokens can be
limited to specific scope of data access, and can be revoked by users
at any time.

Registering Applications
------------------------

All developers who want to make use of OAuth 2 have to register their
application `through the dotCloud website <https://www.dotcloud.com/settings/oauth2/clients>`_.

A registered OAuth application is assigned a unique Client ID and
Client Secret. The secret, as the name implies, should not be shared
with others.

Obtaining Access Token
----------------------

Web Application Flow
~~~~~~~~~~~~~~~~~~~~

First, your web application redirects users to request dotCloud access::

    GET https://www.dotcloud.com/oauth2/authorize?
          client_id=YOUR_CLIENT_ID&
          redirect_uri=CALLBACK_URL&
          response_type=code

Parameters
^^^^^^^^^^

client_id:
  Required - The client ID of your application.
redirect_uri:
  Optional - URL in your application where users will be sent after authorization. The redirect URI must match with the URI you registered through the application settings page.
response_type:
  Required - must be ``code`` for Web application flow

Once user is signed in and authorizes the application, dotCloud redirects users back to your site at ``redirect_uri`` you specified with a temporary code in a ``code`` parameter.

If your `redirect_uri` was ``https://example.com/callback?id=123``,
the user will be redirected to::

    https://example.com/callback?id=123&code=CODE

your application then exchanges the code for an access token::

    POST https://www.dotcloud.com/oauth2/token

    client_id=YOUR_CLIENT_ID&
    client_secret=YOUR_CLIENT_SECRET&
    redirect_uri=CALLBACK_URI&
    code=CODE

Parameters
^^^^^^^^^^
    
client_id:
  Required - The client ID of your application.
client_secret:
  Required - The client secret of your application.
redirect_uri:
  Optional - The callback URL the authentication you just used in the original request.
code:
  Required - The code parameter your callback URI received.
scope:
  Optional - Specify the scope of the token. (See below)

You will get an access token in the response, encoded in JSON.

.. code-block:: javascript

     {"access_token" : "12345678xyz",
      "token_type" : "bearer",
      "expire_in" : 3600,
      "refresh_token" : "987654321abc",
      "scope" : ""}

Your application will save the ``access_token`` and ``refresh_token``
and use the token to make requests, and refresh the token once it is
expired.  See draft 16 Section 6 "Refreshing an Access Token" for
details.

AJAX and desktop Application Flow
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

OAuth 2 endpoint implements all the authentication flows specified in
the OAuth 2 draft 16. You can make use of `4.2 Implicit Grant`` and
``4.3 Resource Owner Password Credentials`` to request OAuth token in
the case of pure AJAX applications (i.e. can not share client secret
in the source code) or non-desktop clients such as desktop
applications with no web browsers.

Scopes
------

As of this writing, dotCloud API version does not implement scopes and
all the authorization requests are assumed that it requests all (read
and write) permissions and scopes as the authenticated user.

In the futue versions of the API, we will allow OAuth authorization
step to request a limited scope and permissions.

Making Requests
---------------

The access token allows you to make requests to the dotCloud API on
behalf of a user. You can use either query parameter or HTTP headers
(using Bearer encoding).

::

    GET /v1/me?access_token=ACCESS_TOKEN HTTP/1.1
    Host: rest.dotcloud.com

Or::

    GET /v1/me HTTP/1.1
    Host: rest.dotcloud.com
    Authorization: Bearer ACCESS_TOKEN

.. _OAuth 2.0 draft 16: http://tools.ietf.org/html/draft-ietf-oauth-v2-16
.. _Top: /0.9/api