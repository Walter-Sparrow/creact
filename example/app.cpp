#include "../src/creact.h"
#include <vector>

CREACT_COMPONENT(link) {
  props.push_back({"class", "link"});
  props.push_back({"href", "https://github.com/"});
  return creact_create_element("a", props, children);
}

CREACT_COMPONENT(green_box) {
  props.push_back({"class", "green_box"});

  creact_node text = creact_create_element("Inner text!");
  creact_node link_component = creact_create_element(
      &link, std::vector<creact_component_prop>(), {text});
  children.push_back(link_component);

  return creact_create_element("p", props, children);
}

int main(void) {
  std::vector<creact_node> children;
  for (int i = 0; i < 10; i++) {
    creact_node text = creact_create_element("Hello, World!");
    creact_node green_box_component = creact_create_element(
        &green_box, std::vector<creact_component_prop>(), {text});
    children.push_back(green_box_component);
  }

  creact_node container =
      creact_create_element("div", {{"class", "container"}}, children);

  creact_render("root", container);
  return 0;
}
