#include "../src/creact.h"

int main(void) {
  creact_component_prop green_box_props[] = {
      {"id", "green_box"},
  };
  creact_element *green_box = creact_create_html_element(
      "green_box", "div", green_box_props, 1, NULL, 0);

  creact_component_prop container_props[] = {
      {"id", "container"},
  };
  creact_element *container = creact_create_html_element(
      "container", "div", container_props, 1, &green_box, 1);

  creact_render("root", container);
  return 0;
}
