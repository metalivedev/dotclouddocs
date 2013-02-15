:title: Java Play! Tutorial
:description: How to deploy the sample chat application that ships with the Play framework on dotCloud.
:keywords: dotCloud, tutorial, documentation, java play!, play framework

Play!
=====

.. include:: ../../dotcloud2note.inc

The Play framework makes it easy to build web applications with Java and
Scala. The framework's focus is on developer productivity and RESTful
architectures.

In this tutorial, we'll walk through the deployment of the sample chat
application that ships with the Play framework.

Create a compressed WAR file
----------------------------

First, download and unzip the latest version of the Play framework. You'll also
want to create a new directory somewhere for your dotCloud app, which will
contain the compressed WAR file and your dotcloud.yml file. Change to the
directory that contains Play and run::

    cd samples-and-tests
    ../play war chat -o /YOUR_APP_DIR/chat.war --zip

Deploy the WAR file to dotCloud
-------------------------------

Now, create a dotCloud app::

    dotcloud create YOUR_APP_NAME

Change to the directory that contains your app and create a dotcloud.yml
file with the following contents::

    www:
        type: java

"www" can be replaced by whichever name you prefer for this service within your
app.

Finally, to push the WAR and dotcloud.yml files to dotCloud, run::

    dotcloud push

That's all it takes! Your app will build automatically, and you can find the
URL of your new app in the output of the push command.
