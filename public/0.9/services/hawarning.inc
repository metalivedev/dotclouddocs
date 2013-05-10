.. warning::
   This service does not scale for High Availability on the
   dotCloud Platform. Please do not use this service if your application
   requires High Availability. 


Can your application continue to run without this service? If not,
then you should choose an alternative, or spend more time planning how
your application will handle outages in this service during machine
reboots or other interruptions to this service.

Suggested alternatives for High Availability
............................................

  +-------------------------+--------------------------+
  | Use This                | Instead of This          |
  +=========================+==========================+
  | MySQL or MongoDB        | PostgreSQL, PostGIS      |
  +-------------------------+--------------------------+
  | Elastic Search (custom) | Solr                     |
  +-------------------------+--------------------------+

