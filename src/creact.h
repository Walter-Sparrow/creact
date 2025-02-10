#pragma once

#include <any>
#include <cstddef>
#include <emscripten/val.h>
#include <functional>
#include <string>
#include <variant>
#include <vector>

struct creact_node;
typedef std::string creact_text;

typedef struct {
  std::string name;
  std::variant<std::string, std::function<void(std::any)>> value;
} creact_component_prop;

typedef struct {
  std::string html_tag;
  std::vector<creact_component_prop> props;
  std::vector<creact_node> children;
  emscripten::val state_node;
} creact_element_html;

typedef struct creact_node (*creact_function_component)(
    std::vector<creact_component_prop>, std::vector<creact_node>);

typedef struct creact_element_function_component {
  creact_function_component component;
  std::vector<creact_component_prop> props;
  std::vector<creact_node> children;
  std::vector<std::any> hooks;
  size_t hook_index;
  emscripten::val state_node;
} creact_element_function_component;

typedef enum : unsigned char {
  creact_element_type_text = 0,
  creact_element_type_html,
  creact_element_type_function_component,
} creact_node_type;

typedef struct creact_node {
  creact_node_type type;
  std::variant<creact_text, creact_element_html,
               creact_element_function_component *>
      element;
} creact_node;

#define CREACT_COMPONENT(function)                                             \
  creact_node function(std::vector<creact_component_prop> props,               \
                       std::vector<creact_node> children)

creact_node creact_create_element(const std::string text);

creact_node creact_create_element(const std::string html_tag,
                                  std::vector<creact_component_prop> props,
                                  std::vector<creact_node> children);

creact_node creact_create_element(creact_function_component component,
                                  std::vector<creact_component_prop> props,
                                  std::vector<creact_node> children);

emscripten::val
creact_create_js_callback(std::function<void(std::any)> callback);

void creact_render_text(emscripten::val html_parent, const std::string text);
emscripten::val creact_render_element(creact_element_html &element,
                                      emscripten::val html_parent);

emscripten::val creact_render(const std::string html_root, creact_node node);
emscripten::val creact_render(emscripten::val html_root, creact_node node);

void creact_update_component(creact_element_function_component *component);

extern creact_element_function_component *current_component;
extern emscripten::val root;
extern creact_node app;

template <typename T>
std::pair<T, std::function<void(T)>> creact_use_state(const T &initialValue) {
  auto *comp = current_component;
  auto &hooks = comp->hooks;
  size_t index = comp->hook_index;
  T state;
  if (index >= hooks.size()) {
    hooks.push_back(initialValue);
    state = initialValue;
  } else {
    state = std::any_cast<T>(hooks[index]);
  }

  auto setter = [index, comp](T newValue) {
    comp->hooks[index] = newValue;
    creact_update_component(comp);
  };

  comp->hook_index++;
  return std::make_pair(state, setter);
}
