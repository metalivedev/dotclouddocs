# See http://sphinx.pocoo.org/ext/tutorial.html#extension-design

from docutils import nodes
from sphinx.util.compat import Directive
_ = lambda x: x


class tabswitcher(nodes.Admonition, nodes.Element):
    pass


class tab(nodes.Admonition, nodes.Element):
    pass


class TabDirective(Directive):

    has_content = True

    def run(self):
        title = self.content[0]
        text = '\n'.join(self.content[1:])
        node = tab(text)
        textnodes, messages = self.state.inline_text(title, self.lineno)
        node += nodes.title(title, *textnodes, classes=['tabtitle'])
        node += messages
        node['classes'] += ['tab']
        self.state.nested_parse(self.content[1:], self.content_offset, node)
        return [node]


class TabswitcherDirective(Directive):

    has_content = True

    def run(self):
        text = '\n'.join(self.content)
        node = tabswitcher(text)
        node['classes'] += ['tabswitcher']
        self.state.nested_parse(self.content, self.content_offset, node)
        return [node]


def visit_tabswitcher_node(self, node):
    self.visit_admonition(node)


def depart_tabswitcher_node(self, node):
    self.depart_admonition(node)


def visit_tab_node(self, node):
    self.visit_admonition(node)


def depart_tab_node(self, node):
    self.depart_admonition(node)

def dummy_null_function(self, node):
    pass

def setup(app):
    app.add_node(tabswitcher,
                 html=(visit_tabswitcher_node, depart_tabswitcher_node),
                 latex=(dummy_null_function, dummy_null_function))
    app.add_node(tab,
                 html=(visit_tab_node, depart_tab_node),
                 latex=(dummy_null_function, dummy_null_function))
    app.add_directive('tabswitcher', TabswitcherDirective)
    app.add_directive('tab', TabDirective)
