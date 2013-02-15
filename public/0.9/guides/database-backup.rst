:title: Scheduling Backups
:description: How to automatically backup services running on dotCloud
:keywords: dotCloud documentation, quick start guide, Backup services, login to service, manual test backup, copy backup script, manual test backup, schedule a backup, FTP, SSH, S3

Scheduling Backups
==================

.. include:: ../dotcloud2note.inc

This will show you how to automatically backup services running on dotCloud.


What Can I Backup?
------------------

This script currently fits:

* MySQL databases (stand-alone or master/slave),
* PostgreSQL and PostGIS databases (the method is exactly the same),
* services that store files in ``~/data``.

When you setup the script, you will have to specify the kind
of backup that you need.

.. warning::
   Making consistent MySQL backups can be tricky. If you are using
   MyISAM tables, the only way to guarantee consistent backups is
   to prevent all writes during the duration of the backup. To make
   sure that your backups will be consistent, the default backup
   script that we provide will do that. It means that for the duration
   of the whole backup, your database will be locked against writes.
   For big databases, it can mean many minutes during which you can't
   UPDATE, INSERT, DELETE, etc. in your database. If you want to avoid
   that, you have to make sure that you use InnoDB, and update the
   backup script to use the ``--single-transaction`` option.


Login to the Service
--------------------

The first step is to get a shell on the service that you want to backup.

This is as easy as ``dotcloud run -A <deployment_name> <service_name>``.

For instance, to get a shell on your database server::

  dotcloud run sql


Copy the Backup Script
----------------------

Download :download:`this backup script <backup.sh>` on your service.
The easiest way to do it is to run the following inside the service::

  curl --output backup.sh http://docs.dotcloud.com/0.9/_downloads/backup.sh

Then, make the script executable::

  chmod +x backup.sh


Choose Between FTP, SSH and S3
------------------------------

Currently, the script can upload your backups using FTP, SSH or S3.

FTP
^^^

If using FTP, you will need to know the hostname of your FTP server, 
as well as the login and password of the FTP user.

SSH
^^^

If using SSH, you will need to set up public key authentication on
the SSH server storing your backups. Here is how to do it.

#. Create an SSH key on the dotCloud service, by running "ssh-keygen".
   Just hit "enter" when prompted for anything (it will use the default
   path to store the key, and will not use a passphrase to encrypt the key,
   which would be an issue for unattended backups anyway).
#. Add the public key (located in ~/.ssh/id_rsa.pub on the dotCloud 
   service) to ~/.ssh/authorized_keys on the backup server (in the account
   that you will use to store the backups).
#. Test the SSH login (this is important, since it will also add the
   server's public key to the known_hosts file; if you don't do that,
   the backup script will fail) by doing "ssh backupuser@backupserver"
   from the dotCloud service. You should get a shell on the backup server.

.. note::
   The supplied backup script does not support SSH password. That's why
   we use a public key. It's usually considered to be a better practice
   anyway.

S3
^^

If you want to use S3, you need to setup your credentials on the database
service first. Log into the service with ``dotcloud run sql bash``, and run
"s3cmd --configure". You will be asked for your AWS credentials. When
asked for a passphrase, don't supply one. You can enable HTTPS however
(it can't hurt)::

  s3cmd --configure

This command will open up an interactive session. You can find the typical configuration below and adapt it to suit your needs::

  Enter new values or accept defaults in brackets with Enter.
  Refer to user manual for detailed description of all options.

  Access key and Secret key are your identifiers for Amazon S3
  Access Key: ...
  Secret Key: ......

  Encryption password is used to protect your files from reading
  by unauthorized persons while in transfer to S3
  Encryption password: 
  Path to GPG program: 

  When using secure HTTPS protocol all communication with Amazon S3
  servers is protected from 3rd party eavesdropping. This method is
  slower than plain HTTP and can't be used if you're behind a proxy
  Use HTTPS protocol [No]: Yes

  New settings:
    Access Key: ...
    Secret Key: ......
    Encryption password: 
    Path to GPG program: None
    Use HTTPS protocol: True
    HTTP Proxy server name: 
    HTTP Proxy server port: 0

  Test access with supplied credentials? [Y/n] Y
  Please wait...
  Success. Your access key and secret key worked fine :-)

  Now verifying that encryption works...
  Not configured. Never mind.

  Save settings? [y/N] y
  Configuration saved to '/home/dotcloud/.s3cfg'

