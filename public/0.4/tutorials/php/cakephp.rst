:title: CakePHP Tutorial
:description: How to create and deploy a simple PHPCake application on dotCloud.
:keywords: dotCloud, tutorial, documentation, cakephp, mysql

CakePHP
=======

In this tutorial, we will create a simple CakePHP application that displays a
form for new posts, saves the posts to a MySQL database, and lists all of the
saved posts.

.. contents::
   :local:
   :depth: 1

Create a Model
--------------

First, you will need to download the latest version of CakePHP and untar
(or unzip) it to a local directory on your computer. Once you have done
that, you can add a Post model. To do this, create a file in app/models
called "post.php". It will need to contain this code:

.. code-block:: php

   <?php

       class Post extends AppModel
       {
           var $name = 'Post';
           var $validate = array(
               'body' => array(
                   'rule' => 'notEmpty'
               )
           );
       }

   ?>


Create a Controller
-------------------

Next, create a file called "posts_controller.php" in app/controllers which
contains this code:

.. code-block:: php

   <?php

       class PostsController extends AppController {
           var $name = 'Posts';
           var $components = array('Session');

           function index() {
               $this->set('posts', $this->Post->find('all'));
           }

           function view($id) {
               $this->Post->id = $id;
               $this->set('post', $this->Post->read());

           }

           function add() {
               if (!empty($this->data)) {
                   if ($this->Post->save($this->data)) {
                       $this->Session->setFlash('Your post has been saved.');
                       $this->redirect(array('action' => 'index'));
                   }
               }
           }
       }

   ?>


Create a View
-------------

Finally, create a directory in app/views called "posts". Inside of that
directory, create a file called "index.ctp":

.. code-block:: php

    <!-- File: /app/views/posts/index.ctp -->

    <h1>Add Post</h1>
    <?php
        echo $this->Form->create('Post', array('action' => 'add'));
        echo $this->Form->input('body', array('rows' => '3'));
        echo $this->Form->end('Save Post');
    ?>

    <h1>Posts</h1>
    <table>
        <tr>
            <th>Id</th>
            <th>Text</th>
        </tr>

        <!-- Here is where we loop through our $posts array, printing out post info -->

        <?php foreach ($posts as $post): ?>
        <tr>
            <td><?php echo $post['Post']['id']; ?></td>
            <td>
                <?php echo $post['Post']['body']; ?>
            </td>
        </tr>
        <?php endforeach; ?>

    </table>


Send requests to the front web controller
-----------------------------------------

To send requests to the front web controller, you'll need to create a file
in app/webroot called "nginx.conf". Inside of this file, add this line:

.. code-block:: nginx

   try_files $uri $uri/ /index.php;

This will make sure that all requests for dynamic content will be directed
to the front web controller, which will route them appropriately.

Create a dotcloud.yml file
--------------------------

To deploy to DotCloud, you need to create a file in the root directory of
your cakephp app (the directory under "app") called "dotcloud.yml" which
describes the structure of your application. For this application, the
dotcloud.yml will look like this:

.. code-block:: yaml

    www:
        type: php
        approot: app/webroot
    mysql:
        type: mysql


Enable automatic database configuration
---------------------------------------

In app/config, you'll find a file called database.php.default. Copy or
rename this file to "database.php", then add a constructor function inside
the DATABASE_CONFIG class which reads the DotCloud 'environment.json" file
and updates the default database settings. The end result will look something
like this:

.. code-block:: php

   <?php

       class DATABASE_CONFIG {

           var $default = array(
               'driver' => 'mysql',
               'persistent' => false,
               'host' => '',
               'port' => '',
               'login' => 'root',
               'password' => '',
               'database' => 'cakephp',
               'prefix' => '',
           );

           var $test = array(
               'driver' => 'mysql',
               'persistent' => false,
               'host' => 'localhost',
               'login' => 'user',
               'password' => 'password',
               'database' => 'test_database_name',
               'prefix' => '',
           );

           function __construct() {
               $json = file_get_contents("/home/dotcloud/environment.json");
               $env = json_decode($json, true);
               $this->default['host'] = $env['DOTCLOUD_MYSQL_MYSQL_HOST'];
               $this->default['port'] = $env['DOTCLOUD_MYSQL_MYSQL_PORT'];
               $this->default['password'] = $env['DOTCLOUD_MYSQL_MYSQL_PASSWORD'];
           }

       }

   ?>

Deploy to DotCloud
------------------

Now you're ready to deploy! Just run the following (if you're using git
or hg, you'll need to commit your changes first)::

   $ dotcloud create my_app
   $ dotcloud push my_app

At the end of the push, you'll see the URL for your newly deployed app. Simply
open that url in your browser (add "/posts" at the end) to see your app.
