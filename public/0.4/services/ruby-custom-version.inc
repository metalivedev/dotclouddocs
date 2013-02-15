Use a specific version of Ruby
------------------------------

If you don't like the default version of Ruby used by your *ruby* or
*ruby-worker* service, you can switch to another one. Here is how.

Edit your dotcloud.yml file to include a "ruby-version" parameter in the
config section:

.. code-block:: yaml

   hello:
     type: ruby
     config:
       ruby-version: X

*X* can be one of the following:

* ree -- for Ruby Enterprise Edition (1.8 series);
* 1.9.2 -- for "plain" Ruby 1.9.2 (the default);
* 1.9.3 -- for "plain" Ruby 1.9.3.

.. note::

   If you ask for a non-existent version, the deployment will fail.
   So please don't try it!

   However, if you specify a non-existent parameter (for example, if you
   use ruby_version instead of ruby-version), the parameter will be
   silently ignored, for back-and-forth compatibility reasons.

.. warning::

   Like all the "config" parameters, the ruby-version cannot be changed
   on a live service. You have to destroy the service and push it again
   with a new configuration in order to change ruby-version.
