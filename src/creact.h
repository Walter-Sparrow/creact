#pragma once

#include <cstddef>

struct creact_node;
typedef const char *creact_text;

typedef struct {
  const char *name;
  const char *value;
} creact_component_prop;

typedef struct {
  creact_component_prop *values;
  size_t len;
} creact_component_props;

typedef struct {
  struct creact_node *values;
  size_t len;
} creact_component_children;

typedef struct {
  const char *key;
  const char *html_tag;
  creact_component_props props;
  creact_component_children children;
} creact_element_html;

typedef struct creact_node (*creact_function_component)(
    creact_component_props, creact_component_children);

typedef struct creact_element_function_component {
  creact_function_component component;
  creact_component_props props;
  creact_component_children children;
} creact_element_function_component;

typedef enum : unsigned char {
  creact_element_type_text = 0,
  creact_element_type_html,
  creact_element_type_function_component,
} creact_node_type;

typedef struct creact_node {
  creact_node_type type;
  union {
    creact_text text;
    creact_element_html *html;
    creact_element_function_component *function_component;
  } element;
} creact_node;

creact_node creact_create_element(const char *text);

creact_node creact_create_element(const char *key, const char *html_tag,
                                  creact_component_props props,
                                  creact_component_children children);

creact_node creact_create_element(creact_function_component component,
                                  creact_component_props props,
                                  creact_component_children children);

void creact_render(const char *html_root, creact_node node);
