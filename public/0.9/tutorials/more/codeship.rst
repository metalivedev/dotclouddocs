:title: Codeship CI
:description: How to use Codeship Continuous Integration with dotCloud
:keywords: dotCloud, tutorial, documentation, Codeship, integration, deployment

Codeship Continuous Integraion
==============================

.. note::

    This tutorial is a condensed version of `this blog post
    <http://blog.dotcloud.com/set-up-continuous-deployment-to-dotcloud-in-5-minutes-with-codeship>`_

`Codeship <https://www.codeship.io>`_ is a hosted continuous
integration (CI) platform that lets you test and deploy your GitHub
and Bitbucket projects to several hosting providers, including the
dotCloud PaaS. Each time you push code into your project, Codeship
will pull the new code and test it with the tests you've defined. If
your tests fail, Codeship will not deploy your application to
dotCloud. But when your tests pass, that means you have a seamless
path from project to deployment, including testing!

Codeship has provided a great, step by step set of videos for
deploying to the dotCloud PaaS:

* `Django app from GitHub to dotCloud <http://vimeo.com/79399311>`_
* `Django app from BitBucket to dotCloud <http://vimeo.com/79892921>`_

All you need to make Codeship work with your dotCloud application is
your API token, available on https://account.dotcloud.com/settings.

Take a look at the video, and you'll be able to get your project
testing and deploying in ten minutes or less.

The general steps are:

0. Set up your dotCloud application. Make sure it is working here.
1. Set up a Codeship account.
2. Link your repository to Codeship so that Codeship gets
   notifications of your code updates.
3. Give Codeship your dotCloud API token and name your project so they
   can push to your dotCloud application.
4. Push new code to Github or Bitbucket and watch it deploy to dotCloud.

Once you know your deployment through Codeship is working, then you
can start adding meaningful tests to Codeship's build process and in
that way you can ensure only working code gets pushed to your dotCloud
application.

