CloudAMQP
=========

This tutorial will walk you through using CloudAMQP's managed RabbitMQ solution to help you use their RabbitMQ service with your dotCloud application.

1. Visit http://CloudAMQP.com and sign up for an account
    a. Add your credit card information (if you want a paid version)
    b. Create a new instance.
        1. Select a Name
        2. Pick plan size
        3. Pick Data center location. We recommend Amazon US-East-1 so that it is as close to dotCloud’s servers as possible.
        4. Click Create
    c. Go to the control panel, and copy the URL for your new instance.

2. Add the environment variables to your dotCloud application. 

    There are two ways to get these variables. You can either look at the RabbitMQ URL, or you can look at the variables listed on the instance details page.

    Here is an Example RabbitMQ URL from CloudAMQP ::

        amqp://myUser:pJEeksq0E@tiger.cloudamqp.com/myUser

    The URLs are broken down like this ::

        amqp://Username:Password@Hostname:Port/VHost

    **Username**
        the username for your RabbitMQ instance.

    **Password**
        the password for your instance.

    **Hostname**
        the host where your RabbitMQ instance lives.

    **Port**
        the port number for your RabbitMQ instance on the host. If not there it defaults to 5672.

    **VHost**
        the virtualHost for your RabbitMQ instance.

    Notice in the example URL above, there is no port number listed, that means it defaults to 5672. 
    
    Using that URL here is our VARIABLES:

    .. code-block:: bash

        CLOUDAMQP_RABBITMQ_AMQP_HOST=tiger.cloudamqp.com
        CLOUDAMQP_RABBITMQ_AMQP_VIRTUALHOST=myUser
        CLOUDAMQP_RABBITMQ_AMQP_LOGIN=myUser
        CLOUDAMQP_RABBITMQ_AMQP_PASSWORD=pJEeksq0E
        CLOUDAMQP_RABBITMQ_AMQP_PORT=5672
        CLOUDAMQP_RABBITMQ_AMQP_URL=amqp://myUser:pJEeksq0E@tiger.cloudamqp.com/myUser

    Given the above variables now we need to add those to our dotCloud application. Replace ``<app_name>`` with the name of your dotCloud application.

    Run this on your command line:

    .. code-block:: bash
        
        $ dotcloud var set <app_name> \
        'CLOUDAMQP_RABBITMQ_AMQP_HOST=tiger.cloudamqp.com' \
        'CLOUDAMQP_RABBITMQ_AMQP_VIRTUALHOST=myUser' \
        'CLOUDAMQP_RABBITMQ_AMQP_LOGIN=myUser' \
        'CLOUDAMQP_RABBITMQ_AMQP_PASSWORD=pJEeksq0E' \
        'CLOUDAMQP_RABBITMQ_AMQP_PORT=5672' \
        'CLOUDAMQP_RABBITMQ_AMQP_URL=amqp://myUser:pJEeksq0E@tiger.cloudamqp.com/myUser'


    After you run the command it will redeploy your application and those variables will now be available to you. To confirm run this command

    .. code-block:: bash

        $ dotcloud var list <app_name>

3. Now that you have those variables available to your application, you will need to reference those variables when configuring your connection to RabbitMQ

Learning More
-------------

For more information you can look at `CloudAMQP's dotCloud documentation <http://www.cloudamqp.com/docs-dotcloud.html>`_ or `RabbitMQ's documentation <http://www.rabbitmq.com/documentation.html>`_ there is a lot of good information there. 

