#pragma once

#include <string>
#include <variant>
#include <vector>

struct creact_node;
typedef std::string creact_text;

typedef struct {
  const char *name;
  const char *value;
} creact_component_prop;

typedef struct {
  std::string key;
  std::string html_tag;
  std::vector<creact_component_prop> props;
  std::vector<creact_node> children;
} creact_element_html;

typedef struct creact_node (*creact_function_component)(
    std::vector<creact_component_prop>, std::vector<creact_node>);

typedef struct creact_element_function_component {
  creact_function_component component;
  std::vector<creact_component_prop> props;
  std::vector<creact_node> children;
} creact_element_function_component;

typedef enum : unsigned char {
  creact_element_type_text = 0,
  creact_element_type_html,
  creact_element_type_function_component,
} creact_node_type;

typedef struct creact_node {
  creact_node_type type;
  std::variant<creact_text, creact_element_html,
               creact_element_function_component>
      element;
} creact_node;

creact_node creact_create_element(const std::string text);

creact_node creact_create_element(const std::string key,
                                  const std::string html_tag,
                                  std::vector<creact_component_prop> props,
                                  std::vector<creact_node> children);

creact_node creact_create_element(creact_function_component component,
                                  std::vector<creact_component_prop> props,
                                  std::vector<creact_node> children);

void creact_render(const std::string html_root, creact_node node);
