API endpoints
=============

Temporary Link to API Top_

GET /me
~~~~~~~

Shortcut URL to access ``/users/<authenticated-username>``.

``/me/`` is a shortcut URL path to request the same resource as
``/users/<username>`` where *username* is a username of the
authenticated user of the request. You can also combine the sub
resource path under ``/me``, such as ``/me/applications`` ::

    GET /v1/me?access_token=... HTTP/1.1
    Host: rest.dotcloud.com

    HTTP/1.1 200 OK
    Content-Type: application/json
    Link: <https://rest.dotcloud.com/v1/me>; rel="self",
          <https://rest.dotcloud.com/v1/users/john123>; rel="canonical"

.. code-block:: javascript

    {
      "request" : {
         "remote_addr": ...
      },
      "object" : {
        "type" : "User",
        "id" : "tag:dotcloud.com,2011,User/john123",
        "uri" : "https://rest.dotcloud.com/v1/users/john123",
        "links" : [
          {
            "rel" : "https://rest.dotcloud.com/1#applications",
            "href" : "https://rest.dotcloud.com/v1/users/miyagawa1/applications",
            "title" : "applications"
          }
        ],
        "last_name" : "Doe",
        "first_name" : "John",
        "username" : "john123"
      },
      "links" : [
        {
          "rel" : "self",
          "href" : "https://rest.dotcloud.com/v1/me"
        },
        {
          "rel" : "canonical",
          "href" : "https://rest.dotcloud.com/v1/users/john123"
        },
        {
          "rel" : "https://rest.dotcloud.com/1#applications",
          "href" : "https://rest.dotcloud.com/v1/users/miyagawa1/applications",
        }
      ]
    }

Canonical URL are available inside each object's links as well as
``rel="canonical"`` links in the response container as well as
in the HTTP header.

GET /users/<username>
~~~~~~~~~~~~~~~~~~~~~
Get user information

This endpoint gets a user information about specific user on
dotCloud. Right now, you *cannot* retrieve arbitrary user
information except yourself, and requesting such data would result
in 403 or 404 responses. This applies to all the sub resources
under the user resource as well.::

    GET /v1/users/john123?access_token=... HTTP/1.1
    Host: rest.dotcloud.com

    HTTP/1.1 200 OK
    Content-Type: application/json
    Link: <https://rest.dotcloud.com/v1/users/john123>; rel="self",
          <https://rest.dotcloud.com/v1/users/john123>; rel="canonical"

.. code-block:: javascript

    {
      "request" : {...}
      "object" : {
        "type" : "User",
        "id" : "tag:dotcloud.com,2011,User/john123",
        "uri" : "https://rest.dotcloud.com/v1/users/john123",
        "links" : [
          {
            "rel" : "https://rest.dotcloud.com/1#applications",
            "href" : "https://rest.dotcloud.com/v1/users/miyagawa1/applications",
            "title" : "applications"
          }
        ],
        "last_name" : "Doe",
        "first_name" : "John",
        "username" : "john123"
      },
      "links" : [...]
    }

GET /users/<username>/activity
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*no docs*

GET /users/<username>/applications
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
List user's applications

This endpoint gives the list of user's applications on dotCloud.::

    GET /users/john123/applications?access_token=... HTTP/1.1
    Host: rest.dotcloud.com

    HTTP/1.1 200 OK
    Content-Type: application/json
    Link: <https://rest.dotcloud.com/v1/users/john123/applications>; rel="self",
          <https://rest.dotcloud.com/v1/users/john123/applications>; rel="canonical"

.. code-block:: javascript

    {
      "request" : {...},
      "objects" : [
        {
          "name" : "ramen",
          "user" : {...},
          "id" : "tag:dotcloud.com,2011,Application/john123/ramen",
          "type" : "Application",
          "uri" : "https://rest.dotcloud.com/v1/users/john123/applications/ramen",
          "links" : [
            {
              "rel" : "https://rest.dotcloud.com/1#services",
              "href" : "https://rest.dotcloud.com/v1/users/john123/applications/ramen/services",
              "title" : "services"
            },
            {
              "rel" : "https://rest.dotcloud.com/1#revision",
              "href" : "https://rest.dotcloud.com/v1/users/john123/applications/ramen/revision",
              "title" : "revision"
            },
            {
               "rel" : "https://rest.dotcloud.com/1#versions",
               "href" : "https://rest.dotcloud.com/v1/users/john123/applications/ramen/versions",
               "title" : "versions"
             },
             {
               "rel" : "https://rest.dotcloud.com/1#push-url",
               "href" : "https://rest.dotcloud.com/v1/users/john123/applications/ramen/push-url",
               "title" : "push-url"
             }
           ],
         }
       ],
       "links" : [...]
     }

POST /users/<username>/applications
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Create a new application

