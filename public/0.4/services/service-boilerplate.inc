Basic Use
---------

Let's assume that you are building a DotCloud application called "ramen".
For the sake of simplicity, we will also assume that everything related
to this application is located in a directory called "ramen-on-dotcloud".

Let's setup our environment::

  $ dotcloud create ramen
  Created application "ramen" using the flavor "sandbox" (Use for development, free and unlimited apps. DO NOT use for production.)
  $ mkdir ramen-on-dotcloud

A DotCloud application is described by a :doc:`/guides/build-file`, which is
a simple YAML file named "dotcloud.yml" located in our "ramen-on-dotcloud" 
directory. To add a new service to our app, we just add the following lines
to "ramen-on-dotcloud/dotcloud.yml":
