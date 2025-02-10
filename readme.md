# CReact - A Minimal C++ UI Library

CReact is a simple, experimental C++ UI library that provides a React-like component system using functions and state management. This project is intended for **educational purposes only** and is **not production-ready**.

## Features

- Functional components with properties (`props`)
- Basic state management (`creact_use_state`)
- Virtual DOM-like element creation (`creact_create_element`)
- Rendering to a root container (`creact_render`)

## Example Usage

Below is an example implementation using CReact:

```cpp
#include "../src/creact.h"
#include <any>
#include <functional>
#include <vector>

CREACT_COMPONENT(link) {
  props.push_back({"class", "link"});
  props.push_back({"href", "https://github.com/"});
  return creact_create_element("a", props, children);
}

CREACT_COMPONENT(counter) {
  auto [state, set_state] = creact_use_state(0);
  creact_node text = creact_create_element("Counter: " + std::to_string(state));

  props.push_back({"class", "counter"});
  props.push_back({"click", [=](std::any) { set_state(state + 1); }});

  children.push_back(text);
  return creact_create_element("button", props, children);
}

CREACT_COMPONENT(green_box) {
  props.push_back({"class", "green_box"});

  creact_node text = creact_create_element("Inner text!");
  creact_node link_component = creact_create_element(
      &link, std::vector<creact_component_prop>(), {text});
  children.push_back(link_component);

  creact_node counter_component =
      creact_create_element(&counter, std::vector<creact_component_prop>(), {});
  children.push_back(counter_component);

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
```

## Limitations

- This library is a **proof-of-concept** and not optimized for performance.
- No real diffing algorithm for efficient updates.
- No support for event delegation or complex state management.
- Currently, styles and event handling are minimal.

## Installation

Since this is an experimental library, you need to manually include the header file:

```cpp
#include "path/to/creact.h"
```

Ensure you have a C++ compiler that supports C++20 or later.

## License

This project is provided as-is for educational purposes. Feel free to experiment, modify, and learn from it!
