#include "../src/creact.h"
#include <cstdlib>

creact_node link(creact_component_props props,
                 creact_component_children children) {
  creact_component_prop link_props_value[] = {
      {"id", "link"},
      {"href", "https://github.com/"},
  };
  creact_component_props link_props = {link_props_value, 2};
  return creact_create_element("link", "a", link_props, children);
}

creact_node green_box(creact_component_props props,
                      creact_component_children children) {
  creact_node text = creact_create_element("Inner text!");
  children.values = (creact_node *)realloc(
      children.values, (children.len + 1) * sizeof(creact_node));
  children.values[children.len++] =
      creact_create_element(&link, {}, {&text, 1});

  creact_component_prop green_box_props_value[] = {
      {"id", "green_box"},
  };
  creact_component_props green_box_props = {green_box_props_value, 1};
  return creact_create_element("green_box", "p", green_box_props, children);
}

int main(void) {
  creact_node text = creact_create_element("Hello, World!");
  creact_node green_box_component =
      creact_create_element(&green_box, {}, {&text, 1});

  creact_component_prop container_props_value[] = {
      {"id", "container"},
  };
  creact_component_props container_props = {container_props_value, 1};
  creact_node container = creact_create_element(
      "container", "div", container_props, {&green_box_component, 1});

  creact_render("root", container);
  return 0;
}
