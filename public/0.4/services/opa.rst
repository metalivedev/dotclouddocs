:title: Opa Service
:description: dotCloud's Opa service lets you build and run applications written in the Opa language.
:keywords: dotCloud documentation, dotCloud service, Opa, chat application, layout Opa service, caveats

Opa
===

.. warning::

   Since the last version of OPA, the current OPA service is deprecated and will
   be removed soon. Meanwhile you're invited to use the following custom service
   which support the last version of the OPA language.

   https://github.com/dotcloud/opa-on-dotcloud

To use it, open a terminal and run the following commands:

.. code-block:: none

   $ git clone git://github.com/dotcloud/opa-on-dotcloud.git
   $ dotcloud create myapp
   $ dotcloud push myapp opa-on-dotcloud

The cloned repository contains an application example called "app.opa" which you
can replace by your own OPA code.
