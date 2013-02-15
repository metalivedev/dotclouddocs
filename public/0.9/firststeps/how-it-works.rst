:title: How it Works
:description: Gain a deeper understanding of how dotCloud works.
:keywords: how it works, dotCloud background, dotCloud code store, dotCloud stack builder, dotCloud service runtime

How it Works
============


Background
----------

Now that you have deployed a :doc:`simple app <quickstart>`, and a more
involved :doc:`app with a database <in-depth>`, we can review the
different steps happening behind a ``dotcloud push``. That should help
you gain a deeper understanding of how the dotCloud platform works, and
what's behind our “zero-downtime pushes” feature.

The Code Store
--------------

We started by adding a dotCloud Build File to our code and using the
client to push the code to dotCloud. When you use the client to push
code, the client will choose the best upload method for your app. For
example, if you have a Git or Mercurial repository, the upload will be
like a repository push. If you do not use a supported Version Control
System, the client will upload your code directly comparing your local
code with any previously uploaded code so that only changes are
uploaded.

.. note::

   You can learn more about the way the CLI chooses the best upload
   method and how to override it in the :doc:`corresponding guide </guides/git-hg>`.

Our internal name for the code store is just "uploader".

.. _firststeps_builder:

The Builder
-----------

Once new code has been uploaded, we start looking for your
:doc:`dotcloud.yml Build File </guides/build-file>`, and we deploy the
stack of services it describes on our dedicated build cluster. Each
service in your stack is built accordingly to a specific set of rules
(e.g: a Python service will run ``"pip"`` to install dependencies
whereas a NodeJS service will run ``"npm"``). In addition to these
predefined rules, you can setup :doc:`hooks </guides/hooks>` to be
executed before or after the build. Finally your application is packaged
and stored for the deployment phase. You can see the builder in action
in the ``"dotcloud push"`` output::

   […]
   ---> Building the application...
   [www] Build started for revision rsync-1339191773365 (clean build)
   [www] I am snapshotsworker_02/bob-2, and I will be your builder today.
   [www] Build completed successfully. Compiled image size is 427KB
   ---> Application build is done
   […]

The Deployer
------------

If your application built successfully, then the platform deploys your
stack on the Sandbox, Live or Enterprise cluster depending on its
:doc:`flavor </guides/flavors>`.

It starts by initializing a full new stack of service, while the current
one (if it's not your first push on this application) is still running
and serving traffic. When the new stack is initialized, the platform
retrieves the application package from the builder and installs it.

Finally the :ref:`postinstall hook <postinstall_hook>` is run on each
service and the new stack is launched. Once this new version of the
application is running and ready to accept requests, we seamlessly
switch the traffic to it (unless you use a :doc:`data directory
</guides/persistent-data>`, in that case a short downtime will happen
while we move it to the new service)::

   […]
   ---> Initializing new services... (This may take a few minutes)
   ---> Using default scaling for service www (1 instance(s)).
   [www.0] Initializing...
   [www.0] Service initialized
   ---> All services have been initialized. Deploying code...
   [www.0] Deploying build revision rsync-1339191773365...
   [www.0] Running postinstall script...
   [www.0] Launching...
   [www.0] Waiting for the instance to become responsive...
   [www.0] Re-routing traffic to the new build...
   [www.0] Successfully deployed build revision rsync-1339191773365
   ---> Deploy finished
   ---> Application fully deployed
   […]

The Stack Runtime
-----------------

During its lifetime, your application is continuously monitored, and
services automatically restarted when error conditions occur.

Each service is independent of the others; and, more importantly,
services never interact directly with the core platform, except
when you deploy (push) new code. That means that your services won't be
impacted when we have to perform maintenance operations on the dotCloud
API. Likewise, if you experience slow response times or errors with the
dotCloud CLI or website, your websites will not be affected since they
are decoupled from those components.

You should now have a much better idea of how the dotCloud platform works. At this
point, you may want to:

- learn more about the :doc:`dotcloud.yml Build File </guides/build-file>`;
- see some more complex examples in our :doc:`tutorials section </tutorials/index>`;
- `signup <https://www.dotcloud.com/register.html>`_? (if it's not
  already done!).
