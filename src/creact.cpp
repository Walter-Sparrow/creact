#include "creact.h"
#include "creact_callback.h"
#include <any>
#include <cstddef>
#include <cstdio>
#include <emscripten.h>
#include <emscripten/val.h>
#include <string>
#include <vector>

creact_element_function_component *current_component = nullptr;
emscripten::val root;
creact_node app;

creact_node creact_create_element(const std::string text) {
  creact_node node;
  node.type = creact_element_type_text;
  node.element = text;
  return node;
}

creact_node creact_create_element(const std::string html_tag,
                                  std::vector<creact_component_prop> props,
                                  std::vector<creact_node> children) {
  creact_node node;
  node.type = creact_element_type_html;
  creact_element_html element;

  element.html_tag = html_tag;
  element.props = props;
  element.children = children;

  node.element = element;
  return node;
}

creact_node creact_create_element(creact_function_component component,
                                  std::vector<creact_component_prop> props,
                                  std::vector<creact_node> children) {
  creact_node node;
  node.type = creact_element_type_function_component;
  auto *element = new creact_element_function_component();
  element->hooks = std::vector<std::any>();
  element->hook_index = 0;
  element->component = component;
  element->props = props;
  element->children = children;
  node.element = element;
  return node;
}

void creact_render_text(emscripten::val html_parent, const std::string text) {
  auto document = emscripten::val::global("document");
  auto elementText = emscripten::val::u8string(text.c_str());
  auto element = document.call<emscripten::val>("createTextNode", elementText);
  html_parent.call<void>("appendChild", element);
}

emscripten::val creact_render_element(creact_element_html &element,
                                      emscripten::val html_parent) {
  auto document = emscripten::val::global("document");
  element.state_node = document.call<emscripten::val>(
      "createElement", emscripten::val(element.html_tag));

  for (auto &prop : element.props) {
    auto prop_name = emscripten::val(prop.name);
    if (std::holds_alternative<std::string>(prop.value)) {
      std::string s = std::get<std::string>(prop.value);
      element.state_node.call<void>("setAttribute", prop_name,
                                    emscripten::val(s));
    } else if (std::holds_alternative<std::function<void(std::any)>>(
                   prop.value)) {
      auto callback = std::get<std::function<void(std::any)>>(prop.value);
      auto wrapper = [=](std::any arg) {
        callback(arg);
        printf("callback called\n");
      };
      emscripten::val js_callback = creact_create_js_callback(wrapper);
      element.state_node.call<void>("addEventListener", prop_name, js_callback);
    }
  }

  return element.state_node;
}

emscripten::val creact_render(const std::string html_root, creact_node node) {
  emscripten::val document = emscripten::val::global("document");
  app = node;
  root = document.call<emscripten::val>("getElementById",
                                        emscripten::val(html_root));

  return creact_render(root, node);
}

emscripten::val creact_render(emscripten::val html_root, creact_node node) {
  if (node.type == creact_element_type_text) {
    creact_render_text(html_root, std::get<creact_text>(node.element));
  } else if (node.type == creact_element_type_function_component) {
    auto element = std::get<creact_element_function_component *>(node.element);

    current_component = element;
    element->hook_index = 0;

    creact_node computedNode =
        element->component(element->props, element->children);

    element->state_node = creact_render(html_root, computedNode);
    return element->state_node;
  } else if (node.type == creact_element_type_html) {
    creact_element_html element = std::get<creact_element_html>(node.element);
    auto node = creact_render_element(element, html_root);
    html_root.call<void>("appendChild", element.state_node);

    for (size_t i = 0; i < element.children.size(); i++) {
      creact_render(element.state_node, element.children[i]);
    }

    return node;
  }

  return html_root;
}

void creact_update_component(creact_element_function_component *comp) {
  current_component = comp;
  comp->hook_index = 0;

  creact_node computedNode = comp->component(comp->props, comp->children);
  emscripten::val parent = comp->state_node["parentNode"];
  emscripten::val new_state_node = creact_render(parent, computedNode);

  parent.call<void>("replaceChild", new_state_node, comp->state_node);
  comp->state_node = new_state_node;
}
