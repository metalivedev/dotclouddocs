<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
    <script src="http://code.opalang.org/plugin.js" type="text/javascript"></script>
    <title >Chat</title>
  </head>
  <body>
  <div class="container">
   <div class="inner">
     <opacode height="306px" width="596px">do Resource.register_external_css(&quot;http://twitter.github.com/bootstrap/assets/css/bootstrap-1.1.1.min.css&quot;)
do Resource.register_external_css(&quot;http://chat.opalang.org/resources/css.css&quot;)

type message = {author: string /**The name of the author (arbitrary string)*/
              ; text: string  /**Content entered by the user*/}

@publish room = Network.cloud(&quot;room&quot;): Network.network(message)

user_update(x: message) =
  line = &lt;div class=&quot;row line&quot;&gt;
  &lt;div class=&quot;span1 columns userpic&quot; /&gt;
  &lt;div class=&quot;span2 columns user&quot;&gt;{x.author}:&lt;/div&gt;
  &lt;div class=&quot;span13 columns message&quot;&gt;{x.text}
  &lt;/div&gt;
  &lt;/div&gt;
  ignore(Dom.put_at_start(#conversation, Dom.of_xhtml(line)))

broadcast(author) =
  text = Dom.get_value(#entry)
  message = {~author ~text}
  do Network.broadcast(message, room)
  Dom.clear_value(#entry)

start() =
  author = Random.string(8)
  &lt;div class=&quot;topbar&quot;&gt;&lt;div class=&quot;fill&quot;&gt;&lt;div class=&quot;container&quot;&gt;&lt;div id=#logo/&gt;&lt;/div&gt;&lt;/div&gt;&lt;/div&gt;
  &lt;div id=#conversation class=&quot;container&quot; onready={_
  -&gt; Network.add_callback(user_update, room)}&gt; &lt;/div&gt;
  &lt;div id=#footer&gt;&lt;div class=&quot;container&quot;&gt;
  &lt;input id=#entry class=&quot;xlarge&quot; onnewline={_ -&gt; broadcast(author)}/&gt;
  &lt;div class=&quot;btn primary&quot; onclick={_ -&gt; broadcast(author)}&gt;Post&lt;/div&gt;
  &lt;/div&gt;&lt;/div&gt;

server = Server.one_page_server(&quot;Chat&quot;, -&gt; start())
     </opacode>
      </div>
  </div>
    <script type="text/javascript">
      (new CodeOpalangPlugin()).init();
    </script>


</body></html>