Sending a ``POST`` request with a JSON payload with ``name`` parameter will create a new application.::

    POST /v1/users/john123/applications?access_token=... HTTP/1.1
    Host: rest.dotcloud.com
    Content-Type: application/json

    { "name" : "myapp", "flavor": "sandbox" }

    HTTP/1.1 201 Created
    Content-Type: application/json
    Location: https://rest.dotcloud.com/v1/users/john123/applicaitons/myapp

.. code-block:: javascript

    {
      "request" : { ... },
      "object" : {
        "type" : "Application",
        "id" : "dotcloud.com,2011,Application/john123/myapp",
        ...
      },
      "links": [ ... ],
    }

Status code ``201`` indicates that your request has successfully
been accepted and a new applicaiton is created. The ``Location``
HTTP response header indicates the URI for the applicaiton you
just created, and the resource is also available in the HTTP
response body in the ``object`` attribute.

Sending an invalid JSON data will result in a ``422 Unprocessable
Entity`` error.::

    POST /v1/users/john123/applications?access_token=... HTTP/1.1
    Host: rest.dotcloud.com
    Content-Type: application/json

    { "name" : "ramen@123" }

    HTTP/1.1 422 Unprocessable Entity
    Content-Type: application/json

.. code-block:: javascript

    {
      "request" : { ... },
      "error" : {
        "title" : "Unprocessable Entity",
        "code" : 422,
        "description" : "Application name must contain only lower-case alphanumerical charaters ..."
      },
      ...
    }

GET /users/<username>/applications/<appname>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Get data about a specific app.

Like listing user's applications, but retrieves an information
about specific applications.::

    GET /v1/users/john123/applications/myapp?access_token=... HTTP/1.1
    Host: rest.dotcloud.com

    HTTP/1 200 OK
    Content-Type: application/json

.. code-block:: javascript

    {
      "request" : {...},
      "object" : {
        "name" : "ramen",
        "user" : {...},
        "id" : "tag:dotcloud.com,2011,Application/john123/ramen",
        "type" : "Application",
        "uri" : "https://rest.dotcloud.com/v1/users/john123/applications/ramen",
        "links" : [
          {
            "rel" : "https://rest.dotcloud.com/1#versions",
            "href" : "https://rest.dotcloud.com/v1/users/john123/applications/ramen/versions",
            "title" : "versions"
          },
          ...
        ],
      },
      "links" : [...]
    }


DELETE /users/<username>/applications/<appname>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Destroy an application.

Sending a ``DELETE`` request to this endpoint will destroy the
application and all of its services.::

    DELETE /users/john123/applications/ramen?access_token=... HTTP/1.1
    Host: rest.dotcloud.com

    HTTP/1.1 204 No Content

A successful ``DELETE`` request will result in a ``204 No Content`` response.

GET /users/<username>/applications/<appname>/activity
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*no docs*

GET /users/<username>/applications/<appname>/environment
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*no docs*

PATCH /users/<username>/applications/<appname>/environment
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*no docs*

GET /users/<username>/applications/<appname>/push-endpoints
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Return a list of supported protocols for pushing code

Two mutually exclusive arguments are available (for dvcs):
    - commit: hash of the commit to create a version from.
    - branch: branch to use (latest commit)

GET /users/<username>/applications/<appname>/revision
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Get the currently deployed revision of the application

This endpoint returns the current revision that the application
is running on. The revision is returned as a string such
as ``git-0123456``.::

    GET /v1/users/john123/applications/ramen/revision?access_token=... HTTP/1.1
    Host: rest.dotcloud.com

    HTTP/1.1 200 OK
    Content-Type: application/json

.. code-block:: javascript

    {
      "request" : { ... },
      "object" : {
        "revision" : "git-4f2a68"
      },
      "links" : [ ... ]
    }

Note that the revision is schema-less and has no ``type`` or ``id``.

GET /users/<username>/applications/<appname>/revisions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
List all the available revisions of an application

GET /users/<username>/applications/<appname>/revisions/<revision>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Retrieve informations about a revision

Accept symbolic names like 'latest' and 'previous'

GET /users/<username>/applications/<appname>/services
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
List services in an application

services endpoint returns the list of services of the application.::

    GET /v1/users/john123/applications/ramen/services?access_token=... HTTP/1.1
    Host: rest.dotcloud.com

    HTTP/1.1 200 OK
    Content-Type: application/json

