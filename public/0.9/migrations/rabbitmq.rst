RabbitMQ
========
We have decided to remove support for our RabbitMQ service. We recommend that if you still need RabbitMQ that you sign up for an account at `CloudAMQP <http://cloudamqp.com>`_.

Prices
------
All payments for RabbitMQ instances on CloudAMQP.com will be processed by CloudAMQP.com

CloudAMQP offers different RabbitMQ plans. For a full list of prices, refer to their `pricing page <http://www.cloudamqp.com/plans.html>`_.

They have a Free plan which you can use for your Sandbox applications, and 3 different paid plans which would be good for any live applications.

Documentation
-------------
CloudAMQP has a `dotCloud specific documentation page <http://www.cloudamqp.com/docs-dotcloud.html>`_ that has lots of great information about their service.

Sign up and Setup
------------------
For details on how to signup for an account and how to configure your dotCloud application to use the new RabbitMQ server. Please refer to our :doc:`CloudAMQP tutorial </tutorials/more/cloudamqp/>`.

Migration Steps
---------------
1. Sign up for a CloudAMQP account
2. Add your new RabbitMQ server credentials to your application.
3. Stop your application, making sure to empty all of the queues on your dotCloud RabbitMQ instance.
4. Change your application code to use the new ENV variables you configured in step 2
5. Start up your application, and make sure it is connecting to the new CloudAMQP RabbitMQ instance.
6. Let dotCloud know that you are finished with your RabbitMQ instance, and we will remove it from your application so that you are no longer charged for it.
