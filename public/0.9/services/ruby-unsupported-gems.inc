Unsupported gems and how to exclude them
----------------------------------------

Some Ruby gems are not suited for environments like dotCloud, and should
not be installed. Those are gems like:

- linecache19 (which is only used for debugging purposes, and should not
  be present on a production environment);
- SystemTimer.

Actually, if you try to install them anyway, you will generally get
a "permission denied" without much consequences.

To exclude those gems without juggling with multiple *Gemfiles*, you
can put them in groups, and then tell bundler to exclude them when 
deploying on dotCloud.

You will need to edit your Build File. Assuming that your Ruby service is
named "www", modify its section like the following example:

.. code-block:: yaml

  www:
    type: ruby
    exclude_bundler_groups:
      - development

Then, in your Gemfile, regroup the unwanted gems in a group:

.. code-block:: ruby

   group :development do
      gem "ruby-debug19"
      gem "my-funky-gem"
   end

If you want to learn more about it, feel free to read:

* `The How and Why of Bundler Groups 
  <http://yehudakatz.com/2010/05/09/the-how-and-why-of-bundler-groups/>`_,
  an insightful blog post by Yehuda Katz;
* the official `Gemfile specification 
  <http://gembundler.com/man/gemfile.5.html>`_.

.. vim: ft=rst:
