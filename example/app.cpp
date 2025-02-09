#include "../src/creact.h"
#include <vector>

creact_node link(std::vector<creact_component_prop> props,
                 std::vector<creact_node> children) {
  props.push_back({"id", "link"});
  props.push_back({"href", "https://github.com/"});
  return creact_create_element("link", "a", props, children);
}

creact_node green_box(std::vector<creact_component_prop> props,
                      std::vector<creact_node> children) {
  props.push_back({"id", "green_box"});

  creact_node text = creact_create_element("Inner text!");
  creact_node link_component = creact_create_element(
      &link, std::vector<creact_component_prop>(), {text});
  children.push_back(link_component);

  return creact_create_element("green_box", "p", props, children);
}

int main(void) {
  creact_node text = creact_create_element("Hello, World!");
  creact_node green_box_component = creact_create_element(
      &green_box, std::vector<creact_component_prop>(), {text});

  creact_node container = creact_create_element(
      "container", "div", {{"id", "container"}}, {green_box_component});

  creact_render("root", container);
  return 0;
}