You should also create a bucket to hold your backups::

  s3cmd mb s3://ramen-backups

You can of course use a different name for your bucket if you like.
You can also use an existing bucket. The backups can be stored directly
in the bucket, or in a directory or subdirectory.

S3 for larger than 5 GB files
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If your dumps are larger than 5 GB, use ``s3multi`` instead of ``s3``.
It will split the backup into chunks, each named with a numerical suffix
(.00, .01, .02, etc.) because ``s3cmd`` cannot handle uploads larger
than 5 GB. To restore a multi-part dump, just concatenate the files
(using ``cat``).


Do a Manual Test Backup
----------------------- 

The general syntax for the backup script is::

  ~/backup.sh <what> <how> <where>

The ``what`` will be ``mysql``, ``pgsql``, or ``data``, depending on
what you want to backup.

The ``how`` will be the backup method explained in the previous section.

The ``where`` is the server/bucket/credentials/... to be used.

From the SSH connection to the dotCloud service, run one of the following,
depending of the method you want to use::

  ~/backup.sh mysql ftp ramenbackups:chowmein@ftp.noodles.com
  ~/backup.sh mysql ssh ramenbackups@vps.noodles.com
  ~/backup.sh mysql s3 ramen-backups
  ~/backup.sh mysql s3multi ramen-backups

Don't forget to:

* replace "mysql" with "pgsql" if you're running this from a PostgreSQL
  service, and with "data" if you are setting up backups for a web app
  container and want to save its non-volatile data;
* replace the example servers/logins/passwords/buckets with yours!

Now check your remote server or bucket: you should see a file with a naming 
scheme similar to ramen-default-sql-0_2011-02-10_14:17:42_UTC.sql.gz
containing a SQL backup.

.. note::
   Since not everyone lives in the same timezone, the backup script will
   use UTC (or GMT) time when generating the file name. The _UTC suffix
   is there to remind you of this fact.


Schedule the Backup Script With a Crontab
-----------------------------------------

From the SSH connection to the dotCloud service, run the following
command::

  crontab - <<EOF
  MAILTO=""
  $[$RANDOM%60] $[$RANDOM%24] * * * ~/backup.sh mysql ftp ramenbackups:chowmein@ftp.noodles.com 2>&1 | mail -s "Backup result for $HOSTNAME" cook@noodles.com
  EOF

Of course, you should adapt the parameters of the backup script, just like
we did in the previous step. Also, you should replace cook@noodle.com with
your email address. Each time the backup script is run (once per day),
you will receive an email notification. The notification will tell you the
size of the backup (watch out for very small sizes, which could indicate
a failure). If an error occurs, the email notification should mention it.

.. note::
   Using the previous recipe, your backups will run once per day. 
   To avoid everyone starting their backups at the same time, the recipe
   uses a little trick: the exact time of the backup will be randomly
   chosen when you create the crontab. Your backup will always be started
   at this same time, until you modify the crontab, of course. 

   If you want to check which time was scheduled for your backups, 
   just do "crontab -l". The minute and hour of the backups are the two
   numbers in front of the three asterisks.

   If want your backups to occur at a fixed time, or at different frequencies,
   feel free to adapt the crontab.

.. note::
   The default emails sent by the cron daemon will contain the full
   command line executed. Including the password specified on the command
   line. To avoid disclosing your passwords, we specify MAILTO="" so
   cron will never send an email by itself; and then we pipe the backup
   script through the "mail" command to specify the email subject ourselves.
