We can now push our application to DotCloud::

  $ dotcloud push ramen ramen-on-dotcloud/
  # upload /home/.../ramen-on-dotcloud/ ssh://dotcloud@uploader.dotcloud.com:21122/ramen
  # rsync
  [...]
  sent 8.11K bytes  received 352 bytes  995.06 bytes/sec
  total size is 14.78K  speedup is 1.75
  Deployment for "ramen" triggered. Will be available in a few seconds.
  2011-06-28 04:27:34 [api] Waiting for the build. (It may take a few minutes)
  2011-06-28 04:27:48 [www.0] service booted
  2011-06-28 04:27:48 [www.0] The build started
  2011-06-28 04:27:48 [www.0] Fetched code revision rsync-1309235251.44
  2011-06-28 04:27:51 [www.0] Reloading nginx configuration: nginx.
  2011-06-28 04:27:51 [www.0] The build finished successfully
  2011-06-28 04:27:51 [api] Deploy finished

  Deployment finished successfully. Your application is available at the following URLs
  www: http://ramen-dotcloud.dotcloud.com/

URLs have been generated for each web service in your application from
your application an account names. Open your browser to see your new
service in action!

If you want to attach a better URL to your application, read the
:doc:`../guides/domains` documentation.