.. code-block:: javascript

    {
      "request" : { ... },
      "objects" : [
        {
          "service_type" : "perl",
          "name" : "www",
          "uri" : "https://rest.dotcloud.com/v1/users/john123/applications/ramen/services/www",
          "application" : { ... },
          "instances" : [
            {
              "service_type" : "perl",
              "cluster" : "dev",
              "container_id" : 0,
              "booted" : true,
              "version" : 1,
              "image_version" : "87ce0731fd95 (latest)",
              "build_config" : {
                "environment" : {},
                "postinstall" : "",
                "instances" : 1,
                "approot" : "www",
                "type" : "perl",
                "config" : {},
                "requirements" : []
              },
              "state" : "running",
              "created" : "2012-04-21T01:22:51.109097Z",
              "revision" : "git-4f2ba68",
              "ports" : [
                {
                  "name" : "ssh",
                  "url" : "ssh://dotcloud@ramen-john123.dotcloud.com:1031"
                },
                {
                  "name" : "http",
                  "url" : "http://ramen-john123.dotcloud.com/"
                },
              ],
              "service_name" : "www",
              "config" : {
                "uwsgi_processes" : 4,
                "static" : "static",
                "plack_env" : "deployment",
                "path" : "/"
              },
              "type" : "Container",
              "id" : "tag:dotcloud.com,2011,Container/john123/ramen/www/0"
            },
          ],
          "domains" : [
            {
              "id" : "tag:dotcloud.com,2011,Domain/john123/ramen/www/ramen-john123.dotcloud.com",
              "type" : "Domain",
              "domain" : "ramen-john123.dotcloud.com",
              "uri" : "https://rest.dotcloud.com/v1/users/john123/applications/ramen/services/www/domains/ramen-john123.dotcloud.com"
            },
          ],
          "type" : "Service",
          "id" : "tag:dotcloud.com,2011,Service/john123/ramen/www",
          "links" : [
            {
              "rel" : "https://rest.dotcloud.com/1#domains",
              "href" : "https://rest.dotcloud.com/v1/users/john123/applications/ramen/services/db/domains",
              "title" : "domains"
            }
          ]
        }
      ],
      "links" : [ ... ]
    }

GET /users/<username>/applications/<appname>/services/<svcname>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Get a service information

This endpoint gives a detailed information about one specific
service of the application. Same as an element in the list
response.

DELETE /users/<username>/applications/<appname>/services/<svcname>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Destroying a service

Sending a ``DELETE`` request to the service endpoint destroys the
specific service of the application..::

    DELETE /v1/users/john123/applications/ramen/services/www?access_token=... HTTP/1.1
    Host: rest.dotcloud.com

    HTTP/1.1 204 No Content

A successful destroy request will result in a ``204 No Content`` response.

PATCH /users/<username>/applications/<appname>/services/<svcname>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*no docs*

GET /users/<username>/applications/<appname>/services/<svcname>/domains
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Getting the list of domains

domains endpoint returns the list of attached domains to the service.::

    GET /v1/users/john123/applications/ramen/services/www/domains?access_token=... HTTP/1.1
    Host: rest.dotcloud.com

.. code-block:: javascript

    {
      "request" : { ... },
      "objects" : [
        {
          "id" : "tag:dotcloud.com,2011,Domain/john123/ramen/www/ramen-john123.dotcloud.com",
          "type" : "Domain",
          "domain" : "ramen-john123.dotcloud.com",
          "uri" : "http://dotcloud-admin:8081/v1/users/john123/applications/ramen/services/www/domains/ramen-john123.dotcloud.com"
        },
        {
          "id" : "tag:dotcloud.com,2011,Domain/john123/ramen/www/myapp.example.com",
          "type" : "Domain",
          "domain" : "myapp.examlpe.com",
          "uri" : "http://dotcloud-admin:8081/v1/users/john123/applications/ramen/services/www/domains/myapp.example.com",
        }
      ],
      "links" : [ ... ]
    }

Domain aliases include the domains dotCloud automatically assigned.

POST /users/<username>/applications/<appname>/services/<svcname>/domains
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Attach a new domain

Sending a ``POST`` request to the domains endpoint will attach a
new domain to the service.::

    POST /v1/users/john123/applications/ramen/services/www/domains?access_token=... HTTP/1.1
    Host: rest.dotcloud.com
    Content-Type: application/json

    { "domain" : "myapp.example.com" }

    HTTP/1.1 201 Created
    Location: https://rest.dotcloud.com/v1/users/john123/applications/ramen/services/www/domains/myapp.example.com

    { ... }

You have to specify a valid domain in the ``domain`` attribute of
the request body. If the specified domain is invalid, you will get
``422 Unprocessable Entity`` error response.

GET /users/<username>/applications/<appname>/services/<svcname>/domains/<domain>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Get information about a domain.

Same as the element in the list response.

DELETE /users/<username>/applications/<appname>/services/<svcname>/domains/<domain>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Detach a domain

Sending a ``DELETE`` request to a domain URI will remove the domain from the service.::

    DELETE /v1/users/john123/applications/ramen/services/www/domains/myapp.example.com?access_token=... HTTP/1.1
    Host: rest.dotcloud.com

    HTTP/1.1 204 No Content

A successful remove request will result in a ``204 No Content`` response.

GET /users/<username>/applications/<appname>/services/<svcname>/instances/<instance_id>/status
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*no docs*

PUT /users/<username>/applications/<appname>/services/<svcname>/instances/<instance_id>/status
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*no docs*

GET /users/<username>/authorized_keys
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*no docs*

GET /users/<username>/private_keys
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*no docs*


.. _Top: /0.9/api

