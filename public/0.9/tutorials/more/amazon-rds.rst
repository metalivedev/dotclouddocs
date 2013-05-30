:title: Amazon RDS
:description: How to use Amazon's RDS with dotCloud.
:keywords: dotCloud, tutorial, documentation, Amazon, RDS

Amazon's RDS
============

.. note::

    Please ensure you're setting this configuration up for "US East
    (N. Virginia) Region"

1. Login to your Amazon AWS Management Console

2. Click the RDS tab (and sign up as necessary)

3. Click "DB Security Groups"

4. Click button "Create DB Security Group" and name it "dotcloud" (or whatever you prefer)

5. Select your newly created Security Group and you'll see that there are no connection types yet.

6. From the drop-down, choose "EC2 Security Group" and enter the following:

    AWS Account ID: 2821-7192-8639 EC2 Security Group: lxc

7. Click the Add button. Now repeat for the following additional connections
(yes, there are 18 total) organized by (EC2 Security Group)-(AWS Account ID)::

    admin-282171928639, gateway-282171928639, lxc-282171928639
    admin-557344332487, gateway-557344332487, lxc-557344332487
    admin-198142415373, gateway-198142415373, lxc-198142415373
    admin-487118584796, gateway-487118584796, lxc-487118584796
    admin-782166217850, gateway-782166217850, lxc-782166217850
    admin-512129773505, gateway-512129773505, lxc-512129773505

8. Now, create a new database instance (or edit an existing one) and add your
new DB Security Group to the list of security groups. You can now access your
RDS database from your dotCloud applications.
