Custom Nginx Configuration
--------------------------

You can specify custom rewrite rules (and actually almost any nginx
setting) by creating a file named *nginx.conf* at the top of your app.

This file will be included by nginx.

.. note::
   You should not include a whole nginx configuration: just the snippets
   that you need!

A typical example would be:

.. code-block:: nginx

   rewrite ^/search/(.*) /engine/?q=$1 ;

Check out the `Nginx wiki <http://wiki.nginx.org/Main>`_ to see all 
available options.

.. note::
   Already have some Apache mod_rewrite RewriteRule statements?
   All of them can be converted to nginx configuration.